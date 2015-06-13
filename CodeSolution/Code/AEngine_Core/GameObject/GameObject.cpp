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
#include "GameObject.h"
#include "Time\AETimeDefs.h"
#include "Base\BaseFunctions.h"
#include "Components\MeshGOC.h"
#include "Components\LightGOC.h"
#include "Components\CameraGOC.h"
#include "Components\PhysicsGOC.h"
#include "Components\AudioSourceGOC.h"
#include "Components\MeshMaterialGOC.h"
#include "Components\MeshAnimationGOC.h"
#include "Components\AudioListenerGOC.h"
#include "Components\GameObjectScriptGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check for Mutex");
GameObject::GameObject(const std::wstring& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	: Object3D(name, position, rotation, scale)
{
}

GameObject::~GameObject()
{
	CleanUp();
}

void GameObject::CleanUp()
{
	////////////////////////////////
	//Clear Game Object Children
	for (auto goIt : m_GameObjectChilds)
	{
		DeleteMem(goIt.second);
	}
	m_GameObjectChilds.clear();

	////////////////////////////////
	//Delete Physics GOC
	DeleteMem(m_PhysicsGOC);

	////////////////////////////////
	//Delete Light GOC
	DeleteMem(m_LightGOC);

	////////////////////////////////
	//Delete Mesh GOC
	DeleteMem(m_MeshGOC);

	////////////////////////////////
	//Delete Camera GOC
	DeleteMem(m_CameraGOC);

	////////////////////////////////
	//Delete Mesh Animations GOC
	DeleteMem(m_MeshAnimationGOC);

	////////////////////////////////
	//Delete Materials GOC
	for(auto meshMaterialGOC : m_MeshMaterialsGOCList)
	{
		DeleteMem(meshMaterialGOC);
	}
	m_MeshMaterialsGOCList.clear();

	////////////////////////////////
	//Delete Game Object Scripts GOC
	for (auto gosGOC : m_GameObjectScriptGOCList)
	{
		DeleteMem(gosGOC);
	}
	m_GameObjectScriptGOCList.clear();

	////////////////////////////////
	//Delete Audio Listener GOC
	DeleteMem(m_AudioListenerGOC);

	////////////////////////////////
	//Delete Game Object Audio Source GOC
	for (auto audioSourceGOC : m_AudioSourceGOCList)
	{
		DeleteMem(audioSourceGOC);
	}
	m_GameObjectScriptGOCList.clear();
}

void GameObject::Initialize()
{
	////////////////////////////////
	//Execute Functions
	for (auto scriptGOC : m_GameObjectScriptGOCList)
	{
		if (scriptGOC->HasScriptInstance())
		{
			scriptGOC->ExecuteScriptInitialize();
		}
	}
}

void GameObject::ConstantUpdate(const TimerParams& timerParams)
{
	////////////////////////////////
	//Execute Functions
	for (auto scriptGOC : m_GameObjectScriptGOCList)
	{
		if (scriptGOC->HasScriptInstance())
		{
			scriptGOC->ExecuteScriptConstantUpdate(timerParams);
		}
	}
}

void GameObject::Update(const TimerParams& timerParams)
{
	////////////////////////////////
	//Execute Functions
	for (auto scriptGOC : m_GameObjectScriptGOCList)
	{
		if (scriptGOC->HasScriptInstance())
		{
			scriptGOC->ExecuteScriptUpdate(timerParams);
		}
	}
}

void GameObject::PostUpdate(const TimerParams& timerParams)
{
	////////////////////////////////
	//Execute Functions
	for (auto scriptGOC : m_GameObjectScriptGOCList)
	{
		if (scriptGOC->HasScriptInstance())
		{
			scriptGOC->ExecuteScriptPostUpdate(timerParams);
		}
	}
}

AEResult GameObject::AddChild(GameObject* child)
{
	AEAssert(child != nullptr);
	if(child == nullptr)
	{
		return AEResult::NullParameter;
	}

	if(this->GetManagerID() == 0)
	{
		AETODO("Add better return message");
		return AEResult::Fail;
	}

	if(child->GetManagerID() != this->GetManagerID())
	{
		AETODO("Add better return message");
		return AEResult::Fail;
	}

	if(HasChild(child->GetUniqueID()))
	{
		return AEResult::ObjExists;
	}

	if(child->IsChild())
	{
		child->m_ParentGameObject->RemoveChild(child->GetUniqueID());
	}
	else
	{
		child->m_GameObjectMoveToChildCallback(child->GetUniqueID());
	}

	child->UpdateTransformToNewParent(this->GetWorldTransform(), this->GetWorldRotationQuat(), this->GetWorldScaleTransform());

	child->m_ParentGameObject = this;
	child->SetParentObject3D(this);

	m_GameObjectChilds[child->GetUniqueID()] = child;

	return AEResult::Ok;
}

AEResult GameObject::RemoveChild(uint64_t childID)
{
	if(!HasChild(childID))
	{
		return AEResult::NotFound;
	}

	////////////////////////////////////
	// Sets the new Transforms
	m_GameObjectChilds[childID]->UpdateTransformToNewParent(AEMathHelpers::Mat4Identity, AEMathHelpers::QuaternionIdentity, AEMathHelpers::Mat4Identity);

	////////////////////////////////////
	// Sets parent to null
	m_GameObjectChilds[childID]->m_ParentGameObject = nullptr;
	m_GameObjectChilds[childID]->SetParentObject3D(nullptr);

	////////////////////////////////////
	// Removes from Child Map
	m_GameObjectChilds.erase(childID);

	return AEResult::Ok;
}

bool GameObject::HasChild(GameObject* child) const
{
	AEAssert(child != nullptr);
	if(child == nullptr)
	{
		return false;
	}

	return HasChild(child->GetUniqueID());
}

bool GameObject::HasChild(uint64_t childID) const
{
	return ( !(m_GameObjectChilds.find(childID) == m_GameObjectChilds.end()) );
}

AEResult GameObject::ValidateGOC(GameObjectComponent* goc)
{
	AEAssert(goc != nullptr);
	if(goc == nullptr)
	{
		return AEResult::NullParameter;
	}

	return AEResult::Ok;
}

AEResult GameObject::SetMeshGOC(MeshGOC* meshGOC)
{
	if(m_MeshGOC != nullptr)
	{
		return AEResult::ObjExists;
	}

	AEResult ret = ValidateGOC(meshGOC);

	if(ret != AEResult::Ok)
	{
		return ret;
	}

	m_MeshGOC = meshGOC;

	return AEResult::Ok;
}
		
AEResult GameObject::RemoveMeshGOC()
{
	DeleteMem(m_MeshGOC);

	return AEResult::Ok;
}

MeshMaterialGOC* GameObject::GetMeshMaterialGOC(uint64_t id)
{
	for(auto meshMaterialGOC : m_MeshMaterialsGOCList)
	{
		if(meshMaterialGOC->GetUniqueID() == id)
		{
			return meshMaterialGOC;
		}
	}

	return nullptr;
}

AEResult GameObject::AddMeshMaterialGOC(MeshMaterialGOC* meshMaterialGOC)
{
	AEAssert(meshMaterialGOC != nullptr);
	if(meshMaterialGOC == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEResult ret = ValidateGOC(meshMaterialGOC);

	if(ret != AEResult::Ok)
	{
		return ret;
	}

	m_MeshMaterialsGOCList.push_back(meshMaterialGOC);

	return AEResult::Ok;
}

AEResult GameObject::RemoveMeshMaterialGOC(uint64_t id)
{
	////////////////////////////////
	//Find Mesh Material GOC
	MeshMaterialGOC* meshMatGOC = GetMeshMaterialGOC(id);

	if(meshMatGOC == nullptr)
	{
		return AEResult::NotFound;
	}

	////////////////////////////////
	//Remove from List
	m_MeshMaterialsGOCList.remove(meshMatGOC);

	////////////////////////////////
	//Delete Memory
	DeleteMem(meshMatGOC);

	return AEResult::Ok;
}

AEResult GameObject::SetLightGOC(LightGOC* lightGOC)
{
	if(m_LightGOC != nullptr)
	{
		return AEResult::ObjExists;
	}

	AEResult ret = ValidateGOC(lightGOC);

	if(ret != AEResult::Ok)
	{
		return ret;
	}

	m_LightGOC = lightGOC;

	return AEResult::Ok;
}

AEResult GameObject::RemoveLightGOC()
{
	DeleteMem(m_LightGOC);

	return AEResult::Ok;
}

AEResult GameObject::SetMeshAnimationGOC(MeshAnimationGOC* meshAnimationGOC)
{
	if (m_MeshAnimationGOC != nullptr)
	{
		return AEResult::ObjExists;
	}

	AEResult ret = ValidateGOC(meshAnimationGOC);
	if (ret != AEResult::Ok)
	{
		return ret;
	}

	m_MeshAnimationGOC = meshAnimationGOC;

	return AEResult::Ok;
}

AEResult GameObject::RemoveMeshAnimationGOC()
{
	DeleteMem(m_MeshAnimationGOC);

	return AEResult::Ok;
}

AEResult GameObject::SetCameraGOC(CameraGOC* cameraGOC)
{
	if (m_CameraGOC != nullptr)
	{
		return AEResult::ObjExists;
	}

	AEResult ret = ValidateGOC(cameraGOC);

	if (ret != AEResult::Ok)
	{
		return ret;
	}

	m_CameraGOC = cameraGOC;

	return AEResult::Ok;
}

AEResult GameObject::RemoveCameraGOC()
{
	DeleteMem(m_CameraGOC);

	return AEResult::Ok;
}

AEResult GameObject::SetAudioListenerGOC(AudioListenerGOC* audioListenerGOC)
{
	if (m_AudioListenerGOC != nullptr)
	{
		return AEResult::ObjExists;
	}

	AEResult ret = ValidateGOC(audioListenerGOC);
	if (ret != AEResult::Ok)
	{
		return ret;
	}

	m_AudioListenerGOC = audioListenerGOC;

	return AEResult::Ok;
}

AEResult GameObject::RemoveAudioListenerGOC()
{
	DeleteMem(m_AudioListenerGOC);

	return AEResult::Ok;
}

GameObjectScriptGOC* GameObject::GetGameObjectScriptGOC(uint64_t id)
{
	for (auto gameObjectScriptGOC : m_GameObjectScriptGOCList)
	{
		if (gameObjectScriptGOC->GetUniqueID() == id)
		{
			return gameObjectScriptGOC;
		}
	}

	return nullptr;
}

AEResult GameObject::AddGameObjectScriptGOC(GameObjectScriptGOC* gameObjectScriptGOC)
{
	AEAssert(gameObjectScriptGOC != nullptr);
	if (gameObjectScriptGOC == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEResult ret = ValidateGOC(gameObjectScriptGOC);

	if (ret != AEResult::Ok)
	{
		return ret;
	}

	m_GameObjectScriptGOCList.push_back(gameObjectScriptGOC);

	return AEResult::Ok;
}

AEResult GameObject::RemoveGameObjectScriptGOC(uint64_t id)
{
	////////////////////////////////
	//Find Game Object Script GOC
	GameObjectScriptGOC* gameObjectScriptGOC = GetGameObjectScriptGOC(id);

	if (gameObjectScriptGOC == nullptr)
	{
		return AEResult::NotFound;
	}

	////////////////////////////////
	//Remove from List
	m_GameObjectScriptGOCList.remove(gameObjectScriptGOC);

	////////////////////////////////
	//Delete Memory
	DeleteMem(gameObjectScriptGOC);

	return AEResult::Ok;
}

AudioSourceGOC* GameObject::GetAudioSourceGOC(const std::wstring& name)
{
	for (auto audioSourceGOC : m_AudioSourceGOCList)
	{
		if (audioSourceGOC->GetName().compare(name) == 0)
		{
			return audioSourceGOC;
		}
	}

	return nullptr;
}

AudioSourceGOC* GameObject::GetAudioSourceGOC(uint64_t id)
{
	for (auto audioSourceGOC : m_AudioSourceGOCList)
	{
		if (audioSourceGOC->GetUniqueID() == id)
		{
			return audioSourceGOC;
		}
	}

	return nullptr;
}

AEResult GameObject::AddAudioSourceGOC(AudioSourceGOC* audioSourceGOC)
{
	AEAssert(audioSourceGOC != nullptr);
	if (audioSourceGOC == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEResult ret = ValidateGOC(audioSourceGOC);
	if (ret != AEResult::Ok)
	{
		return ret;
	}

	m_AudioSourceGOCList.push_back(audioSourceGOC);

	return AEResult::Ok;
}

AEResult GameObject::RemoveAudioSourceGOC(uint64_t id)
{
	////////////////////////////////
	//Find Audio SOurce GOC
	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC(id);

	if (audioSourceGOC == nullptr)
	{
		return AEResult::NotFound;
	}

	////////////////////////////////
	//Remove from List
	m_AudioSourceGOCList.remove(audioSourceGOC);

	////////////////////////////////
	//Delete Memory
	DeleteMem(audioSourceGOC);

	return AEResult::Ok;
}

AEResult GameObject::SetPhysicsGOC(PhysicsGOC* physicsGOC)
{
	if (m_PhysicsGOC != nullptr)
	{
		return AEResult::ObjExists;
	}

	AEResult ret = ValidateGOC(physicsGOC);
	if (ret != AEResult::Ok)
	{
		return ret;
	}

	m_PhysicsGOC = physicsGOC;

	return AEResult::Ok;
}

AEResult GameObject::RemovePhysicsGOC()
{
	DeleteMem(m_PhysicsGOC);

	return AEResult::Ok;
}

GameObjectChildMapIt GameObject::begin()
{
	return m_GameObjectChilds.begin();
}

GameObjectChildMapIt GameObject::end()
{
	return m_GameObjectChilds.end();
}

GameObjectChildMapItConst GameObject::begin() const
{
	return m_GameObjectChilds.begin();
}

GameObjectChildMapItConst GameObject::end() const
{
	return m_GameObjectChilds.end();
}

GameObjectChildMapItConst GameObject::cbegin() const
{
	return m_GameObjectChilds.cbegin();
}

GameObjectChildMapItConst GameObject::cend() const
{
	return m_GameObjectChilds.cend();
}

