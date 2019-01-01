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

#ifndef _SKELETON_H
#define _SKELETON_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Resource\GameResource.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Bone;

/*****************
*   Class Decl   *
******************/
class Skeleton sealed : public GameResource
{
    private:
        std::vector<Bone*> m_BoneHierarchy;

        void CleanUp();

        virtual ~Skeleton();

    public:

        /// <summary>
        /// Skeleton Constructor
        /// </summary>
        /// <param name="resourceName">Resource Name</param>
        Skeleton(const std::string& resourceName);

        inline uint32_t GetSkeletonSize() const
        {
            return (uint32_t)m_BoneHierarchy.size();
        }
        
        AEResult Load() override;

        Bone* GetBone(uint32_t index) const;

        Bone* operator[](uint32_t index) const;
        
        AEResult AddBone(Bone* bone);
};

#endif
