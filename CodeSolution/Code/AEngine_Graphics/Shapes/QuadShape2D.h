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

#ifndef _QUAD_SHAPE_2D_H
#define _QUAD_SHAPE_2D_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "QuadShape.h"
#include "Base\Base.h"
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
class QuadShape2D sealed : public QuadShape
{
    private:
        //Variables
        bool m_Dynamic = false;

        glm::vec4 m_CurrentTexCoords = AEMathHelpers::Vec4fZero;

        RECT m_CurrentSize;

    public:
        //Constructor Destructor.
        QuadShape2D(GraphicDevice* graphicDevice, bool dynamic = false, bool clockWise = true);
        virtual ~QuadShape2D();

        //Framework Methods
        AEResult Resize(const RECT& size, const glm::vec4& textCoords);

        AEResult BuildQuad() override;
};

#endif

