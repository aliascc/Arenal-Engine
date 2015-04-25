/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _CAMERA_MANAGER_H
#define _CAMERA_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class Camera;

/***************
*   Typedefs   *
****************/
typedef std::map<uint64_t, Camera*> CameraMap;

typedef CameraMap::iterator CameraMapIt;

typedef CameraMap::const_iterator CameraMapItConst;

/*****************
*   Class Decl   *
******************/
class CameraManager sealed : public XEObject
{

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		bool m_RemoveMainCamera = false;

		Camera* m_TempMainCamera = nullptr;

		Camera* m_DefaultCamera = nullptr;

		Camera* m_EditorCamera = nullptr;

		Camera* m_MainCamera = nullptr;

		CameraMap m_CameraMap;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// CameraManager Constructor
		/// </summary>
		CameraManager();

		/// <summary>
		/// Default CameraManager Destructor
		/// </summary>
		virtual ~CameraManager();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the number of Cameras Manage by the manager
		/// </summary>
		/// <returns>Number of cameras that are being managed</returns>
		inline uint32_t GetSize() const
		{
			return static_cast<uint32_t>(m_CameraMap.size());
		}

		/// <summary>
		/// Gets the number of Cameras
		/// </summary>
		/// <returns>Returns the number of Cameras</returns>
		uint32_t GetNumberOfCamerass() const;

		inline Camera* GetMainCamera() const
		{
			return m_MainCamera;
		}

		inline Camera* GetDefaultCamera() const
		{
			return m_DefaultCamera;
		}

		inline Camera* GetEditorCamera() const
		{
			return m_EditorCamera;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult SetMainCamera(uint64_t cameraID);

		XEResult SetDefaultCamera(uint64_t cameraID);

		XEResult SetEditorCamera(uint64_t cameraID);

		uint64_t GetDefaultCameraID();

		XEResult SetDefaultCameraAsMain();

		XEResult SetEditorCameraAsMain();

		XEResult AddCamera(Camera* camera);

		const Camera* GetCamera(uint64_t cameraID);

		XEResult RemoveCamera(uint64_t cameraID);

		bool CameraExists(uint64_t cameraID);

		void Update(const TimerParams& timerParams);

		CameraMapIt begin();

		CameraMapIt end();

		CameraMapItConst begin() const;

		CameraMapItConst end() const;

		CameraMapItConst cbegin() const;

		CameraMapItConst cend() const;

#pragma endregion

};

#endif