/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 9/15/2014
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Keyboard.h"
#include "XBoxGamepad.h"
#include "InputAddOnAS.h"
#include "InputManager.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"
#include "XBoxGamepadManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
InputAddOnAS::InputAddOnAS(InputManager* inputManager)
	: AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Input)
	, m_InputManager(inputManager)
{
	XEAssert(inputManager != nullptr);
}

InputAddOnAS::~InputAddOnAS()
{
}

XEResult InputAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(m_InputManager != nullptr);
	if (m_InputManager == nullptr)
	{
		return XEResult::NullObj;
	}

	if (RegisterXEKeysEnum(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterXEXBoxGamepadButtonEnum(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterXEXBoxGamepadMovementsEnum(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterKeyboardObject(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterXBoxGamepadObject(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterXBoxGamepadManagerObject(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterInputManagerObject(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterInputManagerObjectPointer(engine) != XEResult::Ok)
	{
		XETODO("Add log");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterXEKeysEnum(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////////////
	//Register enum XEKeys
	ret = engine->RegisterEnum("XEKeys");
	if (ret < 0)
	{
		return XEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N0", static_cast<uint32_t>(XEKeys::N0));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N1", static_cast<uint32_t>(XEKeys::N1));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N2", static_cast<uint32_t>(XEKeys::N2));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N3", static_cast<uint32_t>(XEKeys::N3));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N4", static_cast<uint32_t>(XEKeys::N4));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N5", static_cast<uint32_t>(XEKeys::N5));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N6", static_cast<uint32_t>(XEKeys::N6));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N7", static_cast<uint32_t>(XEKeys::N7));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N8", static_cast<uint32_t>(XEKeys::N8));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N9", static_cast<uint32_t>(XEKeys::N9));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "A", static_cast<uint32_t>(XEKeys::A));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "B", static_cast<uint32_t>(XEKeys::B));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "C", static_cast<uint32_t>(XEKeys::C));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "D", static_cast<uint32_t>(XEKeys::D));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "E", static_cast<uint32_t>(XEKeys::E));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F", static_cast<uint32_t>(XEKeys::F));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "G", static_cast<uint32_t>(XEKeys::G));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "H", static_cast<uint32_t>(XEKeys::H));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "I", static_cast<uint32_t>(XEKeys::I));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "J", static_cast<uint32_t>(XEKeys::J));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "K", static_cast<uint32_t>(XEKeys::K));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "L", static_cast<uint32_t>(XEKeys::L));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "M", static_cast<uint32_t>(XEKeys::M));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "N", static_cast<uint32_t>(XEKeys::N));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "O", static_cast<uint32_t>(XEKeys::O));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "P", static_cast<uint32_t>(XEKeys::P));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "Q", static_cast<uint32_t>(XEKeys::Q));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "R", static_cast<uint32_t>(XEKeys::R));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "S", static_cast<uint32_t>(XEKeys::S));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "T", static_cast<uint32_t>(XEKeys::T));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "U", static_cast<uint32_t>(XEKeys::U));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "V", static_cast<uint32_t>(XEKeys::V));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "W", static_cast<uint32_t>(XEKeys::W));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "X", static_cast<uint32_t>(XEKeys::X));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "Y", static_cast<uint32_t>(XEKeys::Y));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "Z", static_cast<uint32_t>(XEKeys::Z));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD0", static_cast<uint32_t>(XEKeys::NUMPAD0));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD1", static_cast<uint32_t>(XEKeys::NUMPAD1));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD2", static_cast<uint32_t>(XEKeys::NUMPAD2));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD3", static_cast<uint32_t>(XEKeys::NUMPAD3));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD4", static_cast<uint32_t>(XEKeys::NUMPAD4));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD5", static_cast<uint32_t>(XEKeys::NUMPAD5));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD6", static_cast<uint32_t>(XEKeys::NUMPAD6));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD7", static_cast<uint32_t>(XEKeys::NUMPAD7));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD8", static_cast<uint32_t>(XEKeys::NUMPAD8));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPAD9", static_cast<uint32_t>(XEKeys::NUMPAD9));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMPADENTER", static_cast<uint32_t>(XEKeys::NUMPADENTER));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "MULTIPLY", static_cast<uint32_t>(XEKeys::MULTIPLY));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "ADD", static_cast<uint32_t>(XEKeys::ADD));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "SUBTRACT", static_cast<uint32_t>(XEKeys::SUBTRACT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "DECIMAL", static_cast<uint32_t>(XEKeys::DECIMAL));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "DIVIDE", static_cast<uint32_t>(XEKeys::DIVIDE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_PLUS", static_cast<uint32_t>(XEKeys::OEM_PLUS));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_MINUS", static_cast<uint32_t>(XEKeys::OEM_MINUS));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "UNDERSCORE", static_cast<uint32_t>(XEKeys::UNDERSCORE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_COMMA", static_cast<uint32_t>(XEKeys::OEM_COMMA));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_PERIOD", static_cast<uint32_t>(XEKeys::OEM_PERIOD));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "SPACE", static_cast<uint32_t>(XEKeys::SPACE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "BACKSPACE", static_cast<uint32_t>(XEKeys::BACKSPACE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F1", static_cast<uint32_t>(XEKeys::F1));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F2", static_cast<uint32_t>(XEKeys::F2));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F3", static_cast<uint32_t>(XEKeys::F3));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F4", static_cast<uint32_t>(XEKeys::F4));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F5", static_cast<uint32_t>(XEKeys::F5));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F6", static_cast<uint32_t>(XEKeys::F6));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F7", static_cast<uint32_t>(XEKeys::F7));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F8", static_cast<uint32_t>(XEKeys::F8));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F9", static_cast<uint32_t>(XEKeys::F9));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F10", static_cast<uint32_t>(XEKeys::F10));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F11", static_cast<uint32_t>(XEKeys::F11));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F12", static_cast<uint32_t>(XEKeys::F12));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F13", static_cast<uint32_t>(XEKeys::F13));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F14", static_cast<uint32_t>(XEKeys::F14));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F15", static_cast<uint32_t>(XEKeys::F15));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F16", static_cast<uint32_t>(XEKeys::F16));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F17", static_cast<uint32_t>(XEKeys::F17));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F18", static_cast<uint32_t>(XEKeys::F18));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F19", static_cast<uint32_t>(XEKeys::F19));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F20", static_cast<uint32_t>(XEKeys::F20));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F21", static_cast<uint32_t>(XEKeys::F21));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F22", static_cast<uint32_t>(XEKeys::F22));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F23", static_cast<uint32_t>(XEKeys::F23));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "F24", static_cast<uint32_t>(XEKeys::F24));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "TAB", static_cast<uint32_t>(XEKeys::TAB));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "ENTER", static_cast<uint32_t>(XEKeys::ENTER));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "LSHIFT", static_cast<uint32_t>(XEKeys::LSHIFT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "RSHIFT", static_cast<uint32_t>(XEKeys::RSHIFT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "LCTRL", static_cast<uint32_t>(XEKeys::LCTRL));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "RCTRL", static_cast<uint32_t>(XEKeys::RCTRL));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "LALT", static_cast<uint32_t>(XEKeys::LALT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "RALT", static_cast<uint32_t>(XEKeys::RALT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "PAUSE", static_cast<uint32_t>(XEKeys::PAUSE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "ESCAPE", static_cast<uint32_t>(XEKeys::ESCAPE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "PAGEUP", static_cast<uint32_t>(XEKeys::PAGEUP));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "PAGEDOWN", static_cast<uint32_t>(XEKeys::PAGEDOWN));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "END", static_cast<uint32_t>(XEKeys::END));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "HOME", static_cast<uint32_t>(XEKeys::HOME));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "LEFT", static_cast<uint32_t>(XEKeys::LEFT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "UP", static_cast<uint32_t>(XEKeys::UP));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "RIGHT", static_cast<uint32_t>(XEKeys::RIGHT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "DOWN", static_cast<uint32_t>(XEKeys::DOWN));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "PRINTSCREEN", static_cast<uint32_t>(XEKeys::PRINTSCREEN));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "INSERT", static_cast<uint32_t>(XEKeys::INSERT));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "DEL", static_cast<uint32_t>(XEKeys::DEL));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "CAPSLOCK", static_cast<uint32_t>(XEKeys::CAPSLOCK));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "NUMLOCK", static_cast<uint32_t>(XEKeys::NUMLOCK));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "SCROLLLOCK", static_cast<uint32_t>(XEKeys::SCROLLLOCK));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "VOLUME_MUTE", static_cast<uint32_t>(XEKeys::VOLUME_MUTE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "VOLUME_DOWN", static_cast<uint32_t>(XEKeys::VOLUME_DOWN));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "VOLUME_UP", static_cast<uint32_t>(XEKeys::VOLUME_UP));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "MEDIA_NEXT_TRACK", static_cast<uint32_t>(XEKeys::MEDIA_NEXT_TRACK));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "MEDIA_PREV_TRACK", static_cast<uint32_t>(XEKeys::MEDIA_PREV_TRACK));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "MEDIA_STOP", static_cast<uint32_t>(XEKeys::MEDIA_STOP));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "MEDIA_PLAY_PAUSE", static_cast<uint32_t>(XEKeys::MEDIA_PLAY_PAUSE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_SEMICOLON", static_cast<uint32_t>(XEKeys::OEM_SEMICOLON));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_COLON", static_cast<uint32_t>(XEKeys::OEM_COLON));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_FWR_SLASH", static_cast<uint32_t>(XEKeys::OEM_FWR_SLASH));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_QUESTION", static_cast<uint32_t>(XEKeys::OEM_QUESTION));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_BACK_QUOTE", static_cast<uint32_t>(XEKeys::OEM_BACK_QUOTE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_TILDE", static_cast<uint32_t>(XEKeys::OEM_TILDE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_OPEN_BRACKET", static_cast<uint32_t>(XEKeys::OEM_OPEN_BRACKET));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_OPEN_BRACE", static_cast<uint32_t>(XEKeys::OEM_OPEN_BRACE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_BACK_SLASH", static_cast<uint32_t>(XEKeys::OEM_BACK_SLASH));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_PIPE", static_cast<uint32_t>(XEKeys::OEM_PIPE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_CLOSE_BRACKET", static_cast<uint32_t>(XEKeys::OEM_CLOSE_BRACKET));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_CLOSE_BRACE", static_cast<uint32_t>(XEKeys::OEM_CLOSE_BRACE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_SINGLE_QUOTE", static_cast<uint32_t>(XEKeys::OEM_SINGLE_QUOTE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_DOUBLE_QUOTE", static_cast<uint32_t>(XEKeys::OEM_DOUBLE_QUOTE));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_1", static_cast<uint32_t>(XEKeys::OEM_1));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_2", static_cast<uint32_t>(XEKeys::OEM_2));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_3", static_cast<uint32_t>(XEKeys::OEM_3));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_4", static_cast<uint32_t>(XEKeys::OEM_4));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_5", static_cast<uint32_t>(XEKeys::OEM_5));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_6", static_cast<uint32_t>(XEKeys::OEM_6));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_7", static_cast<uint32_t>(XEKeys::OEM_7));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_8", static_cast<uint32_t>(XEKeys::OEM_8));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_9", static_cast<uint32_t>(XEKeys::OEM_9));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_10", static_cast<uint32_t>(XEKeys::OEM_10));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_11", static_cast<uint32_t>(XEKeys::OEM_11));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_12", static_cast<uint32_t>(XEKeys::OEM_12));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_13", static_cast<uint32_t>(XEKeys::OEM_13));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_14", static_cast<uint32_t>(XEKeys::OEM_14));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_15", static_cast<uint32_t>(XEKeys::OEM_15));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_16", static_cast<uint32_t>(XEKeys::OEM_16));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_17", static_cast<uint32_t>(XEKeys::OEM_17));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_18", static_cast<uint32_t>(XEKeys::OEM_18));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_19", static_cast<uint32_t>(XEKeys::OEM_19));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_20", static_cast<uint32_t>(XEKeys::OEM_20));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_21", static_cast<uint32_t>(XEKeys::OEM_21));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_22", static_cast<uint32_t>(XEKeys::OEM_22));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEKeys", "OEM_23", static_cast<uint32_t>(XEKeys::OEM_23));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterXEXBoxGamepadButtonEnum(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////////////
	//Register enum XEXBoxGamepadButton
	ret = engine->RegisterEnum("XEXBoxGamepadButton");
	if (ret < 0)
	{
		return XEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "Y", static_cast<uint32_t>(XEXBoxGamepadButton::Y));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "X", static_cast<uint32_t>(XEXBoxGamepadButton::X));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "A", static_cast<uint32_t>(XEXBoxGamepadButton::A));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "B", static_cast<uint32_t>(XEXBoxGamepadButton::B));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "RightTop", static_cast<uint32_t>(XEXBoxGamepadButton::RightTop));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "RightBottom", static_cast<uint32_t>(XEXBoxGamepadButton::RightBottom));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "LeftTop", static_cast<uint32_t>(XEXBoxGamepadButton::LeftTop));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "LeftBottom", static_cast<uint32_t>(XEXBoxGamepadButton::LeftBottom));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "DPadUp", static_cast<uint32_t>(XEXBoxGamepadButton::DPadUp));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "DPadDown", static_cast<uint32_t>(XEXBoxGamepadButton::DPadDown));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "DPadLeft", static_cast<uint32_t>(XEXBoxGamepadButton::DPadLeft));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "DPadRight", static_cast<uint32_t>(XEXBoxGamepadButton::DPadRight));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "Back", static_cast<uint32_t>(XEXBoxGamepadButton::Back));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "Start", static_cast<uint32_t>(XEXBoxGamepadButton::Start));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "RightThumb", static_cast<uint32_t>(XEXBoxGamepadButton::RightThumb));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadButton", "LeftThumb", static_cast<uint32_t>(XEXBoxGamepadButton::LeftThumb));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterXEXBoxGamepadMovementsEnum(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////////////
	//Register enum XEXBoxGamepadMovements
	ret = engine->RegisterEnum("XEXBoxGamepadMovements");
	if (ret < 0)
	{
		return XEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadMovements", "LeftThumbY", static_cast<uint32_t>(XEXBoxGamepadMovements::LeftThumbY));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadMovements", "LeftThumbX", static_cast<uint32_t>(XEXBoxGamepadMovements::LeftThumbX));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadMovements", "RightThumbY", static_cast<uint32_t>(XEXBoxGamepadMovements::RightThumbY));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadMovements", "RightThumbX", static_cast<uint32_t>(XEXBoxGamepadMovements::RightThumbX));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadMovements", "LeftTrigger", static_cast<uint32_t>(XEXBoxGamepadMovements::LeftTrigger));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEXBoxGamepadMovements", "RightTrigger", static_cast<uint32_t>(XEXBoxGamepadMovements::RightTrigger));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterKeyboardObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("Keyboard", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "XEResult LockKeyboard(uint64& in lockID)", asMETHOD(Keyboard, LockKeyboard), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "XEResult UnlockKeyboard(uint64 lockID)", asMETHOD(Keyboard, UnlockKeyboard), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool WasKeyPressed(XEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool WasKeyPressed(XEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyUp(XEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyUp(XEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyDown(XEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyDown(XEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsHoldingKey(XEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsHoldingKey(XEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool HasReleasedKey(XEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool HasReleasedKey(XEKeys key, uint64 lockID)", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterXBoxGamepadObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("XBoxGamepad", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool WasButtonPressed(XEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, WasButtonPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool IsButtonUp(XEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, IsButtonUp), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool IsButtonDown(XEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, IsButtonDown), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool IsHoldingButton(XEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, IsHoldingButton), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool HasReleasedButton(XEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, HasReleasedButton), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "float GetMovement(XEXBoxGamepadMovements movButton) const", asMETHOD(XBoxGamepad, GetMovement), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterXBoxGamepadManagerObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("XBoxGamepadManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepadManager", "XBoxGamepad@ GetGamepad(uint32 index) const", asMETHOD(XBoxGamepadManager, GetGamepad), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterInputManagerObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("InputManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "Keyboard@ GetKeyboard() const", asMETHOD(InputManager, GetKeyboard), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "XBoxGamepadManager@ GetXBoxGamepadManager() const", asMETHOD(InputManager, GetXBoxGamepadManager), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "bool IsKeyboardActive() const", asMETHOD(InputManager, IsKeyboardActive), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "bool IsXBoxGamepadManagerActive() const", asMETHOD(InputManager, IsXBoxGamepadManagerActive), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	return XEResult::Ok;
}

XEResult InputAddOnAS::RegisterInputManagerObjectPointer(asIScriptEngine* engine)
{
	int ret = 0;

	XETODO("Change Names for Script");
	ret = engine->RegisterGlobalProperty("InputManager m_InputManager", m_InputManager);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	return XEResult::Ok;
}
