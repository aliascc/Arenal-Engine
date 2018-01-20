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

#ifndef _IVERTEX_BUFFER_H
#define _IVERTEX_BUFFER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\Named.h"
#include "VertexDefs.h"
#include "GraphicsDefs.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"

/********************
*   Forward Decls   *
*********************/
class VertexLayout;

/*****************
*   Class Decl   *
******************/
class IVertexBuffer abstract : public Named
{    
    protected:

        /****************************
         *   Protected Variables    *
         ****************************/
#pragma region Protected Variables

        /// <summary>
        /// Vertex Buffer Ready State
        /// </summary>
        bool m_IsReady = false;
        
        /// <summary>
        /// Size of Vertex Buffer
        /// </summary>
        uint32_t m_Size = 0;

        /// <summary>
        /// DirectX 11 Vertex Buffer
        /// </summary>
        ID3D11Buffer* m_VertexBufferDX = nullptr;
        
        /// <summary>
        /// Buffer Access of the CPU for the Vertex Buffer
        /// </summary>
        GraphicBufferAccess m_BufferAccess = GraphicBufferAccess::None;
        
        /// <summary>
        /// Buffer Usage for the Vertex Buffer, for example: GraphicBufferUsage::Dynamic
        /// </summary>
        GraphicBufferUsage m_BufferUsage = GraphicBufferUsage::Default;

        /// <summary>
        /// Graphic Device associated with the Vertex Buffer
        /// </summary>
        GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// IVertexBuffer Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Vertex Buffer</param>
        /// <param name="memPool">How will the buffer be use</param>
        /// <param name="memPool">How will the CPU Access the memory of the buffer</param>
        IVertexBuffer(GraphicDevice* graphicDevice, GraphicBufferUsage bufferUsage = GraphicBufferUsage::Static, GraphicBufferAccess bufferAccess = GraphicBufferAccess::None)
            : Named(L"")
            , m_BufferAccess(bufferAccess)
            , m_BufferUsage(bufferUsage)
            , m_GraphicDevice(graphicDevice)
        {
            AEAssert(graphicDevice != nullptr);
        }
        
        /// <summary>
        /// Default IVertexBuffer Destructor
        /// </summary>
        virtual ~IVertexBuffer()
        {
            ReleaseCOM(m_VertexBufferDX);
        }

#pragma endregion
        
        /**********************
         *     Get Methods    *
         **********************/
#pragma region Get Methods

        /// <summary>
        /// Returns DirectX Vertex Buffer
        /// </summary>
        /// <returns>DirectX Vertex Buffer</returns>
        inline ID3D11Buffer* GetDXBuffer() const
        {
            return m_VertexBufferDX;
        }

        /// <summary>
        /// Returns if the Vertex Buffer is ready
        /// </summary>
        /// <returns>True if Vertex Buffer is ready</returns>
        inline bool IsReady() const
        { 
            return m_IsReady; 
        }

        /// <summary>
        /// Returns Size of Vertex Buffer
        /// </summary>
        /// <returns>Size of Vertex Buffer</returns>
        inline uint32_t GetSize() const
        { 
            return m_Size; 
        }
        
        /// <summary>
        /// Returns CPU Access flag of Vertex Buffer
        /// </summary>
        /// <returns>CPU Access flag of Vertex Buffer</returns>
        inline GraphicBufferAccess GetBufferAccess() const
        { 
            return m_BufferAccess;
        }
        
        /// <summary>
        /// Returns Usage for Vertex Buffer
        /// </summary>
        /// <returns>Usage for Vertex Buffer</returns>
        inline GraphicBufferUsage GetBufferUsage() const
        {
            return m_BufferUsage; 
        }        
        
        /// <summary>
        /// Returns Vertex Type for this Buffer
        /// </summary>
        /// <returns>Vertex Type for Buffer</returns>
        virtual VertexType GetVertexType() const = 0;

#pragma endregion
        
        /**********************
         *     Set Methods    *
         **********************/
#pragma region Set Methods

#pragma endregion
        

        /**************************
         *   Framework Methods    *
         **************************/
#pragma region Framework Methods
        
        /// <summary>
        /// Builds the Vertex Buffer.
        /// </summary>
        /// <returns>AEResult::Ok if Build Vertex Buffer succeeded</returns>
        virtual AEResult BuildVertexBuffer() = 0;

        /// <summary>
        /// Size of the Vertex that the Vertex Buffer has
        /// </summary>
        /// <returns>Vertex Size</returns>
        virtual uint32_t VertexSize() const = 0;

        /// <summary>
        /// Vertex Layout of the Vertex that the Vertex Buffer has
        /// </summary>
        /// <returns>Vertex Layout of the Vertex</returns>
        virtual const VertexLayout* GetVertexLayout() = 0;

#pragma endregion

};

#endif
