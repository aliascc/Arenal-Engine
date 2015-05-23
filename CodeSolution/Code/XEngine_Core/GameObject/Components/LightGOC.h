/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _LIGHT_GOC_H
#define _LIGHT_GOC_H

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
#include "Lights\LightDefs.h"
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Light;
class GameAsset;
class LightManager;

/*****************
*   Class Decl   *
******************/

class LightGOC sealed : public GameObjectComponent
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		LightManager* m_LightManager = nullptr;

		bool m_DrawFrustumCascadesEnabled = false;

		/// <summary>
		/// Defines if debug draw of the camera is enabled
		/// </summary>
		bool m_DebugDrawEnabled = false;

		/// <summary>
		/// Gets the if the Frustum Draw is a Simple draw
		/// </summary>
		bool m_DrawFrustumSimple = true;

		/// <summary>
		/// Defines if lights's frustum draw is enabled
		/// </summary>
		bool m_DrawFrustumEnabled = false;

		Light* m_Light = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// LightGOC Constructor
		/// </summary>
		/// <param name="gameObject>Game Object that this Component is attached too</param>
		/// <param name="lightManager">Light Manager to associate Light</param>
		LightGOC(GameObject* gameObject, LightManager* lightManager);

		/// <summary>
		/// Default LightGOC Destructor
		/// </summary>
		virtual ~LightGOC();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Light Object
		/// </summary>
		/// <returns>Returns the Light Object</returns>
		inline Light* GetLight() const
		{ 
			return m_Light; 
		}

		/// <summary>
		/// Gets the if the Frustum Draw if enabled
		/// </summary>
		inline bool IsDrawFrustumEnabled() const
		{
			return m_DrawFrustumEnabled;
		}

		/// <summary>
		/// Gets the if the Frustum Draw is a Simple draw
		/// </summary>
		inline bool IsDrawFrustumSimple() const
		{
			return m_DrawFrustumSimple;
		}

		/// <summary>
		/// Gets if the Debug Light mode is to be drawn
		/// </summary>
		inline bool IsDrawDebugEnabled() const
		{
			return m_DebugDrawEnabled;
		}

		/// <summary>
		/// Gets if the Frustum Cascades are to be drawn
		/// </summary>
		inline bool IsDrawFrustumCascadesEnabled() const
		{
			return m_DrawFrustumCascadesEnabled;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the if the Frustum Draw is enabled
		/// </summary>
		/// <param name="enabled">Sets if the frustum is enabled</param>
		inline void SetDrawFrustumEnabled(bool enabled)
		{
			m_DrawFrustumEnabled = enabled;
		}

		/// <summary>
		/// Sets the if the Frustum Draw is a simple draw
		/// </summary>
		/// <param name="enabled">Sets if the frustum is a simple draw</param>
		inline void SetDrawFrustumSimple(bool simple)
		{
			m_DrawFrustumSimple = simple;
		}

		/// <summary>
		/// Sets the if the Debug Light mode is to be drawn
		/// </summary>
		/// <param name="enabled">Sets if the Debug Light mode is to be drawn</param>
		inline void SetDrawDebugEnabled(bool enabled)
		{
			m_DebugDrawEnabled = enabled;
		}

		/// <summary>
		/// Sets if the Frustum Cascades are to be drawn
		/// </summary>
		inline void SetDrawFrustumCascadesEnabled(bool drawCascades)
		{
			m_DrawFrustumCascadesEnabled = drawCascades;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Changes the Light Type
		/// </summary>
		/// <param name="lightType">Light Type</param>
		/// <returns>Returns XEResult::OK if successful</returns>
		XEResult ChangeLightType(LightType lightType);

#pragma endregion

};

#endif