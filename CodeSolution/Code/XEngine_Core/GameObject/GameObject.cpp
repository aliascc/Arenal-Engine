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
#include "Time\XETimeDefs.h"
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
XETODO("Check for Mutex");
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

XEResult GameObject::AddChild(GameObject* child)
{
	XEAssert(child != nullptr);
	if(child == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(this->GetManagerID() == 0)
	{
		XETODO("Add better return message");
		return XEResult::Fail;
	}

	if(child->GetManagerID() != this->GetManagerID())
	{
		XETODO("Add better return message");
		return XEResult::Fail;
	}

	if(HasChild(child->GetUniqueID()))
	{
		return XEResult::ObjExists;
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

	return XEResult::Ok;
}

XEResult GameObject::RemoveChild(uint64_t childID)
{
	if(!HasChild(childID))
	{
		return XEResult::NotFound;
	}

	////////////////////////////////////
	// Sets the new Transforms
	m_GameObjectChilds[childID]->UpdateTransformToNewParent(XEMathHelpers::Mat4Identity, XEMathHelpers::QuaternionIdentity, XEMathHelpers::Mat4Identity);

	////////////////////////////////////
	// Sets parent to null
	m_GameObjectChilds[childID]->m_ParentGameObject = nullptr;
	m_GameObjectChilds[childID]->SetParentObject3D(nullptr);

	////////////////////////////////////
	// Removes from Child Map
	m_GameObjectChilds.erase(childID);

	return XEResult::Ok;
}

bool GameObject::HasChild(GameObject* child) const
{
	XEAssert(child != nullptr);
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

XEResult GameObject::ValidateGOC(GameObjectComponent* goc)
{
	XEAssert(goc != nullptr);
	if(goc == nullptr)
	{
		return XEResult::NullParameter;
	}

	return XEResult::Ok;
}

XEResult GameObject::SetMeshGOC(MeshGOC* meshGOC)
{
	if(m_MeshGOC != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEResult ret = ValidateGOC(meshGOC);

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_MeshGOC = meshGOC;

	return XEResult::Ok;
}
		
XEResult GameObject::RemoveMeshGOC()
{
	DeleteMem(m_MeshGOC);

	return XEResult::Ok;
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

XEResult GameObject::AddMeshMaterialGOC(MeshMaterialGOC* meshMaterialGOC)
{
	XEAssert(meshMaterialGOC != nullptr);
	if(meshMaterialGOC == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = ValidateGOC(meshMaterialGOC);

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_MeshMaterialsGOCList.push_back(meshMaterialGOC);

	return XEResult::Ok;
}

XEResult GameObject::RemoveMeshMaterialGOC(uint64_t id)
{
	////////////////////////////////
	//Find Mesh Material GOC
	MeshMaterialGOC* meshMatGOC = GetMeshMaterialGOC(id);

	if(meshMatGOC == nullptr)
	{
		return XEResult::NotFound;
	}

	////////////////////////////////
	//Remove from List
	m_MeshMaterialsGOCList.remove(meshMatGOC);

	////////////////////////////////
	//Delete Memory
	DeleteMem(meshMatGOC);

	return XEResult::Ok;
}

XEResult GameObject::SetLightGOC(LightGOC* lightGOC)
{
	if(m_LightGOC != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEResult ret = ValidateGOC(lightGOC);

	if(ret != XEResult::Ok)
	{
		return ret;
	}

	m_LightGOC = lightGOC;

	return XEResult::Ok;
}

XEResult GameObject::RemoveLightGOC()
{
	DeleteMem(m_LightGOC);

	return XEResult::Ok;
}

XEResult GameObject::SetMeshAnimationGOC(MeshAnimationGOC* meshAnimationGOC)
{
	if (m_MeshAnimationGOC != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEResult ret = ValidateGOC(meshAnimationGOC);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	m_MeshAnimationGOC = meshAnimationGOC;

	return XEResult::Ok;
}

XEResult GameObject::RemoveMeshAnimationGOC()
{
	DeleteMem(m_MeshAnimationGOC);

	return XEResult::Ok;
}

XEResult GameObject::SetCameraGOC(CameraGOC* cameraGOC)
{
	if (m_CameraGOC != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEResult ret = ValidateGOC(cameraGOC);

	if (ret != XEResult::Ok)
	{
		return ret;
	}

	m_CameraGOC = cameraGOC;

	return XEResult::Ok;
}

XEResult GameObject::RemoveCameraGOC()
{
	DeleteMem(m_CameraGOC);

	return XEResult::Ok;
}

XEResult GameObject::SetAudioListenerGOC(AudioListenerGOC* audioListenerGOC)
{
	if (m_AudioListenerGOC != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEResult ret = ValidateGOC(audioListenerGOC);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	m_AudioListenerGOC = audioListenerGOC;

	return XEResult::Ok;
}

XEResult GameObject::RemoveAudioListenerGOC()
{
	DeleteMem(m_AudioListenerGOC);

	return XEResult::Ok;
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

XEResult GameObject::AddGameObjectScriptGOC(GameObjectScriptGOC* gameObjectScriptGOC)
{
	XEAssert(gameObjectScriptGOC != nullptr);
	if (gameObjectScriptGOC == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = ValidateGOC(gameObjectScriptGOC);

	if (ret != XEResult::Ok)
	{
		return ret;
	}

	m_GameObjectScriptGOCList.push_back(gameObjectScriptGOC);

	return XEResult::Ok;
}

XEResult GameObject::RemoveGameObjectScriptGOC(uint64_t id)
{
	////////////////////////////////
	//Find Game Object Script GOC
	GameObjectScriptGOC* gameObjectScriptGOC = GetGameObjectScriptGOC(id);

	if (gameObjectScriptGOC == nullptr)
	{
		return XEResult::NotFound;
	}

	////////////////////////////////
	//Remove from List
	m_GameObjectScriptGOCList.remove(gameObjectScriptGOC);

	////////////////////////////////
	//Delete Memory
	DeleteMem(gameObjectScriptGOC);

	return XEResult::Ok;
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

XEResult GameObject::AddAudioSourceGOC(AudioSourceGOC* audioSourceGOC)
{
	XEAssert(audioSourceGOC != nullptr);
	if (audioSourceGOC == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = ValidateGOC(audioSourceGOC);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	m_AudioSourceGOCList.push_back(audioSourceGOC);

	return XEResult::Ok;
}

XEResult GameObject::RemoveAudioSourceGOC(uint64_t id)
{
	////////////////////////////////
	//Find Audio SOurce GOC
	AudioSourceGOC* audioSourceGOC = GetAudioSourceGOC(id);

	if (audioSourceGOC == nullptr)
	{
		return XEResult::NotFound;
	}

	////////////////////////////////
	//Remove from List
	m_AudioSourceGOCList.remove(audioSourceGOC);

	////////////////////////////////
	//Delete Memory
	DeleteMem(audioSourceGOC);

	return XEResult::Ok;
}

XEResult GameObject::SetPhysicsGOC(PhysicsGOC* physicsGOC)
{
	if (m_PhysicsGOC != nullptr)
	{
		return XEResult::ObjExists;
	}

	XEResult ret = ValidateGOC(physicsGOC);
	if (ret != XEResult::Ok)
	{
		return ret;
	}

	m_PhysicsGOC = physicsGOC;

	return XEResult::Ok;
}

XEResult GameObject::RemovePhysicsGOC()
{
	DeleteMem(m_PhysicsGOC);

	return XEResult::Ok;
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

