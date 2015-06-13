/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _PHYSICS_MANAGER_H
#define _PHYSICS_MANAGER_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "PxPhysicsAPI.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"
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
		/// PhysX Debugger Connection
		/// </summary>
		physx::PxVisualDebuggerConnection* m_PxDebuggerConnection = nullptr;

		/// <summary>
		/// Default Allocator for PhysX
		/// </summary>
		physx::PxDefaultAllocator m_PxDefaultAllocatorCallback;

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

		AEResult ConnectToPhysXDebugger(const std::wstring& ip = L"127.0.0.1", uint32_t port = 5425, uint32_t timeout = 100);

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