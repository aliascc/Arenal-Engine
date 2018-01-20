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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "MeshContent.h"
#include "ModelContent.h"
#include "SkeletonContent.h"
#include "AnimationContent.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ModelContent::ModelContent()
{
    m_SkeletonContent = new SkeletonContent();
}

ModelContent::~ModelContent()
{
    //Clean Skeleton
    DeleteMem(m_SkeletonContent);

    //Clean Animations
    for(auto itAnim : m_Animations)
    {
        DeleteMem(itAnim.second);
    }

    m_Animations.clear();

    //Clean Meshes
    for(auto itMesh : m_Meshes)
    {
        DeleteMem(itMesh.second);
    }

    m_Meshes.clear();
}
