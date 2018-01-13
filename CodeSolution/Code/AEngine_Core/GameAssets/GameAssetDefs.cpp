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
