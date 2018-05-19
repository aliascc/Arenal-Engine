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

#ifndef _BOUNDING_BOX_H
#define _BOUNDING_BOX_H

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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct BoundingBox sealed : public AEObject
{
    glm::vec3 m_MinPos = AEMathHelpers::Vec3fMin;
    glm::vec3 m_MaxPos = AEMathHelpers::Vec3fMax;

    BoundingBox();

    BoundingBox(const glm::vec3& minPos, const glm::vec3& maxPos);

    virtual ~BoundingBox();

    void PushPositionPoint(const glm::vec3& pos);

    glm::vec3 GetSize() const;
};

#endif
