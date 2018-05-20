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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "MeshPart.h"
#include "GraphicDevice.h"
#include "Vertex\VertexDefs.h"
#include "Base\BaseFunctions.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\VertexBuffer.h"
#include "Vertex\IVertexBuffer.h"
#include "Vertex\Types\VertexPositionNormalColor.h"
#include "Vertex\Types\VertexPositionNormalTexture.h"
#include "Vertex\Types\VertexPositionNormalTextureTexture2.h"
#include "Vertex\Types\VertexPositionNormalTangentBinormalTexture.h"
#include "Vertex\Types\VertexPositionNormalTangentBinormalTextureTexture2.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
MeshPart::MeshPart()
{
}

MeshPart::~MeshPart()
{
    DeleteMem(m_IndexBuffer);
    DeleteMem(m_VertexBuffer);
}

void MeshPart::SetVertexBuffer(IVertexBuffer* vb)
{
    AEAssert(vb != nullptr);

    if(vb == nullptr)
    {
        return;
    }

    DeleteMem(m_VertexBuffer);

    m_VertexBuffer = vb;
}

void MeshPart::SetIndexBuffer(IndexBuffer* ib)
{
    AEAssert(ib != nullptr);

    if(ib == nullptr)
    {
        return;
    }

    DeleteMem(m_IndexBuffer);

    m_IndexBuffer = ib;
}
