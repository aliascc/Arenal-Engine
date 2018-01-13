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

#ifndef _MODEL_CONTENT_H
#define _MODEL_CONTENT_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>
#include <vector>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameContent.h"
#include "Base\AEObject.h"
#include "ContentDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct MeshContent;
struct SkeletonContent;
struct AnimationContent;

/******************
*   Struct Decl   *
*******************/
AETODO("Need to change materials to create material and associate it with shader");
struct ModelContent sealed : public GameContent
{
	std::wstring m_Name = L"";

	std::wstring m_FileName = L"";

	SkeletonContent* m_SkeletonContent = nullptr;

	std::map<std::wstring, AnimationContent*> m_Animations;

	std::vector<MaterialHolder> m_Materials;

	std::map<std::wstring, MeshContent*> m_Meshes;

	ModelContent();
	virtual ~ModelContent();
};

#endif