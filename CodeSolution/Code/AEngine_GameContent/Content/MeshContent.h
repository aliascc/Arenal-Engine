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

#ifndef _MESH_CONTENT_H
#define _MESH_CONTENT_H

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
#include "GameContent.h"
#include "Base\AEObject.h"
#include "Utils\BoundingBox.h"
#include "Utils\BoundingSphere.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct MeshPartContent;

/******************
*   Struct Decl   *
*******************/
struct MeshContent sealed : public GameContent
{
    std::string m_Name = "";

    BoundingBox m_BoundingBox;

    BoundingSphere m_BoundingSphere;

    std::vector<MeshPartContent*> m_MeshParts;

    MeshContent();
    virtual ~MeshContent();
};

#endif
