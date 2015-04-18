/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

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
