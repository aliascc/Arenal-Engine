/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameApp.h"
#include "Keyboard.h"
#include "InputDefs.h"
#include "ImGuiManager.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "GraphicsDefs.h"
#include "GraphicDevice.h"
#include "AudioListener.h"
#include "PhysicsManager.h"
#include "Lights\LightManager.h"
#include "Camera\CameraManager.h"
#include "GameUtils\GameService.h"
#include "GameASAddOns\LogAddOnAS.h"
#include "GameASAddOns\GLMAddOnAS.h"
#include "GameUtils\GameComponent.h"
#include "GameASAddOns\TimeAddOnAS.h"
#include "GameASAddOns\BaseAddOnAS.h"
#include "GameASAddOns\AudioAddOnAS.h"
#include "GameASAddOns\InputAddOnAS.h"
#include "GameASAddOns\ColorAddOnAS.h"
#include "Crash Handling\CrashHandler.h"
#include "GameAssets\GameAssetManager.h"
#include "GameASAddOns\GameCoreAddOnAS.h"
#include "GameObject\GameObjectManager.h"
#include "Resource\GameResourceManager.h"
#include "AngelScript\AngelScriptManager.h"
#include "Core Game Command\CoreCommands.h"
#include "Crash Handling\CrashHandlerDefs.h"
#include "GameUtils\GameServiceCollection.h"
#include "GameASAddOns\LocalizationAddOnAS.h"
#include "GameUtils\GameComponentCollection.h"
#include "GameObject\Scripts\GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameApp::GameApp(HINSTANCE hInstance)
    : m_AppInst(hInstance)
{
#if defined(AE_MEM_CHECK)
    MemLeaks::MemoryBegin();
#endif //AE_MEM_CHECK

#ifdef AE_CRASH_HANDILING
    if (CrashHandlerInst.InitCrashHandling() != AEResult::Ok)
    {
        AETODO("Add error window");
    }
#endif //AE_CRASH_HANDILING
}

GameApp::~GameApp()
{
    CleanUp();

#ifdef AE_CRASH_HANDILING
    CrashHandlerInst.DeinitCrashHandling();
    CrashHandlerInst.DestroyInstance();
#endif //AE_CRASH_HANDILING

#if defined(AE_MEM_CHECK)
    MemLeaks::MemoryEnd();
#endif //AE_MEM_CHECK
}

void GameApp::CleanUp()
{
    AETODO("Check that deletions happen in reverse order of creation");
    //////////////////////////////////////////////////
    // Order of deletion must be maintain
    //
    // - ImGui Manager
    // - Game Services
    // - Game Component Collection
    // - Game Asset Manager (Contains Resources that need to be release before Game Resource Manager is release and calls Game Object Manager to inform about asset releases)
    // - Game Object Manager (Contains Resource that need to be release before Game Resource Manager)
    // - Game Resource Manager (Contains Resources that need to be release before Graphic Device)
    // - Physic Manager (Contains Info that Game Object needs)
    // - Light Manager
    // - Graphic Device
    // - Graphic Debug DX
    // - Logger
    // - Localization Manager
    // - Input Manager
    // - Angel Script Manager
    // - Game Command Manager
    //

    DeleteMem(m_ImGuiManager);

    DeleteMem(m_GameObjectManager);

    DeleteMem(m_GameServiceCollection);

    DeleteMem(m_GameComponentCollection);

    DeleteMem(m_GameAssetManager);

    DeleteMem(m_GameObjectScriptManager);

    DeleteMem(m_GameResourceManager);

    DeleteMem(m_PhysicsManager);

    DeleteMem(m_AudioManager);

    DeleteMem(m_LightManager);

    DeleteMem(m_CameraManager);

    //Delete Graphic Device Last after Components, Resources and Services
    DeleteMem(m_GraphicDevice);

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

    //Report Live Objects and then Delete Graphic Debug
    if (m_GraphicDebugDX != nullptr)
    {
        m_GraphicDebugDX->Report();
    }

    DeleteMem(m_GraphicDebugDX);

#endif

    //Delete Remaining Shared Objects
    DeleteMem(m_InputManager);

    DeleteMem(m_AngelScriptManager);

    //Singleton instance
    GameCommandManager::DestroyInstance();
    Logger::DestroyInstance();
    AudioListener::DestroyInstance();
    LocalizationManager::DestroyInstance();
}

void GameApp::ShutDownGameApp()
{
    m_IsReady = false;
    m_StartShutdown = true;

    AETODO("Use conditional variables");
    while (!m_IsEngineOff)
    {
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }

    CleanUp();
}

AEResult GameApp::ExtractGameEngineConfig()
{
    AEAssert(!m_GameProject.m_EngineConfigFile.empty());

    AEXMLParser newFile;
    if (newFile.LoadFile(m_GameProject.m_EngineConfigFile) != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "INIT_COULDNT_READ_FILE_MSG", __FUNCTION__, m_GameProject.m_EngineConfigFile);
        return AEResult::OpenFileFail;
    }

    AEXMLParser configXML = newFile[AE_ENGINE_CONFIG_MAIN_NODE];
    if (!configXML.IsReady())
    {
        return AEResult::XMLReadError;
    }

    m_GameProject.m_EngineLocation = boost::filesystem::current_path().string();

    uint32_t l_Count = configXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = configXML(i);

        std::string l_Type = child.GetName();
        
        if (l_Type.compare(AE_ENGINE_LOCALIZATION_NODE) == 0)
        {
            m_GameProject.m_GameEngineConfig.m_LocalizationFile        = m_GameProject.m_EngineLocation + AE_ENGINE_BIN_TO_DATA_PATH_ADD + child.GetString(AE_ENGINE_CONFIG_FILE_PROP);
        }
        else if (l_Type.compare(AE_ENGINE_GAMEOPTS) == 0)
        {
            m_GameProject.m_GameEngineConfig.m_GameOptsFile            = m_GameProject.m_EngineLocation + AE_ENGINE_BIN_TO_DATA_PATH_ADD + child.GetString(AE_ENGINE_CONFIG_FILE_PROP);
        }
    }

    return AEResult::Ok;
}

AEResult GameApp::ExtractGameProjectConfig()
{
    AEAssert(!m_GameProject.m_ProjectConfigFile.empty());

    AEXMLParser newFile;
    if (newFile.LoadFile(m_GameProject.m_ProjectConfigFile) != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "INIT_COULDNT_READ_FILE_MSG", __FUNCTION__, m_GameProject.m_ProjectConfigFile);
        return AEResult::OpenFileFail;
    }

    AEXMLParser configXML = newFile[AE_PROJ_CONFIG_MAIN_NODE];
    if (!configXML.IsReady())
    {
        return AEResult::XMLReadError;
    }

    std::size_t foundPos = m_GameProject.m_ProjectConfigFile.find(AE_PROJ_CONFIG_PROJ_FILE);
    m_GameProject.m_ProjectLocation = m_GameProject.m_ProjectConfigFile.erase(foundPos);

    uint32_t l_Count = configXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = configXML(i);

        std::string l_Type = child.GetName();

        if (l_Type.compare(AE_PROJ_DEV_CAPS_NODE) == 0)
        {
            m_GameProject.m_GameProjectConfig.m_DevCapFile              = m_GameProject.m_ProjectLocation + child.GetString(AE_PROJ_CONFIG_FILE_PROP);
        }
        else if (l_Type.compare(AE_PROJ_CONFIG_NODE) == 0)
        {
            m_GameProject.m_ProjectName                                 = child.GetString(AE_PROJ_CONFIG_NAME_PROP);
        }
        else if (l_Type.compare(AE_PROJ_GRAPHIC_OPTS_NODE) == 0)
        {
            m_GameProject.m_GameProjectConfig.m_GraphicOptsFile         = m_GameProject.m_ProjectLocation + child.GetString(AE_PROJ_CONFIG_FILE_PROP);
        }
        else if (l_Type.compare(AE_PROJ_INPUT_NODE) == 0)
        {
            m_GameProject.m_GameProjectConfig.m_InputFile               = m_GameProject.m_ProjectLocation + child.GetString(AE_PROJ_CONFIG_FILE_PROP);
        }
        else if (l_Type.compare(AE_PROJ_LOCALIZATION_NODE) == 0)
        {
            m_GameProject.m_GameProjectConfig.m_LocalizationFile        = m_GameProject.m_ProjectLocation + child.GetString(AE_PROJ_CONFIG_FILE_PROP);
        }
        else if (l_Type.compare(AE_PROJ_ASSET_MANAGER_NODE) == 0)
        {
            m_GameProject.m_GameProjectConfig.m_AssetManagerFile        = m_GameProject.m_ProjectLocation + child.GetString(AE_PROJ_CONFIG_FILE_PROP);
        }
        else if (l_Type.compare(AE_PROJ_GAME_OBJECT_MANAGER_NODE) == 0)
        {
            m_GameProject.m_GameProjectConfig.m_GameObjectManagerFile   = m_GameProject.m_ProjectLocation + child.GetString(AE_PROJ_CONFIG_FILE_PROP);
        }
    }

    return AEResult::Ok;
}

AEResult GameApp::ExtractGameConfigInput()
{
#ifdef AE_EDITOR_MODE

    //In Editor enable all modes
    m_GameConfigInput.m_KeyboardEnabled     = true;
    m_GameConfigInput.m_XBoxGamepadEnabled  = true;

    return AEResult::Ok;

#else //AE_EDITOR_MODE

    AEAssert(!m_GameProject.m_GameProjectConfig.m_InputFile.empty());

    AEXMLParser newFile;
    if (newFile.LoadFile(m_GameProject.m_GameProjectConfig.m_InputFile) != AEResult::Ok)
    {
        std::string msg_error = fmt::format(AELOCMAN.GetLiteral("INIT_COULDNT_READ_FILE_MSG"), __FUNCTION__, m_GameProject.m_GameProjectConfig.m_InputFile);

        AELOGGER.AddNewLog(LogLevel::Error, msg_error);
        return AEResult::OpenFileFail;
    }

    AEXMLParser inputXML = newFile["Input"];
    if (!inputXML.IsReady())
    {
        return AEResult::XMLReadError;
    }

    uint32_t l_Count = inputXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = inputXML(i);

        std::string l_Type = child.GetName();

        if (l_Type.compare("Keyboard") == 0)
        {
            m_GameConfigInput.m_KeyboardEnabled = true;
        }
        else if (l_Type.compare("XBoxGamepad") == 0)
        {
            m_GameConfigInput.m_XBoxGamepadEnabled = true;
        }
    }

    return AEResult::Ok;

#endif // !AE_EDITOR_MODE
}

AEResult GameApp::ExtractGameAppOpts()
{
    AEAssert(!m_GameProject.m_GameEngineConfig.m_GameOptsFile.empty());

    AEXMLParser newFile;
    if (newFile.LoadFile(m_GameProject.m_GameEngineConfig.m_GameOptsFile) != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "INIT_COULDNT_READ_FILE_MSG", __FUNCTION__, m_GameProject.m_GameEngineConfig.m_GameOptsFile);
        return AEResult::OpenFileFail;
    }

    AEXMLParser gameOptXML = newFile["GameOpts"];
    if (!gameOptXML.IsReady())
    {
        return AEResult::XMLReadError;
    }

    uint32_t l_Count = gameOptXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = gameOptXML(i);

        std::string l_Type = child.GetName();
            
        if (l_Type.compare("WindowsPosition") == 0)
        {
            m_GameAppOpts.m_InitialWinPos = child.GetVect2i("Pos");
        }
        else if(l_Type.compare("WinCaption") == 0)
        {
            m_GameAppOpts.m_WinCaption = child.GetString("WP");
        }
        else if(l_Type.compare("WinClassName") == 0)
        {
            m_GameAppOpts.m_WinClassName = child.GetString("WCN");
        }
        else if(l_Type.compare("SampleFPS") == 0)
        {
            m_GameAppOpts.m_SampleFPS = true;
            m_Timer.IsSamplingFPS(true);
        }
#ifdef AE_EDITOR_MODE
        else if(l_Type.compare("ShowConsole") == 0)
        {
            m_GameAppOpts.m_ShowConsole = true;
        }
#endif //AE_EDITOR_MODE
        else if(l_Type.compare("Log") == 0)
        {
            m_GameAppOpts.m_AELogLvl        = AELogHelpers::Str2LogLevel(child.GetString("Level", "NONE", false));
            m_GameAppOpts.m_LogCapacity     = child.GetUInt("Capacity", AE_LOG_CAPACITY, false);
            m_GameAppOpts.m_LogToFile       = child.GetBool("LogToFile", false, false);

            m_GameAppOpts.m_LogFilePath = child.GetString("LogFile", "", m_GameAppOpts.m_LogToFile);

            AETODO("Fix this for Project");
            //if (m_GameAppOpts.m_LogFilePath.empty())
            //{
                m_GameAppOpts.m_LogFilePath = m_GameProject.m_EngineLocation + AE_ENGINE_BIN_TO_DATA_PATH_ADD + AE_PROJ_LOG_FILE_LOC;
            //}
            //else
            //{
            //    m_GameAppOpts.m_LogFilePath = m_GameProject.m_ProjectLocation + "/" + m_GameAppOpts.m_LogFilePath;
            //}
        }
    }

    return AEResult::Ok;
}

AEResult GameApp::ExtractGraphicOpts()
{
#ifdef AE_EDITOR_MODE

    //In Editor set defaults
    m_GraphicOptsPreferred.m_ScreenResolution.x             = 1024;
    m_GraphicOptsPreferred.m_ScreenResolution.y             = 768;
    m_GraphicOptsPreferred.m_FullScreen                     = false;
    m_GraphicOptsPreferred.m_BackBufferFormatWindowed       = "DXGI_FORMAT_R8G8B8A8_UNORM";
    m_GraphicOptsPreferred.m_BackBufferFormatFullScreen     = "DXGI_FORMAT_R8G8B8A8_UNORM";
    m_GraphicOptsPreferred.m_SingleThreaded                 = true;

    return AEResult::Ok;

#else //AE_EDITOR_MODE

    AEAssert(!m_GameProject.m_GameProjectConfig.m_GraphicOptsFile.empty());

    AEXMLParser newFile;
    if (newFile.LoadFile(m_GameProject.m_GameProjectConfig.m_GraphicOptsFile) != AEResult::Ok)
    {
        std::string msg_error = fmt::format(AELOCMAN.GetLiteral("INIT_COULDNT_READ_FILE_MSG"), __FUNCTION__, m_GameProject.m_GameProjectConfig.m_GraphicOptsFile);
        
        AELOGGER.AddNewLog(LogLevel::Error, msg_error);
        return AEResult::OpenFileFail;
    }

    AEXMLParser graphicOptXML = newFile["GraphicOpts"];
    if (!graphicOptXML.IsReady())
    {
        return AEResult::XMLReadError;
    }

    uint32_t l_Count = graphicOptXML.GetNumChildren();
    for (uint32_t i = 0; i < l_Count; ++i)
    {
        AEXMLParser child = graphicOptXML(i);
        std::string l_Type = child.GetName();

        if( l_Type.compare("ScreenResolution") == 0 )
        {
            m_GraphicOptsPreferred.m_ScreenResolution.x             = child.GetInt("Width", 1024);
            m_GraphicOptsPreferred.m_ScreenResolution.y             = child.GetInt("Height", 768);
        }
        else if( l_Type.compare("RenderMode") == 0 )
        {
            m_GraphicOptsPreferred.m_FullScreen                     = child.GetBool("FullScreenMode");
            m_GraphicOptsPreferred.m_BackBufferFormatWindowed       = child.GetString("WindowFormat");
            m_GraphicOptsPreferred.m_BackBufferFormatFullScreen     = child.GetString("FullScreenFormat");
        }
        else if( l_Type.compare("SingleThreaded") == 0)
        {
            m_GraphicOptsPreferred.m_SingleThreaded                 = true;
        }
    }

    return AEResult::Ok;

#endif //!AE_EDITOR_MODE
}

AEResult GameApp::InitProject(const std::string& configProjFile, std::string& errorMsg)
{
    errorMsg = "";

    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(!configProjFile.empty());
    if (configProjFile.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_GameProject.m_ProjectConfigFile = configProjFile;

    if (ExtractGameProjectConfig() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "PROJECT_CONFIG_ERR_MSG");

        return AEResult::Fail;
    }

    AEAssert(!m_GameProject.m_GameProjectConfig.m_LocalizationFile.empty());
    if (AELOCMAN.LoadProjectFile(m_GameProject.m_GameProjectConfig.m_LocalizationFile, m_GameProject.m_ProjectLocation) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    //////////////////////////////////
    //Load Project
    if (LoadGameProjectInfo() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "AE_GAME_APP_LOAD_PROJ_FAIL_ERR_MSG");
        return AEResult::Fail;
    }

    //////////////////////////////////
    //Finish
    AELogHelpers::Log(LogLevel::Info, LogSystem::Core, "AE_GAME_APP_LOAD_PROJ_INFO_MSG");
    return AEResult::Ok;
}

AEResult GameApp::InitGameApp(const std::string& configEngineFile, std::string& errorMsg)
{
    errorMsg = "";

    if(m_IsReady)
    {
        return AEResult::Ok;
    }

    AEAssert(!configEngineFile.empty());
    if (configEngineFile.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_Quiting = false;
    m_StartShutdown = false;

    //Create Logger and Localization Manager
    AELOGGER;
    AELOCMAN;
    GameCommandManager::GetInstance();

    m_GameProject.m_EngineConfigFile = configEngineFile;

    if (ExtractGameEngineConfig() != AEResult::Ok)
    {
        MessageBox(m_GraphicDevice->GetGraphicPP().m_DeviceWindow, "Unable to extract engine configuration.", 0, 0);
        return AEResult::Fail;
    }

    if (InitLocalizationManager() != AEResult::Ok)
    {
        MessageBox(m_GraphicDevice->GetGraphicPP().m_DeviceWindow, "Localization Parameters could not be initialize.", 0, 0);
        return AEResult::Fail;
    }

    if (ExtractGameAppOpts() != AEResult::Ok)
    {
        std::string errMsg = AELOCMAN.GetLiteral("EXTRACT_GAME_OPTS_ERR_MSG");

        MessageBox(m_GraphicDevice->GetGraphicPP().m_DeviceWindow, errMsg.c_str(), 0, 0);

        return AEResult::Fail;
    }

    if (InitLogger() != AEResult::Ok)
    {
        std::string errMsg = AELOCMAN.GetLiteral("LOGGER_INIT_ERR_MSG");

        MessageBox(m_GraphicDevice->GetGraphicPP().m_DeviceWindow, errMsg.c_str(), 0, 0);

        return AEResult::Fail;
    }

#ifdef AE_EDITOR_MODE
    if (m_GameAppOpts.m_ShowConsole)
    {
        OpenConsole();
    }
#endif //AE_EDITOR_MODE

    if (ExtractGraphicOpts() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "EXTRACT_GRAPHIC_OPTS_ERR_MSG");
        return AEResult::Fail;
    }

    if (InitMainWindow() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "INIT_MAIN_WINDOW_ERR_MSG");
        return AEResult::Fail;
    }

    if (Init3D_Device() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "INIT_3D_DEVICE_ERR_MSG");
        return AEResult::Fail;
    }

    if (InitInputManager() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "INIT_INPUT_DEVICES_FAIL_ERR_MSG");
        return AEResult::Fail;
    }

    if (InitPhysicsManager() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "PHYSICS_ENGINE_INIT_ERR_MSG");
        return AEResult::Fail;
    }

    if (InitScriptingEngine() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "SCRIPT_ENGINE_INIT_FAIL_ERR_MSG");
        return AEResult::Fail;
    }

    ////////////////////////////////////////////////
    //Create Camera Manager
    m_CameraManager = new CameraManager();

    ////////////////////////////////////////////////
    //Create Light Manager
    m_LightManager = new LightManager(*m_GraphicDevice);

    ////////////////////////////////////////////////
    //Create Audio Manager
    m_AudioManager = new AudioManager();

    ////////////////////////////////////////////////
    //Create Game Resource Manager
    m_GameResourceManager = new GameResourceManager();

    ////////////////////////////////////////////////
    //Create Game Object Script Manager
    m_GameObjectScriptManager = new GameObjectScriptManager();

    //Initialize Game Component Collection & Game Services Collection
    m_GameComponentCollection = new GameComponentCollection();
    m_GameServiceCollection = new GameServiceCollection();
    
    //Register Data that the Angel Script will have available
    if (RegisterScriptData() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "SCRIPT_ENGINE_REGISTER_DATA_ERR_MSG");
        return AEResult::Fail;
    }

    ////////////////////////////////////////////////
    // Create and Initialize Game Asset Manager
    m_GameAssetManager = new GameAssetManager(*m_GraphicDevice, *m_GameResourceManager, *m_AngelScriptManager, *m_AudioManager, m_GameProject.m_ProjectLocation, m_GameProject.m_ProjectLocation + "/" + AE_PROJ_ASSETS_DIR_LOC);

    if(m_GameAssetManager->Initialize() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "ASSET_MANAGER_INIT_ERR_MSG");
        return AEResult::Fail;
    }

    ////////////////////////////////////////////////
    //Create Game Object Manager
    m_GameObjectManager = new GameObjectManager(*m_GraphicDevice, *m_GameAssetManager, *m_GameObjectScriptManager, *m_AngelScriptManager, *m_LightManager, *m_CameraManager, *m_AudioManager, *m_PhysicsManager);

#ifdef AE_EDITOR_MODE

    ////////////////////////////////////////////////
    //Create ImGui Manager
    m_ImGuiManager = new ImGuiManager(*m_GraphicDevice);
    if (m_ImGuiManager->Initialize() != AEResult::Ok)
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "IMGUI_INIT_ERR_MSG");
        return AEResult::Fail;
    }

#endif

    //////////////////////////////////
    //Game is ready to run
    m_IsReady = true;

    //////////////////////////////////
    //Log that Engine is ready to run
    AELogHelpers::Log(LogLevel::Info, LogSystem::Core, "AE_GAME_APP_READY_INFO_MSG");

    //////////////////////////////////
    //Finish
    return AEResult::Ok;
}

AEResult GameApp::InitMainWindow()
{
    WNDCLASS wc;
    wc.style            = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc      = MainWndProc; 
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = m_AppInst;
    wc.hIcon            = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName     = 0;
    wc.lpszClassName    = m_GameAppOpts.m_WinClassName.c_str();

    if( !RegisterClass(&wc) )
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "REGISTER_CLASS_FAIL_MSG", __FUNCTION__);

        PostQuitMessage(0);
        return AEResult::Fail;
    }

    // Default to a window with a client area rectangle
    RECT R = {0, 0, m_GraphicOptsPreferred.m_ScreenResolution.x, m_GraphicOptsPreferred.m_ScreenResolution.y};

    LONG wsOpt = 0;

    if(m_GraphicOptsPreferred.m_FullScreen)
    {
        wsOpt = WS_POPUP;
    }
    else
    {
        wsOpt = WS_OVERLAPPEDWINDOW;
    }

    AdjustWindowRect(&R, wsOpt, false);
        
    m_MainWnd = CreateWindow(   m_GameAppOpts.m_WinClassName.c_str(), 
                                m_GameAppOpts.m_WinCaption.c_str(), 
                                wsOpt, 
                                m_GameAppOpts.m_InitialWinPos.x, 
                                m_GameAppOpts.m_InitialWinPos.y, 
                                m_GraphicOptsPreferred.m_ScreenResolution.x,
                                m_GraphicOptsPreferred.m_ScreenResolution.y, 
                                nullptr, 
                                nullptr,
                                m_AppInst, 
                                nullptr); 

    if( !m_MainWnd )
    {
        AELogHelpers::Log(LogLevel::Error, LogSystem::Core, "CREATE_WINDOW_FAIL_MSG", __FUNCTION__);

        PostQuitMessage(0);
        return AEResult::Fail;
    }

    SetWindowLongPtr(m_MainWnd, GWLP_USERDATA, (LONG_PTR)this);

    ShowWindow(m_MainWnd, SW_SHOW);
    UpdateWindow(m_MainWnd);

    return AEResult::Ok;
}

AEResult GameApp::Init3D_Device()
{
    //Create Graphics Device
    m_GraphicDevice = new GraphicDevice(m_MainWnd, m_GraphicOptsPreferred);

    AETODO("Examine Check Caps");
    //Check for Graphic Device Capabilities Support
    //if(m_GraphicDevice->CheckDevCaps(m_GameConfig.m_DevCapFile) != AEResult::Ok)
    //{
    //    std::string msgerr = fmt::format(m_LocalizationManager->GetLiteral("CHECK_DEV_CAPS_ERR_MSG"), __FUNCTION__);

    //    m_Logger->AddNewLog(LogLevel::Error, msgerr);
    //    return AEResult::Fail;
    //}

    //Initialize Device
    if(m_GraphicDevice->InitDevice() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

    m_GraphicDebugDX = new GraphicDebugDX(m_GraphicDevice);

    if(m_GraphicDebugDX->Initialize() != AEResult::Ok)
    {
        AETODO("Add log here");
    }

#endif

    return AEResult::Ok;
}

AEResult GameApp::InitLocalizationManager()
{
    AEAssert(!m_GameProject.m_GameEngineConfig.m_LocalizationFile.empty());
    if (AELOCMAN.Initialize(m_GameProject.m_GameEngineConfig.m_LocalizationFile, m_GameProject.m_EngineLocation + AE_ENGINE_BIN_TO_DATA_PATH_ADD) != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameApp::InitInputManager()
{
    if(ExtractGameConfigInput() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    m_InputManager = new InputManager();

    return m_InputManager->Initialize(m_GameConfigInput);
}

AEResult GameApp::InitPhysicsManager()
{
    //Initialize Physics Manager
    m_PhysicsManager = new PhysicsManager();

    return m_PhysicsManager->Initialize();
}

AEResult GameApp::InitScriptingEngine()
{
    m_AngelScriptManager = new AngelScriptManager();

    if(m_AngelScriptManager->Initialize() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameApp::InitLogger()
{
    AELOGGER.SetCapacity(m_GameAppOpts.m_LogCapacity);

    AELOGGER.SetLogLevel(m_GameAppOpts.m_AELogLvl);

    if (AELOGGER.SetLogFilename(m_GameAppOpts.m_LogFilePath) != AEResult::Ok)
    {
        return AEResult::InitLoggerFail;
    }

    if(m_GameAppOpts.m_LogToFile)
    {
        if (AELOGGER.ActivateLogToFile() != AEResult::Ok)
        {
            return AEResult::InitLoggerFail;
        }
    }

    return AEResult::Ok;
}

AEResult GameApp::RegisterScriptData()
{
    /////////////////////////////////////
    //Register Add-ons

    //GLM Add-on
    GLMAddOnAS glmAddOn;
    if (m_AngelScriptManager->RegisterAddOn(glmAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Base Add-on
    BaseAddOnAS baseAddOn;
    if (m_AngelScriptManager->RegisterAddOn(baseAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Color Add-on
    ColorAddOnAS colorAddOn;
    if (m_AngelScriptManager->RegisterAddOn(colorAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Time Add-on
    TimeAddOnAS timeAddOn;
    if (m_AngelScriptManager->RegisterAddOn(timeAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Log Add-on
    LogAddOnAS logAddOn;
    if (m_AngelScriptManager->RegisterAddOn(logAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Localization Add-on
    LocalizationAddOnAS localizationAddOn;
    if (m_AngelScriptManager->RegisterAddOn(localizationAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Input Add-on
    InputAddOnAS inputAddOn(m_InputManager);
    if (m_AngelScriptManager->RegisterAddOn(inputAddOn) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Audio Add-on
    AudioAddOnAS audioAddOnAS;
    if (m_AngelScriptManager->RegisterAddOn(audioAddOnAS) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    // Game Core Add-on
    GameCoreAddOnAS gameCoreAddOnAS(m_GameComponentCollection, m_GameObjectScriptManager);
    if (m_AngelScriptManager->RegisterAddOn(gameCoreAddOnAS) != AEResult::Ok)
    {
        AETODO("Add log");
        return AEResult::Fail;
    }

    return AEResult::Ok;
}

void GameApp::OnResize(uint32_t width, uint32_t heigth)
{
    if(!m_IsReady)
    {
        return;
    }

    if(width == 0 || heigth == 0)
    {
        return;
    }

    glm::ivec2 newSize = { width, heigth };

    ResizeCommand* rc = new ResizeCommand(*this, newSize);

    GameCommandManager::GetInstance().AddCommand(rc);
}

AEResult GameApp::SetFullScreen(bool fullScreenEnable)
{
    GraphicsPresentationParameters* gpp = &m_GraphicDevice->GetGraphicPP();
    
    if(fullScreenEnable)
    {
        if(!gpp->m_Windowed)
        {
            return AEResult::Ok;
        }
        
        //Set Windowed to false
        gpp->m_Windowed = false;

        // Change the window style to a more fullscreen friendly style.
        SetWindowLongPtr(m_MainWnd, GWL_STYLE, WS_POPUP);

        // If we call SetWindowLongPtr, MSDN states that we need to call
        // SetWindowPos for the change to take effect.  In addition, we 
        // need to call this function anyway to update the window dimensions.
        SetWindowPos(m_MainWnd, HWND_TOP, 0, 0, 
                    m_GraphicOptsPreferred.m_ScreenResolution.x, 
                    m_GraphicOptsPreferred.m_ScreenResolution.y, 
                    SWP_NOZORDER | SWP_SHOWWINDOW);    
        
        gpp->m_BackBufferWidth = m_GraphicOptsPreferred.m_ScreenResolution.x;
        gpp->m_BackBufferHeight = m_GraphicOptsPreferred.m_ScreenResolution.y;
    }
    else
    {
        if(gpp->m_Windowed)
        {
            return AEResult::Ok;
        }
        
        //Set Windowed to true
        gpp->m_Windowed = true;
        
        RECT R = {0, 0, m_GraphicOptsPreferred.m_ScreenResolution.x, m_GraphicOptsPreferred.m_ScreenResolution.y};
        AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

        // Change the window style to a more windowed friendly style.
        SetWindowLongPtr(m_MainWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

        // If we call SetWindowLongPtr, MSDN states that we need to call
        // SetWindowPos for the change to take effect.  In addition, we 
        // need to call this function anyway to update the window dimensions.
        SetWindowPos(m_MainWnd, HWND_TOP, m_GameAppOpts.m_InitialWinPos.x, m_GameAppOpts.m_InitialWinPos.y, R.right, R.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);

        glm::ivec2 size = AEGraphicHelpers::GetWindowSize(m_MainWnd);

        gpp->m_BackBufferWidth = size.x;
        gpp->m_BackBufferHeight = size.y;
    }

    //Signal Everything that we need to reset device
    OnLostDevice();
    
    if(m_GraphicDevice->ResetDevice() != AEResult::Ok)
    {
        return AEResult::Fail;
    }

    OnResetDevice();
    
    return AEResult::Ok;
}

void GameApp::OnLostDevice()
{
    m_GameComponentCollection->OnLostDeviceCollection();
}

void GameApp::OnResetDevice()
{
    m_GameComponentCollection->OnResetDeviceCollection();
}

AEResult GameApp::AddComponent(GameComponent* gc)
{
    if(!m_IsReady)
    {
        return AEResult::Fail;
    }

    if(m_GameComponentCollection->Add(gc) != AEResult::Ok)
    {
        AETODO("Change to Literal");
        AEAssert(!"Fail to add Game Component, game component already in collection");

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameApp::RemoveComponent(GameComponent* gc)
{
    return m_GameComponentCollection->Remove(gc);
}

AEResult GameApp::RegisterGameService(const std::string& serviceName, GameService* gs)
{
    if(!m_IsReady)
    {
        return AEResult::Fail;
    }

    if(m_GameServiceCollection->Add(serviceName, gs) != AEResult::Ok)
    {
        AETODO("Change to Literal");
        AEAssert(!"Fail to Register Game Service, game service name already registered");

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult GameApp::UnRegisterGameService(const std::string& serviceName)
{
    if(!m_IsReady)
    {
        return AEResult::Fail;
    }

    return m_GameServiceCollection->Remove(serviceName);
}

GameService* GameApp::GetGameServiceBase(const std::string& serviceName) const
{
    if(!m_IsReady)
    {
        return nullptr;
    }

    return m_GameServiceCollection->GetGameService(serviceName);
}

void GameApp::PreRender()
{
    m_GraphicDevice->Clear();
}

void GameApp::PostRender()
{
    m_GraphicDevice->Present();
}

int GameApp::Run()
{
    if(!m_IsReady)
    {
        return -1;
    }

    m_IsEngineOff = false;

    MSG  msg;
    msg.message = WM_NULL;

    m_Timer.Update();

    //Initialize Game and Load Content
    InternalInitialize();
    InternalLoadContent();

    while(msg.message != WM_QUIT && !m_StartShutdown)
    {
        // If there are Window messages then process them.
        if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE | PM_NOYIELD))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // Otherwise, do animation/game stuff.
        else
        {
            // If the application is inactive then free some CPU cycles to other 
            // applications and then continue on to the next frame.
            if(m_AppInactive)
            {
                AETODO("If application is inactive, update needs to run but we can skip render");
                AETODO("REMOVE THIS!!!")
            }

            //Update Timer
            m_Timer.Update();

            //Constant Update
            InternalConstantUpdate();

            //Frame Timer
            const TimerParams& frameTimerParams = m_Timer.GetFrameTimerParams();

            //Update Game
            InternalUpdate(frameTimerParams);

            //A Post Update, so stuff like cameras can be updated after all the scene has
            InternalPostUpdate(frameTimerParams);

            //Render Game
            InternalRender(frameTimerParams);

            //Run Game Commands
            GameCommandManager::GetInstance().ExecuteCommands();
        }
    }

    m_Quiting = true;

    UnLoadContent();

    m_IsEngineOff = true;

    return (int)msg.wParam;
}

void GameApp::InternalInitialize()
{
    //Game App Initialize
    Initialize();

    //Game Component Initialize
    m_GameComponentCollection->InitializeCollection();
}

void GameApp::InternalLoadContent()
{
    //Game App LoadContent
    LoadContent();

    //Game Component LoadContent
    m_GameComponentCollection->LoadContentCollection();
}

void GameApp::InternalUnLoadContent()
{
    //Game App UnLoadContent
    UnLoadContent();

    //Game Component UnLoadContent
    m_GameComponentCollection->UnLoadContentCollection();
}

void GameApp::InternalRender(const TimerParams& timerParams)
{
    //Pre Render Commands
    PreRender();

    //Game App Render
    Render(timerParams);

    //Game Component Render
    m_GameComponentCollection->RenderCollection(timerParams);

    //Post Render Commands
    PostRender();
}

void GameApp::InternalConstantUpdate()
{
    const TimerParams& constUpdateTimerParams = m_Timer.GetConstantUpdateTimerParams();
    while (m_Timer.NeedToRunConstantUpdate())
    {
        //Game App Constant Update
        ConstantUpdate(constUpdateTimerParams);

        //Game Component Constant Update
        m_GameComponentCollection->ConstantUpdateCollection(constUpdateTimerParams);
    }
}

void GameApp::InternalUpdate(const TimerParams& timerParams)
{
    //Game App Update
    Update(timerParams);

    //Game Component Update
    m_GameComponentCollection->UpdateCollection(timerParams);
}

void GameApp::InternalPostUpdate(const TimerParams& timerParams)
{
    //Game App Update
    PostUpdate(timerParams);

    //Game Component Update
    m_GameComponentCollection->PostUpdateCollection(timerParams);
}

void GameApp::InternalOnLostDevice()
{
    //Game App Update
    OnLostDevice();

    //Game Component Update
    m_GameComponentCollection->OnLostDeviceCollection();
}

void GameApp::InternalOnResetDevice()
{
    //Game App Update
    OnResetDevice();

    //Game Component Update
    m_GameComponentCollection->OnResetDeviceCollection();
}

AEResult GameApp::SaveGameInfo()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEResult ret = AEResult::Ok;

    ret = AELOCMAN.SaveToXML(m_GameProject.m_GameProjectConfig.m_LocalizationFile, m_GameProject.m_ProjectLocation);
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");
        return ret;
    }

    ret = m_GameAssetManager->SaveToXML(m_GameProject.m_GameProjectConfig.m_AssetManagerFile);
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");
        return ret;
    }

    ret = m_GameObjectManager->SaveToXML(m_GameProject.m_GameProjectConfig.m_GameObjectManagerFile);
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");
        return ret;
    }

    return AEResult::Ok;
}

AEResult GameApp::LoadGameProjectInfo()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEResult ret = AEResult::Ok;

    ret = AELOCMAN.LoadProjectFile(m_GameProject.m_GameProjectConfig.m_LocalizationFile, m_GameProject.m_ProjectLocation);
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");
        return ret;
    }

    ret = m_GameAssetManager->LoadAssetManagerFile(m_GameProject.m_GameProjectConfig.m_AssetManagerFile);
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");
        return ret;
    }

    ret = m_GameObjectManager->LoadGameObjectManagerFile(m_GameProject.m_GameProjectConfig.m_GameObjectManagerFile);
    if (ret != AEResult::Ok)
    {
        AETODO("Log error");
        return ret;
    }

    return AEResult::Ok;
}

AEResult GameApp::CreateProjectFolder(const std::string& projectFolder, const std::string& projectName, bool createFolder)
{
    //////////////////////////////////////
    //Pre-checks
    if (projectFolder.empty() || projectName.empty())
    {
        return AEResult::EmptyString;
    }

    //////////////////////////////////////
    //Create Project Folder
    if (createFolder)
    {
        if (!boost::filesystem::create_directory(projectFolder))
        {
            AETODO("Better return code");
            return AEResult::Fail;
        }
    }

    //////////////////////////////////////
    //Create Config Project File
    std::ofstream configFile(projectFolder + "/" + AE_PROJ_CONFIG_PROJ_FILE);
    if (!configFile.is_open())
    {
        return AEResult::OpenFileFail;
    }

    configFile << XEGameAppHelpers::BuildConfigProjectFile(projectName);
    configFile.close();

    //////////////////////////////////////
    //Create Sub-folders
    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_DATA_DIR))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_ASSETS_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_ASSET_MANAGER_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_GLOBAL_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_INPUT_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_GAME_OBJECTS_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_LOCALIZATION_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (!boost::filesystem::create_directory(projectFolder + "/" + AE_PROJ_LOG_DIR_LOC))
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    //////////////////////////////////////
    //Create XML Files

    //Asset Manager File
    std::ofstream assetManagerFile(projectFolder + "/" + AE_PROJ_ASSET_MANAGER_FILE_LOC);
    if (!assetManagerFile.is_open())
    {
        return AEResult::OpenFileFail;
    }
    assetManagerFile << XEGameAppHelpers::BuildAssetFile();
    assetManagerFile.close();

    //Localization File
    std::ofstream projectLocalizationFile(projectFolder + "/" + AE_PROJ_LOCALIZATION_PROJ_FILE_LOC);
    if (!projectLocalizationFile.is_open())
    {
        return AEResult::OpenFileFail;
    }
    projectLocalizationFile << XEGameAppHelpers::BuildLocalizationFile();
    projectLocalizationFile.close();

    //Game Object Manager File
    std::ofstream gameObjectManagerFile(projectFolder + "/" + AE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC);
    if (!gameObjectManagerFile.is_open())
    {
        return AEResult::OpenFileFail;
    }
    gameObjectManagerFile << XEGameAppHelpers::BuildGameObjectManagerFile();
    gameObjectManagerFile.close();

    //////////////////////////////////////
    //Finish
    return AEResult::Ok;
}

LRESULT GameApp::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
    RECT clientRect = {0, 0, 0, 0};
    uint32_t width  = 0;
    uint32_t height = 0;

    if (msg >= WM_KEYFIRST && msg <= WM_KEYLAST)
    {
        if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN || msg == WM_KEYUP || msg == WM_SYSKEYUP)
        {
            TCHAR ch = (TCHAR)wParam;

            //Lets us know if the char is extended, use to distinguish between LCTRL & RCTRL...
            //More in http://msdn.microsoft.com/en-us/library/windows/desktop/ms646280(v=vs.85).aspx
            bool isExtended = (lParam & (1 << 24)) != 0; 

            //Tell how to process key, if up or down
            bool isDown = false;

            //If the WM tells us it is down, then set to true
            if(msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN)
            {
                isDown = true;
            }

            m_InputManager->ProcessKeyboardWindows(ch, isExtended, isDown);
        }
    }

    switch( msg )
    {
        // WM_ACTIVE is sent when the window is activated or deactivated.
        // We pause the game when the main window is deactivated and 
        // unpause it when it becomes active.
        case WM_ACTIVATE:
            if( LOWORD(wParam) == WA_INACTIVE )
            {
                m_AppInactive = true;
            }
            else
            {
                m_AppInactive = false;
            }

            return 0;

        // WM_SIZE is sent when the user resizes the window.  
        case WM_SIZE:

            width  = LOWORD(lParam);
            height = HIWORD(lParam);

            if( wParam == SIZE_MINIMIZED )
            {
                m_AppInactive = true;
                m_Minimized = true;
                m_Maximized = false;
            }
            else if( wParam == SIZE_MAXIMIZED )
            {
                m_AppInactive = false;
                m_Minimized = false;
                m_Maximized = true;

                OnResize(width, height);
            }
            // Restored is any resize that is not a minimize or maximize.
            // For example, restoring the window to its default size
            // after a minimize or maximize, or from dragging the resize
            // bars.
            else if( wParam == SIZE_RESTORED )
            {
                // Restoring from minimized state?
                if(m_Minimized)
                {
                    m_AppInactive = false;
                    m_Minimized = false;
                    OnResize(width, height);
                }
                // Restoring from maximized state?
                else if(m_Maximized)
                {
                    m_AppInactive = false;
                    m_Maximized = false;
                    OnResize(width, height);
                }
                else if(m_Resizing)
                {
                    // If user is dragging the resize bars, we do not resize 
                    // the buffers here because as the user continuously 
                    // drags the resize bars, a stream of WM_SIZE messages are
                    // sent to the window, and it would be pointless (and slow)
                    // to resize for each WM_SIZE message received from dragging
                    // the resize bars.  So instead, we reset after the user is 
                    // done resizing the window and releases the resize bars, which 
                    // sends a WM_EXITSIZEMOVE message.
                }
                else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
                {
                    OnResize(width, height);
                }
            }

            return 0;

        case WM_ENTERSIZEMOVE:
            m_AppInactive = true;
            m_Resizing  = true;
            return 0;

        // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
        // Here we reset everything based on the new window dimensions.
        case WM_EXITSIZEMOVE:
            GetClientRect(m_MainWnd, &clientRect);

            width  = (uint32_t)clientRect.right;
            height = (uint32_t)clientRect.bottom;

            m_AppInactive = false;
            m_Resizing  = false;

            OnResize(width, height);

            return 0;

        // WM_CLOSE is sent when the user presses the 'X' button in the
        // caption bar menu.
        case WM_CLOSE:
            DestroyWindow(m_MainWnd);
            return 0;

        // WM_DESTROY is sent when the window is being destroyed.
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        // WM_INPUTLANGCHANGEREQUEST is sent when a change in language is requested
        // by the user
        case WM_INPUTLANGCHANGE:
            m_InputManager->ReloadKeyboardLaylout();
            
            //No return to pass change to DefWindowProc and accept the change
            break;

        case WM_KILLFOCUS:
            m_InputManager->LoseFocus();

            //No return to pass change to DefWindowProc and accept the change
            break;
    }

    return DefWindowProc(m_MainWnd, msg, wParam, lParam);
}

#ifdef AE_EDITOR_MODE

AEResult GameApp::EditorPlay()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_GameEditorPlayState == GameEditorPlayState::Playing)
    {
        return AEResult::Ok;
    }

    AEResult ret = AEResult::Ok;

    ret = m_CameraManager->SetDefaultCameraAsMain();

    m_GameEditorPlayState = GameEditorPlayState::Playing;

    return ret;
}

AEResult GameApp::EditorPause()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_GameEditorPlayState == GameEditorPlayState::Paused || m_GameEditorPlayState == GameEditorPlayState::Stop)
    {
        return AEResult::Ok;
    }

    AEResult ret = AEResult::Ok;

    m_GameEditorPlayState = GameEditorPlayState::Paused;

    return ret;
}

AEResult GameApp::EditorStop()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if (m_GameEditorPlayState == GameEditorPlayState::Stop)
    {
        return AEResult::Ok;
    }

    AEResult ret = AEResult::Ok;

    ret = m_CameraManager->SetEditorCameraAsMain();

    m_GameEditorPlayState = GameEditorPlayState::Stop;

    return ret;
}

void GameApp::OpenConsole()
{
    if (AllocConsole() != TRUE)
    {
        AETODO("ADD LOG MESSAGE");
        return;
    }

    HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((intptr_t)handle_out, _O_TEXT);
    FILE* hf_out = _fdopen(hCrt, "w");
    setvbuf(hf_out, nullptr, _IONBF, 1);
    *stdout = *hf_out;

    HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
    hCrt = _open_osfhandle((intptr_t)handle_in, _O_TEXT);
    FILE* hf_in = _fdopen(hCrt, "r");
    setvbuf(hf_in, nullptr, _IONBF, 128);
    *stdin = *hf_in;
}

#endif //AE_EDITOR MODE

/////////////////////////////////
//Static Variables and Methods//
////////////////////////////////
LRESULT CALLBACK  GameApp::MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    GameApp* gameApp = (GameApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if(gameApp != nullptr)
    {
        return gameApp->MsgProc(msg, wParam, lParam);
    }
    else
    {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
