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

#ifndef _CAMERA_GOC_H
#define _CAMERA_GOC_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameObject\GameObjectComponent.h"

/********************
*   Forward Decls   *
*********************/
class Camera;
class CameraManager;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/

class CameraGOC sealed : public GameObjectComponent
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Camera Manager that manages the games camera.
        /// </summary>
        CameraManager* m_CameraManager = nullptr;

        /// <summary>
        /// Defines if the object is ready to run.
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Defines if debug draw of the camera is enabled
        /// </summary>
        bool m_DebugDrawEnabled = false;

        /// <summary>
        /// Gets the if the Frustum Draw is a Simple draw
        /// </summary>
        bool m_DrawFrustumSimple = true;

        /// <summary>
        /// Defines if camera's frustum draw is enabled
        /// </summary>
        bool m_DrawFrustumEnabled = false;

        /// <summary>
        /// Camera associated with this GOC
        /// </summary>
        Camera* m_Camera = nullptr;

        /// <summary>
        /// Graphic Device that are use with the Game App.
        /// </summary>
        GraphicDevice* m_GraphicDevice = nullptr;

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// CameraGOC Constructor
        /// </summary>
        /// <param name="gameObject">Game Object that this Component is attached too</param>
        /// <param name="cameraManager">Camera Manager to associate Camera.</param>
        /// <param name="graphicDevice">Graphic Device that are use with the Game App.</param>
        CameraGOC(GameObject* gameObject, CameraManager* cameraManager, GraphicDevice* graphicDevice);

        /// <summary>
        /// Default CameraGOC Destructor
        /// </summary>
        virtual ~CameraGOC();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets the Camera link to this GOC
        /// </summary>
        Camera* GetCamera() const
        {
            return m_Camera;
        }

        /// <summary>
        /// Gets if the Debug Camera mode is to be drawn
        /// </summary>
        inline bool IsDebugDrawEnabled() const
        {
            return m_DebugDrawEnabled;
        }

        /// <summary>
        /// Gets the if the Frustum Draw is a Simple draw
        /// </summary>
        inline bool IsDrawFrustumSimple() const
        {
            return m_DrawFrustumSimple;
        }

        /// <summary>
        /// Gets the if the Frustum Draw if enabled
        /// </summary>
        inline bool IsDrawFrustumEnabled() const
        {
            return m_DrawFrustumEnabled;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        /// <summary>
        /// Sets the if the Debug Camera mode is to be drawn
        /// </summary>
        /// <param name="enabled">Sets if the Debug Camera mode is to be drawn</param>
        inline void SetDebugDrawEnabled(bool enabled)
        {
            m_DebugDrawEnabled = enabled;
        }

        /// <summary>
        /// Sets the if the Frustum Draw is enabled
        /// </summary>
        /// <param name="enabled">Sets if the frustum is enabled</param>
        inline void SetDrawFrustumEnabled(bool enabled)
        {
            m_DrawFrustumEnabled = enabled;
        }

        /// <summary>
        /// Sets the if the Frustum Draw is to be draw simple
        /// </summary>
        /// <param name="enabled">Sets if the frustum is to be draw simple</param>
        inline void SetDrawFrustumSimple(bool enabled)
        {
            m_DrawFrustumSimple = enabled;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Determines if this GOC has the default camera.
        /// </summary>
        /// <return>True is this GOC is the default camera.</return>
        bool IsDefaultCamera();

        /// <summary>
        /// Sets the camera as the default
        /// </summary>
        /// <return>AEResult::Ok on successful, otherwise error code.</return>
        AEResult SetAsDefaultCamera();

#pragma endregion

};

#endif
