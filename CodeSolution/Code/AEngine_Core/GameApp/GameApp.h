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

#pragma once

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicsDefs.h"
#include "GameApp\GameAppDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameService;
class AEXMLParser;
class ImGuiManager;
class InputManager;
struct TimerParams;
class LightManager;
class AudioManager;
class GraphicDevice;
class GameComponent;
class CameraManager;
class GraphicDebugDX;
class PhysicsManager;
class ResourceManager;
class GameAssetManager;
class GameObjectManager;
class GameCommandManager;
class AngelScriptManager;
class GameResourceManager;
class GameServiceCollection;
class GameComponentCollection;
class GameObjectScriptManager;

/*****************
*   Class Decl   *
******************/
class GameApp abstract : public AEObject
{
private:

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

    /// <summary>
    /// Graphic Debug Helpers
    /// </summary>
    GraphicDebugDX* m_GraphicDebugDX = nullptr;

#endif //AE_GRAPHIC_DEBUG_DEVICE

    AETODO("To be change to conditional variables");
    bool m_IsEngineOff = true;

    void InternalInitialize();

    void InternalLoadContent();

    void InternalUnLoadContent();

    void InternalRender(const TimerParams& timerParams);

    void InternalConstantUpdate();

    void InternalUpdate(const TimerParams& timerParams);

    void InternalPostUpdate(const TimerParams& timerParams);

    void InternalOnLostDevice();

    void InternalOnResetDevice();

    //Static Callback function for Windows Message Loop
    static LRESULT CALLBACK MainWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    //Methods
    AEResult ExtractGameEngineConfig();
    AEResult ExtractGameProjectConfig();
    AEResult ExtractGameAppOpts();
    AEResult ExtractGraphicOpts();
    AEResult ExtractGameConfigInput();

    AEResult InitLocalizationManager();
    AEResult InitLogger();
    AEResult InitMainWindow();
    AEResult InitInputManager();
    AEResult InitScriptingEngine();
    AEResult InitPhysicsManager();
    AEResult Init3D_Device();

    /// <summary>
    /// Commands that needs to run before the call to the Game Loop Render
    /// </summary>
    void PreRender();

    /// <summary>
    /// Commands that need to be run after rendering the Game Loop
    /// </summary>
    void PostRender();

    /// <summary>
    /// Registers all of the information needed of the Game
    /// to the scripting engine
    /// </summary>
    /// <returns>OK when successful, otherwise an error code</returns>
    AEResult RegisterScriptData();

    /// <summary>
    /// Cleans up the memory use by the application and
    /// frees resources. This is called on shutdown
    /// </summary>
    void CleanUp();

#ifdef AE_EDITOR_MODE

    /// <summary>
    /// Opens a Console
    /// </summary>
    void OpenConsole();

#endif //AE_EDITOR_MODE

protected:

    /// <summary>
    /// Windows Application Instance Handle
    /// </summary>
    HINSTANCE m_AppInst = nullptr;

    /// <summary>
    /// Main Window Handle of the Game App
    /// </summary>
    HWND m_MainWnd = nullptr;

    /// <summary>
    /// Determines if the Application is inactive
    /// (When it does not have focus)
    /// </summary>
    bool m_AppInactive = false;

    /// <summary>
    /// The Game App is exiting
    /// </summary>
    bool m_Quiting = false;

    /// <summary>
    /// If True the Game App has been requested to start shuting down
    /// </summary>
    bool m_StartShutdown = false;

    /// <summary>
    /// Determines if the Game App has been initialize
    /// and it is ready to start running
    /// </summary>
    bool m_IsReady = false;

    /// <summary>
    /// Determines if the Game Window is Minimize
    /// </summary>
    bool m_Minimized = false;

    /// <summary>
    /// Determines if the Game Window is Maximize
    /// </summary>
    bool m_Maximized = false;

    /// <summary>
    /// The Game Window is currently resizing.
    /// </summary>
    bool m_Resizing = false;

    /// <summary>
    /// Class to Manage all script functions.
    /// </summary>
    AngelScriptManager* m_AngelScriptManager = nullptr;
    
    /// <summary>
    /// Class to manager all the input resources.
    /// </summary>
    InputManager* m_InputManager = nullptr;

    /// <summary>
    /// Class to manage all Resources that need to be loaded from
    /// disk to memory
    /// </summary>
    GameResourceManager* m_GameResourceManager = nullptr;

    /// <summary>
    /// Class to manage all Game Objects
    /// </summary>
    GameObjectManager* m_GameObjectManager = nullptr;

    /// <summary>
    /// Class to manage all Game Assets 
    /// </summary>
    GameAssetManager* m_GameAssetManager = nullptr;

    /// <summary>
    /// Class to manage all the Lights
    /// </summary>
    LightManager* m_LightManager = nullptr;

    /// <summary>
    /// Class to manage all the Game Object Scripts
    /// </summary>
    GameObjectScriptManager* m_GameObjectScriptManager = nullptr;

    /// <summary>
    /// Class to manage all the Game Object Cameras
    /// </summary>
    CameraManager* m_CameraManager = nullptr;

    /// <summary>
    /// Class to manage all the Game Object Cameras
    /// </summary>
    AudioManager* m_AudioManager = nullptr;

    /// <summary>
    /// Class to manage all the Game Object Physics
    /// </summary>
    PhysicsManager* m_PhysicsManager = nullptr;

    /// <summary>
    /// Application Init options
    /// </summary>
    GameAppOpts m_GameAppOpts;

    /// <summary>
    /// Graphic Options
    /// </summary>
    GraphicOptsPreferred m_GraphicOptsPreferred;

    /// <summary>
    /// Game APp Graphic Device
    /// </summary>
    GraphicDevice* m_GraphicDevice = nullptr;

    /// <summary>
    /// Input Init Options
    /// </summary>
    GameConfigInput m_GameConfigInput;

    /// <summary>
    /// Game App Timer
    /// </summary>
    AETime m_Timer;

    /// <summary>
    /// Game Component Collection of the Game App
    /// Here the Game App will have all the active game components
    /// </summary>
    GameComponentCollection* m_GameComponentCollection = nullptr;

    /// <summary>
    /// Game Services Available to the Game App
    /// </summary>
    GameServiceCollection* m_GameServiceCollection = nullptr;

#ifdef AE_EDITOR_MODE

    /// <summary>
    /// ImGui Manager
    /// </summary>
    ImGuiManager* m_ImGuiManager = nullptr;

    /// <summary>
    /// Game Project Configuration Options
    /// </summary>
    GameProject m_GameProject;

    /// <summary>
    /// Determines if the Game Simulation is running or not
    /// </summary>
    GameEditorPlayState m_GameEditorPlayState = GameEditorPlayState::Stop;

#endif //AE_EDITOR_MODE

    //Framework Methods
    virtual void OnResize(uint32_t width, uint32_t heigth);

public:

    /// <summary>
    /// GameApp Constructor
    /// </summary>
    /// <param name="hInstance">Handle to Instance for the Game App window</param>
    GameApp(HINSTANCE hInstance);

    /// <summary>
    /// Default GameApp Destructor
    /// </summary>
    virtual ~GameApp();

    inline HINSTANCE GetAppInst() const
    {
        return m_AppInst;
    }

    inline HWND GetMainWnd() const
    {
        return m_MainWnd;
    }

    inline GameAppOpts GetGameAppOpts() const
    {
        return m_GameAppOpts;
    }

    inline GraphicDevice& GetGraphicsDevice()
    {
        AEAssert(m_GraphicDevice != nullptr);
        return *m_GraphicDevice;
    }

    inline const AETime& GetTimer() const
    {
        return m_Timer;
    }

    inline GameEditorPlayState GetGameEditorPlayState() const
    {
        return m_GameEditorPlayState;
    }

    /// <summary>
    /// Gets Physics Manager Instance for the Game Application
    /// </summary>
    /// <returns>Physics Manager Instance</returns>
    inline PhysicsManager& GetPhysicsManager()
    {
        AEAssert(m_PhysicsManager != nullptr);
        return *m_PhysicsManager;
    }

    /// <summary>
    /// Gets Light Manager Instance for the Game Application
    /// </summary>
    /// <returns>Light Manager Instance</returns>
    inline LightManager& GetLightManager()
    {
        AEAssert(m_LightManager != nullptr);
        return *m_LightManager;
    }

    /// <summary>
    /// Gets Game Resource Manager Instance for the Game Application
    /// </summary>
    /// <returns>Game Resource Manager Instance</returns>
    inline GameResourceManager& GetGameResourceManager()
    {
        AEAssert(m_GameResourceManager != nullptr);
        return *m_GameResourceManager;
    }

    /// <summary>
    /// Gets Game Object Manager Instance for the Game Application
    /// </summary>
    /// <returns>Game Object Manager Instance</returns>
    inline GameObjectManager& GetGameObjectManager()
    {
        AEAssert(m_GameObjectManager != nullptr);
        return *m_GameObjectManager;
    }

    /// <summary>
    /// Gets Game Asset Manager Instance for the Game Application
    /// </summary>
    /// <returns>Game Asset Manager Instance</returns>
    inline GameAssetManager& GetGameAssetManager()
    {
        AEAssert(m_GameAssetManager != nullptr);
        return *m_GameAssetManager;
    }

    /// <summary>
    /// Gets Angel Script Manager Instance for the Game Application
    /// </summary>
    /// <returns>Angel Script Manager Instance</returns>
    inline AngelScriptManager& GetAngelScriptManager()
    {
        AEAssert(m_AngelScriptManager != nullptr);
        return *m_AngelScriptManager;
    }

    /// <summary>
    /// Gets Input Manager Instance for the Game Application
    /// </summary>
    /// <returns>Input Manager Instance</returns>
    inline InputManager& GetInputManager()
    {
        AEAssert(m_InputManager != nullptr);
        return *m_InputManager;
    }

    /// <summary>
    /// Gets Game Object Script Manager Instance for the Game Application
    /// </summary>
    /// <returns>Game Object Script Manager Instance</returns>
    inline GameObjectScriptManager& GetGameObjectScriptManager()
    {
        AEAssert(m_GameObjectScriptManager != nullptr);
        return *m_GameObjectScriptManager;
    }

    /// <summary>
    /// Gets Camera Manager Instance for the Game Application
    /// </summary>
    /// <returns>Game Object Script Manager Instance</returns>
    inline CameraManager& GetCameraManager()
    {
        AEAssert(m_CameraManager != nullptr);
        return *m_CameraManager;
    }

    /// <summary>
    /// Gets Audio Manager Instance for the Game Application
    /// </summary>
    /// <returns>Audio Manager Instance</returns>
    inline AudioManager& GetAudioManager()
    {
        AEAssert(m_AudioManager != nullptr);
        return *m_AudioManager;
    }

    /// <summary>
    /// Gets ImGui Manager Instance for the Game Application
    /// </summary>
    /// <returns>ImGui Manager Instance</returns>
    inline ImGuiManager& GetImGuiManager()
    {
        AEAssert(m_ImGuiManager != nullptr);
        return *m_ImGuiManager;
    }

    /// <summary>
    /// Gets Game Component Collection Instance for the Game Application
    /// </summary>
    /// <returns>Game Component Collection Instance</returns>
    inline GameComponentCollection& GetGameComponentCollection()
    {
        AEAssert(m_GameComponentCollection != nullptr);
        return *m_GameComponentCollection;
    }

    inline bool IsNewProject() const
    {
        return (m_GameProject.m_ProjectName.empty());
    }

    inline bool IsReady() const
    {
        return m_IsReady;
    }

    int Run();

#ifdef AE_EDITOR_MODE
    inline void SetFullScreen(bool fullScreenEnable)
    {
    }
#else
    void SetFullScreen(bool fullScreenEnable);
#endif

    LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

    AEResult AddComponent(GameComponent* gc);

    AEResult RemoveComponent(GameComponent* gc);

    AEResult RegisterGameService(const std::string& serviceName, GameService* gc);

    AEResult UnRegisterGameService(const std::string& serviceName);

    AEResult InitGameApp(const std::string& configEngineFile, std::string& errorMsg);

    AEResult InitProject(const std::string& configProjFile, std::string& errorMsg);

    GameService* GetGameServiceBase(const std::string& serviceName) const;

    void ShutDownGameApp();

    AEResult SaveGameInfo();

    AEResult LoadGameProjectInfo();

    AEResult CreateProjectFolder(const std::string& projectFolder, const std::string& projectName, bool createFolder);

    void Resize(uint32_t width, uint32_t heigth);

    virtual void Initialize() = 0;

    virtual void LoadContent() = 0;

    virtual void UnLoadContent() = 0;

    virtual void Render(const TimerParams& timerParams) = 0;

    virtual void ConstantUpdate(const TimerParams& timerParams) = 0;

    virtual void Update(const TimerParams& timerParams) = 0;

    virtual void PostUpdate(const TimerParams& timerParams) = 0;

    virtual void OnLostDevice() = 0;

    virtual void OnResetDevice() = 0;

    template<class T>
    T* GetGameService(const std::string& serviceName) const
    {
        return reinterpret_cast<T*>(GetGameServiceBase(serviceName));
    }

#ifdef AE_EDITOR_MODE

    void ResizeEditor(uint32_t width, uint32_t heigth);

    /// <summary>
    /// Starts the game simulation
    /// </summary>
    /// <returns>AEResult::Ok on successful, otherwise error code.</returns>
    AEResult EditorPlay();

    /// <summary>
    /// Pause the game simulation
    /// </summary>
    /// <returns>AEResult::Ok on successful, otherwise error code.</returns>
    AEResult EditorPause();

    /// <summary>
    /// Stop the game simulation
    /// </summary>
    /// <returns>AEResult::Ok on successful, otherwise error code.</returns>
    AEResult EditorStop();

#endif //AE_EDITOR_MODE

#pragma endregion

};