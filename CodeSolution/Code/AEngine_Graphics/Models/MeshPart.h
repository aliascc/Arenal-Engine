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

