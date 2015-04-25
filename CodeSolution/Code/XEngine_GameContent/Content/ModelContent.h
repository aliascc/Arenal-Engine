/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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
#include "Base\XEObject.h"
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
XETODO("Need to change materials to create material and associate it with shader");
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