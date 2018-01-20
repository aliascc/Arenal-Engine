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
#include "Base\AEObject.h"

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
