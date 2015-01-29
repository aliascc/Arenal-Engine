/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/24/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _DEBUG_STATS_H
#define _DEBUG_STATS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"
#include "XEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class SpriteFontXE;
class CameraUpdater;
class SpriteBatchXE;
class BasicColorMaterial;

template<class T>
class VertexBuffer;

/**************
*   Structs   *
***************/
struct DebugStatsConfig
{
	std::wstring m_SpriteFontFile = L"";
	Color m_TextColor = XEColors::White;
	bool m_FPSEnabled = true;
	bool m_AxisEnabled = true;
	float m_AxisLength = 1.0f;
	bool m_GridEnabled = true;
	Color m_GridColor = XEColors::Black;
	float m_GridSize = 10.0f;
	float m_GridUnits = 1.0f;

	DebugStatsConfig()
	{
	}
};

/*****************
*   Class Decl   *
******************/
class DebugStats : public DrawableGameComponent
{
	protected:
		//Variables
		DebugStatsConfig		m_DebugStatsConfig; // No need to initialize
		SpriteBatchXE*			m_SpriteBatchXE = nullptr;
		SpriteFontXE*			m_SpriteFontXE = nullptr;
		VertexBuffer<VertexPositionColor>* m_GridVertexBuffer = nullptr;
		VertexBuffer<VertexPositionColor>* m_AxisVertexBuffer = nullptr;
		BasicColorMaterial*		m_BasicColorMaterial = nullptr;
		CameraUpdater*			m_CameraUpdater = nullptr;
		glm::mat4				m_WVP = XEMathHelpers::Mat4Identity;

		void InitializeAxisAndGrid();

	public:
		//Constructor Destructor.
		DebugStats(GameApp* gameApp, const DebugStatsConfig& debugStatsConfig, const std::wstring& gameComponentName = XE_DEBUG_STATS_DEF_COMPONENT_NAME, const std::wstring& cameraServiceName = XE_CAMERA_UPDATER_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_DebugStats);
		virtual ~DebugStats();

		//Gets
		
		//Framework Methods
		inline bool GetGridEnabled() const
		{
			return m_DebugStatsConfig.m_GridEnabled;
		}
		
		inline void SetGridEnabled(bool enable)
		{
			m_DebugStatsConfig.m_GridEnabled = enable;
		}

		inline bool GetAxisEnabled()
		{
			return m_DebugStatsConfig.m_AxisEnabled;
		}
		
		inline void SetAxisEnabled(bool enable)
		{
			m_DebugStatsConfig.m_AxisEnabled = enable;
		}

		inline bool GetFPSEnabled()
		{
			return m_DebugStatsConfig.m_FPSEnabled;
		}
		
		inline void SetFPSEnabled(bool enable)
		{
			m_DebugStatsConfig.m_FPSEnabled = enable;
		}

		//Drawable Game Component Override methods
		void					Initialize					() override;
		void					LoadContent					() override;
		void					Update						(const TimerParams& timerParams) override;
		void					Render						(const TimerParams& timerParams) override;
		void					OnLostDevice				() override;
		void					OnResetDevice				() override;
};

#endif