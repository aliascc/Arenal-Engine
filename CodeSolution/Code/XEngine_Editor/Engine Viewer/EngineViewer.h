/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _ENGINE_VIEWER_H
#define _ENGINE_VIEWER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameApp\GameApp.h"

#include "PhysicsDefs.h"

/********************
*   Forward Decls   *
*********************/
class Console;
class DebugStats;
class InputHandler;
struct TimerParams;
class CameraUpdater;
class GameAudioUpdate;
class GameLightsUpdate;
class GamePhysicsUpdate;
class GameAnimationsUpdate;
class ForwardPlusRendering;
class GameObjectCameraUpdate;
class GameObjectScriptCaller;
class GameObjectsDebugVisualizer;

class MeshAsset;
class TextureAsset;
class ShaderAsset;

/*****************
*   Class Decl   *
******************/
class EngineViewer sealed : public GameApp
{
	private:
		InputHandler* m_InputHandler = nullptr;
		DebugStats* m_DebugStats = nullptr;
		Console* m_Console = nullptr;
		GameLightsUpdate* m_GameLightsUpdate = nullptr;
		ForwardPlusRendering* m_ForwardPlusRendering = nullptr;
		CameraUpdater* m_CameraUpdater = nullptr;
		GameObjectsDebugVisualizer* m_GameObjectDebugVisualizer = nullptr;
		GameAnimationsUpdate* m_GameAnimationsUpdate = nullptr;
		GameObjectScriptCaller* m_GameObjectScriptCaller = nullptr;
		GameObjectCameraUpdate* m_GameObjectCameraUpdate = nullptr;
		GameAudioUpdate* m_GameAudioUpdate = nullptr;
		GamePhysicsUpdate* m_GamePhysicsUpdate = nullptr;

		//To be removed
		bool m_AutoTestLoaded = false;
		void AutoLoadTest();
		void AutoLoadTestAddGO(const glm::vec3& pos, MeshAsset* model, ShaderAsset* vs, ShaderAsset* ps, TextureAsset* text, CollisionShape collisionShape);

	public:
		//Constructor and Destroyer
		EngineViewer(HINSTANCE hInstance, const std::wstring& configEngineFile);
		virtual ~EngineViewer();

		//Override methods from parent class
		void			Initialize				() override;
		void			LoadContent				() override;
		void			UnLoadContent			() override;
		void			OnLostDevice			() override;
		void			OnResetDevice			() override;
		void			ConstantUpdate			(const TimerParams& timerParams) override;
		void			Update					(const TimerParams& timerParams) override;
		void			PostUpdate				(const TimerParams& timerParams) override;
		void			Render					(const TimerParams& timerParams) override;
};

#endif