/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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
	AEAssert(inputManager != nullptr);
}

InputAddOnAS::~InputAddOnAS()
{
}

AEResult InputAddOnAS::Register(asIScriptEngine* engine)
{
	AEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(m_InputManager != nullptr);
	if (m_InputManager == nullptr)
	{
		return AEResult::NullObj;
	}

	if (RegisterAEKeysEnum(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterAEXBoxGamepadButtonEnum(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterAEXBoxGamepadMovementsEnum(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterKeyboardObject(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterXBoxGamepadObject(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterXBoxGamepadManagerObject(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterInputManagerObject(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterInputManagerObjectPointer(engine) != AEResult::Ok)
	{
		AETODO("Add log");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_LOC_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterAEKeysEnum(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////////////
	//Register enum AEKeys
	ret = engine->RegisterEnum("AEKeys");
	if (ret < 0)
	{
		return AEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N0", static_cast<uint32_t>(AEKeys::N0));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N1", static_cast<uint32_t>(AEKeys::N1));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N2", static_cast<uint32_t>(AEKeys::N2));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N3", static_cast<uint32_t>(AEKeys::N3));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N4", static_cast<uint32_t>(AEKeys::N4));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N5", static_cast<uint32_t>(AEKeys::N5));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N6", static_cast<uint32_t>(AEKeys::N6));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N7", static_cast<uint32_t>(AEKeys::N7));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N8", static_cast<uint32_t>(AEKeys::N8));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N9", static_cast<uint32_t>(AEKeys::N9));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "A", static_cast<uint32_t>(AEKeys::A));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "B", static_cast<uint32_t>(AEKeys::B));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "C", static_cast<uint32_t>(AEKeys::C));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "D", static_cast<uint32_t>(AEKeys::D));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "E", static_cast<uint32_t>(AEKeys::E));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F", static_cast<uint32_t>(AEKeys::F));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "G", static_cast<uint32_t>(AEKeys::G));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "H", static_cast<uint32_t>(AEKeys::H));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "I", static_cast<uint32_t>(AEKeys::I));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "J", static_cast<uint32_t>(AEKeys::J));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "K", static_cast<uint32_t>(AEKeys::K));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "L", static_cast<uint32_t>(AEKeys::L));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "M", static_cast<uint32_t>(AEKeys::M));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "N", static_cast<uint32_t>(AEKeys::N));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "O", static_cast<uint32_t>(AEKeys::O));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "P", static_cast<uint32_t>(AEKeys::P));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "Q", static_cast<uint32_t>(AEKeys::Q));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "R", static_cast<uint32_t>(AEKeys::R));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "S", static_cast<uint32_t>(AEKeys::S));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "T", static_cast<uint32_t>(AEKeys::T));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "U", static_cast<uint32_t>(AEKeys::U));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "V", static_cast<uint32_t>(AEKeys::V));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "W", static_cast<uint32_t>(AEKeys::W));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "X", static_cast<uint32_t>(AEKeys::X));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "Y", static_cast<uint32_t>(AEKeys::Y));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "Z", static_cast<uint32_t>(AEKeys::Z));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD0", static_cast<uint32_t>(AEKeys::NUMPAD0));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD1", static_cast<uint32_t>(AEKeys::NUMPAD1));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD2", static_cast<uint32_t>(AEKeys::NUMPAD2));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD3", static_cast<uint32_t>(AEKeys::NUMPAD3));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD4", static_cast<uint32_t>(AEKeys::NUMPAD4));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD5", static_cast<uint32_t>(AEKeys::NUMPAD5));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD6", static_cast<uint32_t>(AEKeys::NUMPAD6));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD7", static_cast<uint32_t>(AEKeys::NUMPAD7));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD8", static_cast<uint32_t>(AEKeys::NUMPAD8));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPAD9", static_cast<uint32_t>(AEKeys::NUMPAD9));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMPADENTER", static_cast<uint32_t>(AEKeys::NUMPADENTER));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "MULTIPLY", static_cast<uint32_t>(AEKeys::MULTIPLY));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "ADD", static_cast<uint32_t>(AEKeys::ADD));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "SUBTRACT", static_cast<uint32_t>(AEKeys::SUBTRACT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "DECIMAL", static_cast<uint32_t>(AEKeys::DECIMAL));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "DIVIDE", static_cast<uint32_t>(AEKeys::DIVIDE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_PLUS", static_cast<uint32_t>(AEKeys::OEM_PLUS));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_MINUS", static_cast<uint32_t>(AEKeys::OEM_MINUS));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "UNDERSCORE", static_cast<uint32_t>(AEKeys::UNDERSCORE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_COMMA", static_cast<uint32_t>(AEKeys::OEM_COMMA));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_PERIOD", static_cast<uint32_t>(AEKeys::OEM_PERIOD));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "SPACE", static_cast<uint32_t>(AEKeys::SPACE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "BACKSPACE", static_cast<uint32_t>(AEKeys::BACKSPACE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F1", static_cast<uint32_t>(AEKeys::F1));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F2", static_cast<uint32_t>(AEKeys::F2));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F3", static_cast<uint32_t>(AEKeys::F3));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F4", static_cast<uint32_t>(AEKeys::F4));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F5", static_cast<uint32_t>(AEKeys::F5));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F6", static_cast<uint32_t>(AEKeys::F6));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F7", static_cast<uint32_t>(AEKeys::F7));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F8", static_cast<uint32_t>(AEKeys::F8));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F9", static_cast<uint32_t>(AEKeys::F9));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F10", static_cast<uint32_t>(AEKeys::F10));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F11", static_cast<uint32_t>(AEKeys::F11));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F12", static_cast<uint32_t>(AEKeys::F12));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F13", static_cast<uint32_t>(AEKeys::F13));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F14", static_cast<uint32_t>(AEKeys::F14));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F15", static_cast<uint32_t>(AEKeys::F15));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F16", static_cast<uint32_t>(AEKeys::F16));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F17", static_cast<uint32_t>(AEKeys::F17));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F18", static_cast<uint32_t>(AEKeys::F18));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F19", static_cast<uint32_t>(AEKeys::F19));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F20", static_cast<uint32_t>(AEKeys::F20));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F21", static_cast<uint32_t>(AEKeys::F21));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F22", static_cast<uint32_t>(AEKeys::F22));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F23", static_cast<uint32_t>(AEKeys::F23));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "F24", static_cast<uint32_t>(AEKeys::F24));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "TAB", static_cast<uint32_t>(AEKeys::TAB));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "ENTER", static_cast<uint32_t>(AEKeys::ENTER));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "LSHIFT", static_cast<uint32_t>(AEKeys::LSHIFT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "RSHIFT", static_cast<uint32_t>(AEKeys::RSHIFT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "LCTRL", static_cast<uint32_t>(AEKeys::LCTRL));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "RCTRL", static_cast<uint32_t>(AEKeys::RCTRL));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "LALT", static_cast<uint32_t>(AEKeys::LALT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "RALT", static_cast<uint32_t>(AEKeys::RALT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "PAUSE", static_cast<uint32_t>(AEKeys::PAUSE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "ESCAPE", static_cast<uint32_t>(AEKeys::ESCAPE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "PAGEUP", static_cast<uint32_t>(AEKeys::PAGEUP));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "PAGEDOWN", static_cast<uint32_t>(AEKeys::PAGEDOWN));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "END", static_cast<uint32_t>(AEKeys::END));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "HOME", static_cast<uint32_t>(AEKeys::HOME));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "LEFT", static_cast<uint32_t>(AEKeys::LEFT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "UP", static_cast<uint32_t>(AEKeys::UP));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "RIGHT", static_cast<uint32_t>(AEKeys::RIGHT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "DOWN", static_cast<uint32_t>(AEKeys::DOWN));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "PRINTSCREEN", static_cast<uint32_t>(AEKeys::PRINTSCREEN));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "INSERT", static_cast<uint32_t>(AEKeys::INSERT));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "DEL", static_cast<uint32_t>(AEKeys::DEL));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "CAPSLOCK", static_cast<uint32_t>(AEKeys::CAPSLOCK));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "NUMLOCK", static_cast<uint32_t>(AEKeys::NUMLOCK));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "SCROLLLOCK", static_cast<uint32_t>(AEKeys::SCROLLLOCK));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "VOLUME_MUTE", static_cast<uint32_t>(AEKeys::VOLUME_MUTE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "VOLUME_DOWN", static_cast<uint32_t>(AEKeys::VOLUME_DOWN));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "VOLUME_UP", static_cast<uint32_t>(AEKeys::VOLUME_UP));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "MEDIA_NEXT_TRACK", static_cast<uint32_t>(AEKeys::MEDIA_NEXT_TRACK));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "MEDIA_PREV_TRACK", static_cast<uint32_t>(AEKeys::MEDIA_PREV_TRACK));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "MEDIA_STOP", static_cast<uint32_t>(AEKeys::MEDIA_STOP));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "MEDIA_PLAY_PAUSE", static_cast<uint32_t>(AEKeys::MEDIA_PLAY_PAUSE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_SEMICOLON", static_cast<uint32_t>(AEKeys::OEM_SEMICOLON));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_COLON", static_cast<uint32_t>(AEKeys::OEM_COLON));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_FWR_SLASH", static_cast<uint32_t>(AEKeys::OEM_FWR_SLASH));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_QUESTION", static_cast<uint32_t>(AEKeys::OEM_QUESTION));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_BACK_QUOTE", static_cast<uint32_t>(AEKeys::OEM_BACK_QUOTE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_TILDE", static_cast<uint32_t>(AEKeys::OEM_TILDE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_OPEN_BRACKET", static_cast<uint32_t>(AEKeys::OEM_OPEN_BRACKET));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_OPEN_BRACE", static_cast<uint32_t>(AEKeys::OEM_OPEN_BRACE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_BACK_SLASH", static_cast<uint32_t>(AEKeys::OEM_BACK_SLASH));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_PIPE", static_cast<uint32_t>(AEKeys::OEM_PIPE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_CLOSE_BRACKET", static_cast<uint32_t>(AEKeys::OEM_CLOSE_BRACKET));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_CLOSE_BRACE", static_cast<uint32_t>(AEKeys::OEM_CLOSE_BRACE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_SINGLE_QUOTE", static_cast<uint32_t>(AEKeys::OEM_SINGLE_QUOTE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_DOUBLE_QUOTE", static_cast<uint32_t>(AEKeys::OEM_DOUBLE_QUOTE));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_1", static_cast<uint32_t>(AEKeys::OEM_1));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_2", static_cast<uint32_t>(AEKeys::OEM_2));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_3", static_cast<uint32_t>(AEKeys::OEM_3));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_4", static_cast<uint32_t>(AEKeys::OEM_4));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_5", static_cast<uint32_t>(AEKeys::OEM_5));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_6", static_cast<uint32_t>(AEKeys::OEM_6));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_7", static_cast<uint32_t>(AEKeys::OEM_7));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_8", static_cast<uint32_t>(AEKeys::OEM_8));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_9", static_cast<uint32_t>(AEKeys::OEM_9));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_10", static_cast<uint32_t>(AEKeys::OEM_10));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_11", static_cast<uint32_t>(AEKeys::OEM_11));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_12", static_cast<uint32_t>(AEKeys::OEM_12));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_13", static_cast<uint32_t>(AEKeys::OEM_13));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_14", static_cast<uint32_t>(AEKeys::OEM_14));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_15", static_cast<uint32_t>(AEKeys::OEM_15));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_16", static_cast<uint32_t>(AEKeys::OEM_16));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_17", static_cast<uint32_t>(AEKeys::OEM_17));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_18", static_cast<uint32_t>(AEKeys::OEM_18));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_19", static_cast<uint32_t>(AEKeys::OEM_19));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_20", static_cast<uint32_t>(AEKeys::OEM_20));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_21", static_cast<uint32_t>(AEKeys::OEM_21));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_22", static_cast<uint32_t>(AEKeys::OEM_22));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEKeys", "OEM_23", static_cast<uint32_t>(AEKeys::OEM_23));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterAEXBoxGamepadButtonEnum(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////////////
	//Register enum AEXBoxGamepadButton
	ret = engine->RegisterEnum("AEXBoxGamepadButton");
	if (ret < 0)
	{
		return AEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "Y", static_cast<uint32_t>(AEXBoxGamepadButton::Y));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "X", static_cast<uint32_t>(AEXBoxGamepadButton::X));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "A", static_cast<uint32_t>(AEXBoxGamepadButton::A));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "B", static_cast<uint32_t>(AEXBoxGamepadButton::B));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "RightTop", static_cast<uint32_t>(AEXBoxGamepadButton::RightTop));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "RightBottom", static_cast<uint32_t>(AEXBoxGamepadButton::RightBottom));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "LeftTop", static_cast<uint32_t>(AEXBoxGamepadButton::LeftTop));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "LeftBottom", static_cast<uint32_t>(AEXBoxGamepadButton::LeftBottom));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "DPadUp", static_cast<uint32_t>(AEXBoxGamepadButton::DPadUp));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "DPadDown", static_cast<uint32_t>(AEXBoxGamepadButton::DPadDown));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "DPadLeft", static_cast<uint32_t>(AEXBoxGamepadButton::DPadLeft));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "DPadRight", static_cast<uint32_t>(AEXBoxGamepadButton::DPadRight));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "Back", static_cast<uint32_t>(AEXBoxGamepadButton::Back));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "Start", static_cast<uint32_t>(AEXBoxGamepadButton::Start));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "RightThumb", static_cast<uint32_t>(AEXBoxGamepadButton::RightThumb));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadButton", "LeftThumb", static_cast<uint32_t>(AEXBoxGamepadButton::LeftThumb));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterAEXBoxGamepadMovementsEnum(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////////////
	//Register enum AEXBoxGamepadMovements
	ret = engine->RegisterEnum("AEXBoxGamepadMovements");
	if (ret < 0)
	{
		return AEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadMovements", "LeftThumbY", static_cast<uint32_t>(AEXBoxGamepadMovements::LeftThumbY));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadMovements", "LeftThumbX", static_cast<uint32_t>(AEXBoxGamepadMovements::LeftThumbX));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadMovements", "RightThumbY", static_cast<uint32_t>(AEXBoxGamepadMovements::RightThumbY));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadMovements", "RightThumbX", static_cast<uint32_t>(AEXBoxGamepadMovements::RightThumbX));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadMovements", "LeftTrigger", static_cast<uint32_t>(AEXBoxGamepadMovements::LeftTrigger));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEXBoxGamepadMovements", "RightTrigger", static_cast<uint32_t>(AEXBoxGamepadMovements::RightTrigger));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterKeyboardObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("Keyboard", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "AEResult LockKeyboard(uint64& in lockID)", asMETHOD(Keyboard, LockKeyboard), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "AEResult UnlockKeyboard(uint64 lockID)", asMETHOD(Keyboard, UnlockKeyboard), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool WasKeyPressed(AEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool WasKeyPressed(AEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyUp(AEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyUp(AEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyDown(AEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsKeyDown(AEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsHoldingKey(AEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool IsHoldingKey(AEKeys key, uint64 lockID) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool HasReleasedKey(AEKeys key) const", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("Keyboard", "bool HasReleasedKey(AEKeys key, uint64 lockID)", asMETHOD(Keyboard, WasKeyPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterXBoxGamepadObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("XBoxGamepad", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool WasButtonPressed(AEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, WasButtonPressed), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool IsButtonUp(AEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, IsButtonUp), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool IsButtonDown(AEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, IsButtonDown), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool IsHoldingButton(AEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, IsHoldingButton), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "bool HasReleasedButton(AEXBoxGamepadButton button) const", asMETHOD(XBoxGamepad, HasReleasedButton), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepad", "float GetMovement(AEXBoxGamepadMovements movButton) const", asMETHOD(XBoxGamepad, GetMovement), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterXBoxGamepadManagerObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("XBoxGamepadManager", 0, asOBJ_REF | asOBJ_NOCOUNT);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("XBoxGamepadManager", "XBoxGamepad@ GetGamepad(uint32 index) const", asMETHOD(XBoxGamepadManager, GetGamepad), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterInputManagerObject(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterObjectType("InputManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
	if (ret < 0)
	{
		return AEResult::RegObjTypeFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "Keyboard@ GetKeyboard() const", asMETHOD(InputManager, GetKeyboard), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "XBoxGamepadManager@ GetXBoxGamepadManager() const", asMETHOD(InputManager, GetXBoxGamepadManager), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "bool IsKeyboardActive() const", asMETHOD(InputManager, IsKeyboardActive), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	ret = engine->RegisterObjectMethod("InputManager", "bool IsXBoxGamepadManagerActive() const", asMETHOD(InputManager, IsXBoxGamepadManagerActive), asCALL_THISCALL);
	if (ret < 0)
	{
		return AEResult::RegObjMethodFail;
	}

	return AEResult::Ok;
}

AEResult InputAddOnAS::RegisterInputManagerObjectPointer(asIScriptEngine* engine)
{
	int ret = 0;

	AETODO("Change Names for Script");
	ret = engine->RegisterGlobalProperty("InputManager m_InputManager", m_InputManager);
	if (ret < 0)
	{
		return AEResult::RegGlobalPropFail;
	}

	return AEResult::Ok;
}
