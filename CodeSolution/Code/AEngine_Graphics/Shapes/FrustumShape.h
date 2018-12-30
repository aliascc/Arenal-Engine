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

#ifndef _FRUSTUM_SHAPE_H
#define _FRUSTUM_SHAPE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Vertex\VertexBuffer.h"
#include "Vertex\Types\VertexPositionColor.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Frustum;
class GraphicDevice;
class IVertexBuffer;

/**************
*   Defines   *
***************/

#define AE_FRUSTUM_SIMPLE_SHAPE_SIZE    24
#define AE_FRUSTUM_SHAPE_SIZE            252

/*****************
*   Class Decl   *
******************/
class FrustumShape sealed : public AEObject
{
    private:
        //Variables
        bool m_IsReady = false;

        VertexPositionColor m_Vtx[AE_FRUSTUM_SHAPE_SIZE];

        VertexBuffer<VertexPositionColor>* m_VB = nullptr;

        GraphicDevice& m_GraphicDevice;

        AEResult UpdateFrustumShapeVtx(const Frustum* frustum);

    public:
        //Constructor Destructor.
        FrustumShape(GraphicDevice& graphicDevice);
        virtual ~FrustumShape();

        //Get Methods
        inline IVertexBuffer* GetVertexBuffer()
        {
            return m_VB;
        }

        //Framework Methods
        AEResult BuildFrustumShape();

        AEResult UpdateFrustumShape(const Frustum* frustum);

        AEResult DrawFrustumShape(bool simple = false);
};

#endif

