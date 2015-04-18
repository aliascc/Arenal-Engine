
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 2/21/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
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
#include "Base\Base.h"
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

		m_VertexDeclaration->PushBackElement(VertexElement(0, 0, XE_VVT_SHORT2, XE_DECLM_DEFAULT, XE_VU_POSITION, 0));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 4, XE_VVT_FLOAT3, XE_DECLM_DEFAULT, XE_VU_POSITION, 1));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 16, XE_VVT_FLOAT3, XE_DECLM_DEFAULT, XE_VU_NORMAL, 0));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 28, XE_VVT_FLOAT, XE_DECLM_DEFAULT, XE_VU_TEXCOORD, 0));
		m_VertexDeclaration->PushBackElement(VertexElement(0, 32, XE_VVT_COLOR, XE_DECLM_DEFAULT, XE_VU_COLOR, 0));

		m_VertexDeclaration->BuildVertexDeclaration(graphicDevice);

		return m_VertexDeclaration;
	}

	//Delete Vertex Declaration
	static void DeleteVertexDeclaration()
	{
		DeleteMem(m_VertexDeclaration);
	}

	static XE_VertexType GetVertexType()
	{
		return XE_VERTEX_TYPE_CUSTOM | XE_VERTEX_TYPE_PARTICLE;
	}

	static uint32_t VertexSize()
	{
		return sizeof(VertexParticle);
	}
};

#endif