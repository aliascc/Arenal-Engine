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

#ifndef _ANIMATION_ASSET_H
#define _ANIMATION_ASSET_H

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
#include "GameAssets\GameAsset.h"

/********************
*   Forward Decls   *
*********************/
class Animation;
class GraphicDevice;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class AnimationAsset sealed : public GameAsset
{    
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        Animation* m_Animation = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Loads an Asset Resource to Memory
        /// </summary>
        /// <returns>AEResult::OK if successful</returns>
        AEResult LoadAssetResource() override;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// AnimationAsset Constructor
        /// </summary>
        /// <param name="filePath">File Path of Game Asset to Load</param>
        /// <param name="gameResourceManager">Resource Manager to handle the Asset Resource</param>
        AnimationAsset(const std::string& filePath, GameResourceManager* gameResourceManager);

        /// <summary>
        /// Default AnimationAsset Destructor
        /// </summary>
        virtual ~AnimationAsset();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        Animation* GetAnimationReference();

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

#pragma endregion

};

#endif
