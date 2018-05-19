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
#include "BoundingSphere.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BoundingSphere::BoundingSphere()
{
}

BoundingSphere::BoundingSphere(float radius)
    : m_Radius(radius)
{
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::PushPositionPoint(const glm::vec3& pos)
{
    if (m_Radius < glm::abs(pos.x))
    {
        m_Radius = pos.x;
    }

    if (m_Radius < glm::abs(pos.y))
    {
        m_Radius = pos.y;
    }

    if (m_Radius < glm::abs(pos.z))
    {
        m_Radius = pos.z;
    }
}
