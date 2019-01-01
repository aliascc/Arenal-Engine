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

#ifndef _VERTEX_PARTICLE_H
#define _VERTEX_PARTICLE_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Vertex\VertexDeclaration.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/********************************
*		Struct Decl				*
*********************************/
struct VertexParticle
{       
	// Stores which corner of the particle quad this vertex represents.
    int16_t m_Corner_X;
	int16_t m_Corner_Y;

	// Stores the starting position of the particle
	float m_x;
	float m_y;
	float m_z;
	
    // Stores the starting velocity of the particle.
	float m_v_x;
	float m_v_y;
	float m_v_z;
	
    // The time (in seconds) at which this particle was created.
	float m_Time;
	
    // Four random values, used to make each particle look slightly different.
	unsigned long m_RandomColorRGBA;
	
	//Vertex Declaration
	static VertexDeclaration* m_VertexDeclaration;

	//Get Vertex Declaration
	static const VertexDeclaration* GetVertexDeclaration(GraphicDevice* graphicDevice)
	{
		if(m_VertexDeclaration != nullptr)
		{
			return m_VertexDeclaration;
		}

		m_VertexDeclaration = VertexDeclaration::CreateVertexDeclarationInst();

		m_VertexDeclaration->PushBackElement(VertexElement(0, 0, AE_VVT_SHORT2, AE_DECLM_DEFAULT, AE_VU_POSITION, 0));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 4, AE_VVT_FLOAT3, AE_DECLM_DEFAULT, AE_VU_POSITION, 1));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 16, AE_VVT_FLOAT3, AE_DECLM_DEFAULT, AE_VU_NORMAL, 0));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 28, AE_VVT_FLOAT, AE_DECLM_DEFAULT, AE_VU_TEXCOORD, 0));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 32, AE_VVT_COLOR, AE_DECLM_DEFAULT, AE_VU_COLOR, 0));

		m_VertexDeclaration->BuildVertexDeclaration(graphicDevice);

		return m_VertexDeclaration;
	}

	//Delete Vertex Declaration
	static void DeleteVertexDeclaration()
	{
		DeleteMem(m_VertexDeclaration);
	}

	static AE_VertexType GetVertexType()
	{
		return AE_VERTEX_TYPE_CUSTOM | AE_VERTEX_TYPE_PARTICLE;
	}

	static uint32_t VertexSize()
	{
		return sizeof(VertexParticle);
	}
};

#endif