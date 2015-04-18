/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 5/14/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

#pragma once

#ifndef _DEFERRED_LIGHT_MANAGER_H
#define _DEFERRED_LIGHT_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <string>
#include <map>
#include <vector>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\GameComponent.h"
#include "Base\Base.h"
#include "DeferredRenderingDefs.h"
#include "Lights\LightCollection.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class Light;

/*****************
*   Class Decl   *
******************/
class DeferredLightManager : public GameComponent
{
	protected:

		/****************************
		 *   Protected Variables    *
		 ****************************/
#pragma region Protected Variables

		///<summary>
		///Light Vector
		///</summart>
		LightCollection m_LightCollection;

		///<summary>
		///Service Name
		///</summary>
		const std::string m_ServiceName;

#pragma endregion

		/**************************
		 *   Protected Methods    *
		 **************************/
#pragma region Protected Methods

#pragma endregion

	public:

		/*****************************************
		 *   Constructor & Destructor Methods   *
		 *****************************************/
#pragma region Constructor & Destructor Methods

		///<summary>
		///DeferredLightManager Constructor
		///</summary>
		DeferredLightManager(GameApp* gameApp, const std::string& serviceName = "DeferredLightManagerService",  const std::string& name = "Deferred Light Manager", uint32_t callOrder = 3);

		///<summary>
		///Default DeferredLightManager Destructor
		///</summary>
		virtual ~DeferredLightManager();

#pragma endregion
		
		/**********************
		 *     Get Methods    *
		 **********************/
#pragma region Get Methods

		///<summary>
		///Gets the Light Collection
		///</summary>
		///<returns>Returns the Collection of Lights</returns>
		const LightCollection& GetLightCollection() const
		{
			return m_LightCollection;
		}

#pragma endregion
		
		/**********************
		 *     Set Methods    *
		 **********************/
#pragma region Set Methods

#pragma endregion
		
		/****************************
		 *     Framework Methods    *
		 ****************************/
#pragma region Framework Methods
		
		///<summary>
		///Initialize Methods of Game Component
		///Here the GC's variables are initiated and GC is prepare to run
		///</summary>
		void Initialize();

		///<summary>
		///Load's the Content of the Game Component to memory
		///</summary>
		void LoadContent();

		///<summary>
		///Unloads the Content of the Game Component that are in memory
		///</summary>
		void UnLoadContent();

		///<summary>
		///Update method for the Game Component called each frame if the GC is active
		///</summary>
		///<param name="timerParams">Game Timer Parameters</param>
		void Update(const TimerParams& timerParams);

		///<summary>
		///If the Graphic Device is Lost this method will be called and GC should handle decide what to do 
		///</summary>
		void OnLostDevice();

		///<summary>
		///If the Graphic Device is Lost and it is recover this methods will be called and GC should handle decide what to do 
		///</summary>
		void OnResetDevice();

#pragma endregion

};

#endif