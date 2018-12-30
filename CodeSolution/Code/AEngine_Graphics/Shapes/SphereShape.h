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

#ifndef _SPHERE_SHAPE_H
#define _SPHERE_SHAPE_H

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

        GraphicDevice& m_GraphicDevice;

        //Temp Variables
        std::vector<VertexPositionColor> m_VtxBuff;

        std::vector<uint16_t> m_IdxBuff;

        std::map<std::string, uint32_t> m_VtxMap;

        //Methods
        uint16_t AddVtx(VertexPositionColor& vtx);
        void AddIdx(uint16_t idx);
        uint16_t GetMiddlePoint(uint32_t vtxIndex1, uint32_t vtxIndex2);

    public:
        //Constructor Destructor.
        SphereShape(GraphicDevice& graphicDevice, uint32_t recursionLevels, const Color& color = AEColors::Red, bool clockWise = false);
        virtual ~SphereShape();

        //Get Methods
        inline IVertexBuffer* GetVertexBuffer()
        {
            return m_VB;
        }

        inline IndexBuffer* GetIndexBuffer()
        {
            return m_IB;
        }

        //Framework Methods
        AEResult CreateSphere(uint32_t recursionLevels, const Color& color = AEColors::Red, bool clockWise = false);
        AEResult BuildSphere();
        AEResult DrawSphere();
};

#endif

