/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _VERTEX_BUFFER_CONTENT_H
#define _VERTEX_BUFFER_CONTENT_H

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
#include "Base\Base.h"
#include "GameContent.h"
#include "Base\XEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct IVertexBufferContent abstract : public GameContent
{
	uint32_t m_Size = 0;

	IVertexBufferContent();
	virtual ~IVertexBufferContent();

	virtual uint32_t GetSizeOfVertex() = 0;

	virtual void* GetBuffer() = 0;
};

template <class T>
struct VertexBufferContent : public IVertexBufferContent
{
	T* m_Buffer = nullptr;

	VertexBufferContent()
	{
	}

	virtual ~VertexBufferContent()
	{
		DeleteMem(m_Buffer);
	}

	uint32_t GetSizeOfVertex() override
	{
		return T::VertexSize();
	}
	
	void* GetBuffer() override
	{
		return (void*)m_Buffer;
	}
};

#endif