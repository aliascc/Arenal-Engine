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

#ifndef _BONE_H
#define _BONE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class Bone sealed : public Named
{
    private:
        glm::mat4 m_LocalMatrix = AEMathHelpers::Mat4Identity;

        glm::mat4 m_WorldMatrix = AEMathHelpers::Mat4Identity;

        glm::mat4 m_OffSetMatrix = AEMathHelpers::Mat4Identity;

        int32_t m_ParentIndex = -1;

        int32_t m_Index = -1;

    public:
        Bone();
        ~Bone();

        inline const glm::mat4& GetWorldMatrix() const 
        {
            return m_WorldMatrix;
        }

        inline const glm::mat4& GetLocalMatrix() const 
        {
            return m_LocalMatrix;
        }

        inline const glm::mat4& GetOffSetMatrix() const 
        {
            return m_OffSetMatrix;
        }

        inline int32_t GetParentIndex() const
        {
            return m_ParentIndex;
        }

        inline int32_t GetIndex() const
        {
            return m_Index;
        }

        inline void SetIndex(int32_t index)
        {
            m_Index = index;
        }

        inline void SetParentIndex(int32_t index)
        {
            m_ParentIndex = index;
        }

        inline void SetWorldMatrix(const glm::mat4& world)
        {
            m_WorldMatrix = world;
        }

        inline void SetLocalMatrix(const glm::mat4& local)
        {
            m_LocalMatrix = local;
        }

        inline void SetOffSetMatrix(const glm::mat4& offsetMatrix)
        {
            m_OffSetMatrix = offsetMatrix;
        }
};

#endif
