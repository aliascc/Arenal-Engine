/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SPHERE_SHAPE_H
#define _SPHERE_SHAPE_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Color\Color.h"
#include "Base\AEObject.h"
#include "Color\AEColorDefs.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IndexBuffer;
class IVertexBuffer;

/*****************
*   Class Decl   *
******************/
class SphereShape sealed : public AEObject
{
	private:
		//Variables
		Color m_Color = AEColors::White;

		uint32_t m_RecursionLevels = 1;

		bool m_IsReady = false;

		bool m_ClockWise = false;

		VertexBuffer<VertexPositionColor>* m_VB = nullptr;

		IndexBuffer* m_IB = nullptr;

		GraphicDevice* m_GraphicDevice = nullptr;

		//Temp Variables
		std::vector<VertexPositionColor> m_VtxBuff;

		std::vector<uint16_t> m_IdxBuff;

		std::map<std::wstring, uint32_t> m_VtxMap;

		//Methods
		uint16_t AddVtx(VertexPositionColor& vtx);
		void AddIdx(uint16_t idx);
		uint16_t GetMiddlePoint(uint32_t vtxIndex1, uint32_t vtxIndex2);

	public:
		//Constructor Destructor.
		SphereShape(GraphicDevice* graphicDevice, uint32_t recursionLevels, const Color& color = AEColors::Red, bool clockWise = false);
		virtual ~SphereShape();

		//Get Methods
		IVertexBuffer*						GetVertexBuffer		()									{ return m_VB; }
		IndexBuffer*						GetIndexBuffer		()									{ return m_IB; }

		//Framework Methods
		AEResult							CreateSphere		(uint32_t recursionLevels, const Color& color = AEColors::Red, bool clockWise = false);
		AEResult							BuildSphere			();
		AEResult							DrawSphere			();
};

#endif

