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

#ifndef _MESH_H
#define _MESH_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\BoundingBox.h"
#include "Utils\BoundingSphere.h"
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/
class MeshPart;
class GraphicDevice;

/***************
*   Typedefs   *
****************/

/// <summary>
/// Typedef for Vector to contain MeshPart
/// </summary>
typedef std::vector<MeshPart*> MeshPartVector;

typedef MeshPartVector::iterator MeshPartVectorIt;

typedef MeshPartVector::const_iterator MeshPartVectorItConst;

/*****************
*   Class Decl   *
******************/
class Mesh : public GameResource
{
    protected:

        /// <summary>
        /// Vector containing the different Mesh Parts that make up the mesh
        /// </summary>
        MeshPartVector m_MeshPartVector;

        GraphicDevice& m_GraphicDevice;

        BoundingBox m_BoundingBox;

        BoundingSphere m_BoundingSphere;

        void CleanUp();

        /************************************
        *   Protected Destructor Methods   *
        ************************************/
#pragma region Protected Destructor Methods

        /// <summary>
        /// Default Mesh Destructor
        /// Protected so only Childs can access it
        /// </summary>
        virtual ~Mesh();

#pragma endregion

    public:

        /**************************
        *   Constructor Methods   *
        ***************************/
#pragma region Constructor Methods

        /// <summary>
        /// Mesh Constructor
        /// </summary>
        /// <param name="graphicDevice">Graphic Device use to create Mesh</param>
        /// <param name="resourceName">Resource Name</param>
        Mesh(GraphicDevice& graphicDevice, const std::string& resourceName);

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        uint32_t GetNumberMeshParts() const
        {
            return (uint32_t)m_MeshPartVector.size();
        }

        MeshPart* GetMeshPart(uint32_t index);

        const BoundingBox& GetBoundingBox() const
        {
            return m_BoundingBox;
        }

        const BoundingSphere& GetBoundingSphere() const
        {
            return m_BoundingSphere;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        virtual AEResult Load() override;

        AEResult AddMeshPart(MeshPart* mp);

        MeshPartVectorIt begin();

        MeshPartVectorIt end();

        MeshPartVectorItConst begin() const;

        MeshPartVectorItConst end() const;

        MeshPartVectorItConst cbegin() const;

        MeshPartVectorItConst cend() const;

        MeshPart* operator[](uint32_t index);

#pragma endregion

        // Prevent copying.
        Mesh(Mesh const&) = delete;
        Mesh& operator= (Mesh const&) = delete;
};

#endif

