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

#ifndef _INDEX_BUFFER_H
#define _INDEX_BUFFER_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "VertexDefs.h"
#include "GraphicsDefs.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class IndexBuffer sealed : public Named
{
    private:

        /*************************
         *   Private Variables   *
         *************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX 11 Index Buffer
        /// </summary>
        ID3D11Buffer* m_IndexBufferDX = nullptr;

        /// <summary>
        /// Index Buffer Type
        /// </summary>
        IndexBufferType m_IBType = IndexBufferType::Index16;

        /// <summary>
        /// Index Buffer (RAM) for 32bit Indexes
        /// </summary>
        uint32_t* m_IndexBuffer_32 = nullptr;

        /// <summary>
        /// Index Buffer (RAM) for 16bit Indexes
        /// </summary>
        uint16_t* m_IndexBuffer_16 = nullptr;

        /// <summary>
        /// Size of Index Buffer
        /// </summary>
        uint32_t m_Size = 0;

        /// <summary>
        /// Index Buffer Ready State
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Buffer Access of the CPU for the Index Buffer
        /// </summary>
        GraphicBufferAccess m_BufferAccess = GraphicBufferAccess::None;

        /// <summary>
        /// Buffer Usage for the Index Buffer, for example: GraphicBufferUsage::Dynamic
        /// </summary>
        GraphicBufferUsage m_BufferUsage = GraphicBufferUsage::Default;

        /// <summary>
        /// Graphic Device associated with the Index Buffer
        /// </summary>
        GraphicDevice& m_GraphicDevice;

        /// <summary>
        /// When Map, this is the Index Buffer(VRAM) for 32bit
        /// </summary>
        uint32_t* m_VideoIB_32 = nullptr;

        /// <summary>
        /// When Map, this is the Index Buffer(VRAM) for 16bit
        /// </summary>
        uint16_t* m_VideoIB_16 = nullptr;

#pragma endregion
        
        /*********************************
         *   Private Framework Methods   *
         *********************************/
#pragma region Private Framework Methods

        /// <summary>
        /// Generic Function to handle Updates to the array of the Index Buffer
        /// </summary>
        /// <param name="IBType">Index Buffer Type</param>
        /// <param name="offset">Offset in array to where to begin copying from</param>
        /// <param name="indexData">Index Array</param>
        /// <param name="startIndex">Start Index in Index Buffer to begin the copy</param>
        /// <param name="elementCount">Number of elements to copy</param>
        /// <param name="resourceMap">Tells how the Index Buffer will be access (Valid only: Write, WriteDiscard, WriteNoOverwrite)</param>
        /// <returns>AEResult::Ok if copy succeeded</returns>
        AEResult UpdateIndexBufferPrivate(IndexBufferType IBType, uint32_t offset, void* indexData, uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap);

        /// <summary>
        /// Commit new Data from Index Buffer in RAM to VRAM
        /// </summary>
        /// <param name="offset">Offset in Index Buffer(RAM) to where to begin copying from</param>
        /// <param name="elementCount">Number of elements to copy</param>
        /// <returns>AEResult::Ok commit succeeded</returns>
        AEResult CommitIndexData(uint32_t offset, uint32_t elementCount);

        /// <summary>
        /// Map the VRAM Index Buffer
        /// </summary>
        /// <param name="resourceMap">Tells how the Index Buffer will be access</param>
        /// <returns>AEResult::Ok Map succeeded</returns>
        AEResult MapBuffer(GraphicResourceMap resourceMap);

        /// <summary>
        /// UnMap the Index Buffer from VRAM
        /// </summary>
        /// <returns>AEResult::Ok UnMap succeeded</returns>
        AEResult UnMapBuffer();

        /// <summary>
        /// Cleans up the Memory of the Index Buffer
        /// </summary>
        void CleanUpIB();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// IndexBuffer Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this Index Buffer</param>
        /// <param name="bufferUsage">How will the buffer be use</param>
        /// <param name="bufferAccess">How will the CPU Access the memory of the buffer</param>
        IndexBuffer(GraphicDevice& graphicDevice, GraphicBufferUsage bufferUsage = GraphicBufferUsage::Static, GraphicBufferAccess bufferAccess = GraphicBufferAccess::None);

        /// <summary>
        /// Default IndexBuffer Destructor
        /// </summary>
        virtual ~IndexBuffer();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Returns DirectX Index Buffer
        /// </summary>
        /// <returns>DirectX Index Buffer</returns>
        inline ID3D11Buffer* GetDXBuffer() const
        {
            return m_IndexBufferDX;
        }

        /// <summary>
        /// Returns if the Index Buffer Type
        /// </summary>
        /// <returns>True if Index Buffer is ready</returns>
        inline IndexBufferType GetIndexBufferType() const
        { 
            return m_IBType; 
        }

        /// <summary>
        /// Returns Size of Index (32bit or 16bit)
        /// </summary>
        /// <returns>Size of Index</returns>
        inline uint32_t GetIndexSize() const
        { 
            return (m_IBType == IndexBufferType::Index16 ? sizeof(uint16_t) : sizeof(uint32_t)); 
        }

        /// <summary>
        /// Returns if the Index Buffer is ready
        /// </summary>
        /// <returns>True if Index Buffer is ready</returns>
        inline bool IsReady() const
        { 
            return m_IsReady; 
        }

        /// <summary>
        /// Returns Size of Index Buffer
        /// </summary>
        /// <returns>Size of Index Buffer</returns>
        inline uint32_t GetSize() const
        { 
            return m_Size; 
        }

        /// <summary>
        /// Gets the Buffer Usage for the Structured Buffer
        /// </summary>
        /// <returns>Buffer Usage for the Structured Buffer</returns>
        inline GraphicBufferUsage GetBufferUsage() const
        {
            return m_BufferUsage;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /*************************
         *   Framework Methods   *
         *************************/
#pragma region Framework Methods

        /// <summary>
        /// Copies the array to the Index Buffer and recreates Index Buffer.
        /// This will delete the entire index buffer and build it again.
        /// To only update use UpdateIndexBuffer function.
        /// </summary>
        /// <param name="indexBuffer">Array to be copied</param>
        /// <param name="size">Size of the Array</param>
        void CopyToIndexBuffer(uint32_t indexBuffer[], uint32_t size);

        /// <summary>
        /// Copies the array to the Index Buffer and recreates Index Buffer.
        /// This will delete the entire index buffer and build it again.
        /// To only update use UpdateIndexBuffer function.
        /// </summary>
        /// <param name="indexBuffer">Array to be copied</param>
        /// <param name="size">Size of the Array</param>
        void CopyToIndexBuffer(uint16_t indexBuffer[], uint32_t size);

        /// <summary>
        /// Copies the array to the Index Buffer and recreates Index Buffer.
        /// This will delete the entire index buffer and build it again.
        /// To only update use UpdateIndexBuffer function.
        /// </summary>
        /// <param name="indexBuffer">Array to be copied</param>
        void CopyToIndexBuffer(const std::vector<uint32_t>& indexBuffer);

        /// <summary>
        /// Copies the array to the Index Buffer and recreates Index Buffer.
        /// This will delete the entire index buffer and build it again.
        /// To only update use UpdateIndexBuffer function.
        /// </summary>
        /// <param name="indexBuffer">Array to be copied</param>
        void CopyToIndexBuffer(const std::vector<uint16_t>& indexBuffer);

        /// <summary>
        /// Copies the data from the array to the Index Buffer
        /// </summary>
        /// <param name="offset">Offset in array to where to begin copying from</param>
        /// <param name="indexData">Index Array</param>
        /// <param name="startIndex">Start Index in Index Buffer to begin the copy</param>
        /// <param name="elementCount">Number of elements to copy</param>
        /// <param name="resourceMap">Tells how the Index Buffer will be access (Valid only: Write, WriteDiscard, WriteNoOverwrite)</param>
        /// <returns>AEResult::Ok if copy succeeded</returns>
        AEResult UpdateIndexBuffer(uint32_t offset, uint32_t indexData[], uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap);

        /// <summary>
        /// Copies the data from the array to the Index Buffer
        /// </summary>
        /// <param name="offset">Offset in array to where to begin copying from</param>
        /// <param name="indexData">Index Array</param>
        /// <param name="startIndex">Start Index in Index Buffer to begin the copy</param>
        /// <param name="elementCount">Number of elements to copy</param>
        /// <param name="resourceMap">Tells how the Index Buffer will be access (Valid only: Write, WriteDiscard, WriteNoOverwrite)</param>
        /// <returns>AEResult::Ok if copy succeeded</returns>
        AEResult UpdateIndexBuffer(uint32_t offset, uint16_t indexData[], uint32_t startIndex, uint32_t elementCount, GraphicResourceMap resourceMap);

        /// <summary>
        /// Builds the Index Buffer.
        /// </summary>
        /// <returns>AEResult::Ok if Build Index Buffer succeeded</returns>
        AEResult BuildIndexBuffer();

#pragma endregion

};

#endif
