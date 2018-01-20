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

#ifndef _CUBE_MESH_H
#define _CUBE_MESH_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>
#include <vector>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Models\Mesh.h"
#include "Resource\GameResource.h"
#include "Vertex\Types\VertexPositionNormalTexture.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/**************
*   Defines   *
***************/

/// <summary>
/// Cube Mesh Name
/// </summary>
#define AE_CUBE_MESH_NAME                        L"Cube Mesh"

/***************
*   Typedefs   *
****************/

/*****************
*   Class Decl   *
******************/
class CubeMesh sealed : public Mesh
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        bool m_IsLoaded = false;

        std::vector<VertexPositionNormalTexture> m_VtxBuff;

        std::vector<uint16_t> m_IdxBuff;

        std::map<std::wstring, uint16_t> m_VtxMap;

        uint16_t AddVtx(const VertexPositionNormalTexture& vtx);

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

#pragma endregion

        /*********************************
        *   Private Destructor Methods   *
        **********************************/
#pragma region Private Destructor Methods

        /// <summary>
        /// Default CubeMesh Destructor
        /// </summary>
        virtual ~CubeMesh();

#pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
#pragma region Constructor Methods

        /// <summary>
        /// CubeMesh Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device use to create Mesh</param>
        /// <param name="resourceName">Resource Name</param>
        CubeMesh(GraphicDevice* graphicDevice, const std::wstring& resourceName);

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult Load() override;

#pragma endregion

        // Prevent copying.
        CubeMesh(CubeMesh const&) = delete;
        CubeMesh& operator= (CubeMesh const&) = delete;
};

#endif

