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

#ifndef _LIGHT_MANAGER_H
#define _LIGHT_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "LightDefs.h"
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Light;
class Camera;
class SpotLight;
class GraphicDevice;
class Texture2DArray;
class DirectionalLight;

/***************
*   Typedefs   *
****************/
typedef std::vector<Light*> LightsVector;

typedef std::map<uint64_t, Light*> LightsMap;

typedef LightsMap::iterator LightsMapIt;

typedef LightsMap::const_iterator LightsMapItConst;

/*****************
*   Class Decl   *
******************/
class LightManager sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        bool m_MapChanged = false;

        bool m_FXVectorChanged = true;

        bool m_FXSpotShadowVectorChanged = false;

        bool m_FXDirShadowVectorChanged = false;

        bool m_LightShadowTexturesChanged = false;

        uint32_t m_NumSpotLightsWithShadows = 0;

        uint32_t m_NumDirLightsWithShadows = 0;

        LightsMap m_LightsMap;

        LightsVector m_LightsVector;

        LightFX m_LightFXVec[AE_MAX_LIGHTS];

        SpotLightShadowInfoFX m_SpotLightShadowInfoFX[AE_MAX_LIGHTS];

        LightCascadeInfoFX m_DirLightShadowInfoFX[AE_MAX_LIGHTS];

        Texture2DArray* m_SpotLightShadowTextureArray = nullptr;

        Texture2DArray* m_DirLightShadowTextureArray = nullptr;

        GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        AEResult UpdateSpotLightShadowInformation(SpotLight* light, uint32_t& shadowTextIdx);

        AEResult UpdateDirLightShadowInformation(DirectionalLight* light, uint32_t& shadowTextIdx, Camera* camera);

        AEResult UpdateSpotLightShadowTextureArray(uint32_t size);

        AEResult UpdateDirLightShadowTextureArray(uint32_t size);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// LightManager Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device to associate lights with</param>
        LightManager(GraphicDevice* graphicDevice);

        /// <summary>
        /// Default LightManager Destructor
        /// </summary>
        virtual ~LightManager();

#pragma endregion

        /*******************
         *   Get Methods   *
         *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets the number of Lights
        /// </summary>
        /// <returns>Returns the number of Light</returns>
        uint32_t GetNumberOfLights() const;

        /// <summary>
        /// Get if container is full
        /// </summary>
        /// <returns>Returns if the container is full</returns>
        inline bool IsContainerFull() const
        {
            return (GetNumberOfLights() >= AE_MAX_LIGHTS);
        }

        /// <summary>
        /// Gets the Light FX Vector
        /// </summary>
        /// <returns>Returns the Light FX Vector</returns>
        inline const LightFX* GetLightFXVector() const
        {
            return m_LightFXVec;
        }

        /// <summary>
        /// Gets the Shadow Spot Light Info FX Vector
        /// </summary>
        /// <returns>Returns the Shadow Spot Light Info FX Vector</returns>
        inline const SpotLightShadowInfoFX* GetSpotLightShadowInfoFXVector() const
        {
            return m_SpotLightShadowInfoFX;
        }

        /// <summary>
        /// Gets the Shadow Directional Light Info FX Vector
        /// </summary>
        /// <returns>Returns the Shadow Directional Light Info FX Vector</returns>
        inline const LightCascadeInfoFX* GetDirLightShadowInfoFXVector() const
        {
            return m_DirLightShadowInfoFX;
        }

        /// <summary>
        /// Gets if the Light FX Vector has changed
        /// </summary>
        /// <returns>Returns if the Light FX Vector has changed</returns>
        inline bool HasFXVectorChanged() const
        {
            return m_FXVectorChanged;
        }

        /// <summary>
        /// Gets if the Shadow Directional Light Info FX Vector has changed
        /// </summary>
        /// <returns>Returns if the Shadow Directional Light Info FX Vector has changed</returns>
        inline bool HasFXDirShadowVectorChanged() const
        {
            return m_FXDirShadowVectorChanged;
        }

        /// <summary>
        /// Gets if the Shadow Spot Light Info FX Vector has changed
        /// </summary>
        /// <returns>Returns if the Shadow Spot Light Info FX Vector has changed</returns>
        inline bool HasFXSpotShadowVectorChanged() const
        {
            return m_FXSpotShadowVectorChanged;
        }

        /// <summary>
        /// Gets the number of Spot Lights with Shadows
        /// </summary>
        /// <returns>Returns the number of Spot Lights with Shadows</returns>
        inline uint32_t GetNumSpotLightsWithShadows() const
        {
            return m_NumSpotLightsWithShadows;
        }

        /// <summary>
        /// Gets the number of Directional Lights with Shadows
        /// </summary>
        /// <returns>Returns the number of Directional Lights with Shadows</returns>
        inline uint32_t GetNumDirLightsWithShadows() const
        {
            return m_NumDirLightsWithShadows;
        }

        /// <summary>
        /// Gets the Spot Light Shadow Texture Array
        /// </summary>
        /// <returns>Spot Light Shadow Texture Array for the Lights</returns>
        inline Texture2DArray* GetSpotLightShadowTextureArray() const
        {
            return m_SpotLightShadowTextureArray;
        }

        /// <summary>
        /// Gets the Directional Light Shadow Texture Array
        /// </summary>
        /// <returns>Direction Light Shadow Texture Array for the Lights</returns>
        inline Texture2DArray* GetDirLightShadowTextureArray() const
        {
            return m_DirLightShadowTextureArray;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        void ClearFXVectorChangeFlag();

        void ClearFXSpotShadowVectorChangedFlag();

        void ClearFXDirShadowVectorChangedFlag();

        AEResult AddLight(Light* light);

        AEResult RemoveLight(uint64_t lightID);

        AEResult RemoveLight(Light* light);

        bool LightExists(uint64_t lightID);

        AEResult Update(Camera* camera);

        LightsMapIt begin();

        LightsMapIt end();

        LightsMapItConst begin() const;

        LightsMapItConst end() const;

        LightsMapItConst cbegin() const;

        LightsMapItConst cend() const;

#pragma endregion

};

#endif
