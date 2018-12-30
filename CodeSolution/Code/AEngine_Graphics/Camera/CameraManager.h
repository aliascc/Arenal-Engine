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

#ifndef _CAMERA_MANAGER_H
#define _CAMERA_MANAGER_H

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
class CameraManager sealed : public AEObject
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

        //Delete copy constructor/operator
        CameraManager(const CameraManager&) = delete;
        CameraManager& operator=(const CameraManager&) = delete;

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

        AEResult SetMainCamera(uint64_t cameraID);

        AEResult SetDefaultCamera(uint64_t cameraID);

        AEResult SetEditorCamera(uint64_t cameraID);

        uint64_t GetDefaultCameraID();

        AEResult SetDefaultCameraAsMain();

        AEResult SetEditorCameraAsMain();

        AEResult AddCamera(Camera* camera);

        const Camera* GetCamera(uint64_t cameraID);

        AEResult RemoveCamera(uint64_t cameraID);

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
