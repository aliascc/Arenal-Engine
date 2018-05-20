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

#ifndef _MESH_GOC_H
#define _MESH_GOC_H

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
#include "GameObject\GameObjectDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Mesh;
class GameAsset;
class MeshAsset;

/*****************
*   Class Decl   *
******************/


class MeshGOC sealed : public GameObjectComponent
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Marks if a Mesh uses alpha so it can go through the correct rendering pipeline
        /// </summary>
        bool m_UsesAlpha = false;

        /// <summary>
        /// Game Object Pair with Mesh that this Game Object Component uses
        /// </summary>
        GameObjectAssetPair<Mesh> m_Mesh;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void MeshAssetDeletion(GameAsset* asset);

        AEResult ClearMeshAsset(bool informGameAsset = true);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// MeshGOC Constructor
        /// </summary>
        /// <param name="gameObject">Game Object that this Component is attached too</param>
        MeshGOC(GameObject* gameObject);

        /// <summary>
        /// Default MeshGOC Destructor
        /// </summary>
        virtual ~MeshGOC();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        inline Mesh* GetMeshResource() const
        {
            return m_Mesh.m_ResourceAsset;
        }

        inline uint64_t GetMeshAssetID() const
        {
            return m_Mesh.m_AssetID;
        }

        std::string GetMeshName() const;

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        AEResult SetMeshAsset(MeshAsset* asset);

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        AEResult RemoveMeshAsset();

#pragma endregion

};

#endif
