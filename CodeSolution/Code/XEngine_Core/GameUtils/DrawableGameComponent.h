/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _DRAWABLE_GAME_COMPONENT_H
#define _DRAWABLE_GAME_COMPONENT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class DrawableGameComponent abstract : public GameComponent
{
	//Friend class
	friend class GameComponentCollection;

	private:

		//Find out if we are a game component or drawable game component
		inline bool IsDrawableGameComponent() const override
		{
			return true;
		}

	protected:
		//Variable to check if we can draw or not
		bool m_Visible = true;
		GraphicDevice* m_GraphicDevice = nullptr;

	public:
		//Constructor Destructor
		DrawableGameComponent(GameApp* gameApp, const std::wstring& name = L"", uint32_t callOrder = 100);
		virtual ~DrawableGameComponent();

		//Gets
		inline bool GetVisible() const
		{
			return m_Visible;
		}

		//Sets
		inline void SetVisible(bool visible)
		{
			m_Visible = visible;
		}

		//Framework Methods
		virtual void Render(const TimerParams& timerParams);
};

#endif