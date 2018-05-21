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

#ifndef _OBJECT3D_H
#define _OBJECT3D_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicUtilsDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class Object3D : public UniqueAEObjectNamed
{
    typedef std::pair<uint64_t, Object3DChangedEvent> Object3DChangedEventPair;
    typedef std::list<Object3DChangedEventPair> Object3DChangedEventList;

    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Position of Game Object
        /// </summary>
        glm::vec3 m_Position = AEMathHelpers::Vec3fZero;

        /// <summary>
        /// Rotation of Game Object
        /// </summary>
        glm::vec3 m_Rotation = AEMathHelpers::Vec3fZero;

        /// <summary>
        /// Scale of Game Object
        /// </summary>
        glm::vec3 m_Scale = AEMathHelpers::Vec3fOne;

        /// <summary>
        /// Parent 3D Object
        /// </summary>
        Object3D* m_ParentObject3D = nullptr;

        Object3DChangedEventList m_Object3DChangedEventList;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void CallObject3DChangedEvents(Object3DChangeEventType changeType, uint64_t cbIdIgnore);

#pragma endregion

    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Private Variables

#pragma endregion
        
    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Object3D Constructor
        /// </summary>
        /// <param name="name">Name of Game Object</param>
        /// <param name="position">Position of Game Object</param>
        /// <param name="rotation">Rotation of Game Object in Degrees</param>
        /// <param name="scale">Scale of Game Object</param>
        Object3D(const std::string& name = "", const glm::vec3& position = AEMathHelpers::Vec3fZero, const glm::vec3& rotation = AEMathHelpers::Vec3fZero, const glm::vec3& scale = AEMathHelpers::Vec3fOne);

        /// <summary>
        /// Default Object3D Destructor
        /// </summary>
        virtual ~Object3D();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets Position of the Game Object
        /// </summary>
        /// <returns>Position of 3D Object</returns>
        inline const glm::vec3& GetPosition() const
        {
            return m_Position;
        }

        /// <summary>
        /// Gets Yaw of the Game Object in Degrees
        /// </summary>
        /// <returns>Yaw of the Game Object</returns>
        inline float GetYaw() const
        {
            return m_Rotation.y;
        }

        /// <summary>
        /// Gets Roll of the Game Object in Degrees
        /// </summary>
        /// <returns>Roll of the Game Object</returns>
        inline float GetRoll() const
        {
            return m_Rotation.z;
        }

        /// <summary>
        /// Gets Pitch of the Game Object in Degrees
        /// </summary>
        /// <returns>Pitch of the Game Object</returns>
        inline float GetPitch() const
        {
            return m_Rotation.x;
        }

        /// <summary>
        /// Gets the Rotation of the Game Object in Degrees
        /// </summary>
        /// <returns>Rotation Vector of 3D Object</returns>
        inline const glm::vec3& GetRotation() const
        {
            return m_Rotation;
        }

        /// <summary>
        /// Gets Scale of the Game Object 
        /// </summary>
        /// <returns>Scale of the Game Object</returns>
        inline const glm::vec3& GetScale() const
        {
            return m_Scale;
        }

        /// <summary>
        /// Gets Object 3D Parent
        /// </summary>
        inline Object3D* GetParentObject3D() const
        {
            return m_ParentObject3D;
        }

        /// <summary>
        /// Gets the World Position for this Game Object
        /// </summary>
        /// <returns>World Position</returns>
        glm::vec3 GetWorldPosition() const;

        /// <summary>
        /// Gets the World Rotation Angles for this Game Object
        /// </summary>
        /// <returns>World Rotation Angles</returns>
        glm::vec3 GetWorldRotationAngles() const;

        /// <summary>
        /// Gets the World Rotation Quaternion for this Game Object
        /// </summary>
        /// <returns>World Rotation Quaternion</returns>
        glm::quat GetWorldRotationQuat() const;

        /// <summary>
        /// Gets the World Scale Transformation Matrix for this Game Object
        /// </summary>
        /// <returns>World Scale Transformation Matrix</returns>
        glm::mat4 GetWorldScaleTransform() const;

        /// <summary>
        /// Gets the World Transformation Matrix for this Game Object
        /// </summary>
        /// <returns>World Transformation Matrix</returns>
        glm::mat4 GetWorldTransform() const;

        /// <summary>
        /// Gets the Local Rotation Quaternion for this Game Object
        /// </summary>
        /// <returns>Local Rotation Quaternion</returns>
        glm::quat GetLocalRotationQuat() const;

        /// <summary>
        /// Gets the Local Scale Transformation Matrix for this Game Object
        /// </summary>
        /// <returns>Local Scale Transformation Matrix</returns>
        glm::mat4 GetLocalScaleTransform() const;

        /// <summary>
        /// Gets the Local Transformation Matrix for this Game Object
        /// </summary>
        /// <returns>Local Transformation Matrix</returns>
        glm::mat4 GetLocalTransform() const;

        /// <summary>
        /// Gets the Direction Position for this Game Object
        /// </summary>
        glm::vec3 GetDirection() const;

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        /// <summary>
        /// Set Object 3D Parent
        /// </summary>
        /// <param name="position">Parent Object 3D</param>
        inline void SetParentObject3D(Object3D* parent, uint64_t cbIdIgnore = 0)
        {
            if (m_ParentObject3D == parent)
            {
                return;
            }

            m_ParentObject3D = parent;

            CallObject3DChangedEvents(Object3DChangeEventType::ParentChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Position of a 3D Object
        /// </summary>
        /// <param name="position">New Position of the Game Object</param
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetPosition(const glm::vec3& position, uint64_t cbIdIgnore = 0)
        {
            if (m_Position == position)
            {
                return;
            }

            m_Position = position;

            CallObject3DChangedEvents(Object3DChangeEventType::PositionChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Position of X Axis of a 3D Object
        /// </summary>
        /// <param name="posX">New Position for X Axis of the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetPositionX(float posX, uint64_t cbIdIgnore = 0)
        {
            if (m_Position.x == posX)
            {
                return;
            }

            m_Position.x = posX;

            CallObject3DChangedEvents(Object3DChangeEventType::PositionChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Position of Y Axis of a 3D Object
        /// </summary>
        /// <param name="posY">New Position for Y Axis of the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetPositionY(float posY, uint64_t cbIdIgnore = 0)
        {
            if (m_Position.y == posY)
            {
                return;
            }

            m_Position.y = posY;

            CallObject3DChangedEvents(Object3DChangeEventType::PositionChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Position of Z Axis of a 3D Object
        /// </summary>
        /// <param name="posZ">New Position for Z Axis of the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetPositionZ(float posZ, uint64_t cbIdIgnore = 0)
        {
            if (m_Position.z == posZ)
            {
                return;
            }

            m_Position.z = posZ;

            CallObject3DChangedEvents(Object3DChangeEventType::PositionChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Rotation Vector of the Game Object
        /// </summary>
        /// <param name="rotation">New Rotation the Game Object in Degrees</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetRotation(const glm::vec3& rotation, uint64_t cbIdIgnore = 0)
        {
            if (m_Rotation == rotation)
            {
                return;
            }

            m_Rotation = rotation;

            CallObject3DChangedEvents(Object3DChangeEventType::RotationChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Yaw of a 3D Object
        /// </summary>
        /// <param name="yaw">New Yaw of the Game Object in Degrees</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetYaw(float yaw, uint64_t cbIdIgnore = 0)
        {
            if (m_Rotation.y == yaw)
            {
                return;
            }

            m_Rotation.y = yaw;

            CallObject3DChangedEvents(Object3DChangeEventType::RotationChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Roll of a 3D Object
        /// </summary>
        /// <param name="roll">New Roll of the Game Object in Degrees</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetRoll(float roll, uint64_t cbIdIgnore = 0)
        {
            if (m_Rotation.z == roll)
            {
                return;
            }

            m_Rotation.z = roll;

            CallObject3DChangedEvents(Object3DChangeEventType::RotationChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Pitch of a 3D Object
        /// </summary>
        /// <param name="pitch">New Pitch of the Game Object in Degrees</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetPitch(float pitch, uint64_t cbIdIgnore = 0)
        {
            if (m_Rotation.x == pitch)
            {
                return;
            }

            m_Rotation.x = pitch;

            CallObject3DChangedEvents(Object3DChangeEventType::RotationChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Scale of the Game Object
        /// </summary>
        /// <param name="scale">New Scale the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetScale(const glm::vec3& scale, uint64_t cbIdIgnore = 0)
        {
            if (m_Scale == scale)
            {
                return;
            }

            m_Scale = scale;

            CallObject3DChangedEvents(Object3DChangeEventType::ScaleChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Scale of X Axis of a 3D Object
        /// </summary>
        /// <param name="sclX">New Scale for X Axis of the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetScaleX(float sclX, uint64_t cbIdIgnore = 0)
        {
            if (m_Scale.x == sclX)
            {
                return;
            }

            m_Scale.x = sclX;

            CallObject3DChangedEvents(Object3DChangeEventType::ScaleChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Scale of Y Axis of a 3D Object
        /// </summary>
        /// <param name="sclY">New Scale for Y Axis of the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetScaleY(float sclY, uint64_t cbIdIgnore = 0)
        {
            if (m_Scale.y == sclY)
            {
                return;
            }

            m_Scale.y = sclY;

            CallObject3DChangedEvents(Object3DChangeEventType::ScaleChange, cbIdIgnore);
        }

        /// <summary>
        /// Sets the Scale of Z Axis of a 3D Object
        /// </summary>
        /// <param name="sclZ">New Scale for Z Axis of the Game Object</param>
        /// <param name="cbIdIgnore">Callback ID to ignore to send event about change</param>
        inline void SetScaleZ(float sclZ, uint64_t cbIdIgnore = 0)
        {
            if (m_Scale.z == sclZ)
            {
                return;
            }

            m_Scale.z = sclZ;

            CallObject3DChangedEvents(Object3DChangeEventType::ScaleChange, cbIdIgnore);
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        bool ExistsObject3DChangeEventCallback(uint64_t id);

        AEResult AddObject3DChangeEventCallback(uint64_t id, Object3DChangedEvent changeEventCallback);

        AEResult RemoveObject3DChangeEventCallback(uint64_t id);

        /// <summary>
        /// Updates Game Objects transform so it will not move places when changing parents
        /// </summary>
        /// <param name="newParentTransform">The new Parent's World Transform</param>
        /// <param name="newParentQuaternionRot">The new Parent's World Quaternion Rotation</param>
        /// <param name="newParentScaleTransform">The new Parent's World Scale Transform</param>
        void UpdateTransformToNewParent(const glm::mat4& newParentTransform, const glm::quat& newParentQuaternionRot, const glm::mat4& newParentScaleTransform);

#pragma endregion

};

#endif
