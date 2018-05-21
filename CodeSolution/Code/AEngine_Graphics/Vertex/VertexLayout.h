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

#ifndef _VERTEX_LAYOUT_H
#define _VERTEX_LAYOUT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class VertexLayout sealed : public Named
{
    private:
        ID3D11InputLayout* m_VertexLayoutDX = nullptr;
        bool m_IsReady = false;

    public:
        //Constructor and Destructor
        VertexLayout();
        virtual ~VertexLayout();

        //Framework Methods
        AEResult BuildVertexLayout(GraphicDevice* graphicDevice, const BYTE shaderByteCode[], uint32_t shadeByteCodeSize, const D3D11_INPUT_ELEMENT_DESC vertexDesc[], uint32_t elementCount, const std::string& name = "");

        //Get Methods
        inline ID3D11InputLayout* GetDXLayout() const 
        { 
            return m_VertexLayoutDX; 
        }

        inline bool IsReady() const
        {
            return m_IsReady;
        }
};

#endif
