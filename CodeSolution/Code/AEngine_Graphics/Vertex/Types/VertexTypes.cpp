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
#include "VertexTypes.h"
#include "Vertex\VertexLayout.h"

/********************
*   Variable Decl   *
*********************/
//Initialize VL to nullptr
VertexLayout* VertexPosition::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionColor::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormal::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionTexture::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalColor::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalTexture::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalTextureTexture2::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalTextureIndicesWeight::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalTangentBinormalTexture::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalTangentBinormalTextureTexture2::m_VertexLayout = nullptr;

//Initialize VL to nullptr
VertexLayout* VertexPositionNormalTangentBinormalTextureIndicesWeight::m_VertexLayout = nullptr;
