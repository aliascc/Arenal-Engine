/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _GAME_APP_H
#define _GAME_APP_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Time\AETime.h"
#include "GraphicsDefs.h"
#include "Base\AEObject.h"
#include "GameApp\GameAppDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameService;
class AEXMLParser;
class InputManager;
struct TimerParams;
class LightManager;
class AudioManager;
class GraphicDevice;
class GameComponent;
class CameraManager;
class PhysicsManager;
class ResourceManager;
class GameAssetManager;
class GameObjectManager;
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

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables 

#if defined(AE_GRAPHIC_DEBUG_DEVICE)

        /// <summary>
        /// Graphic Debug Helpers
        /// </summary>
        GraphicDebugDX* m_GraphicDebugDX = nullptr;

#endif //AE_GRAPHIC_DEBUG_DEVICE

        AETODO("Remove this variable when GameCommandManager has been implemented");
        bool m_ResizeRequested = false;
        glm::ivec2 m_NewResize = AEMathHelpers::Vec2iZero;

        AETODO("To be change to conditional variables");
        bool m_IsEngineOff = true;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods 

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

#pragma endregion

    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Protected Variables

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
        /// Game Project Configuration Options
        /// </summary>
        GameProject m_GameProject;

        /// <summary>
        /// Determines if the Game Simulation is running or not
        /// </summary>
        GameEditorPlayState m_GameEditorPlayState = GameEditorPlayState::Stop;

#endif //AE_EDITOR_MODE

#pragma endregion

        /************************
        *   Protected Methods   *
        *************************/
#pragma region Protected Methods

        //Framework Methods
        virtual void OnResize(uint32_t width, uint32_t heigth);
        virtual void OnLostDevice();
        virtual void OnResetDevice();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameApp Constructor
        /// </summary>
        /// <param name="hInstance">Handle to Instance for the Game App window</param>
        GameApp(HINSTANCE hInstance);

        /// <summary>
        /// Default GameApp Destructor
        /// </summary>
        virtual ~GameApp();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

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

        inline GraphicDevice* GetGraphicsDevice() const
        {
            return m_GraphicDevice;
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
        inline PhysicsManager* GetPhysicsManager() const
        {
            return m_PhysicsManager;
        }

        /// <summary>
        /// Gets Light Manager Instance for the Game Application
        /// </summary>
        /// <returns>Light Manager Instance</returns>
        inline LightManager* GetLightManager() const
        {
            return m_LightManager;
        }

        /// <summary>
        /// Gets Game Resource Manager Instance for the Game Application
        /// </summary>
        /// <returns>Game Resource Manager Instance</returns>
        inline GameResourceManager* GetGameResourceManager() const
        {
            return m_GameResourceManager;
        }

        /// <summary>
        /// Gets Game Object Manager Instance for the Game Application
        /// </summary>
        /// <returns>Game Object Manager Instance</returns>
        inline GameObjectManager* GetGameObjectManager() const
        {
            return m_GameObjectManager;
        }

        /// <summary>
        /// Gets Game Asset Manager Instance for the Game Application
        /// </summary>
        /// <returns>Game Asset Manager Instance</returns>
        inline GameAssetManager* GetGameAssetManager() const
        {
            return m_GameAssetManager;
        }

        /// <summary>
        /// Gets Angel Script Manager Instance for the Game Application
        /// </summary>
        /// <returns>Angel Script Manager Instance</returns>
        inline AngelScriptManager* GetAngelScriptManager() const
        {
            return m_AngelScriptManager;
        }

        /// <summary>
        /// Gets Input Manager Instance for the Game Application
        /// </summary>
        /// <returns>Input Manager Instance</returns>
        inline InputManager* GetInputManager() const
        {
            return m_InputManager;
        }

        /// <summary>
        /// Gets Game Object Script Manager Instance for the Game Application
        /// </summary>
        /// <returns>Game Object Script Manager Instance</returns>
        inline GameObjectScriptManager* GetGameObjectScriptManager() const
        {
            return m_GameObjectScriptManager;
        }

        /// <summary>
        /// Gets Camera Manager Instance for the Game Application
        /// </summary>
        /// <returns>Game Object Script Manager Instance</returns>
        inline CameraManager* GetCameraManager() const
        {
            return m_CameraManager;
        }

        /// <summary>
        /// Gets Audio Manager Instance for the Game Application
        /// </summary>
        /// <returns>Audio Manager Instance</returns>
        inline AudioManager* GetAudioManager() const
        {
            return m_AudioManager;
        }

        inline bool IsNewProject() const
        {
            return (m_GameProject.m_ProjectName.compare("") == 0);
        }

        inline bool IsReady() const
        {
            return m_IsReady;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        int Run();

        AEResult SetFullScreen(bool fullScreenEnable);

        LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

        AEResult AddComponent(GameComponent* gc);

        AEResult RemoveComponent(GameComponent* gc);

        AEResult RegisterGameService(const std::string& serviceName, GameService* gc);

        AEResult UnRegisterGameService(const std::string& serviceName);

        AEResult InitGameApp(const std::string& configEngineFile, const std::string& configProjFile, std::string& errorMsg);

        GameService* GetGameServiceBase(const std::string& serviceName) const;

        void ShutDownGameApp();

        AEResult SaveGameInfo();

        AEResult LoadGameProjectInfo();

        AEResult CreateProjectFolder(const std::string& projectFolder, const std::string& projectName, bool createFolder);

        virtual void Initialize();

        virtual void LoadContent();

        virtual void UnLoadContent();

        virtual void Render(const TimerParams& timerParams);

        virtual void ConstantUpdate(const TimerParams& timerParams);

        virtual void Update(const TimerParams& timerParams);

        virtual void PostUpdate(const TimerParams& timerParams);

        template<class T>
        T* GetGameService(const std::string& serviceName) const
        {
            return reinterpret_cast<T*>(GetGameServiceBase(serviceName));
        }

#ifdef AE_EDITOR_MODE
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

#endif
