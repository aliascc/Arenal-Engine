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

#ifndef _ICONSTANT_BUFFER_H
#define _ICONSTANT_BUFFER_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>
#include <d3d11_1.h>
#include <unordered_map>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\Named.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
struct ShaderCustomVariable;

/*****************
*   Class Decl   *
******************/
class ConstantBuffer sealed : public Named
{
    /// <summary>
    /// Typedef for Shader Variable Map
    /// </summary>
    typedef std::unordered_map<std::wstring, ShaderCustomVariable*> VariableMap;
    
    /// <summary>
    /// Typedef for Shader Variable Vector
    /// </summary>
    typedef std::vector<ShaderCustomVariable*> VariableVector;

    private:

        /**************************
         *   Private Variables   *
         **************************/
#pragma region Private Variables

        /// <summary>
        /// DirectX 11 Buffer to hold Constant Buffer of the Shader
        /// </summary>
        ID3D11Buffer* m_ConstantBufferDX = nullptr;

        /// <summary>
        /// Graphic Device associated with the Texture
        /// </summary>
        GraphicDevice* m_GraphicDevice = nullptr;

        /// <summary>
        /// Index to bind the Constant Buffer to the Shader Register
        /// </summary>
        uint32_t m_BindIndex = 0;

        /// <summary>
        /// Lets know the Constant Buffer if it is up and ready
        /// to process data and information
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Constant Buffer Data in RAM
        /// </summary>
        void* m_ConstantBuffer = nullptr;

        /// <summary>
        /// Constant Buffer Data Size
        /// </summary>
        uint32_t m_ConstantBufferDataSize = 0;

        /// <summary>
        /// Map of all the variables the Constant Buffer has for fast access
        /// </summary>
        VariableMap m_VariableMap;

        /// <summary>
        /// Vector of all the Variables in order of how they are presented in memory
        /// </summary>
        VariableVector m_VariableVector;

        /// <summary>
        /// Lets know Constant Buffer to update VRAM cause variables has been modified
        /// </summary>
        bool m_NeedUpdate = false;

#pragma endregion

        /************************
         *   Private Methods    *
         ************************/
#pragma region Private Methods
        
        /// <summary>
        /// Calculates the size of the Constant Buffer based on the variables
        /// </summary>
        uint32_t CalculateSize();
            
        // Prevent copying.
        ConstantBuffer(ConstantBuffer const&);
        ConstantBuffer& operator= (ConstantBuffer const&);

#pragma endregion
        
    public:

        /*****************************************
         *   Constructor & Destructor Methods   *
         *****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default ConstantBuffer Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with</param>
        /// <param name="bindIndex">Bind Index of the Constant Buffer</param>
        /// <param name="name">Name of the Constant Buffer</param>
        ConstantBuffer(GraphicDevice* graphicDevice, uint32_t bindIndex, const std::wstring& name = L"");

        /// <summary>
        /// Default ConstantBuffer Destructor
        /// </summary>
        virtual ~ConstantBuffer();

#pragma endregion
        
        /**********************
         *     Get Methods    *
         **********************/
#pragma region Get Methods
        
        /// <summary>
        /// Returns the DirectX Buffer
        /// </summary>
        /// <returns>DirectX Buffer</returns>
        inline ID3D11Buffer* GetConstantBufferDX() const
        { 
            return m_ConstantBufferDX; 
        }

        /// <summary>
        /// Returns if the Constant Buffer has been initialized and is ready
        /// </summary>
        /// <returns>True if it has been initialized successfully</returns>
        inline bool IsReady() const
        { 
            return m_IsReady; 
        }

        /// <summary>
        /// Gets the Constant Buffer total size
        /// </summary>
        /// <returns>Constant Buffer Size</returns>
        inline uint32_t GetConstantBufferDataSize() const
        { 
            return m_ConstantBufferDataSize; 
        }

        /// <summary>
        /// Gets the Bind Index of the Constant Buffer to where to bind to the Shader Register
        /// </summary>
        /// <returns>Bind Index for the Constant Buffer</returns>
        inline uint32_t GetBindIndex() const
        { 
            return m_BindIndex; 
        }

        inline uint32_t GetNumberOfVariables() const
        {
            return static_cast<uint32_t>(m_VariableVector.size());
        }

        const ShaderCustomVariable* GetVariable(uint32_t index) const;

        const ShaderCustomVariable* GetVariable(const std::wstring& name) const;

#pragma endregion
        
        /**********************
         *     Set Methods    *
         **********************/
#pragma region Set Methods

        /// <summary>
        /// Sets the Bind Index of the Constant Buffer to where to bind it to the Shader Register
        /// </summary>
        inline void SetBindIndex(uint32_t bindIndex)
        { 
            m_BindIndex = bindIndex; 
        }

#pragma endregion

        /**************************
         *   Framework Methods    *
         **************************/
#pragma region Framework Methods

        /// <summary>
        /// Initializes Constant Buffer in memory
        /// </summary>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult Initialize();
        
        /// <summary>
        /// Apply Constant Buffer Changes to VRAM if needed
        /// </summary>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult Apply();

        /// <summary>
        /// Checks if a variable exists in the Constant Buffer
        /// </summary>
        /// <param name="name">Name of the Variable</param>
        /// <returns>True if it exists</returns>
        bool Exists(const std::wstring& name) const;

        /// <summary>
        /// Adds a Variable to the Constant Buffer
        /// </summary>
        /// <remarks>If Constant Buffer has been initialized then adding a variable will result in failure</remarks>
        /// <param name="scv">Custom Shader Variable to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddVariable(ShaderCustomVariable* scv);

        /// <summary>
        /// Sets a value to an specific variable
        /// </summary>
        /// <param name="varName">Name of the variable</param>
        /// <param name="value">Value to set to the variable</param>
        /// <param name="varSize">Max size to copy to memory, this is use to try to not invaded unwanted memory</param>
        /// <param name="user">Determine if variable should be user variable only</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult SetValue(const std::wstring& varName, const void* value, uint32_t varSize, bool user = false);
        
        /// <summary>
        /// Gets a value to an specific variable
        /// </summary>
        /// <param name="varName">Name of the variable</param>
        /// <param name="value">Variable where to set the value</param>
        /// <param name="varSize">Max size to copy from memory, this is use to try to not invaded unwanted memory</param>
        /// <param name="user">Determine if variable should be user variable only</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult GetValue(const std::wstring& varName, void* value, uint32_t varSize, bool user = false);

        /// <summary>
        /// Sets a value to an specific variable
        /// </summary>
        /// <remarks>Template class to provide variable validation at compile and runtime</remarks>
        /// <param name="varName">Name of the variable</param>
        /// <param name="value">Value to set to the variable</param>
        /// <param name="user">Determine if variable should be user variable only</param>
        /// <returns>AEResult::Ok if successful</returns>
        template<class T>
        AEResult SetValueT(const std::wstring& varName, const T& value, bool user = false)
        {
            return SetValue(varName, &value, (uint32_t)sizeof(T), user);
        }
        
        /// <summary>
        /// Gets a value to an specific variable
        /// </summary>
        /// <remarks>Template class to provide variable validation at compile and runtime</remarks>
        /// <param name="varName">Name of the variable</param>
        /// <param name="value">Variable where to set the value</param>
        /// <param name="user">Determine if variable should be user variable only</param>
        /// <returns>AEResult::Ok if successful</returns>
        template<class T>
        AEResult GetValueT(const std::wstring& varName, T& value, bool user = false)
        {
            return GetValue(varName, &value, (uint32_t)sizeof(T), user);
        }

        bool ContainsUserVariables();

#pragma endregion

};

#endif
