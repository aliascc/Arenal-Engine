/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 3/4/2014
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\gtx\compatibility.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "FrustumShape.h"
#include "Utils\Frustum.h"
#include "GraphicDevice.h"
#include "Math\XEMathDefs.h"
#include "Base\BaseFunctions.h"
#include "Color\XEColorDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
FrustumShape::FrustumShape(GraphicDevice* graphicDevice)
	: m_GraphicDevice(graphicDevice)
{
	XEAssert(m_GraphicDevice != nullptr);

	memset(m_Vtx, 0, sizeof(VertexPositionColor) * 24);
}

FrustumShape::~FrustumShape()
{
	DeleteMem(m_VB);
}

XEResult FrustumShape::BuildFrustumShape()
{
	DeleteMem(m_VB);
	m_IsReady = false;

	memset(m_Vtx, 0, sizeof(VertexPositionColor) * XE_FRUSTUM_SHAPE_SIZE);

	//Near Plane
	m_Vtx[0].m_Color = XEColors::Red;
	m_Vtx[1].m_Color = XEColors::Red;
	m_Vtx[2].m_Color = XEColors::Red;
	m_Vtx[3].m_Color = XEColors::Red;
	m_Vtx[4].m_Color = XEColors::Red;
	m_Vtx[5].m_Color = XEColors::Red;
	m_Vtx[6].m_Color = XEColors::Red;
	m_Vtx[7].m_Color = XEColors::Red;

	//Far Plane
	m_Vtx[8].m_Color = XEColors::Cyan;
	m_Vtx[9].m_Color = XEColors::Cyan;
	m_Vtx[10].m_Color = XEColors::Cyan;
	m_Vtx[11].m_Color = XEColors::Cyan;
	m_Vtx[12].m_Color = XEColors::Cyan;
	m_Vtx[13].m_Color = XEColors::Cyan;
	m_Vtx[14].m_Color = XEColors::Cyan;
	m_Vtx[15].m_Color = XEColors::Cyan;

	//Left Plane
	m_Vtx[16].m_Color = XEColors::White;
	m_Vtx[17].m_Color = XEColors::White;
	m_Vtx[18].m_Color = XEColors::White;
	m_Vtx[19].m_Color = XEColors::White;

	//Right Plane
	m_Vtx[20].m_Color = XEColors::White;
	m_Vtx[21].m_Color = XEColors::White;
	m_Vtx[22].m_Color = XEColors::White;
	m_Vtx[23].m_Color = XEColors::White;

	for (uint32_t x = 0, i = 24; x < 19; ++x)
	{
		//Near Plane
		m_Vtx[i].m_Color		= XEColors::Red;
		m_Vtx[i + 1].m_Color	= XEColors::Red;
		i += 2;

		//Far Plane
		m_Vtx[i].m_Color		= XEColors::Cyan;
		m_Vtx[i + 1].m_Color	= XEColors::Cyan;
		i += 2;

		//Right Plane
		m_Vtx[i].m_Color		= XEColors::White;
		m_Vtx[i + 1].m_Color	= XEColors::White;
		i += 2;

		//Left Plane
		m_Vtx[i].m_Color		= XEColors::White;
		m_Vtx[i + 1].m_Color	= XEColors::White;
		i += 2;

		//Top
		m_Vtx[i].m_Color		= XEColors::Yellow;
		m_Vtx[i + 1].m_Color	= XEColors::Yellow;
		i += 2;

		//Bottom
		m_Vtx[i].m_Color		= XEColors::Yellow;
		m_Vtx[i + 1].m_Color	= XEColors::Yellow;
		i += 2;
	}

	m_VB = new VertexBuffer<VertexPositionColor>(m_GraphicDevice, GraphicBufferUsage::Dynamic, GraphicBufferAccess::Write);
	m_VB->CopyVertexBuffer(m_Vtx, XE_FRUSTUM_SHAPE_SIZE);
	m_VB->BuildVertexBuffer();

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult FrustumShape::UpdateFrustumShapeVtx(const Frustum* frustum)
{
	XEAssert(frustum != nullptr);
	if (frustum == nullptr)
	{
		return XEResult::NullObj;
	}

	const FrustumCorners& corners = frustum->GetCorners();

	//Near Plane
	m_Vtx[0].m_Position = corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT];
	m_Vtx[1].m_Position = corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT];
	m_Vtx[2].m_Position = corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT];
	m_Vtx[3].m_Position = corners[XE_FRUSTUM_NEAR_TOP_RIGHT];
	m_Vtx[4].m_Position = corners[XE_FRUSTUM_NEAR_TOP_RIGHT];
	m_Vtx[5].m_Position = corners[XE_FRUSTUM_NEAR_TOP_LEFT];
	m_Vtx[6].m_Position = corners[XE_FRUSTUM_NEAR_TOP_LEFT];
	m_Vtx[7].m_Position = corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT];

	//Far Plane
	m_Vtx[8].m_Position = corners[XE_FRUSTUM_FAR_BOTTOM_LEFT];
	m_Vtx[9].m_Position = corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT];
	m_Vtx[10].m_Position = corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT];
	m_Vtx[11].m_Position = corners[XE_FRUSTUM_FAR_TOP_RIGHT];
	m_Vtx[12].m_Position = corners[XE_FRUSTUM_FAR_TOP_RIGHT];
	m_Vtx[13].m_Position = corners[XE_FRUSTUM_FAR_TOP_LEFT];
	m_Vtx[14].m_Position = corners[XE_FRUSTUM_FAR_TOP_LEFT];
	m_Vtx[15].m_Position = corners[XE_FRUSTUM_FAR_BOTTOM_LEFT];

	//Left Plane
	m_Vtx[16].m_Position = corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT];
	m_Vtx[17].m_Position = corners[XE_FRUSTUM_FAR_BOTTOM_LEFT];
	m_Vtx[18].m_Position = corners[XE_FRUSTUM_NEAR_TOP_LEFT];
	m_Vtx[19].m_Position = corners[XE_FRUSTUM_FAR_TOP_LEFT];

	//Right Plane
	m_Vtx[20].m_Position = corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT];
	m_Vtx[21].m_Position = corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT];
	m_Vtx[22].m_Position = corners[XE_FRUSTUM_NEAR_TOP_RIGHT];
	m_Vtx[23].m_Position = corners[XE_FRUSTUM_FAR_TOP_RIGHT];

	for (uint32_t x = 0, i = 24; x < 19; ++x)
	{
		//NearPlane
		m_Vtx[i].m_Position.y		= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].y,		corners[XE_FRUSTUM_NEAR_TOP_LEFT].y,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.y	= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].y,	corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].y,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.x		= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].x,		corners[XE_FRUSTUM_NEAR_TOP_LEFT].x,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.x	= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].x,	corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].x,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.z		= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].z,		corners[XE_FRUSTUM_NEAR_TOP_LEFT].z,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.z	= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].z,	corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].z,	((float)x + 1.0f) / 20.0f);

		i += 2;

		//FarPlane
		m_Vtx[i].m_Position.y		= glm::lerp(corners[XE_FRUSTUM_FAR_TOP_RIGHT].y,		corners[XE_FRUSTUM_FAR_TOP_LEFT].y,		((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.y	= glm::lerp(corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].y,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].y,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.x		= glm::lerp(corners[XE_FRUSTUM_FAR_TOP_RIGHT].x,		corners[XE_FRUSTUM_FAR_TOP_LEFT].x,		((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.x	= glm::lerp(corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].x,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].x,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.z		= glm::lerp(corners[XE_FRUSTUM_FAR_TOP_RIGHT].z,		corners[XE_FRUSTUM_FAR_TOP_LEFT].z,		((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.z	= glm::lerp(corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].z,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].z,	((float)x + 1.0f) / 20.0f);

		i += 2;

		//RightPlane
		m_Vtx[i].m_Position.y		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].y,	corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].y,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.y	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].y,		corners[XE_FRUSTUM_FAR_TOP_RIGHT].y,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.x		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].x,	corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].x,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.x	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].x,		corners[XE_FRUSTUM_FAR_TOP_RIGHT].x,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.z		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].z,	corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].z,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.z	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].z,		corners[XE_FRUSTUM_FAR_TOP_RIGHT].z,	((float)x + 1.0f) / 20.0f);

		i += 2;

		//LeftPlane
		m_Vtx[i].m_Position.y		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].y,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].y,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.y	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_LEFT].y,		corners[XE_FRUSTUM_FAR_TOP_LEFT].y,		((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.x		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].x,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].x,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.x	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_LEFT].x,		corners[XE_FRUSTUM_FAR_TOP_LEFT].x,		((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.z		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].z,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].z,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.z	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_LEFT].z,		corners[XE_FRUSTUM_FAR_TOP_LEFT].z,		((float)x + 1.0f) / 20.0f);

		i += 2;

		//Top
		m_Vtx[i].m_Position.y		= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].y,		corners[XE_FRUSTUM_FAR_TOP_RIGHT].y,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.y	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_LEFT].y,		corners[XE_FRUSTUM_FAR_TOP_LEFT].y,		((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.x		= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].x,		corners[XE_FRUSTUM_FAR_TOP_RIGHT].x,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.x	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_LEFT].x,		corners[XE_FRUSTUM_FAR_TOP_LEFT].x,		((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.z		= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_RIGHT].z,		corners[XE_FRUSTUM_FAR_TOP_RIGHT].z,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.z	= glm::lerp(corners[XE_FRUSTUM_NEAR_TOP_LEFT].z,		corners[XE_FRUSTUM_FAR_TOP_LEFT].z,		((float)x + 1.0f) / 20.0f);

		i += 2;

		//Bottom
		m_Vtx[i].m_Position.y		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].y,	corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].y,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.y	= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].y,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].y,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.x		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].x,	corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].x,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.x	= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].x,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].x,	((float)x + 1.0f) / 20.0f);

		m_Vtx[i].m_Position.z		= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT].z,	corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT].z,	((float)x + 1.0f) / 20.0f);
		m_Vtx[i + 1].m_Position.z	= glm::lerp(corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT].z,		corners[XE_FRUSTUM_FAR_BOTTOM_LEFT].z,	((float)x + 1.0f) / 20.0f);

		i += 2;
	}


	return XEResult::Ok;
}

XEResult FrustumShape::UpdateFrustumShape(const Frustum* frustum)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEAssert(frustum != nullptr);
	if (frustum == nullptr)
	{
		return XEResult::NullObj;
	}

	XETODO("Change returns");
	if (UpdateFrustumShapeVtx(frustum) != XEResult::Ok)
	{
		return XEResult::Ok;
	}

	if (m_VB->SetVertexData(0, m_Vtx, 0, XE_FRUSTUM_SHAPE_SIZE, D3D11_MAP_WRITE_DISCARD) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult FrustumShape::DrawFrustumShape(bool simple)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XETODO("add better return codes");
	if (m_GraphicDevice->SetVertexBuffer(m_VB) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	if (m_GraphicDevice->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	uint32_t vtxSize = m_VB->GetSize();

	if (simple)
	{
		vtxSize = XE_FRUSTUM_SIMPLE_SHAPE_SIZE;
	}

	if (m_GraphicDevice->Draw(vtxSize, 0) != XEResult::Ok)
	{
		return XEResult::Fail;
	}

	return XEResult::Ok;
}
