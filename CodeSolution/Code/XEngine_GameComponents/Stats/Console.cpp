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

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"
#include "fastformat\shims\conversion\filter_type\reals.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Console.h"
#include "Keyboard.h"
#include "Color\Color.h"
#include "Time\XETime.h"
#include "GraphicDevice.h"
#include "GameApp\GameApp.h"
#include "ScriptConsoleLine.h"
#include "Input\InputHandler.h"
#include "Base\BaseFunctions.h"
#include "Wrappers\SpriteFontXE.h"
#include "Wrappers\SpriteBatchXE.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "AngelScript\AngelScriptManager.h"
#include "Graphic Extensions\Materials\Basic\QuadColorMaterial.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XETODO("Make H & W params")
Console::Console(GameApp* gameApp, const std::wstring& gameComponentName, const std::wstring& inputHandlerServiceName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
	, m_ConsoleWidth(m_GraphicDevice->GetGraphicPP().m_BackBufferWidth)
	, m_InputHandlerServiceName(inputHandlerServiceName)
{
	ZeroMemory(m_ConsoleLine, XE_CONSOLE_LINE_MEM_SIZE);
}

Console::~Console()
{
	DeleteMem(m_SpriteFontXE);
	DeleteMem(m_SpriteBatchXE);

	DeleteMem(m_QuadColorMaterial);

	ReleaseAngel(m_ConsoleExecFunc);
	ReleaseAngel(m_ScriptConsoleLineArray);
	ReleaseAngel(m_ConsoleContext);
}

void Console::Initialize()
{
	m_QuadColorMaterial = new QuadColorMaterial(m_GraphicDevice, m_GameResourceManager, L"XE_Console_QuadColorMaterial");
	
	m_SpriteBatchXE = new SpriteBatchXE(m_GraphicDevice);

	XETODO("Set font to parameter");
	m_SpriteFontXE = new SpriteFontXE(m_GraphicDevice, XE_BASE_FS_PATH L"Data\\Fonts\\arial.spritefont");

	m_Input = m_GameApp->GetGameService<InputHandler>(m_InputHandlerServiceName);
	XEAssert(m_Input != nullptr);

	if (!(m_Input && m_Input->IsKeyboardActive()))
	{
		XETODO("Add log message");
		m_IsReady = false;
	}

	DrawableGameComponent::Initialize();
}

void Console::LoadContent()
{
	XEResult ret = XEResult::Ok;

	ret = m_SpriteBatchXE->Initialize();
	if (ret != XEResult::Ok)
	{
		XETODO("Log message");
		m_IsReady = false;
	}

	ret = m_SpriteFontXE->Initialize();
	if (ret != XEResult::Ok)
	{
		XETODO("Log message");
		m_IsReady = false;
	}

	ret = m_QuadColorMaterial->LoadContent();
	if(ret != XEResult::Ok)
	{
		XETODO("Log message");
		m_IsReady = false;
	}

	XETODO("Add background color as parameter at constructor");
	XETODO("Check to see if we can implement easy access to variables in QuadColorMaterial class, if so remove all includes for CB, shader props");
	ret = m_QuadColorMaterial->GetPSProps()->GetConstantBuffer(L"_XE_CB_Color")->SetValueT<glm::vec4>(L"u_Color", m_BackgroundColor);
	if (ret != XEResult::Ok)
	{
		XETODO("Log message");
		m_IsReady = false;
	}

	ret = RegisterConsoleScriptInfo();
	if (ret != XEResult::Ok)
	{
		XETODO("Log message");
		m_IsReady = false;
	}

	m_CharDim = m_SpriteFontXE->MeasureString(L"|");

	DrawableGameComponent::LoadContent();
}

void Console::Update(const TimerParams& timerParams)
{
	if (m_IsReady)
	{
		Keyboard* keyboard = m_Input->GetKeyboard();
		if (keyboard->IsHoldingKey(XEKeys::LCTRL, m_ConsoleLockKeyboard) && keyboard->WasKeyPressed(XEKeys::F12, m_ConsoleLockKeyboard))
		{
			switch (m_ConsoleState)
			{
				case ConsoleStates::Hide:
					m_TimePass = 0.0f;
					m_ConsoleState = ConsoleStates::Entering;
					break;

				case ConsoleStates::Entering:
					m_TimePass = m_ConsolePresentTime - m_TimePass;
					m_ConsoleState = ConsoleStates::Exiting;
					break;

				case ConsoleStates::Exiting:
					m_TimePass = m_ConsolePresentTime - m_TimePass;
					m_ConsoleState = ConsoleStates::Entering;
					break;

				case ConsoleStates::Present:
					keyboard->UnlockKeyboard(m_ConsoleLockKeyboard);
					m_ConsoleLockKeyboard = 0;
					m_TimePass = 0.0f;
					m_ConsoleState = ConsoleStates::Exiting;
					break;
			}
		}

		if (m_ConsoleState != ConsoleStates::Hide && m_ConsoleState != ConsoleStates::Present)
		{
			UpdateConsolePosition(timerParams);
		}

		if (m_ConsoleState == ConsoleStates::Present)
		{
			UpdateConsoleLine(timerParams);
		}
	}

	DrawableGameComponent::Update(timerParams);
}

void Console::UpdateConsolePosition(const TimerParams& timerParams)
{
	Keyboard* keyboard = m_Input->GetKeyboard();

	float elapsedTime = (float)timerParams.m_ElapsedTime;
	
	m_TimePass += elapsedTime;

	if(m_TimePass >= m_ConsolePresentTime)
	{
		m_TimePass = 0.0f;

		if(m_ConsoleState == ConsoleStates::Entering)
		{
			m_CurrentConsoleHeight = (float)m_ConsoleHeight;

			if (keyboard->LockKeyboard(m_ConsoleLockKeyboard) != XEResult::Ok)
			{
				XETODO("Add log");
			}

			m_ConsoleState = ConsoleStates::Present;
		}
		else if(m_ConsoleState == ConsoleStates::Exiting)
		{
			m_CurrentConsoleHeight = 0.0f;

			m_ConsoleState = ConsoleStates::Hide;
		}

		return;
	}

	float mu = m_TimePass / m_ConsolePresentTime;

	if(m_ConsoleState == ConsoleStates::Entering)
	{
		m_CurrentConsoleHeight = XEMathHelpers::Lerp<float>(0.0f, (float)m_ConsoleHeight, mu);
	}
	else if(m_ConsoleState == ConsoleStates::Exiting)
	{
		m_CurrentConsoleHeight = XEMathHelpers::Lerp<float>((float)m_ConsoleHeight, 0.0f, mu);
	}
}

void Console::UpdateConsoleLine(const TimerParams& timerParams)
{
	m_TimeBetweenUnderscore += (float)timerParams.m_ElapsedTime;

	if(m_TimeBetweenUnderscore >= m_TimeUnderscore)
	{
		m_TimeBetweenUnderscore  -= m_TimeUnderscore;

		m_ShowNewCharUnderscore = !m_ShowNewCharUnderscore;
	}

	if(LookInCmdHistory())
	{
		return;
	}

	Keyboard* keyboard = m_Input->GetKeyboard();

	if (keyboard->IsHoldingKey(XEKeys::LCTRL, m_ConsoleLockKeyboard) && keyboard->WasKeyPressed(XEKeys::C, m_ConsoleLockKeyboard))
	{
		memset(m_ConsoleLine, 0, XE_CONSOLE_LINE_MEM_SIZE);
		m_ConsoleLinePos = 0;

		return;
	}

	if (keyboard->WasKeyPressed(XEKeys::PAGEDOWN, m_ConsoleLockKeyboard))
	{
		if(m_ConsolePrintHistoryPos > 0)
		{
			--m_ConsolePrintHistoryPos;
		}
	}
	
	if (keyboard->WasKeyPressed(XEKeys::PAGEUP, m_ConsoleLockKeyboard))
	{
		if(m_ConsolePrintHistoryPos < m_ConsolePrintHistory.size() - 1)
		{
			m_ConsolePrintHistoryPos++;
		}
	}

	if (keyboard->WasKeyPressed(XEKeys::ENTER, m_ConsoleLockKeyboard) && m_ConsoleLinePos > 0)
	{
		ConsoleExecScript();

		return;
	}

	if (keyboard->IsHoldingKey(XEKeys::BACKSPACE, m_ConsoleLockKeyboard) && m_ConsoleLinePos > 0)
	{
		if(m_FirstBackspace)
		{
			--m_ConsoleLinePos;

			m_ConsoleLine[m_ConsoleLinePos] = 0;

			m_FirstBackspace = false;
		}

		m_TimeSinceLastBackSpace += (float)timerParams.m_ElapsedTime;

		if(m_TimeSinceLastBackSpace >= XE_CONSOLE_BACKSPACE_TIME)
		{
			--m_ConsoleLinePos;

			m_ConsoleLine[m_ConsoleLinePos] = 0;

			m_TimeSinceLastBackSpace -= XE_CONSOLE_BACKSPACE_TIME;
		}

		return;
	}	
	else if (keyboard->WasKeyPressed(XEKeys::BACKSPACE, m_ConsoleLockKeyboard) && m_ConsoleLinePos > 0)
	{
		m_TimeSinceLastBackSpace = XE_CONSOLE_BACKSPACE_FIRST_TIME;
		m_FirstBackspace = true;

		return;
	}

	if (m_ConsoleLinePos >= XE_CONSOLE_MAX_LINE_CHARS - 1)
	{
		return;
	}

	wchar_t cc = keyboard->GetCurrentPressedChar(m_ConsoleLockKeyboard);
	if (cc == L'')
	{
		return;
	}

	m_ConsoleLine[m_ConsoleLinePos] = cc;
	++m_ConsoleLinePos;
}

bool Console::LookInCmdHistory()
{
	bool changed = false;

	Keyboard* keyboard = m_Input->GetKeyboard();

	if (keyboard->WasKeyPressed(XEKeys::UP, m_ConsoleLockKeyboard))
	{
		if(m_CurrentCMDHistory + 1 >= (int32_t)m_ConsoleCmdHistory.size())
		{
			return false;
		}

		++m_CurrentCMDHistory;

		changed = true;
	}
	
	if (keyboard->WasKeyPressed(XEKeys::DOWN, m_ConsoleLockKeyboard))
	{
		if(m_CurrentCMDHistory == -1)
		{
			return false;
		}

		--m_CurrentCMDHistory;

		if(m_CurrentCMDHistory == -1)
		{
			ZeroMemory(m_ConsoleLine, XE_CONSOLE_LINE_MEM_SIZE);
			m_ConsoleLinePos = 0;

			return true;
		}

		changed = true;
	}

	if (changed)
	{
		auto it = m_ConsoleCmdHistory.begin();

		for(int i = 0; i < m_CurrentCMDHistory; ++i, ++it);

		ZeroMemory(m_ConsoleLine, XE_CONSOLE_LINE_MEM_SIZE);
		memcpy(m_ConsoleLine, it->c_str(), it->size() * sizeof(wchar_t));
		m_ConsoleLinePos = (uint32_t)it->size();

		return true;
	}

	return false;
}

XEResult Console::ConsoleExecScript()
{
	int ret = 0;

	std::wstring exec(m_ConsoleLine);

	m_ConsolePrintHistoryPos = 0;

	ZeroMemory(m_ConsoleLine, XE_CONSOLE_LINE_MEM_SIZE);
	m_ConsoleLinePos = 0;

	bool addHis = true;

	if(m_ConsoleCmdHistory.size() > 0 && m_ConsoleCmdHistory.begin()->compare(exec) == 0)
	{
		addHis = false;
	}

	if(addHis)
	{
		m_ConsoleCmdHistory.push_front(exec);
		if(m_ConsoleCmdHistory.size() > XE_CONSOLE_MAX_CMD_HISTORY)
		{
			m_ConsoleCmdHistory.pop_back();
		}
	}
	m_CurrentCMDHistory = -1;

	ret = m_ConsoleContext->Prepare(m_ConsoleExecFunc);
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	ret = m_ConsoleContext->SetArgObject(0, &exec);
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	ret = m_ConsoleContext->Execute();
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	ret = m_ConsoleContext->Unprepare();
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	if(m_ScriptConsoleLineArray != nullptr)
	{
		ProcessConsoleRetArray();

		ReleaseAngel(m_ScriptConsoleLineArray);
	}

	return XEResult::Ok;
}

XEResult Console::ProcessConsoleRetArray()
{
	if(m_ScriptConsoleLineArray == nullptr)
	{
		return XEResult::NullObj;
	}

	uint32_t size = m_ScriptConsoleLineArray->GetSize();
	for(uint32_t i = 0; i < size; ++i)
	{
		ScriptConsoleLine* ret = reinterpret_cast<ScriptConsoleLine*>(m_ScriptConsoleLineArray->At(i));

		XEAssert(ret->m_SA_Colors != nullptr && ret->m_SA_Strings != nullptr);
		if(ret->m_SA_Colors == nullptr || ret->m_SA_Strings == nullptr)
		{
			return XEResult::NullObj;
		}

		XEAssert(ret->m_SA_Colors->GetSize() == ret->m_SA_Strings->GetSize());
		if(ret->m_SA_Colors->GetSize() != ret->m_SA_Strings->GetSize())
		{
			XETODO("Set better return");
			//return XEResult::WrongSize;
			return XEResult::Fail;
		}

		ConsoleShowLine newLine;

		uint32_t subSize = ret->m_SA_Strings->GetSize();
		for(uint32_t j = 0; j < subSize; ++j)
		{
			Color color = *reinterpret_cast<Color*>(ret->m_SA_Colors->At(j));
			std::wstring line = *reinterpret_cast<std::wstring*>(ret->m_SA_Strings->At(j));

			newLine.m_Colors.push_back(color);
			newLine.m_ShowCols.push_back(line);
		}

		m_ConsolePrintHistory.push_front(newLine);
	}

	while(m_ConsolePrintHistory.size() > XE_CONSOLE_MAX_PRINT_HISTORY )
	{
		m_ConsolePrintHistory.pop_back();
	}

	ReleaseAngel(m_ScriptConsoleLineArray);

	return XEResult::Ok;
}

void Console::Render(const TimerParams& timerParams)
{
	if(m_ConsoleState == ConsoleStates::Hide)
	{
		return;
	}

	m_GraphicDevice->BeginEvent(L"Console");

	//Set Alpha Blend State
	m_GraphicDevice->SetBlendState(GraphicBlendStates::m_AlphaBlendState);

	//Draw Console Background
	m_QuadColorMaterial->Apply();

	RECT size = {0, 0, m_ConsoleWidth, (uint32_t)m_CurrentConsoleHeight };
	m_GraphicDevice->DrawQuad2D(size);

	//Set Blend State to Default
	m_GraphicDevice->SetBlendState(nullptr);
	
	//Draw Console Lines if Console is Present (i.e. not entering or exiting)
	if(m_ConsoleState != ConsoleStates::Present)
	{
		m_GraphicDevice->EndEvent();

		return;
	}

	glm::vec2 stride = XEMathHelpers::Vec2fZero;
	float posX = 0.0f;
	float posY = (float)(m_ConsoleHeight - (m_CharDim.y * 2));
	uint32_t posHis = 0;

	m_SpriteBatchXE->Begin();
		
	auto it = m_ConsolePrintHistory.begin();
	auto itEnd = m_ConsolePrintHistory.end();

	while(posY >= 0 && it != itEnd)
	{
		if(m_ConsolePrintHistoryPos <= posHis)
		{
			posX = 0;

			uint32_t size = (uint32_t)it->m_ShowCols.size();
			for(uint32_t i = 0; i < size; ++i)
			{
				m_SpriteFontXE->DrawString(m_SpriteBatchXE, it->m_ShowCols[i], glm::vec2(posX, posY), it->m_Colors[i]);
				stride = m_SpriteFontXE->MeasureString(it->m_ShowCols[i]);
				posX += stride.x + m_CharDim.x;
			}
			posY -= stride.y;
		}

		++posHis;
		++it;
	}

	posX = 0;
	m_SpriteFontXE->DrawString(m_SpriteBatchXE, L">", glm::vec2(0, (float)(m_ConsoleHeight - m_CharDim.y)), m_FontColor);
	stride = m_SpriteFontXE->MeasureString(L">");
	posX += stride.x;

	if(m_ConsoleLinePos != 0)
	{
		m_SpriteFontXE->DrawString(m_SpriteBatchXE, m_ConsoleLine, glm::vec2(posX, (float)(m_ConsoleHeight - m_CharDim.y)), m_FontColor);
		stride = m_SpriteFontXE->MeasureString(m_ConsoleLine);
		posX += stride.x;
	}

	if(m_ShowNewCharUnderscore)
	{
		m_SpriteFontXE->DrawString(m_SpriteBatchXE, L"_", glm::vec2(posX, (float)(m_ConsoleHeight - m_CharDim.y)), m_FontColor);
	}

	m_SpriteBatchXE->End();

	m_GraphicDevice->EndEvent();

	DrawableGameComponent::Render(timerParams);
}

void Console::OnLostDevice()
{
	DrawableGameComponent::OnLostDevice();
}

void Console::OnResetDevice()
{
	//Set New m_ConsoleWidth
	m_ConsoleWidth = m_GraphicDevice->GetGraphicPP().m_BackBufferWidth;

	DrawableGameComponent::OnResetDevice();
}

XEResult Console::RegisterConsoleScriptInfo()
{
	int ret = 0;

	std::string sConsoleModuleName = XE_Base::WideStr2String(m_ConsoleModuleName);

	AngelScriptManager* asManager = m_GameApp->GetAngelScriptManager();

	ret = asManager->GetASEngine()->RegisterObjectType("ScriptConsoleLine", sizeof(ScriptConsoleLine), asOBJ_VALUE | asOBJ_APP_CLASS);
	if(ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	ret = asManager->GetASEngine()->RegisterObjectProperty("ScriptConsoleLine", "array<wstring>@ m_SA_Strings",  asOFFSET(ScriptConsoleLine, m_SA_Strings));
	if(ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = asManager->GetASEngine()->RegisterObjectProperty("ScriptConsoleLine", "array<Color>@ m_SA_Colors", asOFFSET(ScriptConsoleLine, m_SA_Colors));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = asManager->GetASEngine()->RegisterObjectMethod("ScriptConsoleLine", "ScriptConsoleLine &opAssign(const ScriptConsoleLine& in)", asMETHOD(ScriptConsoleLine, operator=), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	ret = asManager->GetASEngine()->RegisterObjectBehaviour("ScriptConsoleLine", asBEHAVE_CONSTRUCT, "void XEAS_ScriptConsoleLineConstructor()", asFUNCTION(ScriptConsoleLine::Constructor), asCALL_CDECL_OBJLAST);
	if(ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = asManager->GetASEngine()->RegisterObjectBehaviour("ScriptConsoleLine", asBEHAVE_DESTRUCT, "void XEAS_ScriptConsoleLineDestructor()", asFUNCTION(ScriptConsoleLine::Destructor), asCALL_CDECL_OBJLAST);
	if(ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = asManager->GetASEngine()->RegisterGlobalProperty("array<ScriptConsoleLine> @m_ScriptConsoleLineArray", &m_ScriptConsoleLineArray);
	if(ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	XETODO("Add this somewhere not here");
	if(asManager->LoadScript(L"..\\Data\\Scripts\\Console.as", m_ConsoleModuleName) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	m_ConsoleContext = asManager->GetASEngine()->CreateContext();
	if(m_ConsoleContext == nullptr)
	{
		return XEResult::Fail;
	}

	m_ConsoleExecFunc = asManager->GetASEngine()->GetModule(sConsoleModuleName.c_str())->GetFunctionByDecl("void ConsoleExec(wstring)");
	if(m_ConsoleExecFunc == nullptr)
	{
		return XEResult::Fail;
	}
	m_ConsoleExecFunc->AddRef();

	asIScriptFunction* func = asManager->GetASEngine()->GetModule(sConsoleModuleName.c_str())->GetFunctionByDecl("void InitializeConsoleScript()");
	if(func == nullptr)
	{
		return XEResult::Fail;
	}

	ret = m_ConsoleContext->Prepare(func);
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	ret = m_ConsoleContext->Execute();
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	ret = m_ConsoleContext->Unprepare();
	if(ret < 0)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}
