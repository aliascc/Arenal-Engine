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

#ifndef _PHYSICS_MANAGER_H
#define _PHYSICS_MANAGER_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ErrorCallbackPhysX.h"

/********************
*   Forward Decls   *
*********************/
class PhysicsActor;
struct TimerParams;

/**************
*   Typedef   *
***************/
typedef std::map<uint64_t, PhysicsActor*> PhysicsActorMap;
typedef PhysicsActorMap::iterator PhysicsActorMapIt;
typedef PhysicsActorMap::const_iterator PhysicsActorMapItConst;

/*****************
*   Class Decl   *
******************/

class PhysicsManager sealed : public AEObject
{
    private:

    /************************
    *   Private Variables   *
    *************************/
#pragma region Private Variables

        /// <summary>
        /// PhysX Default CPU Dispatcher
        /// </summary>
        physx::PxDefaultCpuDispatcher* m_CpuDispatcher = nullptr;

        /// <summary>
        /// PhysX Pvd Debugger Connection
        /// </summary>
        physx::PxPvd* m_PxPvd = nullptr;

        /// <summary>
        /// PhysX Pvd Transport
        /// </summary>
        physx::PxPvdTransport* m_PxPvdTransport = nullptr;

        /// <summary>
        /// Default Allocator for PhysX
        /// </summary>
        physx::PxDefaultAllocator m_PxDefaultAllocatorCallback;

        /// <summary>
        /// Defines if the PVD is connected
        /// </summary>
        bool mIsPVDConnected = false;

        /// <summary>
        /// PhysX Foundation Instance
        /// </summary>
        physx::PxFoundation* m_PxFoundation = nullptr;

        /// <summary>
        /// PhysX Instance
        /// </summary>
        physx::PxPhysics* m_PxPhysics = nullptr;

        /// <summary>
        /// Error Callback Class to log PhysX Errors
        /// </summary>
        ErrorCallbackPhysX m_ErrorCallbackPhysX;

        /// <summary>
        /// PhysX Scene Instance
        /// </summary>
        physx::PxScene* m_PxScene = nullptr;

        /// <summary>
        /// Default Gravity
        /// </summary>
        float m_DefaultGravity = 9.8f;

        /// <summary>
        /// Defines if physics engine is ready to process data.
        /// </summary>
        bool m_IsReady = false;

        PhysicsActorMap m_PhysicsActorMap;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        void CleanUp();

        AEResult UpdatePhysicsActorObject3D();

#pragma endregion

    public:

    /***************************************
    *   Constructor & Destructor Methods   *
    ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default PhysicsManager Constructor
        /// </summary>
        PhysicsManager();

        /// <summary>
        /// Default PhysicsManager Destructor
        /// </summary>
        virtual ~PhysicsManager();

#pragma endregion

    /******************
    *   Get Methods   *
    *******************/
#pragma region Get Methods

        physx::PxPhysics* GetPhysX() const
        {
            return m_PxPhysics;
        }

        physx::PxScene* GetPxScene() const
        {
            return m_PxScene;
        }

        inline bool IsReady() const
        {
            return m_IsReady;
        }

#pragma endregion

    /******************
    *   Set Methods   *
    *******************/
#pragma region Set Methods

#pragma endregion

    /************************
    *   Framework Methods   *
    *************************/
#pragma region Framework Methods

        AEResult Initialize();

        AEResult Update(const TimerParams& timerParams);

        AEResult ConnectToPhysXDebugger(const std::string& ip = "127.0.0.1", uint32_t port = 5425, uint32_t timeout = 100);

        AEResult DisconnectToPhysXDebugger();

        AEResult AddPhysicsActor(PhysicsActor* physicsActor);

        AEResult RemovePhysicsActor(uint64_t id, bool deleteObj = true);

        bool ExistsPhysicsActor(uint64_t id);

        PhysicsActorMapIt begin();

        PhysicsActorMapIt end();

        PhysicsActorMapItConst begin() const;

        PhysicsActorMapItConst end() const;

        PhysicsActorMapItConst cbegin() const;

        PhysicsActorMapItConst cend() const;

#pragma endregion

};

#endif
