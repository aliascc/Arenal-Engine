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
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"
#include "AEGameComponentsDefs.h"
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
class SpriteBatchAE;
class SpriteFontAE;
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
		SpriteFontAE*					m_SpriteFontAE = nullptr;
		SpriteBatchAE*					m_SpriteBatchAE = nullptr;
		Color							m_BackgroundColor = ConsoleBackgroundDefaultColor;
		Color							m_FontColor = AEColors::White;
		ConsoleStates					m_ConsoleState = ConsoleStates::Hide;
		InputHandler*					m_Input = nullptr;
		QuadColorMaterial*				m_QuadColorMaterial = nullptr;
		float							m_CurrentConsoleHeight = 0.0f;
		uint32_t						m_ConsoleHeight = AE_CONSOLE_DEFAULT_HEIGHT;
		uint32_t						m_ConsoleWidth = 0;
		float							m_ConsolePresentTime = AE_CONSOLE_DEFAULT_PRESENT_TIME;
		float							m_TimePass = 0.0f;
		wchar_t							m_ConsoleLine[AE_CONSOLE_MAX_LINE_CHARS];
		uint32_t						m_ConsoleLinePos = 0;
		glm::vec2						m_CharDim = AEMathHelpers::Vec2fZero;
		const float						m_TimeUnderscore = AE_CONSOLE_DEFAULT_TIME_UNDERSCORE;
		bool							m_ShowNewCharUnderscore = true;
		float							m_TimeBetweenUnderscore = 0.0f;
		uint32_t						m_ConsolePrintHistoryPos = 0;
		int32_t							m_CurrentCMDHistory = -1;

		std::list<std::wstring>			m_ConsoleCmdHistory;
		std::list<ConsoleShowLine>		m_ConsolePrintHistory;

		uint64_t						m_ConsoleLockKeyboard;

		bool							m_FirstBackspace = true;
		float							m_TimeSinceLastBackSpace = AE_CONSOLE_BACKSPACE_FIRST_TIME;

		std::wstring					m_ConsoleModuleName = AE_CONSOLE_SCRIPT_MOD_NAME;
		CScriptArray*					m_ScriptConsoleLineArray = nullptr;
		asIScriptContext*				m_ConsoleContext = nullptr;
		asIScriptFunction*				m_ConsoleExecFunc = nullptr;

		std::wstring					m_InputHandlerServiceName = L"";

		bool m_IsReady = true;

		//Private Framework Methods
		void							UpdateConsolePosition		(const TimerParams& timerParams);
		void							UpdateConsoleLine			(const TimerParams& timerParams);
		
		bool							LookInCmdHistory			();

		AEResult						RegisterConsoleScriptInfo	();
		AEResult						ConsoleExecScript			();
		AEResult						ProcessConsoleRetArray		();

	public:
		//Constructor Destructor.
		Console(GameApp* gameApp, const std::wstring& gameComponentName = AE_CONSOLE_DEF_COMPONENT_NAME, const std::wstring& inputHandlerServiceName = AE_INPUT_HANDLER_DEF_SERVICE_NAME, uint32_t callOrder = AEGameComponentCallOrder::_AE_GCCO_Console);
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