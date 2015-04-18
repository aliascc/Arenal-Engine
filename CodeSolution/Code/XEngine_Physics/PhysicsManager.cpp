/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
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
#include "PhysicsActor.h"
#include "PhysicsManager.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

XETODO("Add mutex");
PhysicsManager::PhysicsManager()
{
}

PhysicsManager::~PhysicsManager()
{
	CleanUp();
}

void PhysicsManager::CleanUp()
{
	for each (auto actor in m_PhysicsActorMap)
	{
		PhysicsActor* PhysicsActor = actor.second;

		m_PxScene->removeActor(*PhysicsActor->GetPxRigidActor());

		DeleteMem(PhysicsActor);
	}
	m_PhysicsActorMap.clear();

	ReleasePhysX(m_PxDebuggerConnection);
	ReleasePhysX(m_PxScene);
	ReleasePhysX(m_CpuDispatcher);
	ReleasePhysX(m_PxPhysics);
	ReleasePhysX(m_PxFoundation);
}

XEResult PhysicsManager::Initialize()
{
	if (m_IsReady)
	{
		return XEResult::Ok;
	}

	////////////////////////////////////
	//Initialize Physics Objects

	m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxDefaultAllocatorCallback, m_ErrorCallbackPhysX);
	if (m_PxFoundation == nullptr)
	{
		CleanUp();

		XETODO("Log, Better return code");
		return XEResult::Fail;
	}

	m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, physx::PxTolerancesScale());
	if (m_PxPhysics == nullptr)
	{
		CleanUp();

		XETODO("Log, Better return code");
		return XEResult::Fail;
	}

	// Create the scene
	physx::PxSceneDesc sceneDesc(m_PxPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -m_DefaultGravity, 0.0f);

	if (!sceneDesc.cpuDispatcher)
	{
		m_CpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);

		sceneDesc.cpuDispatcher = m_CpuDispatcher;
	}

	if (!sceneDesc.filterShader)
	{
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
	}

	m_PxScene = m_PxPhysics->createScene(sceneDesc);
	if (m_PxScene == nullptr)
	{
		CleanUp();

		XETODO("Log, Better return code");
		return XEResult::Fail;
	}

	////////////////////////////////////
	//Finish
	m_IsReady = true;

	return XEResult::Ok;
}

XEResult PhysicsManager::ConnectToPhysXDebugger(const std::wstring& ip, uint32_t port, uint32_t timeout)
{
	//////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_PxDebuggerConnection != nullptr)
	{
		XETODO("better ret code");
		return XEResult::Fail;
	}

	//////////////////////////////
	//Get Connection Manager
	physx::PxVisualDebuggerConnectionManager* pxDebuggerConnManager = m_PxPhysics->getPvdConnectionManager();
	if (pxDebuggerConnManager == nullptr)
	{
		return XEResult::NullObj;
	}

	//////////////////////////////
	//Get Variables needed
	std::string ipStr = XE_Base::WideStr2String(ip);
	physx::PxVisualDebuggerConnectionFlags connectionFlags = physx::PxVisualDebuggerExt::getAllConnectionFlags();

	//////////////////////////////
	//Connect
	m_PxDebuggerConnection = physx::PxVisualDebuggerExt::createConnection(pxDebuggerConnManager, ipStr.c_str(), port, timeout, connectionFlags);
	if (m_PxDebuggerConnection == nullptr)
	{
		XETODO("better ret code");
		return XEResult::Fail;
	}

	//////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult PhysicsManager::DisconnectToPhysXDebugger()
{
	//////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if (m_PxDebuggerConnection == nullptr)
	{
		return XEResult::Ok;
	}

	//////////////////////////////
	//Release Debugger and it disconnects
	m_PxDebuggerConnection->disconnect();

	ReleasePhysX(m_PxDebuggerConnection);

	//////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult PhysicsManager::Update(const TimerParams& timerParams)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	float elapsedTime = (float)timerParams.m_ElapsedTime;

	m_PxScene->simulate(elapsedTime);

	m_PxScene->fetchResults(true);

	UpdatePhysicsActorObject3D();

	return XEResult::Ok;
}

XEResult PhysicsManager::UpdatePhysicsActorObject3D()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	for each (auto actor in m_PhysicsActorMap)
	{
		PhysicsActor* physicsActor = actor.second;

		physicsActor->UpdateObject3D();
	}

	return XEResult::Ok;
}

bool PhysicsManager::ExistsPhysicsActor(uint64_t id)
{
	return (m_PhysicsActorMap.find(id) != m_PhysicsActorMap.end());
}

XEResult PhysicsManager::AddPhysicsActor(PhysicsActor* physicsActor)
{
	XEAssert(physicsActor != nullptr);
	if (physicsActor == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (!physicsActor->IsReady())
	{
		XETODO("Add better return code");
		return XEResult::Fail;
	}

	if (ExistsPhysicsActor(physicsActor->GetUniqueID()))
	{
		return XEResult::ObjExists;
	}

	m_PxScene->addActor(*physicsActor->GetPxRigidActor());

	m_PhysicsActorMap[physicsActor->GetUniqueID()] = physicsActor;

	return XEResult::Ok;
}

XEResult PhysicsManager::RemovePhysicsActor(uint64_t id, bool deleteObj)
{
	if (!ExistsPhysicsActor(id))
	{
		return XEResult::NotFound;
	}

	PhysicsActor* physicsActor = m_PhysicsActorMap[id];

	m_PxScene->removeActor(*physicsActor->GetPxRigidActor());

	m_PhysicsActorMap.erase(id);

	if (deleteObj)
	{
		DeleteMem(physicsActor);
	}

	return XEResult::Ok;
}
