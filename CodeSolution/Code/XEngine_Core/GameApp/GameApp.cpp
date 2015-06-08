/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "GameApp.h"
#include "Keyboard.h"
#include "InputDefs.h"
#include "AudioManager.h"
#include "InputManager.h"
#include "GraphicsDefs.h"
#include "GraphicDevice.h"
#include "Logger\Logger.h"
#include "AudioListener.h"
#include "PhysicsManager.h"
#include "XML\XEXMLParser.h"
#include "Time\XETimeDefs.h"
#include "Logger\LoggerDefs.h"
#include "Base\BaseLocations.h"
#include "Base\BaseFunctions.h"
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
#include "GameAssets\GameAssetManager.h"
#include "GameASAddOns\GameCoreAddOnAS.h"
#include "GameObject\GameObjectManager.h"
#include "Resource\GameResourceManager.h"
#include "AngelScript\AngelScriptManager.h"
#include "GameUtils\GameServiceCollection.h"
#include "GameASAddOns\LocalizationAddOnAS.h"
#include "Localization\LocalizationManager.h"
#include "GameUtils\GameComponentCollection.h"
#include "GameObject\Scripts\GameObjectScriptManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameApp::GameApp(HINSTANCE hInstance, GameAppRunOpt gameAppRunOpt)
	: m_AppInst(hInstance)
	, m_GameAppRunOpt(gameAppRunOpt)
{
	#if defined(_DEBUG)
		MemLeaks::MemoryBegin();
	#endif
}

GameApp::~GameApp()
{

	#if defined(_DEBUG)
		MemLeaks::MemoryEnd();
	#endif
}

void GameApp::CleanUp()
{
	XETODO("Check that deletions happen in reverse order of creation");
	//////////////////////////////////////////////////
	// Order of deletion must be maintain
	//
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
	//

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

#if defined(_DEBUG) || defined(PROFILE)

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

	//Delete Logger and Localization Manager instance
	Logger::DestroyInstance();
	AudioListener::DestroyInstance();
	LocalizationManager::DestroyInstance();
}

void GameApp::ShutDownGameApp()
{
	m_IsReady = false;
	m_StartShutdown = true;

	while (!m_IsEngineOff)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(500));
	}

	CleanUp();
}

XEResult GameApp::ExtractGameEngineConfig()
{
	XEAssert(!m_GameProject.m_EngineConfigFile.empty());

	XEXMLParser newFile;
	if (newFile.LoadFile(m_GameProject.m_EngineConfigFile) != XEResult::Ok)
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, m_GameProject.m_EngineConfigFile);
		
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser configXML = newFile[XE_ENGINE_CONFIG_MAIN_NODE];
	if (!configXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	m_GameProject.m_EngineLocation = boost::filesystem::current_path().wstring();

	uint32_t l_Count = configXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = configXML(i);

		std::wstring l_Type = child.GetName();
		
		if (l_Type.compare(XE_ENGINE_LOCALIZATION_NODE) == 0)
		{
			m_GameProject.m_GameEngineConfig.m_LocalizationFile		= m_GameProject.m_EngineLocation + XE_ENGINE_BIN_TO_DATA_PATH_ADD + child.GetString(XE_ENGINE_CONFIG_FILE_PROP);
		}
		else if (l_Type.compare(XE_ENGINE_GAMEOPTS) == 0)
		{
			m_GameProject.m_GameEngineConfig.m_GameOptsFile			= m_GameProject.m_EngineLocation + XE_ENGINE_BIN_TO_DATA_PATH_ADD + child.GetString(XE_ENGINE_CONFIG_FILE_PROP);
		}
	}

	return XEResult::Ok;
}

XEResult GameApp::ExtractGameProjectConfig()
{
	XEAssert(!m_GameProject.m_ProjectConfigFile.empty());

	XEXMLParser newFile;
	if (newFile.LoadFile(m_GameProject.m_ProjectConfigFile) != XEResult::Ok)
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, m_GameProject.m_ProjectConfigFile);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser configXML = newFile[XE_PROJ_CONFIG_MAIN_NODE];
	if (!configXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	std::size_t foundPos = m_GameProject.m_ProjectConfigFile.find(XE_PROJ_CONFIG_PROJ_FILE);
	m_GameProject.m_ProjectLocation = m_GameProject.m_ProjectConfigFile.erase(foundPos);

	uint32_t l_Count = configXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = configXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(XE_PROJ_DEV_CAPS_NODE) == 0)
		{
			m_GameProject.m_GameProjectConfig.m_DevCapFile				= m_GameProject.m_ProjectLocation + child.GetString(XE_PROJ_CONFIG_FILE_PROP);
		}
		else if (l_Type.compare(XE_PROJ_CONFIG_NODE) == 0)
		{
			m_GameProject.m_ProjectName									= child.GetString(XE_PROJ_CONFIG_NAME_PROP);
		}
		else if (l_Type.compare(XE_PROJ_GRAPHIC_OPTS_NODE) == 0)
		{
			m_GameProject.m_GameProjectConfig.m_GraphicOptsFile			= m_GameProject.m_ProjectLocation + child.GetString(XE_PROJ_CONFIG_FILE_PROP);
		}
		else if (l_Type.compare(XE_PROJ_INPUT_NODE) == 0)
		{
			m_GameProject.m_GameProjectConfig.m_InputFile				= m_GameProject.m_ProjectLocation + child.GetString(XE_PROJ_CONFIG_FILE_PROP);
		}
		else if (l_Type.compare(XE_PROJ_LOCALIZATION_NODE) == 0)
		{
			m_GameProject.m_GameProjectConfig.m_LocalizationFile		= m_GameProject.m_ProjectLocation + child.GetString(XE_PROJ_CONFIG_FILE_PROP);
		}
		else if (l_Type.compare(XE_PROJ_ASSET_MANAGER_NODE) == 0)
		{
			m_GameProject.m_GameProjectConfig.m_AssetManagerFile		= m_GameProject.m_ProjectLocation + child.GetString(XE_PROJ_CONFIG_FILE_PROP);
		}
		else if (l_Type.compare(XE_PROJ_GAME_OBJECT_MANAGER_NODE) == 0)
		{
			m_GameProject.m_GameProjectConfig.m_GameObjectManagerFile	= m_GameProject.m_ProjectLocation + child.GetString(XE_PROJ_CONFIG_FILE_PROP);
		}
	}

	return XEResult::Ok;
}

XEResult GameApp::ExtractGameConfigInput()
{
	//In Editor enable all modes
	if (m_GameAppRunOpt == GameAppRunOpt::EditorMode)
	{
		m_GameConfigInput.m_KeyboardEnabled		= true;
		m_GameConfigInput.m_XBoxGamepadEnabled	= true;

		return XEResult::Ok;
	}

	XEAssert(!m_GameProject.m_GameProjectConfig.m_InputFile.empty());

	XEXMLParser newFile;
	if (newFile.LoadFile(m_GameProject.m_GameProjectConfig.m_InputFile) != XEResult::Ok)
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, m_GameProject.m_GameProjectConfig.m_InputFile);
		
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser inputXML = newFile[L"Input"];
	if (!inputXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	uint32_t l_Count = inputXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = inputXML(i);

		std::wstring l_Type = child.GetName();

		if (l_Type.compare(L"Keyboard") == 0)
		{
			m_GameConfigInput.m_KeyboardEnabled = true;
		}
		else if (l_Type.compare(L"XBoxGamepad") == 0)
		{
			m_GameConfigInput.m_XBoxGamepadEnabled = true;
		}
	}

	return XEResult::Ok;
}

XEResult GameApp::ExtractGameAppOpts()
{
	XEAssert(!m_GameProject.m_GameEngineConfig.m_GameOptsFile.empty());

	XEXMLParser newFile;
	if (newFile.LoadFile(m_GameProject.m_GameEngineConfig.m_GameOptsFile) != XEResult::Ok)
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, m_GameProject.m_GameEngineConfig.m_GameOptsFile);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser gameOptXML = newFile[L"GameOpts"];
	if (!gameOptXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	uint32_t l_Count = gameOptXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = gameOptXML(i);

		std::wstring l_Type = child.GetName();
			
		if (l_Type.compare(L"WindowsPosition") == 0)
		{
			m_GameAppOpts.m_InitialWinPos = child.GetVect2i(L"Pos");
		}
		else if(l_Type.compare(L"WinCaption") == 0)
		{
			m_GameAppOpts.m_WinCaption = child.GetString(L"WP");
		}
		else if(l_Type.compare(L"WinClassName") == 0)
		{
			m_GameAppOpts.m_WinClassName = child.GetString(L"WCN");
		}
		else if(l_Type.compare(L"SampleFPS") == 0)
		{
			m_GameAppOpts.m_SampleFPS = true;
			m_Timer.IsSamplingFPS(true);
		}
		else if(l_Type.compare(L"ShowConsole") == 0)
		{
			m_GameAppOpts.m_ShowConsole = true;
		}
		else if(l_Type.compare(L"Log") == 0)
		{
			m_GameAppOpts.m_XELogLvl	= XELogHelpers::Str2LogLevel(child.GetString(L"Level", L"NONE", false));
			m_GameAppOpts.m_LogCapacity	= child.GetUInt(L"Capacity", XE_LOG_CAPACITY, false);
			m_GameAppOpts.m_LogToFile	= child.GetBool(L"LogToFile", false, false);

			m_GameAppOpts.m_LogFilePath = child.GetString(L"LogFile", L"", m_GameAppOpts.m_LogToFile);
			if (m_GameAppOpts.m_LogFilePath.compare(L"") == 0)
			{
				m_GameAppOpts.m_LogFilePath = m_GameProject.m_EngineLocation + XE_ENGINE_BIN_TO_DATA_PATH_ADD + XE_PROJ_LOG_FILE_LOC;
			}
			else
			{
				m_GameAppOpts.m_LogFilePath = m_GameProject.m_ProjectLocation + L"/" + m_GameAppOpts.m_LogFilePath;
			}
		}
	}

	return XEResult::Ok;
}

XEResult GameApp::ExtractGraphicOpts()
{
	//In Editor set defaults
	if (m_GameAppRunOpt == GameAppRunOpt::EditorMode)
	{
		m_GraphicOptsPreferred.m_ScreenResolution.x			= 1024;
		m_GraphicOptsPreferred.m_ScreenResolution.y			= 768;
		m_GraphicOptsPreferred.m_FullScreen					= false;
		m_GraphicOptsPreferred.m_BackBufferFormatWindowed	= L"DXGI_FORMAT_R8G8B8A8_UNORM";
		m_GraphicOptsPreferred.m_BackBufferFormatFullScreen	= L"DXGI_FORMAT_R8G8B8A8_UNORM";
		m_GraphicOptsPreferred.m_SingleThreaded				= true;

		return XEResult::Ok;
	}

	XEAssert(!m_GameProject.m_GameProjectConfig.m_GraphicOptsFile.empty());

	XEXMLParser newFile;
	if (newFile.LoadFile(m_GameProject.m_GameProjectConfig.m_GraphicOptsFile) != XEResult::Ok)
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"INIT_COULDNT_READ_FILE_MSG"), __FUNCTIONW__, m_GameProject.m_GameProjectConfig.m_GraphicOptsFile);
		
		XELOGGER->AddNewLog(LogLevel::Error, msg_error);
		return XEResult::OpenFileFail;
	}

	XEXMLParser graphicOptXML = newFile[L"GraphicOpts"];
	if (!graphicOptXML.IsReady())
	{
		return XEResult::XMLReadError;
	}

	uint32_t l_Count = graphicOptXML.GetNumChildren();
	for (uint32_t i = 0; i < l_Count; ++i)
	{
		XEXMLParser child = graphicOptXML(i);
		std::wstring l_Type = child.GetName();

		if( l_Type.compare(L"ScreenResolution") == 0 )
		{
			m_GraphicOptsPreferred.m_ScreenResolution.x				= child.GetInt(L"Width", 1024);
			m_GraphicOptsPreferred.m_ScreenResolution.y				= child.GetInt(L"Height", 768);
		}
		else if( l_Type.compare(L"RenderMode") == 0 )
		{
			m_GraphicOptsPreferred.m_FullScreen						= child.GetBool(L"FullScreenMode");
			m_GraphicOptsPreferred.m_BackBufferFormatWindowed		= child.GetString(L"WindowFormat");
			m_GraphicOptsPreferred.m_BackBufferFormatFullScreen		= child.GetString(L"FullScreenFormat");
		}
		else if( l_Type.compare(L"SingleThreaded") == 0)
		{
			m_GraphicOptsPreferred.m_SingleThreaded					= true;
		}
	}

	return XEResult::Ok;
}

XEResult GameApp::InitGameApp(const std::wstring& configEngineFile, const std::wstring& configProjFile, std::wstring& errorMsg)
{
	errorMsg = L"";

	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	XEAssert(!configEngineFile.empty());
	XEAssert(!configProjFile.empty());
	if (configEngineFile.empty() || configProjFile.empty())
	{
		return XEResult::EmptyFilename;
	}

	m_Quiting = false;
	m_StartShutdown = false;

	//Create Logger and Localization Manager
	XELOGGER;
	XELOCMAN;

	m_GameProject.m_EngineConfigFile = configEngineFile;
	m_GameProject.m_ProjectConfigFile = configProjFile;

	if (ExtractGameEngineConfig() != XEResult::Ok)
	{
		errorMsg = XELOCMAN->GetLiteral(L"EXTRACT_GAME_CONFIG_ERR_MSG");
		
		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);
		
		return XEResult::Fail;
	}

	if (ExtractGameProjectConfig() != XEResult::Ok)
	{
		XETODO("Add literal for project config");
		errorMsg = XELOCMAN->GetLiteral(L"EXTRACT_GAME_CONFIG_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (InitLocalizationManager() != XEResult::Ok)
	{
		/*Hard coded strings here because Localization Manager failed to start*/
		errorMsg = L"Localization Parameters could not be initialize";

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (ExtractGameAppOpts() != XEResult::Ok)
	{
		errorMsg = XELOCMAN->GetLiteral(L"EXTRACT_GAME_OPTS_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (InitLogger() != XEResult::Ok)
	{
		XETODO("Add to loc files");
		errorMsg = L"Log Error";
		//m_LocalizationManager->GetLiteral(L"EXTRACT_GAME_OPTS_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (m_GameAppOpts.m_ShowConsole)
	{
		OpenConsole();
	}

	if (ExtractGraphicOpts() != XEResult::Ok)
	{
		errorMsg = XELOCMAN->GetLiteral(L"EXTRACT_GRAPHIC_OPTS_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (!m_ForeignMainWindow)
	{
		if (InitMainWindow() != XEResult::Ok)
		{
			errorMsg = XELOCMAN->GetLiteral(L"INIT_MAIN_WINDOW_ERR_MSG");

			XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

			return XEResult::Fail;
		}
	}
	else
	{
		//If using External Foreign Window, disable FullScreen at Startup
		m_GraphicOptsPreferred.m_FullScreen = false;
	}
	
	if (Init3D_Device() != XEResult::Ok)
	{
		errorMsg = XELOCMAN->GetLiteral(L"INIT_3D_DEVICE_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (InitInputManager() != XEResult::Ok)
	{
		errorMsg = XELOCMAN->GetLiteral(L"INIT_INPUT_DEVICES_FAIL_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (InitPhysicsManager() != XEResult::Ok)
	{
		XETODO("Add correct log msg")
		//errorMsg = XELOCMAN->GetLiteral(L"SCRIPT_ENGINE_INIT_FAIL_ERR_MSG");
		//XELOGGER->AddNewLog(LogLevel::Error, errorMsg);

		return XEResult::Fail;
	}

	if (InitScriptingEngine() != XEResult::Ok)
	{
		errorMsg = XELOCMAN->GetLiteral(L"SCRIPT_ENGINE_INIT_FAIL_ERR_MSG");

		XELOGGER->AddNewLog(LogLevel::Error, errorMsg);
		
		return XEResult::Fail;
	}

	////////////////////////////////////////////////
	//Create Camera Manager
	m_CameraManager = new CameraManager();

	////////////////////////////////////////////////
	//Create Light Manager
	m_LightManager = new LightManager(m_GraphicDevice);

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
	if (RegisterScriptData() != XEResult::Ok)
	{
		XETODO("Add error box message here");

		return XEResult::Fail;
	}

	////////////////////////////////////////////////
	// Create and Initialize Game Asset Manager
	m_GameAssetManager = new GameAssetManager(m_GraphicDevice, m_GameResourceManager, m_AngelScriptManager, m_AudioManager, m_GameProject.m_ProjectLocation, m_GameProject.m_ProjectLocation + L"/" + XE_PROJ_ASSETS_DIR_LOC);

	if(m_GameAssetManager->Initialize() != XEResult::Ok)
	{
		XETODO("Better init of asset game manager");
		/*errorMsg = m_LocalizationManager->GetLiteral(L"SCRIPT_ENGINE_INIT_FAIL_ERR_MSG");

		m_Logger->AddNewLog(LogLevel::Error, errorMsg);*/

		return XEResult::Fail;
	}

	////////////////////////////////////////////////
	//Create Game Object Manager
	m_GameObjectManager = new GameObjectManager(m_GraphicDevice, m_GameAssetManager, m_GameObjectScriptManager, m_AngelScriptManager, m_LightManager, m_CameraManager, m_AudioManager, m_PhysicsManager);

	//////////////////////////////////
	//Game is ready to run
	m_IsReady = true;

	//////////////////////////////////
	//Log that Engine is ready to run
	XELOGGER->AddNewLog(LogLevel::Info, XELOCMAN->GetLiteral(L"XE_GAME_APP_READY_INFO_MSG"));

	//////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult GameApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc; 
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_AppInst;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= m_GameAppOpts.m_WinClassName.c_str();

	if( !RegisterClass(&wc) )
	{
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"REGISTER_CLASS_FAIL_MSG"), __FUNCTIONW__);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);

		PostQuitMessage(0);
		return XEResult::Fail;
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
		
	m_MainWnd = CreateWindow(	m_GameAppOpts.m_WinClassName.c_str(), 
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
		std::wstring msg_error = fmt::format(XELOCMAN->GetLiteral(L"CREATE_WINDOW_FAIL_MSG"), __FUNCTIONW__);

		XELOGGER->AddNewLog(LogLevel::Error, msg_error);

		PostQuitMessage(0);
		return XEResult::Fail;
	}

	SetWindowLongPtr(m_MainWnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(m_MainWnd, SW_SHOW);
	UpdateWindow(m_MainWnd);

	return XEResult::Ok;
}

XEResult GameApp::Init3D_Device()
{
	//Create Graphics Device
	m_GraphicDevice = new GraphicDevice(m_MainWnd, m_GraphicOptsPreferred);

	XETODO("Examine Check Caps");
	//Check for Graphic Device Capabilities Support
	//if(m_GraphicDevice->CheckDevCaps(m_GameConfig.m_DevCapFile) != XEResult::Ok)
	//{
	//	std::wstring msgerr = fmt::format(m_LocalizationManager->GetLiteral(L"CHECK_DEV_CAPS_ERR_MSG"), __FUNCTIONW__);

	//	m_Logger->AddNewLog(LogLevel::Error, msgerr);
	//	return XEResult::Fail;
	//}

	//Initialize Device
	if(m_GraphicDevice->InitDevice() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

#if defined(_DEBUG) || defined(PROFILE)

	m_GraphicDebugDX = new GraphicDebugDX(m_GraphicDevice);

	if(m_GraphicDebugDX->Initialize() != XEResult::Ok)
	{
		XETODO("Add log here");
	}

#endif

	return XEResult::Ok;
}

XEResult GameApp::InitLocalizationManager()
{
	XEAssert(!m_GameProject.m_GameEngineConfig.m_LocalizationFile.empty());
	if (XELOCMAN->Initialize(m_GameProject.m_GameEngineConfig.m_LocalizationFile, m_GameProject.m_EngineLocation + XE_ENGINE_BIN_TO_DATA_PATH_ADD) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	XEAssert(!m_GameProject.m_GameProjectConfig.m_LocalizationFile.empty());
	if (XELOCMAN->LoadProjectFile(m_GameProject.m_GameProjectConfig.m_LocalizationFile, m_GameProject.m_ProjectLocation) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameApp::InitInputManager()
{
	if(ExtractGameConfigInput() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	m_InputManager = new InputManager();

	return m_InputManager->Initialize(m_GameConfigInput);
}

XEResult GameApp::InitPhysicsManager()
{
	//Initialize Physics Manager
	m_PhysicsManager = new PhysicsManager();

	return m_PhysicsManager->Initialize();
}

XEResult GameApp::InitScriptingEngine()
{
	m_AngelScriptManager = new AngelScriptManager();

	if(m_AngelScriptManager->Initialize() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameApp::InitLogger()
{
	XELOGGER->SetCapacity(m_GameAppOpts.m_LogCapacity);

	XELOGGER->SetLogLevel(m_GameAppOpts.m_XELogLvl);

	if (XELOGGER->SetLogFilename(m_GameAppOpts.m_LogFilePath) != XEResult::Ok)
	{
		return XEResult::InitLoggerFail;
	}

	if(m_GameAppOpts.m_LogToFile)
	{
		if (XELOGGER->ActivateLogToFile() != XEResult::Ok)
		{
			return XEResult::InitLoggerFail;
		}
	}

	return XEResult::Ok;
}

XEResult GameApp::RegisterScriptData()
{
	/////////////////////////////////////
	//Register Add-ons

	//GLM Add-on
	GLMAddOnAS glmAddOn;
	if (m_AngelScriptManager->RegisterAddOn(glmAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Base Add-on
	BaseAddOnAS baseAddOn;
	if (m_AngelScriptManager->RegisterAddOn(baseAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Color Add-on
	ColorAddOnAS colorAddOn;
	if (m_AngelScriptManager->RegisterAddOn(colorAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Time Add-on
	TimeAddOnAS timeAddOn;
	if (m_AngelScriptManager->RegisterAddOn(timeAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Log Add-on
	LogAddOnAS logAddOn;
	if (m_AngelScriptManager->RegisterAddOn(logAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Localization Add-on
	LocalizationAddOnAS localizationAddOn;
	if (m_AngelScriptManager->RegisterAddOn(localizationAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Input Add-on
	InputAddOnAS inputAddOn(m_InputManager);
	if (m_AngelScriptManager->RegisterAddOn(inputAddOn) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Audio Add-on
	AudioAddOnAS audioAddOnAS;
	if (m_AngelScriptManager->RegisterAddOn(audioAddOnAS) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	// Game Core Add-on
	GameCoreAddOnAS gameCoreAddOnAS(m_GameComponentCollection, m_GameObjectScriptManager);
	if (m_AngelScriptManager->RegisterAddOn(gameCoreAddOnAS) != XEResult::Ok)
	{
		XETODO("Add log");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

GameAppScopedLock GameApp::GetGameAppScopedLock()
{
	return GameAppScopedLock(&m_GameAppMutex);
}

void GameApp::Initialize()
{
	m_GameComponentCollection->InitializeCollection();
}

void GameApp::LoadContent()
{
	m_GameComponentCollection->LoadContentCollection();
}

void GameApp::UnLoadContent()
{
	m_GameComponentCollection->UnLoadContentCollection();
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

	//Lock this request as it can come in from other threads to be modified
	//or the screen is been resized
	std::lock_guard<std::mutex> lock(m_GameAppMutex);

	m_ResizeRequested = true;

	m_NewResize.x = width;
	m_NewResize.y = heigth;
}

XEResult GameApp::SetFullScreen(bool fullScreenEnable)
{
	GraphicsPresentationParameters* gpp = &m_GraphicDevice->GetGraphicPP();
	
	if(fullScreenEnable)
	{
		if(!gpp->m_Windowed)
		{
			return XEResult::Ok;
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
			return XEResult::Ok;
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

		glm::ivec2 size = XEGraphicHelpers::GetWindowSize(m_MainWnd);

		gpp->m_BackBufferWidth = size.x;
		gpp->m_BackBufferHeight = size.y;
	}

	//Signal Everything that we need to reset device
	OnLostDevice();
	
	if(m_GraphicDevice->ResetDevice() != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	OnResetDevice();
	
	return XEResult::Ok;
}

void GameApp::OnLostDevice()
{
	m_GameComponentCollection->OnLostDeviceCollection();
}

void GameApp::OnResetDevice()
{
	m_GameComponentCollection->OnResetDeviceCollection();
}

XEResult GameApp::AddComponent(GameComponent* gc)
{
	if(!m_IsReady)
	{
		return XEResult::Fail;
	}

	if(m_GameComponentCollection->Add(gc) != XEResult::Ok)
	{
		XETODO("Change to Literal");
		XEAssert(!"Fail to add Game Component, game component already in collection");

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameApp::RemoveComponent(GameComponent* gc)
{
	return m_GameComponentCollection->Remove(gc);
}

XEResult GameApp::RegisterGameService(const std::wstring& serviceName, GameService* gs)
{
	if(!m_IsReady)
	{
		return XEResult::Fail;
	}

	if(m_GameServiceCollection->Add(serviceName, gs) != XEResult::Ok)
	{
		XETODO("Change to Literal");
		XEAssert(!"Fail to Register Game Service, game service name already registered");

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult GameApp::UnRegisterGameService(const std::wstring& serviceName)
{
	if(!m_IsReady)
	{
		return XEResult::Fail;
	}

	return m_GameServiceCollection->Remove(serviceName);
}

GameService* GameApp::GetGameServiceBase(const std::wstring& serviceName) const
{
	if(!m_IsReady)
	{
		return nullptr;
	}

	return m_GameServiceCollection->GetGameService(serviceName);
}

XEResult GameApp::EditorPlay()
{
	if (!m_IsReady || m_GameAppRunOpt == GameAppRunOpt::GameMode)
	{
		return XEResult::NotReady;
	}

	if (m_GameEditorPlayState == GameEditorPlayState::Playing)
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	ret = m_CameraManager->SetDefaultCameraAsMain();

	m_GameEditorPlayState = GameEditorPlayState::Playing;

	return ret;
}

XEResult GameApp::EditorPause()
{
	if (!m_IsReady || m_GameAppRunOpt == GameAppRunOpt::GameMode)
	{
		return XEResult::NotReady;
	}

	if (m_GameEditorPlayState == GameEditorPlayState::Paused || m_GameEditorPlayState == GameEditorPlayState::Stop)
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	m_GameEditorPlayState = GameEditorPlayState::Paused;

	return ret;
}

XEResult GameApp::EditorStop()
{
	if (!m_IsReady || m_GameAppRunOpt == GameAppRunOpt::GameMode)
	{
		return XEResult::NotReady;
	}

	if (m_GameEditorPlayState == GameEditorPlayState::Stop)
	{
		return XEResult::Ok;
	}

	XEResult ret = XEResult::Ok;

	ret = m_CameraManager->SetEditorCameraAsMain();

	m_GameEditorPlayState = GameEditorPlayState::Stop;

	return ret;
}

void GameApp::OpenConsole()
{
	if(AllocConsole() != TRUE)
	{
		XETODO("ADD LOG MESSAGE");
		return;
	}

	HANDLE handle_out = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long) handle_out, _O_TEXT);
	FILE* hf_out = _fdopen(hCrt, "w");
	setvbuf(hf_out, nullptr, _IONBF, 1);
	*stdout = *hf_out;

	HANDLE handle_in = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long) handle_in, _O_TEXT);
	FILE* hf_in = _fdopen(hCrt, "r");
	setvbuf(hf_in, nullptr, _IONBF, 128);
	*stdin = *hf_in;
}

XEResult GameApp::SetMainWindow(HWND mainWindow)
{
	XEAssert(mainWindow != nullptr);

	if(m_IsReady || m_MainWnd != nullptr || mainWindow == nullptr)
	{
		return XEResult::Fail;
	}

	m_MainWnd = mainWindow;
	m_ForeignMainWindow = true;

	return XEResult::Ok;
}

void GameApp::PreRender()
{
	if(m_ResizeRequested == true)
	{
		m_ResizeRequested = false;

		OnLostDevice();

		m_GraphicDevice->Resize(m_NewResize.x, m_NewResize.y);

		OnResetDevice();
	}
}

void GameApp::PostRender()
{
	m_GraphicDevice->Present();
}

void GameApp::Render(const TimerParams& timerParams)
{
	m_GameComponentCollection->RenderCollection(timerParams);
}

void GameApp::ConstantUpdate(const TimerParams& timerParams)
{
	m_GameComponentCollection->ConstantUpdateCollection(timerParams);
}

void GameApp::Update(const TimerParams& timerParams)
{
	m_GameComponentCollection->UpdateCollection(timerParams);
}

void GameApp::PostUpdate(const TimerParams& timerParams)
{
	m_GameComponentCollection->PostUpdateCollection(timerParams);
}

void GameApp::RunConstantUpdate()
{
	//Let the thread sleep for 0.5 seconds before beginning
	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	while(!m_Quiting)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));

		m_GameAppMutex.lock();

		m_ConstantUpdateMutex.lock();

		m_Timer.UpdateConstantUpdateTimer();
		ConstantUpdate(m_Timer.GetConstantUpdateTimerParams());
		m_Timer.PostUpdateConstantUpdateTimer();

		m_GameAppMutex.unlock();

		m_ConstantUpdateMutex.unlock();
	}
}

void GameApp::RunUpdate()
{
	m_Timer.UpdateUpdateTimer();
	Update(m_Timer.GetUpdateTimerParams());
	m_Timer.PostUpdateUpdateTimer();
}

void GameApp::RunPostUpdate()
{
	m_Timer.UpdatePostUpdateTimer();
	PostUpdate(m_Timer.GetPostUpdateTimerParams());
	m_Timer.PostUpdatePostUpdateTimer();
}

void GameApp::RunRender()
{
	m_Timer.UpdateRenderTimer();
	PreRender();
	Render(m_Timer.GetRenderTimerParams());
	PostRender();
	m_Timer.PostUpdateRenderTimer();
}

void GameApp::StartConstantUpdateThread()
{
	m_ConstantUpdateThread = std::thread(&GameApp::RunConstantUpdate, this);
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

	m_Timer.UpdateAllTimers();

	//Initialize Game and Load Content
	Initialize();
	LoadContent();
	
	//Start Constant Update Thread
	StartConstantUpdateThread();

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
			// If the application is paused then free some CPU cycles to other 
			// applications and then continue on to the next frame.
			if(m_AppPaused )
			{
				std::this_thread::sleep_for(std::chrono::seconds(20));
				continue;
			}

			//Lock this request as it can come in from other threads to be modified
			//Let the thread sleep if the FPS is going fast as we need to give some time
			//to the other thread to get the lock before the loop
			//if (m_Timer.GetFPS() >= 60)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(500));
			}
			
			m_GameAppMutex.lock();

			m_ConstantUpdateMutex.lock();

			//Update Timer
			m_Timer.UpdateFrameTimer();

			//Update Game
			RunUpdate();

			//A Post Update, so stuff like cameras can be updated after all the scene has
			RunPostUpdate();

			//Render Game
			RunRender();

			m_GameAppMutex.unlock();

			m_ConstantUpdateMutex.unlock();
		}
	}

	m_Quiting = true;

	//Join Threads
	m_ConstantUpdateThread.join();

	UnLoadContent();

	m_IsEngineOff = true;

	return (int)msg.wParam;
}

XEResult GameApp::SaveGameInfo()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEResult ret = XEResult::Ok;

	ret = XELOCMAN->SaveToXML(m_GameProject.m_GameProjectConfig.m_LocalizationFile, m_GameProject.m_ProjectLocation);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");
		return ret;
	}

	ret = m_GameAssetManager->SaveToXML(m_GameProject.m_GameProjectConfig.m_AssetManagerFile);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");
		return ret;
	}

	ret = m_GameObjectManager->SaveToXML(m_GameProject.m_GameProjectConfig.m_GameObjectManagerFile);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");
		return ret;
	}

	return XEResult::Ok;
}

XEResult GameApp::LoadGameProjectInfo()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEResult ret = XEResult::Ok;

	ret = XELOCMAN->LoadProjectFile(m_GameProject.m_GameProjectConfig.m_LocalizationFile, m_GameProject.m_ProjectLocation);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");
		return ret;
	}

	ret = m_GameAssetManager->LoadAssetManagerFile(m_GameProject.m_GameProjectConfig.m_AssetManagerFile);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");
		return ret;
	}

	ret = m_GameObjectManager->LoadGameObjectManagerFile(m_GameProject.m_GameProjectConfig.m_GameObjectManagerFile);
	if (ret != XEResult::Ok)
	{
		XETODO("Log error");
		return ret;
	}

	return XEResult::Ok;
}

XEResult GameApp::CreateProjectFolder(const std::wstring& projectFolder, const std::wstring& projectName, bool createFolder)
{
	//////////////////////////////////////
	//Pre-checks
	if (projectFolder.empty() || projectName.empty())
	{
		return XEResult::EmptyString;
	}

	//////////////////////////////////////
	//Create Project Folder
	if (createFolder)
	{
		if (!boost::filesystem::create_directory(projectFolder))
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	//////////////////////////////////////
	//Create Config Project File
	std::wofstream configFile(projectFolder + L"/" + XE_PROJ_CONFIG_PROJ_FILE);
	if (!configFile.is_open())
	{
		return XEResult::OpenFileFail;
	}

	configFile << XEGameAppHelpers::BuildConfigProjectFile(projectName);
	configFile.close();

	//////////////////////////////////////
	//Create Sub-folders
	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_DATA_DIR))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_ASSETS_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_ASSET_MANAGER_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_GLOBAL_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_INPUT_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_GAME_OBJECTS_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_LOCALIZATION_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (!boost::filesystem::create_directory(projectFolder + L"/" + XE_PROJ_LOG_DIR_LOC))
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	//////////////////////////////////////
	//Create XML Files

	//Asset Manager File
	std::wofstream assetManagerFile(projectFolder + L"/" + XE_PROJ_ASSET_MANAGER_FILE_LOC);
	if (!assetManagerFile.is_open())
	{
		return XEResult::OpenFileFail;
	}
	assetManagerFile << XEGameAppHelpers::BuildAssetFile();
	assetManagerFile.close();

	//Localization File
	std::wofstream projectLocalizationFile(projectFolder + L"/" + XE_PROJ_LOCALIZATION_PROJ_FILE_LOC);
	if (!projectLocalizationFile.is_open())
	{
		return XEResult::OpenFileFail;
	}
	projectLocalizationFile << XEGameAppHelpers::BuildLocalizationFile();
	projectLocalizationFile.close();

	//Game Object Manager File
	std::wofstream gameObjectManagerFile(projectFolder + L"/" + XE_PROJ_GAME_OBJECT_MANAGER_FILE_LOC);
	if (!gameObjectManagerFile.is_open())
	{
		return XEResult::OpenFileFail;
	}
	gameObjectManagerFile << XEGameAppHelpers::BuildGameObjectManagerFile();
	gameObjectManagerFile.close();

	//////////////////////////////////////
	//Finish
	return XEResult::Ok;
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
				m_AppPaused = true;
			}
			else
			{
				m_AppPaused = false;
			}

			return 0;

		// WM_SIZE is sent when the user resizes the window.  
		case WM_SIZE:

			width  = LOWORD(lParam);
			height = HIWORD(lParam);

			if( wParam == SIZE_MINIMIZED )
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				m_AppPaused = false;
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
					m_AppPaused = false;
					m_Minimized = false;
					OnResize(width, height);
				}
				// Restoring from maximized state?
				else if(m_Maximized)
				{
					m_AppPaused = false;
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
			m_AppPaused = true;
			m_Resizing  = true;
			return 0;

		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		case WM_EXITSIZEMOVE:
			GetClientRect(m_MainWnd, &clientRect);

			width  = (uint32_t)clientRect.right;
			height = (uint32_t)clientRect.bottom;

			m_AppPaused = false;
			m_Resizing  = false;

			OnResize(width, height);

			return 0;

		// WM_CLOSE is sent when the user presses the 'X' button in the
		// caption bar menu.
		case WM_CLOSE:
			if(!m_ForeignMainWindow)
			{
				DestroyWindow(m_MainWnd);
			}
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

