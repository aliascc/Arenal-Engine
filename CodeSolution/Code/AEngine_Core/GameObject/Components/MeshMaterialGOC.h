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

#ifndef _MESH_MATERIAL_GOC_H
#define _MESH_MATERIAL_GOC_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Shaders\ShaderDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Shader;
class GameAsset;
class HullShader;
class PixelShader;
class ShaderAsset;
class VertexShader;
class DomainShader;
class TextureAsset;
class GraphicDevice;
class ComputeShader;
class GeometryShader;
class ShaderProperties;

/*****************
*   Class Decl   *
******************/
class MeshMaterialGOC sealed : public GameObjectComponent
{
    private:

        /*************************
         *   Private Variables   *
         *************************/
#pragma region Private Variables

        std::wstring m_Name = L"";
        
        /// <summary>
        /// Game Object Pair with Vertex Shader that this Game Object Component uses
        /// <summary>
        GameObjectAssetPair<VertexShader> m_VertexShader;

        ShaderProperties* m_VertexShaderProps = nullptr;

        /// <summary>
        /// Game Object Pair with Pixel Shader that this Game Object Component uses
        /// <summary>
        GameObjectAssetPair<PixelShader> m_PixelShader;

        ShaderProperties* m_PixelShaderProps = nullptr;

        /// <summary>
        /// Game Object Pair with Geometry Shader that this Game Object Component uses
        /// <summary>
        GameObjectAssetPair<GeometryShader> m_GeometryShader;

        ShaderProperties* m_GeometryShaderProps = nullptr;

        /// <summary>
        /// Game Object Pair with Compute Shader that this Game Object Component uses
        /// <summary>
        GameObjectAssetPair<ComputeShader> m_ComputeShader;

        ShaderProperties* m_ComputeShaderProps = nullptr;

        /// <summary>
        /// Game Object Pair with Domain Shader that this Game Object Component uses
        /// <summary>
        GameObjectAssetPair<DomainShader> m_DomainShader;

        ShaderProperties* m_DomainShaderProps = nullptr;

        /// <summary>
        /// Game Object Pair with Hull Shader that this Game Object Component uses
        /// <summary>
        GameObjectAssetPair<HullShader> m_HullShader;

        ShaderProperties* m_HullShaderProps = nullptr;

#pragma endregion

        /***********************
         *   Private Methods   *
         ***********************/
#pragma region Private Methods

        void ShaderAssetDeletion(GameAsset* asset);

        void ShaderAssetReload(GameAsset* asset);

        void TextureAssetDeletion(GameAsset* asset);

        void TextureAssetReload(GameAsset* asset);

        AEResult VertexShaderAssetReload(ShaderAsset* asset);

        AEResult PixelShaderAssetReload(ShaderAsset* asset);

        AEResult GeometryShaderAssetReload(ShaderAsset* asset);

        AEResult ComputeShaderAssetReload(ShaderAsset* asset);

        AEResult HullShaderAssetReload(ShaderAsset* asset);

        AEResult DomainShaderAssetReload(ShaderAsset* asset);

        AEResult RemoveShaderAsset(ShaderType shaderType, bool informGameAsset = true, uint64_t assetID = 0);

        AEResult CreateShaderProperties(ShaderType shaderType);

        AEResult DeleteShaderProperties(ShaderType shaderType);

        template<class T>
        AEResult CreateShaderPropertiesTemplate(GameObjectAssetPair<T>& gameAssetPair, ShaderProperties** shaderProperties);

        template<class T>
        AEResult ShaderPropertiesReloadTemplate(GameObjectAssetPair<T>& gameAssetPair, ShaderProperties** shaderProperties);

        template<class T>
        std::wstring GetShaderResourceNameTemplate(const GameObjectAssetPair<T>& gameAssetPair) const;

        template<class T>
        AEResult SetShaderAssetTemplate(ShaderAsset* asset, ShaderType shaderType, GameObjectAssetPair<T>& gameAssetPair);
        
        template<class T>
        AEResult ClearShaderAssetTemplate(GameObjectAssetPair<T>& gameAssetPair, bool informGameAsset = true);

#pragma endregion

    public:

        /****************************************
         *   Constructor & Destructor Methods   *
         ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// MeshMaterialGOC Constructor
        /// </summary>
        /// <param name="gameObject">Game Object that this Component is attached too</param>
        /// <param name="name">Name of the Mesh Material</param>
        MeshMaterialGOC(GameObject* gameObject, const std::wstring& name = L"");

        /// <summary>
        /// Default MeshMaterialGOC Destructor
        /// </summary>
        virtual ~MeshMaterialGOC();

#pragma endregion

        /*******************
         *   Get Methods   *
         *******************/
#pragma region Get Methods

        inline VertexShader* GetVertexShaderResource() const
        {
            return m_VertexShader.m_ResourceAsset;
        }

        inline uint64_t GetVertexShaderAssetID() const
        {
            return m_VertexShader.m_AssetID;
        }

        inline ShaderProperties* GetVertexShaderProperties() const
        {
            return m_VertexShaderProps;
        }

        std::wstring GetVertexShaderName() const;

        inline PixelShader* GetPixelShaderResource() const
        {
            return m_PixelShader.m_ResourceAsset;
        }

        inline uint64_t GetPixelShaderAssetID() const
        {
            return m_PixelShader.m_AssetID;
        }

        inline ShaderProperties* GetPixelShaderProperties() const
        {
            return m_PixelShaderProps;
        }

        std::wstring GetPixelShaderName() const;

        inline GeometryShader* GetGeometryShaderResource() const
        {
            return m_GeometryShader.m_ResourceAsset;
        }

        inline uint64_t GetGeometryShaderAssetID() const
        {
            return m_GeometryShader.m_AssetID;
        }

        inline ShaderProperties* GetGeometryShaderProperties() const
        {
            return m_GeometryShaderProps;
        }

        std::wstring GetGeometryShaderName() const;

        inline ComputeShader* GetComputeShaderResource() const
        {
            return m_ComputeShader.m_ResourceAsset;
        }

        inline uint64_t GetComputeShaderAssetID() const
        {
            return m_ComputeShader.m_AssetID;
        }

        inline ShaderProperties* GetComputeShaderProperties() const
        {
            return m_ComputeShaderProps;
        }

        std::wstring GetComputeShaderName() const;

        inline HullShader* GetHullShaderResource() const
        {
            return m_HullShader.m_ResourceAsset;
        }

        inline uint64_t GetHullShaderAssetID() const
        {
            return m_HullShader.m_AssetID;
        }

        inline ShaderProperties* GetHullShaderProperties() const
        {
            return m_HullShaderProps;
        }

        std::wstring GetHullShaderName() const;

        inline DomainShader* GetDomainShaderResource() const
        {
            return m_DomainShader.m_ResourceAsset;
        }

        inline uint64_t GetDomainShaderAssetID() const
        {
            return m_DomainShader.m_AssetID;
        }

        inline ShaderProperties* GetDomainShaderProperties() const
        {
            return m_DomainShaderProps;
        }

        std::wstring GetDomainShaderName() const;

        inline const std::wstring& GetName() const
        {
            return m_Name;
        }

#pragma endregion

        /*******************
         *   Set Methods   *
         *******************/
#pragma region Set Methods

        AEResult SetVertexShaderAsset(ShaderAsset* asset);

        AEResult SetPixelShaderAsset(ShaderAsset* asset);

        AEResult SetGeometryShaderAsset(ShaderAsset* asset);

        AEResult SetComputeShaderAsset(ShaderAsset* asset);

        AEResult SetHullShaderAsset(ShaderAsset* asset);

        AEResult SetDomainShaderAsset(ShaderAsset* asset);

        void SetName(const std::wstring& name)
        {
            m_Name = name;
        }

#pragma endregion

        /*************************
         *   Framework Methods   *
         *************************/
#pragma region Framework Methods

        AEResult RemoveVertexShaderAsset();

        AEResult RemovePixelShaderAsset();

        AEResult RemoveGeometryShaderAsset();

        AEResult RemoveComputeShaderAsset();

        AEResult RemoveHullShaderAsset();

        AEResult RemoveDomainShaderAsset();

        AEResult ApplyShaders(GraphicDevice* graphicDevice);

        AEResult UnApplyShaders(GraphicDevice* graphicDevice);

#pragma endregion

};

#endif
