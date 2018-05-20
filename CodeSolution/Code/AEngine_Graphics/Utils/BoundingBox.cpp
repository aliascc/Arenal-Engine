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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "BoundingBox.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos)
    : m_MinPos(minPos)
    , m_MaxPos(maxPos)
{
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::PushPositionPoint(const glm::vec3& pos)
{
    if (m_MinPos.x > pos.x)
    {
        m_MinPos.x = pos.x;
    }
    if (m_MinPos.y > pos.y)
    {
        m_MinPos.y = pos.y;
    }
    if (m_MinPos.z > pos.z)
    {
        m_MinPos.z = pos.z;
    }

    if (m_MaxPos.x < pos.x)
    {
        m_MaxPos.x = pos.x;
    }
    if (m_MaxPos.y < pos.y)
    {
        m_MaxPos.y = pos.y;
    }
    if (m_MaxPos.z < pos.z)
    {
        m_MaxPos.z = pos.z;
    }
}

glm::vec3 BoundingBox::GetSize() const
{
    return (m_MaxPos - m_MinPos);
}
