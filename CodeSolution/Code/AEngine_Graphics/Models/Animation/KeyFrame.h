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

#ifndef _KEY_FRAME_H
#define _KEY_FRAME_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
#include "Models\Skinning\SkinningDefs.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class KeyFrame sealed : public AEObject
{
    private:
        BonePose m_BonePose;

        int32_t m_BoneIndex = -1;

        float m_Time = 0.0f;

    public:
        KeyFrame();
        KeyFrame(int32_t boneIndex, float time, const BonePose& bonePose);
        ~KeyFrame();

        inline void SetTime(float time)
        {
            m_Time = time;
        }

        inline void SetBonePose(const BonePose& bonePose)
        {
            m_BonePose = bonePose;
        }

        inline void SetBoneIndex(int32_t boneIndex)
        {
            m_BoneIndex = boneIndex;
        }

        inline float GetTime() const
        {
            return m_Time;
        }

        inline int32_t GetBoneIndex() const
        {
            return m_BoneIndex;
        }

        inline const BonePose& GetBonePose() const
        {
            return m_BonePose;
        }

        bool operator<(const KeyFrame& keyframe) const;
};

#endif

