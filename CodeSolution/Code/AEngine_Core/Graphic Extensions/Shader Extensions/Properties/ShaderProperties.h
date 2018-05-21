
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

#ifndef _SHADER_PROPERTIES_H
#define _SHADER_PROPERTIES_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\ShaderSignatures.h"
#include "GameObject\GameObjectDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Shader;
class Sampler;
class Texture;
class GameAsset;
class TextureArray;
class SimpleBuffer;
class TextureAsset;
class GraphicDevice;
class ConstantBuffer;
class StructuredBuffer;
class ShaderTextureBinding;

/*****************
*   Class Decl   *
******************/
class ShaderProperties sealed : public AEObject
{
    template<class T>
    struct ShaderPropertyObject
    {
        bool m_IsOverwriten = false;
        bool m_IsOwn = true;
        bool m_IsOverrideRW = false;
        bool m_SetRW = false;
        bool m_IsOverrideBindIndex = false;
        uint32_t m_BindIndex = 0;
        T* m_Object = nullptr;

        ShaderPropertyObject()
        {
        }

        ~ShaderPropertyObject()
        {
            if(m_IsOwn)
            {
                DeleteMem(m_Object);
            }
        }
    };

    typedef std::map<std::string, ConstantBuffer*> ConstantBufferMap;
    typedef std::vector<ConstantBuffer*> ConstantBufferVector;

    typedef std::map<std::string, ShaderPropertyObject<SimpleBuffer>*> SimpleBufferMap;
    typedef std::vector<ShaderPropertyObject<SimpleBuffer>*> SimpleBufferVector;

    typedef std::map<std::string, ShaderPropertyObject<StructuredBuffer>*> StructuredBufferMap;
    typedef std::vector<ShaderPropertyObject<StructuredBuffer>*> StructuredBufferVector;

    typedef std::map<std::string, ShaderTextureBinding*> TextureBindingMap;
    typedef std::vector<ShaderTextureBinding*> TextureBindingVector;

    typedef std::map<std::string, ShaderPropertyObject<TextureArray>*> TextureArrayMap;
    typedef std::vector<ShaderPropertyObject<TextureArray>*> TextureArrayVector;

    typedef std::map<std::string, ShaderPropertyObject<Sampler>*> SamplerMap;
    typedef std::vector<ShaderPropertyObject<Sampler>*> SamplerVector;

    typedef std::map<std::string, GameObjectAssetPair<Texture>> TextureAssetMap;

    private:

        /**************************
         *   Private Variables   *
         **************************/
#pragma region Private Variables

        /// <summary>
        /// Map containing all the Constant Buffers for the Shader
        /// </summary>
        ConstantBufferMap m_ConstantBufferMap;

        /// <summary>
        /// Vector containing all the Constant Buffers for the Shader for easy access
        /// </summary>
        ConstantBufferVector m_ConstantBufferVector;

        /// <summary>
        /// Map containing all the Simple Buffers for the Shader
        /// </summary>
        SimpleBufferMap m_SimpleBufferMap;

        /// <summary>
        /// Vector containing all the Simple Buffers for the Shader for easy access
        /// </summary>
        SimpleBufferVector m_SimpleBufferVector;

        /// <summary>
        /// Map containing all the Structured Buffers for the Shader
        /// </summary>
        StructuredBufferMap m_StructuredBufferMap;

        /// <summary>
        /// Vector containing all the Structured Buffers for the Shader for easy access
        /// </summary>
        StructuredBufferVector m_StructuredBufferVector;

        /// <summary>
        /// Map containing all the Texture Bindings for the Shader
        /// </summary>
        TextureBindingMap m_TextureBindingMap;

        /// <summary>
        /// Vector containing all the Texture Bindings for the Shader for easy access
        /// </summary>
        TextureBindingVector m_TextureBindingVector;

        /// <summary>
        /// Map the contains relationship between Texture and the Texture Asset
        /// </summary>
        TextureAssetMap m_TextureAssetMap;

        /// <summary>
        /// Map containing all the Texture Arrays for the Shader
        /// </summary>
        TextureArrayMap m_TextureArrayMap;

        /// <summary>
        /// Vector containing all the Texture Arrays for the Shader for easy access
        /// </summary>
        TextureArrayVector m_TextureArrayVector;

        /// <summary>
        /// Map containing all the Samplers for the Shader
        /// </summary>
        SamplerMap m_SamplerMap;

        /// <summary>
        /// Vector containing all the Samplers for the Shader for easy access
        /// </summary>
        SamplerVector m_SamplerVector;

        /// <summary>
        /// Lets know the Shader Properties to which shader stage they need to be applied
        /// </summary>
        ShaderType m_ShaderType = ShaderType::VertexShader;

        /// <summary>
        /// Graphic Device that is associated with this Object
        /// </summary>
        GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        AEResult CopyTexturePairData(const std::string& textureName, const GameObjectAssetPair<Texture>& otherTexturePair);

        //////////////////////////////

        AEResult AddConstantBuffersFromSigList(const ConstantBufferSignatureList& constBufferSigList);

        AEResult AddSamplersFromSigList(const SamplerSignatureList& samplerSigList);

        AEResult AddSimpleBuffersFromSigList(const SimpleBufferSignatureList& simpleBufferSignatureList);

        AEResult AddStructuredBuffersFromSigList(const StructuredBufferSignatureList& structuredBufferSigList);

        AEResult AddTextureBindingsFromSigList(const TextureInputSignatureList& textuBindingSigList);

        AEResult AddTextureArraysFromSigList(const TextureArrayInputSignatureList& textArrayInputSigList);

        //////////////////////////////

        void CleanUp();

        void TextureAssetDeletion(GameAsset* asset);

        AEResult SetTextureToBinding(ShaderTextureBinding* stb, TextureAsset* asset);

        AEResult SetTextureToBinding(ShaderTextureBinding* stb, Texture* baseTexture);

        AEResult AddTextureAssetBinding(const std::string& name, TextureAsset* asset);

        AEResult RemoveTextureAssetBinding(const std::string& name, bool informGameAsset = true);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// ShaderProperties Constructor
        /// </summary>
        /// <param name="shaderType">Shader Stage where this Properties can be applied</param>
        /// <param name="graphicDevice">Graphic Device to Associate to this Graphics</param>
        ShaderProperties(ShaderType shaderType, GraphicDevice* graphicDevice);

        /// <summary>
        /// Default Shader Destructor
        /// </summary>
        virtual ~ShaderProperties();

#pragma endregion
        
        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline uint32_t GetNumberOfConstantBuffers() const
        {
            return static_cast<uint32_t>(m_ConstantBufferVector.size());
        }

        inline uint32_t GetNumberOfShaderTextureBindings() const
        {
            return static_cast<uint32_t>(m_TextureBindingVector.size());
        }

#pragma endregion
        
        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Adds a Constant Buffer to the Shader Properties
        /// </summary>
        /// <param name="cb">Constant Buffer to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddConstantBuffer(ConstantBuffer* cb);

        /// <summary>
        /// Adds a Sampler to the Shader Properties
        /// </summary>
        /// <param name="sampler">Sampler to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddSampler(Sampler* sampler);

        /// <summary>
        /// Removes a Sampler from the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Sampler</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult RemoveSampler(const std::string& name);

        /// <summary>
        /// Overrides a Sampler of the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Sampler</param>
        /// <param name="simpleBuffer">Sampler to add</param>
        /// <param name="overrideBindIndex">Overwrites Bind Index</param>
        /// <param name="bindIndex">New Bind Index</param>
        /// <param name="deleteAtShaderDestroy">When Shader Properties get destroyed, destroy the object as well</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult OverrideSampler(const std::string& name, Sampler* sampler, bool overrideBindIndex = false, uint32_t bindIndex = 0, bool deleteAtShaderDestroy = false);

        /// <summary>
        /// Adds a Simple Buffer to the Shader Properties
        /// </summary>
        /// <param name="simpleBuffer">Simple Buffer to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddSimpleBuffer(SimpleBuffer* simpleBuffer);

        /// <summary>
        /// Removes a Simple Buffer from the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Simple Buffer</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult RemoveSimpleBuffer(const std::string& name);

        /// <summary>
        /// Overrides a Simple Buffer of the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Simple Buffer</param>
        /// <param name="simpleBuffer">Simple Buffer to add</param>
        /// <param name="overwriteRW">Overwrites Buffer RW Setting</param>
        /// <param name="setRW">Tells how to set the buffer as a RW or R Only</param>
        /// <param name="overrideBindIndex">Overwrites Bind Index</param>
        /// <param name="bindIndex">New Bind Index</param>
        /// <param name="deleteAtShaderDestroy">When Shader Properties get destroyed, destroy the object as well</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult OverrideSimpleBuffer(const std::string& name, SimpleBuffer* simpleBuffer, bool overwriteRW = false, bool setRW = false, bool overrideBindIndex = false, uint32_t bindIndex = 0, bool deleteAtShaderDestroy = false);

        /// <summary>
        /// Adds a Structured Buffer to the Shader Properties
        /// </summary>
        /// <param name="structuredBuffer">Structured Buffer to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddStructuredBuffer(StructuredBuffer* structuredBuffer);

        /// <summary>
        /// Removes a Structured Buffer from the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Structured Buffer</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult RemoveStructuredBuffer(const std::string& name);

        /// <summary>
        /// Overrides a Structured Buffer of the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Structured Buffer</param>
        /// <param name="structuredBuffer">Structured Buffer to add</param>
        /// <param name="overwriteRW">Overwrites Buffer RW Setting</param>
        /// <param name="setRW">Tells how to set the buffer as a RW or R Only</param>
        /// <param name="overrideBindIndex">Overwrites Bind Index</param>
        /// <param name="bindIndex">New Bind Index</param>
        /// <param name="deleteAtShaderDestroy">When Shader Properties get destroyed, destroy the object as well</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult OverrideStructuredBuffer(const std::string& name, StructuredBuffer* structuredBuffer, bool overwriteRW = false, bool setRW = false, bool overrideBindIndex = false, uint32_t bindIndex = 0, bool deleteAtShaderDestroy = false);

        /// <summary>
        /// Adds a Shader Texture Binding to the Shader Properties
        /// </summary>
        /// <param name="stb">Shader Texture Binding to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddShaderTextureBinding(ShaderTextureBinding* stb);

        /// <summary>
        /// Adds a Texture Array to the Shader Properties
        /// </summary>
        /// <param name="textureArray">Texture Array to add</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult AddTextureArray(TextureArray* textureArray);

        /// <summary>
        /// Removes a Texture Array from the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Texture Array</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult RemoveTextureArray(const std::string& name);

        /// <summary>
        /// Overrides a Texture Array of the Shader Properties
        /// </summary>
        /// <param name="name">Name of the Texture Array</param>
        /// <param name="textureArray">Texture Array to add</param>
        /// <param name="overrideBindIndex">Overwrites Bind Index</param>
        /// <param name="bindIndex">New Bind Index</param>
        /// <param name="deleteAtShaderDestroy">When Shader Properties get destroyed, destroy the object as well</param>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult OverrideTextureArray(const std::string& name, TextureArray* textureArray, bool overrideBindIndex = false, uint32_t bindIndex = 0, bool deleteAtShaderDestroy = false);

        /// <summary>
        /// Checks if a Constant Buffer exists in the Map
        /// </summary>
        /// <param name="name">Name of the Constant Buffer</param>
        /// <returns>True if it exists</returns>
        bool ConstantBufferExists(const std::string& name) const;

        /// <summary>
        /// Checks if a Sampler exists in the Map
        /// </summary>
        /// <param name="name">Name of the Sampler</param>
        /// <returns>True if it exists</returns>
        bool SamplerExists(const std::string& name) const;

        /// <summary>
        /// Checks if a Simple Buffer exists in the Map
        /// </summary>
        /// <param name="name">Name of the Simple Buffer</param>
        /// <returns>True if it exists</returns>
        bool SimpleBufferExists(const std::string& name) const;

        /// <summary>
        /// Checks if a Structured Buffer exists in the Map
        /// </summary>
        /// <param name="name">Name of the Structured Buffer</param>
        /// <returns>True if it exists</returns>
        bool StructuredBufferExists(const std::string& name) const;

        /// <summary>
        /// Checks if a Shader Texture Binding exists in the Map
        /// </summary>
        /// <param name="name">Name of the Shader Texture Binding</param>
        /// <returns>True if it exists</returns>
        bool ShaderTextureBindingExists(const std::string& name) const;

        /// <summary>
        /// Checks if a Texture Array exists in the Map
        /// </summary>
        /// <param name="name">Name of the Texture Array</param>
        /// <returns>True if it exists</returns>
        bool TextureArrayExists(const std::string& name) const;

        /// <summary>
        /// Gets the Constant Buffer from the map
        /// </summary>
        /// <param name="name">Name of the Constant Buffer</param>
        /// <returns>Shader Constant Buffer if found, if not returns null</returns>
        ConstantBuffer* GetConstantBuffer(const std::string& name);

        /// <summary>
        /// Gets the Constant Buffer from the map
        /// </summary>
        /// <param name="index">Index of the Constant Buffer</param>
        /// <returns>Shader Constant Buffer if found, if not returns null</returns>
        ConstantBuffer* GetConstantBuffer(uint32_t index);

        /// <summary>
        /// Gets a Sampler from the map
        /// </summary>
        /// <param name="name">Name of the Sampler</param>
        /// <returns>Shader Sampler if found, if not returns null</returns>
        Sampler* GetSampler(const std::string& name);

        /// <summary>
        /// Gets a Sampler from the map
        /// </summary>
        /// <param name="index">Index of the Sampler</param>
        /// <returns>Shader Sampler if found, if not returns null</returns>
        Sampler* GetSampler(uint32_t index);

        /// <summary>
        /// Gets the Simple Buffer from the map
        /// </summary>
        /// <param name="name">Name of the Simple Buffer</param>
        /// <returns>Shader Simple Buffer if found, if not returns null</returns>
        SimpleBuffer* GetSimpleBuffer(const std::string& name);

        /// <summary>
        /// Gets the Simple Buffer from the map
        /// </summary>
        /// <param name="index">Index of the Simple Buffer</param>
        /// <returns>Shader Simple Buffer if found, if not returns null</returns>
        SimpleBuffer* GetSimpleBuffer(uint32_t index);

        /// <summary>
        /// Gets the Structured Buffer from the map
        /// </summary>
        /// <param name="name">Name of the Structured Buffer</param>
        /// <returns>Shader Structured Buffer if found, if not returns null</returns>
        StructuredBuffer* GetStructuredBuffer(const std::string& name);

        /// <summary>
        /// Gets the Structured Buffer from the map
        /// </summary>
        /// <param name="index">Index of the Structured Buffer</param>
        /// <returns>Shader Structured Buffer if found, if not returns null</returns>
        StructuredBuffer* GetStructuredBuffer(uint32_t index);

        /// <summary>
        /// Gets the Shader Texture Binding
        /// </summary>
        /// <param name="name">Name of the Texture Binding</param>
        /// <returns>Shader Texture Binding from the Map if found, if not returns null</returns>
        const ShaderTextureBinding* GetShaderTextureBinding(const std::string& name) const;

        /// <summary>
        /// Gets the Shader Texture Binding
        /// </summary>
        /// <param name="index">Index of the Texture Binding</param>
        /// <returns>Shader Texture Binding from the Map if found, if not returns null</returns>
        const ShaderTextureBinding* GetShaderTextureBinding(uint32_t index) const;

        /// <summary>
        /// Gets the Texture Array from the map
        /// </summary>
        /// <param name="name">Name of the Texture Array</param>
        /// <returns>Texture Array if found, if not returns null</returns>
        TextureArray* GetTextureArray(const std::string& name);

        /// <summary>
        /// Gets the Texture Array from the map
        /// </summary>
        /// <param name="index">Index of the Structured Buffer</param>
        /// <returns>Texture Array if found, if not returns null</returns>
        TextureArray* GetTextureArray(uint32_t index);

        AEResult SetTexture(const std::string& name, TextureAsset* asset);

        AEResult SetTexture(const std::string& name, Texture* texture);

        AEResult SetTexture(uint32_t index, TextureAsset* asset);

        AEResult CreateFromShaderSignatures(Shader* shader);

        AEResult CopyVariableData(const ShaderProperties* otherShaderProps);

        uint64_t GetTextureAssetID(const std::string& name);

        AEResult ApplyAll();

        AEResult ApplyConstantBuffers();

        AEResult ApplySamplers();

        AEResult ApplyStructuredBuffers();

        AEResult ApplySimpleBuffers();

        AEResult ApplyTextureBindings();

        AEResult ApplyTextureArrays();

        AEResult UnApplyAll();

        AEResult UnApplyConstantBuffers();

        AEResult UnApplySamplers();

        AEResult UnApplyStructuredBuffers();

        AEResult UnApplySimpleBuffers();

        AEResult UnApplyTextureBindings();

        AEResult UnApplyTextureArrays();

#pragma endregion

        ShaderProperties(ShaderProperties const&) = delete;
        ShaderProperties& operator= (ShaderProperties const&) = delete;
};

#endif
