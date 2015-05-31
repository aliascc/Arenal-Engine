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
#include <string>

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "DebugStats.h"
#include "Time\XETime.h"
#include "Camera\Camera.h"
#include "GameApp\GameApp.h"
#include "Base\BaseFunctions.h"
#include "Vertex\VertexBuffer.h"
#include "Camera\CameraUpdater.h"
#include "Wrappers\SpriteFontXE.h"
#include "Wrappers\SpriteBatchXE.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Graphic Extensions\Materials\Basic\BasicColorMaterial.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
DebugStats::DebugStats(GameApp* gameApp, const DebugStatsConfig& debugStatsConfig, const std::wstring& gameComponentName, const std::wstring& cameraServiceName, uint32_t callOrder)
	: DrawableGameComponent(gameApp, gameComponentName, callOrder)
	, m_DebugStatsConfig(debugStatsConfig)
{
	XETODO("Add ready flag to check this");
	m_CameraUpdater = m_GameApp->GetGameService<CameraUpdater>(cameraServiceName);
	XEAssert(m_CameraUpdater != nullptr);
}

DebugStats::~DebugStats()
{
	DeleteMem(m_SpriteFontXE);
	DeleteMem(m_SpriteBatchXE);
	DeleteMem(m_GridVertexBuffer);
	DeleteMem(m_AxisVertexBuffer);
	DeleteMem(m_BasicColorMaterial);
}

void DebugStats::Initialize()
{
	m_GridVertexBuffer = new VertexBuffer<VertexPositionColor>(m_GraphicDevice);

	m_AxisVertexBuffer = new VertexBuffer<VertexPositionColor>(m_GraphicDevice);

	XETODO("Set Define for name of material");
	m_BasicColorMaterial = new BasicColorMaterial(m_GraphicDevice, m_GameResourceManager, L"XE_DebugStats_BasicColorMaterial");
	
	m_SpriteBatchXE = new SpriteBatchXE(m_GraphicDevice);

	m_SpriteFontXE = new SpriteFontXE(m_GraphicDevice, m_DebugStatsConfig.m_SpriteFontFile);

	DrawableGameComponent::Initialize();
}

void DebugStats::LoadContent()
{	
	InitializeAxisAndGrid();
	
	XETODO("Need to check ret for Load Content");
	m_SpriteBatchXE->Initialize();

	m_SpriteFontXE->Initialize();

	m_BasicColorMaterial->LoadContent();	
	XETODO("Check to see if we can implement easy access to variables in BasicColorMaterial class, if so remove all includes for CB, shader props");
	m_BasicColorMaterial->GetVSProps()->GetConstantBuffer(L"_XE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_XE_World", XEMathHelpers::Mat4Identity);

	DrawableGameComponent::LoadContent();
}

void DebugStats::InitializeAxisAndGrid()
{
	//Init Axis
	VertexPositionColor axis[6];
	ZeroMemory(axis, VertexPositionColor::VertexSize() * 6);
	
	//X
	axis[0].m_Position.x	= 0.0f;
	axis[0].m_Position.y	= 0.0f;
	axis[0].m_Position.z	= 0.0f;
	axis[0].m_Color			= XEColors::Red;

	axis[1].m_Position.x	= m_DebugStatsConfig.m_AxisLength;
	axis[1].m_Position.y	= 0.0f;
	axis[1].m_Position.z	= 0.0f;
	axis[1].m_Color			= XEColors::Red;

	//Y
	axis[2].m_Position.x	= 0.0f;
	axis[2].m_Position.y	= 0.0f;
	axis[2].m_Position.z	= 0.0f;
	axis[2].m_Color			= XEColors::Green;

	axis[3].m_Position.x	= 0.0f;
	axis[3].m_Position.y	= m_DebugStatsConfig.m_AxisLength;
	axis[3].m_Position.z	= 0.0f;
	axis[3].m_Color			= XEColors::Green;

	//Z
	axis[4].m_Position.x	= 0.0f;
	axis[4].m_Position.y	= 0.0f;
	axis[4].m_Position.z	= 0.0f;
	axis[4].m_Color			= XEColors::Blue;

	axis[5].m_Position.x	= 0.0f;
	axis[5].m_Position.y	= 0.0f;
	axis[5].m_Position.z	= m_DebugStatsConfig.m_AxisLength;
	axis[5].m_Color			= XEColors::Blue;
	
	m_AxisVertexBuffer->CopyVertexBuffer(axis, 6);

	//Init Grid
	uint32_t linesPerSide = (uint32_t)(m_DebugStatsConfig.m_GridSize / m_DebugStatsConfig.m_GridUnits);
	uint32_t totalLines = linesPerSide * 2;
	uint32_t vbSize = (totalLines * 2) + 4;
	float startGrid = -(m_DebugStatsConfig.m_GridUnits * linesPerSide) / 2;

	VertexPositionColor* lines = new VertexPositionColor[vbSize];
	ZeroMemory(lines, VertexPositionColor::VertexSize() * (vbSize));

	uint32_t i = 0;
	for(uint32_t lineCount = 0; lineCount <= linesPerSide; i += 2, ++lineCount)
	{
		lines[i].m_Position.x = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
		lines[i].m_Position.y = 0.0f;
		lines[i].m_Position.z = (-m_DebugStatsConfig.m_GridSize / 2.0f);
		lines[i].m_Color = (lines[i].m_Position.x != 0) ? XEColors::Gray : XEColors::Black;

		lines[i + 1].m_Position.x = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
		lines[i + 1].m_Position.y = 0.0f;
		lines[i + 1].m_Position.z = (m_DebugStatsConfig.m_GridSize / 2.0f);
		lines[i + 1].m_Color = (lines[i + 1].m_Position.x != 0) ? XEColors::Gray : XEColors::Black;
	}

	for(uint32_t lineCount = 0; lineCount <= linesPerSide; i += 2, ++lineCount)
	{
		lines[i].m_Position.x = (-m_DebugStatsConfig.m_GridSize / 2.0f);
		lines[i].m_Position.y = 0.0f;
		lines[i].m_Position.z = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
		lines[i].m_Color = (lines[i].m_Position.z != 0) ? XEColors::Gray : XEColors::Black;

		lines[i + 1].m_Position.x = (m_DebugStatsConfig.m_GridSize / 2.0f);
		lines[i + 1].m_Position.y = 0.0f;
		lines[i + 1].m_Position.z = startGrid + (lineCount * m_DebugStatsConfig.m_GridUnits);
		lines[i + 1].m_Color = (lines[i + 1].m_Position.z != 0) ? XEColors::Gray : XEColors::Black;
	}

	m_GridVertexBuffer->CopyVertexBuffer(lines, vbSize);

	DeleteMemArr(lines);
}

void DebugStats::Update(const TimerParams& timerParams)
{
	XETODO("move all this to post update or beginning of render");

	XETODO("Do check for this and var to know if everything is init");
	Camera* currentCamera = m_CameraUpdater->GetMainCamera();
	XEAssert(currentCamera != nullptr);

	XETODO("Check to see if we can implement easy access to variables in BasicLineMaterial & BasicColorMaterial class, if so remove all includes for CB, shader props");
	m_BasicColorMaterial->GetVSProps()->GetConstantBuffer(L"_XE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_XE_View", currentCamera->GetViewMatrix());
	m_BasicColorMaterial->GetVSProps()->GetConstantBuffer(L"_XE_CB_World_View_Proj")->SetValueT<glm::mat4>(L"_XE_Projection", currentCamera->GetProjectionMatrix());

	DrawableGameComponent::Update(timerParams);
}

void DebugStats::Render(const TimerParams& timerParams)
{
	glm::vec2 stride(0.0f, 0.0f);
	glm::vec2 pos(10.0f, 10.0f);
	std::wstring msg = L"";

	m_GraphicDevice->BeginEvent(L"Debug Stats");

	if(m_DebugStatsConfig.m_GridEnabled || m_DebugStatsConfig.m_AxisEnabled)
	{
		m_GraphicDevice->BeginEvent(L"Debug Grid Axis");

		//Set Topology to LineList for Both Axis and Grid
		m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

		//Draw Grid
		if(m_DebugStatsConfig.m_GridEnabled)
		{
			m_GraphicDevice->SetVertexBuffer(m_GridVertexBuffer);

			m_BasicColorMaterial->Apply();

			m_GraphicDevice->Draw(m_GridVertexBuffer->GetSize(), 0);
		}

		//Draw Axis
		if(m_DebugStatsConfig.m_AxisEnabled)
		{
			m_GraphicDevice->SetVertexBuffer(m_AxisVertexBuffer);

			m_BasicColorMaterial->Apply();

			m_GraphicDevice->Draw(m_AxisVertexBuffer->GetSize(), 0);
		}

		m_GraphicDevice->EndEvent();
	}

	if(m_DebugStatsConfig.m_FPSEnabled)
	{
		m_GraphicDevice->BeginEvent(L"Debug Text");

		m_SpriteBatchXE->Begin();

		XETODO("Change to Localization Literals");
		msg = fmt::format(L"FPS: {0}", m_GameApp->GetTimer().GetFPS());
		m_SpriteFontXE->DrawString(m_SpriteBatchXE, msg, pos, m_DebugStatsConfig.m_TextColor);
		stride = m_SpriteFontXE->MeasureString(msg);
		pos.y += stride.y;

		msg = fmt::format(L"Milliseconds/Frame: {0}", m_GameApp->GetTimer().GetMilliPerFrame());
		m_SpriteFontXE->DrawString(m_SpriteBatchXE, msg, pos, m_DebugStatsConfig.m_TextColor);
		stride = m_SpriteFontXE->MeasureString(msg);
		pos.y += stride.y;

		m_SpriteBatchXE->End();
	
		m_GraphicDevice->EndEvent();
	}
	
	m_GraphicDevice->EndEvent();
	
	DrawableGameComponent::Render(timerParams);
}

void DebugStats::OnLostDevice()
{	
	DrawableGameComponent::OnLostDevice();
}

void DebugStats::OnResetDevice()
{
	DrawableGameComponent::OnResetDevice();
}