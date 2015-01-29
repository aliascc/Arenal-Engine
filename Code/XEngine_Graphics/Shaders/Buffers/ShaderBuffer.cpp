
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/7/2013
* 
* Desc:
*
*
* Copyright 2013
*
*********************************************************/


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
#include "Base\BaseFunctions.h"
#include "Shaders\Buffers\ConstantBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderBuffer::ShaderBuffer(const std::wstring& name, uint32_t bindIndex, bool createAsRW, GraphicDevice* graphicDevice)
	: ShaderBinding(name, bindIndex)
	, m_GraphicDevice(graphicDevice)
	, m_IsRWEnabled(createAsRW)
{
	XEAssert(m_GraphicDevice != nullptr);
	XEAssert(!name.empty());
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
