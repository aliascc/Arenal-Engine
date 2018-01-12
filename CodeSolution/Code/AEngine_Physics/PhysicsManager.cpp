/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
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

AETODO("Add mutex");
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

	ReleasePhysX(m_PxPvdTransport);
	ReleasePhysX(m_PxScene);
	ReleasePhysX(m_CpuDispatcher);
	ReleasePhysX(m_PxPhysics);
	ReleasePhysX(m_PxPvd);
	ReleasePhysX(m_PxFoundation);
}

AEResult PhysicsManager::Initialize()
{
	if (m_IsReady)
	{
		return AEResult::Ok;
	}

	////////////////////////////////////
	//Initialize Physics Objects

	m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxDefaultAllocatorCallback, m_ErrorCallbackPhysX);
	if (m_PxFoundation == nullptr)
	{
		CleanUp();

		AETODO("Log, Better return code");
		return AEResult::Fail;
	}

	m_PxPvd = physx::PxCreatePvd(*m_PxFoundation);
	if (m_PxFoundation == nullptr)
	{
		CleanUp();

		AETODO("Log, Better return code");
		return AEResult::Fail;
	}

	m_PxPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, physx::PxTolerancesScale(), m_PxPvd);
	if (m_PxPhysics == nullptr)
	{
		CleanUp();

		AETODO("Log, Better return code");
		return AEResult::Fail;
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

		AETODO("Log, Better return code");
		return AEResult::Fail;
	}

	////////////////////////////////////
	//Finish
	m_IsReady = true;

	return AEResult::Ok;
}

AEResult PhysicsManager::ConnectToPhysXDebugger(const std::wstring& ip, uint32_t port, uint32_t timeout)
{
	//////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if (mIsPVDConnected)
	{
		return AEResult::Ok;
	}

	//////////////////////////////
	//Get Connection Manager
	std::string ipStr = AE_Base::WideStr2String(ip);
	m_PxPvdTransport = physx::PxDefaultPvdSocketTransportCreate(ipStr.c_str(), port, timeout);
	if (m_PxPvdTransport == nullptr)
	{
		AETODO("better ret code");
		return AEResult::Fail;
	}

	//////////////////////////////
	//Connect to PVD
	bool ret = m_PxPvd->connect(*m_PxPvdTransport, physx::PxPvdInstrumentationFlag::eALL);
	if (!ret)
	{
		ReleasePhysX(m_PxPvdTransport);

		AETODO("better ret code");
		return AEResult::Fail;
	}

	mIsPVDConnected = true;

	//////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult PhysicsManager::DisconnectToPhysXDebugger()
{
	//////////////////////////////
	//Pre-checks
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if (!mIsPVDConnected)
	{
		return AEResult::Ok;
	}

	//////////////////////////////
	//Release Debugger and it disconnects
	m_PxPvd->disconnect();
	ReleasePhysX(m_PxPvdTransport);

	mIsPVDConnected = false;

	//////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult PhysicsManager::Update(const TimerParams& timerParams)
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	float elapsedTime = (float)timerParams.m_ElapsedTime;

	m_PxScene->simulate(elapsedTime);

	m_PxScene->fetchResults(true);

	UpdatePhysicsActorObject3D();

	return AEResult::Ok;
}

AEResult PhysicsManager::UpdatePhysicsActorObject3D()
{
	if (!m_IsReady)
	{
		return AEResult::NotReady;
	}

	for each (auto actor in m_PhysicsActorMap)
	{
		PhysicsActor* physicsActor = actor.second;

		physicsActor->UpdateObject3D();
	}

	return AEResult::Ok;
}

bool PhysicsManager::ExistsPhysicsActor(uint64_t id)
{
	return (m_PhysicsActorMap.find(id) != m_PhysicsActorMap.end());
}

AEResult PhysicsManager::AddPhysicsActor(PhysicsActor* physicsActor)
{
	AEAssert(physicsActor != nullptr);
	if (physicsActor == nullptr)
	{
		return AEResult::NullParameter;
	}

	if (!physicsActor->IsReady())
	{
		AETODO("Add better return code");
		return AEResult::Fail;
	}

	if (ExistsPhysicsActor(physicsActor->GetUniqueID()))
	{
		return AEResult::ObjExists;
	}

	m_PxScene->addActor(*physicsActor->GetPxRigidActor());

	m_PhysicsActorMap[physicsActor->GetUniqueID()] = physicsActor;

	return AEResult::Ok;
}

AEResult PhysicsManager::RemovePhysicsActor(uint64_t id, bool deleteObj)
{
	if (!ExistsPhysicsActor(id))
	{
		return AEResult::NotFound;
	}

	PhysicsActor* physicsActor = m_PhysicsActorMap[id];

	m_PxScene->removeActor(*physicsActor->GetPxRigidActor());

	m_PhysicsActorMap.erase(id);

	if (deleteObj)
	{
		DeleteMem(physicsActor);
	}

	return AEResult::Ok;
}

PhysicsActorMapIt PhysicsManager::begin()
{
	return m_PhysicsActorMap.begin();
}

PhysicsActorMapIt PhysicsManager::end()
{
	return m_PhysicsActorMap.end();
}

PhysicsActorMapItConst PhysicsManager::begin() const
{
	return m_PhysicsActorMap.begin();
}

PhysicsActorMapItConst PhysicsManager::end() const
{
	return m_PhysicsActorMap.end();
}

PhysicsActorMapItConst PhysicsManager::cbegin() const
{
	return m_PhysicsActorMap.cbegin();
}

PhysicsActorMapItConst PhysicsManager::cend() const
{
	return m_PhysicsActorMap.cend();
}
