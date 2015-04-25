/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _CONSOLE_H
#define _CONSOLE_H

/**********************
*   System Includes   *
***********************/
#include <list>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "angelscript.h"
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "ConsoleDefs.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"
#include "XEGameComponentsDefs.h"
#include "GameUtils\DrawableGameComponent.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class InputHandler;
class SpriteBatchXE;
class SpriteFontXE;
class QuadColorMaterial;

/*****************
*   Class Decl   *
******************/
class Console sealed : public DrawableGameComponent
{
	private:
		//enum
		enum class ConsoleStates : uint32_t
		{
			Present = 0,
			Entering,
			Exiting,
			Hide
		};

		//Variables
		SpriteFontXE*					m_SpriteFontXE = nullptr;
		SpriteBatchXE*					m_SpriteBatchXE = nullptr;
		Color							m_BackgroundColor = ConsoleBackgroundDefaultColor;
		Color							m_FontColor = XEColors::White;
		ConsoleStates					m_ConsoleState = ConsoleStates::Hide;
		InputHandler*					m_Input = nullptr;
		QuadColorMaterial*				m_QuadColorMaterial = nullptr;
		float							m_CurrentConsoleHeight = 0.0f;
		uint32_t						m_ConsoleHeight = XE_CONSOLE_DEFAULT_HEIGHT;
		uint32_t						m_ConsoleWidth = 0;
		float							m_ConsolePresentTime = XE_CONSOLE_DEFAULT_PRESENT_TIME;
		float							m_TimePass = 0.0f;
		wchar_t							m_ConsoleLine[XE_CONSOLE_MAX_LINE_CHARS];
		uint32_t						m_ConsoleLinePos = 0;
		glm::vec2						m_CharDim = XEMathHelpers::Vec2fZero;
		const float						m_TimeUnderscore = XE_CONSOLE_DEFAULT_TIME_UNDERSCORE;
		bool							m_ShowNewCharUnderscore = true;
		float							m_TimeBetweenUnderscore = 0.0f;
		uint32_t						m_ConsolePrintHistoryPos = 0;
		int32_t							m_CurrentCMDHistory = -1;

		std::list<std::wstring>			m_ConsoleCmdHistory;
		std::list<ConsoleShowLine>		m_ConsolePrintHistory;

		uint64_t						m_ConsoleLockKeyboard;

		bool							m_FirstBackspace = true;
		float							m_TimeSinceLastBackSpace = XE_CONSOLE_BACKSPACE_FIRST_TIME;

		std::wstring					m_ConsoleModuleName = XE_CONSOLE_SCRIPT_MOD_NAME;
		CScriptArray*					m_ScriptConsoleLineArray = nullptr;
		asIScriptContext*				m_ConsoleContext = nullptr;
		asIScriptFunction*				m_ConsoleExecFunc = nullptr;

		std::wstring					m_InputHandlerServiceName = L"";

		bool m_IsReady = true;

		//Private Framework Methods
		void							UpdateConsolePosition		(const TimerParams& timerParams);
		void							UpdateConsoleLine			(const TimerParams& timerParams);
		
		bool							LookInCmdHistory			();

		XEResult						RegisterConsoleScriptInfo	();
		XEResult						ConsoleExecScript			();
		XEResult						ProcessConsoleRetArray		();

	public:
		//Constructor Destructor.
		Console(GameApp* gameApp, const std::wstring& gameComponentName = XE_CONSOLE_DEF_COMPONENT_NAME, const std::wstring& inputHandlerServiceName = XE_INPUT_HANDLER_DEF_SERVICE_NAME, uint32_t callOrder = XEGameComponentCallOrder::_XE_GCCO_Console);
		virtual ~Console();

		//Gets
		
		//Framework Methods

		//Drawable Game Component Override methods
		void							Initialize					() override;
		void							LoadContent					() override;
		void							Update						(const TimerParams& timerParams) override;
		void							Render						(const TimerParams& timerParams) override;
		void							OnLostDevice				() override;
		void							OnResetDevice				() override;
};

#endif