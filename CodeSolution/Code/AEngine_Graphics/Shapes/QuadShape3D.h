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

#ifndef _QUAD_SHAPE_3D_H
#define _QUAD_SHAPE_3D_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape.h"
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionTexture.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class IndexBuffer;
class GraphicDevice;
class IVertexBuffer;

/*****************
*   Class Decl   *
******************/
class QuadShape3D sealed : public QuadShape
{
	public:
		//Constructor Destructor.
		QuadShape3D(GraphicDevice* graphicDevice, bool clockWise = true);
		virtual ~QuadShape3D();

		//Framework Methods
		AEResult BuildQuad() override;
};

#endif

