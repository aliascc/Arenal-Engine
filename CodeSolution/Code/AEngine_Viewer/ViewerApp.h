/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VIEWER_APP
#define _VIEWER_APP

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
#include "GameApp\GameApp.h"

/********************
*   Forward Decls   *
*********************/
struct TimerParams;

class IndexBuffer;
struct VertexPositionColor;
struct VertexPositionTexture;
class InputHandler;
class DebugStats;
class Console;
class SpriteBatchAE;
class SpriteFontAE;
class Texture2D;
class AnimationPlayer;

class SkinningMaterial;
class DiffuseTextureMaterial;

class GameObject;

template<class T>
class VertexBuffer;

/*****************
*   Class Decl   *
******************/
class ViewerApp sealed : public GameApp
{
	private:

		DiffuseTextureMaterial* m_DiffuseTextureMaterial;
		SkinningMaterial* m_SkinningMaterial;

		InputHandler* m_InputHandler;
		DebugStats* m_DebugStats;
		SpriteBatchAE* m_SpriteBatchAE;
		SpriteFontAE* m_SpriteFontAE;
		Console* m_Console;
		AnimationPlayer* m_AnimationPlayer;

		Texture2D* m_LoboTexture;

		GameObject* m_GameObjectLobo;

		//void DrawSkinModels(Model* model);
		void DrawGameObject(GameObject* gameObject);

	public:
		//Constructor and Destroyer
		ViewerApp(HINSTANCE hInstance);
		virtual ~ViewerApp();

		//
		void CallBackAnim1();

		//Override methods from parent class
		void			Initialize				() override;
		void			LoadContent				() override;
		void			UnLoadContent			() override;
		void			OnLostDevice			() override;
		void			OnResetDevice			() override;
		void			Update					(const TimerParams& timerParams) override;
		void			Render					(const TimerParams& timerParams) override;
};

#endif

