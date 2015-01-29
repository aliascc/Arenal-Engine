
/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

#pragma once

#ifndef _MESH_CONTENT_H
#define _MESH_CONTENT_H

/**********************
*   System Includes   *
***********************/
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
	std::wstring m_Name = L"";

	BoundingBox m_BoundingBox;

	BoundingSphere m_BoundingSphere;

	std::vector<MeshPartContent*> m_MeshParts;

	MeshContent();
	virtual ~MeshContent();
};

#endif