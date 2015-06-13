/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameAssetDefs.h"
#include "Models\Custom\CubeMesh.h"
#include "Models\Custom\IcoSphereMesh.h"
#include "Shaders\Custom\DiffuseTextureVS.h"
#include "Shaders\Custom\DiffuseTexturePS.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*******************************
*   Struct GameAssetBuiltIns   *
********************************/
GameAssetBuiltIns::GameAssetBuiltIns()
{
}

GameAssetBuiltIns::~GameAssetBuiltIns()
{
	AERelease(m_CubeMesh);
	AERelease(m_IcoSphereMesh);
	AERelease(m_DiffuseTextureVS);
	AERelease(m_DiffuseTexturePS);
}

/************************************
*   Struct GameAssetEventHandlers   *
*************************************/
GameAssetEventHandlers::GameAssetEventHandlers()
{
}

GameAssetEventHandlers::GameAssetEventHandlers(uint64_t objID, OnGameAssetDeletionEvent onGameAssetDeletionEvent, OnGameAssetReloadEvent onGameAssetReloadEvent, OnGameAssetPreReloadEvent onGameAssetPreReloadEvent)
	: m_ObjID(objID)
	, m_OnGameAssetDeletionEvent(onGameAssetDeletionEvent)
	, m_OnGameAssetReloadEvent(onGameAssetReloadEvent)
	, m_OnGameAssetPreReloadEvent(onGameAssetPreReloadEvent)
{
}
