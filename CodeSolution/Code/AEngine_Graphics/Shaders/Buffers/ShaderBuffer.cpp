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
#include "ShaderBuffer.h"
#include "GraphicDevice.h"
#include "Shaders\Buffers\ConstantBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderBuffer::ShaderBuffer(const std::string& name, uint32_t bindIndex, bool createAsRW, GraphicDevice* graphicDevice)
    : ShaderBinding(name, bindIndex)
    , m_GraphicDevice(graphicDevice)
    , m_IsRWEnabled(createAsRW)
{
    AEAssert(m_GraphicDevice != nullptr);
    AEAssert(!name.empty());
}

ShaderBuffer::~ShaderBuffer()
{
    CleanUp();
}

void ShaderBuffer::CleanUp()
{
    ReleaseCOM(m_BufferSRVDX);
    ReleaseCOM(m_BufferDX);
    ReleaseCOM(m_BufferUAVDX);
}
