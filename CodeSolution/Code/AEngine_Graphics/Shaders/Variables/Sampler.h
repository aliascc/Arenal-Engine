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

#ifndef _SAMPLER_H
#define _SAMPLER_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Shaders\ShaderDefs.h"
#include "Shaders\Bindings\ShaderBinding.h"

/********************
*   Forward Decls   *
*********************/
class Texture;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class Sampler sealed : public ShaderBinding
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Graphic Device to be associated with this object
        /// </summary>
        GraphicDevice& m_GraphicDevice;

        /// <summary>
        /// DirectX Sampler
        /// </summary>
        ID3D11SamplerState* m_SamplerDX = nullptr;

        /// <summary>
        /// Determines if the class is ready to process data
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Determines if Sampler needs to reinitialize cause a variable change
        /// </summary>
        bool m_NeedsReinit = false;

        ShaderFilter m_ShaderFilter = ShaderFilter::MinMagMipLinear;

        ShaderTextureAddressMode m_TextureAddressModeU = ShaderTextureAddressMode::Clamp;

        ShaderTextureAddressMode m_TextureAddressModeV = ShaderTextureAddressMode::Clamp;

        ShaderTextureAddressMode m_TextureAddressModeW = ShaderTextureAddressMode::Clamp;

        float m_MinLOD = -std::numeric_limits<float>::max();

        float m_MaxLOD = std::numeric_limits<float>::max();

        float m_MipLODBias = 0.0f;

        uint32_t m_MaxAnisotropy = 1;

        ShaderCompFunct m_CompFunction = ShaderCompFunct::Never;

        Color m_BorderColor = AEColors::White;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default Sampler Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to be associated with this object</param>
        /// <param name="name">Name of the Sampler</param>
        /// <param name="bindIndex">Index where to bind</param>
        Sampler(GraphicDevice& graphicDevice, const std::string& name, uint32_t bindIndex);

        /// <summary>
        /// Default Sampler Destructor
        /// </summary>
        virtual ~Sampler();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets DirectX Sampler
        /// </summary>
        inline ID3D11SamplerState* GetSamplerDX() const
        {
            return m_SamplerDX;
        }

        /// <summary>
        /// Gets if the class is ready to process data
        /// </summary>
        inline bool IsReady() const
        {
            return m_IsReady;
        }

        /// <summary>
        /// Gets if Sampler needs to reinitialize cause a variable change
        /// </summary>
        inline bool NeedsReinit() const
        {
            return m_NeedsReinit;
        }

        inline ShaderFilter GetShaderFilter() const
        {
            return m_ShaderFilter;
        }

        inline ShaderTextureAddressMode GetTextureAddressModeU() const
        {
            return m_TextureAddressModeU;
        }

        inline ShaderTextureAddressMode GetTextureAddressModeV() const
        {
            return m_TextureAddressModeV;
        }

        inline ShaderTextureAddressMode GetTextureAddressModeW() const
        {
            return m_TextureAddressModeW;
        }

        inline float GetMinLOD() const
        {
            return m_MinLOD;
        }

        inline float GetMaxLOD() const
        {
            return m_MaxLOD;
        }

        inline float GetMipLODBias() const
        {
            return m_MipLODBias;
        }

        inline uint32_t GetMaxAnisotropy() const
        {
            return m_MaxAnisotropy;
        }

        inline ShaderCompFunct GetCompFunction() const
        {
            return m_CompFunction;
        }

        inline const Color& GetBorderColor() const
        {
            return m_BorderColor;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        inline void SetShaderFilter(ShaderFilter shaderFilter)
        {
            if (m_ShaderFilter == shaderFilter)
            {
                return;
            }
            
            m_ShaderFilter = shaderFilter;
            m_NeedsReinit = true;
        }

        inline void GetTextureAddressModeU(ShaderTextureAddressMode textureAddressMode)
        {
            if (m_TextureAddressModeU == textureAddressMode)
            {
                return;
            }

            m_TextureAddressModeU = textureAddressMode;
            m_NeedsReinit = true;
        }

        inline void GetTextureAddressModeV(ShaderTextureAddressMode textureAddressMode)
        {
            if (m_TextureAddressModeV == textureAddressMode)
            {
                return;
            }

            m_TextureAddressModeV = textureAddressMode;
            m_NeedsReinit = true;
        }

        inline void GetTextureAddressModeW(ShaderTextureAddressMode textureAddressMode)
        {
            if (m_TextureAddressModeW == textureAddressMode)
            {
                return;
            }

            m_TextureAddressModeW = textureAddressMode;
            m_NeedsReinit = true;
        }

        inline void GetMinLOD(float minLOD)
        {
            if (m_MinLOD == minLOD)
            {
                return;
            }

            m_MinLOD = minLOD;
            m_NeedsReinit = true;
        }

        inline void GetMaxLOD(float maxLOD)
        {
            if (m_MaxLOD == maxLOD)
            {
                return;
            }

            m_MaxLOD = maxLOD;
            m_NeedsReinit = true;
        }

        inline void GetMipLODBias(float mipLODBias)
        {
            if (m_MipLODBias == mipLODBias)
            {
                return;
            }

            m_MipLODBias = mipLODBias;
            m_NeedsReinit = true;
        }

        inline void GetMaxAnisotropy(uint32_t maxAnisotropy)
        {
            if (m_MaxAnisotropy == maxAnisotropy)
            {
                return;
            }

            m_MaxAnisotropy = maxAnisotropy;
            m_NeedsReinit = true;
        }

        inline void GetCompFunction(ShaderCompFunct compFunction)
        {
            if (m_CompFunction == compFunction)
            {
                return;
            }

            m_CompFunction = compFunction;
            m_NeedsReinit = true;
        }

        inline void GetBorderColor(const Color& borderColor)
        {
            if (m_BorderColor == borderColor)
            {
                return;
            }

            m_BorderColor = borderColor;
            m_NeedsReinit = true;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Initialize Sampler
        /// </summary>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult Initialize(ShaderFilter shaderFilter = ShaderFilter::MinMagMipLinear, ShaderTextureAddressMode textureAddressModeU = ShaderTextureAddressMode::Clamp, ShaderTextureAddressMode textureAddressModeV = ShaderTextureAddressMode::Clamp, ShaderTextureAddressMode textureAddressModeW = ShaderTextureAddressMode::Clamp, float minLOD = -std::numeric_limits<float>::max(), float maxLOD = std::numeric_limits<float>::max(), float mipLODBias = 0.0f, uint32_t maxAnisotropy = 1, ShaderCompFunct compFunction = ShaderCompFunct::Never, const Color& borderColor = AEColors::White);

        /// <summary>
        /// Re-initialize Sampler with current object settings
        /// </summary>
        /// <returns>AEResult::Ok if successful</returns>
        AEResult Reinitialize();

#pragma endregion

};

#endif
