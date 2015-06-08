/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_APP_H
#define _GAME_APP_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <thread>
#include <string>
#include <stdint.h>
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Time\XETime.h"
#include "GraphicsDefs.h"
#include "Base\XEObject.h"
#include "GameApp\GameAppDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameService;
class XEXMLParser;
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
class GameApp abstract : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables 

#if defined(_DEBUG) || defined(PROFILE)
		GraphicDebugDX* m_GraphicDebugDX = nullptr;
#endif

		//Main Mutex for Game App
		std::mutex m_GameAppMutex;

		//Thread for Constant Update
		std::thread m_ConstantUpdateThread;
		std::mutex m_ConstantUpdateMutex;

		bool m_ResizeRequested = false;
		glm::ivec2 m_NewResize = XEMathHelpers::Vec2iZero;

		GameAppRunOpt m_GameAppRunOpt = GameAppRunOpt::GameMode;

		bool m_IsEngineOff = true;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods 

		//Static Callback function for Windows Message Loop
		static LRESULT CALLBACK MainWndProc (HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		//Methods
		XEResult ExtractGameEngineConfig();
		XEResult ExtractGameProjectConfig();
		XEResult ExtractGameAppOpts();
		XEResult ExtractGraphicOpts();
		XEResult ExtractGameConfigInput();

		XEResult InitLocalizationManager();
		XEResult InitLogger();
		XEResult InitMainWindow();
		XEResult InitInputManager();
		XEResult InitScriptingEngine();
		XEResult InitPhysicsManager();
		XEResult Init3D_Device();

		void StartConstantUpdateThread();
		void RunConstantUpdate();
		void RunUpdate();
		void RunPostUpdate();
		void RunRender();
		void PreRender();
		void PostRender();

		void OpenConsole();

		XEResult RegisterScriptData();

		void CleanUp();

#pragma endregion

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		// Application, Windows
		HINSTANCE m_AppInst = nullptr;
		HWND m_MainWnd = nullptr;
		bool m_AppPaused = false;
		bool m_Quiting = false;
		bool m_StartShutdown = false;
		bool m_IsReady = false;
		bool m_ForeignMainWindow = false;
		bool m_Minimized = false;
		bool m_Maximized = false;
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

		//Application Init options
		GameAppOpts m_GameAppOpts;

		//3D App variables
		GraphicOptsPreferred m_GraphicOptsPreferred;
		GraphicDevice* m_GraphicDevice = nullptr;

		//Input Init Options
		GameConfigInput m_GameConfigInput;

		//Timer
		XETime m_Timer;

		//Component & Service Collection
		GameComponentCollection* m_GameComponentCollection = nullptr;
		GameServiceCollection* m_GameServiceCollection = nullptr;

		//Game Editor Playing
		GameEditorPlayState m_GameEditorPlayState = GameEditorPlayState::Stop;

		GameProject m_GameProject;

		/// <summary>
		/// Determine if the Engine is running under an editor or full game
		/// </summary>
		bool m_IsEditorActive = false;

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
		/// <param name="gameAppRunOpt">Game App Run Option</param>
		GameApp(HINSTANCE hInstance, GameAppRunOpt gameAppRunOpt);

		/// <summary>
		/// Default GameApp Destructor
		/// </summary>
		virtual ~GameApp();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline HINSTANCE					GetAppInst					() const											{ return m_AppInst; }
		inline HWND							GetMainWnd					() const											{ return m_MainWnd; }
		inline GameAppOpts					GetGameAppOpts				() const											{ return m_GameAppOpts; }
		inline GraphicDevice*				GetGraphicsDevice			() const											{ return m_GraphicDevice; }
		inline const XETime&				GetTimer					() const											{ return m_Timer; }

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
			return (m_GameProject.m_ProjectName.compare(L"") == 0);
		}

		/// <summary>
		/// Gets Game Application Running Mode (Game/Editor)
		/// </summary>
		/// <returns>Game App running mode</returns>
		inline GameAppRunOpt GetGameAppRunOpt() const
		{
			return m_GameAppRunOpt;
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

		XEResult SetMainWindow(HWND mainWindow);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Starts the game simulation
		/// </summary>
		/// <returns>XEResult::Ok on successful, otherwise error code.</returns>
		XEResult EditorPlay();

		/// <summary>
		/// Pause the game simulation
		/// </summary>
		/// <returns>XEResult::Ok on successful, otherwise error code.</returns>
		XEResult EditorPause();

		/// <summary>
		/// Stop the game simulation
		/// </summary>
		/// <returns>XEResult::Ok on successful, otherwise error code.</returns>
		XEResult EditorStop();

		/// <summary>
		/// Gets a Game App Scope Mutex to lock Game App while another thread needs
		/// to work without the Game Loop running
		/// </summary>
		/// <returns>Returns GameAppScopedLock to Lock Game App Loop</returns>
		GameAppScopedLock GetGameAppScopedLock();

		int Run();

		XEResult SetFullScreen(bool fullScreenEnable);

		LRESULT MsgProc(UINT msg, WPARAM wParam, LPARAM lParam);

		XEResult AddComponent(GameComponent* gc);

		XEResult RemoveComponent(GameComponent* gc);

		XEResult RegisterGameService(const std::wstring& serviceName, GameService* gc);

		XEResult UnRegisterGameService(const std::wstring& serviceName);

		XEResult InitGameApp(const std::wstring& configEngineFile, const std::wstring& configProjFile, std::wstring& errorMsg);

		GameService* GetGameServiceBase(const std::wstring& serviceName) const;

		void ShutDownGameApp();

		XEResult SaveGameInfo();

		XEResult LoadGameProjectInfo();

		XEResult CreateProjectFolder(const std::wstring& projectFolder, const std::wstring& projectName, bool createFolder);

		virtual void Initialize();

		virtual void LoadContent();

		virtual void UnLoadContent();

		virtual void Render(const TimerParams& timerParams);

		virtual void ConstantUpdate(const TimerParams& timerParams);

		virtual void Update(const TimerParams& timerParams);

		virtual void PostUpdate(const TimerParams& timerParams);

		template<class T>
		T* GetGameService(const std::wstring& serviceName) const
		{
			return reinterpret_cast<T*>(GetGameServiceBase(serviceName));
		}

#pragma endregion

};

#endif