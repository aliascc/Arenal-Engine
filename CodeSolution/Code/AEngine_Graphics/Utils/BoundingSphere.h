/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#ifndef _BOUNDING_SPHERE_H
#define _BOUNDING_SPHERE_H

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

/******************
*   Struct Decl   *
*******************/
struct BoundingSphere sealed : public AEObject
{
    float m_Radius = 0.0f;

    BoundingSphere();

    BoundingSphere(float radius);

    virtual ~BoundingSphere();

    void PushPositionPoint(const glm::vec3& pos);
};

#endif
