/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MESH_PART_H
#define _MESH_PART_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
class IVertexBuffer;
class IndexBuffer;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class MeshPart : public AEObject
{
	protected:
		//Variables
		uint32_t m_TexMaterialID = 0;

		IVertexBuffer* m_VertexBuffer = nullptr;

		IndexBuffer* m_IndexBuffer = nullptr;

	public:

		//Constructor Destructor.
		MeshPart();
		virtual ~MeshPart();

		//Sets
		void					SetVertexBuffer			(IVertexBuffer* vb);
		void					SetIndexBuffer			(IndexBuffer* ib);

		inline void SetTexMaterialID(uint32_t texMaterialID)
		{
			m_TexMaterialID = texMaterialID;
		}

		//Gets
		inline IVertexBuffer*		GetVertexBuffer			() const								{ return m_VertexBuffer; }
		inline IndexBuffer*			GetIndexBuffer			() const								{ return m_IndexBuffer; }
		inline uint32_t				GetTexMaterialID		() const								{ return m_TexMaterialID; }

		//Framework Methods
};

#endif

