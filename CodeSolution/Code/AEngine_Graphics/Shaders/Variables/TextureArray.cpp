/*
* Copyright (c) 2018 <Carlos Chac�n>
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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "TextureArray.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
TextureArray::TextureArray(GraphicDevice* graphicDevice, const std::wstring& name, TextureType textureType, uint32_t bindIndex)
	: ShaderBinding(name, bindIndex)
	, m_GraphicDevice(graphicDevice)
	, m_TextureType(textureType)
{
	AEAssert(m_GraphicDevice != nullptr);
}

TextureArray::~TextureArray()
{
	ReleaseCOM(m_TextureArraySRV);
}