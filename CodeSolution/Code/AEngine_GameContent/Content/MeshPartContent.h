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

#ifndef _MESH_PART_CONTENT_H
#define _MESH_PART_CONTENT_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameContent.h"
#include "Base\AEObject.h"
#include "Vertex\VertexDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct IVertexBufferContent;

/******************
*   Struct Decl   *
*******************/
struct MeshPartContent sealed : public GameContent
{
	IVertexBufferContent* m_VtxBuff = nullptr;

	uint32_t m_MatID = 0;

	VertexType m_VertexType = VertexType::VtxNone;

	uint16_t* m_IdxBuf16 = nullptr;

	uint32_t m_IdxSize = 0;

	MeshPartContent();
	virtual ~MeshPartContent();
};

#endif