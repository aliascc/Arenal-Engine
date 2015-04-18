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

#pragma once

#ifndef _GAME_OBJECT_H
#define _GAME_OBJECT_H

/**********************
*   System Includes   *
***********************/
#include <map>
#include <list>
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Utils\Object3D.h"
#include "GameObjectDefs.h"
#include "Math\XEMathDefs.h"

/********************
*   Forward Decls   *
*********************/
class MeshGOC;
class LightGOC;
class CameraGOC;
class GameObject;
class PhysicsGOC;
struct TimerParams;
class AudioSourceGOC;
class MeshMaterialGOC;
class AudioListenerGOC;
class MeshAnimationGOC;
class GameObjectScriptGOC;
class GameObjectComponent;

/**************
*   Typedef   *
***************/

/// <summary>
/// Typedef for Map of GameObject Childs with uint64_t as index
/// <summary>
typedef std::map<uint64_t, GameObject*> GameObjectChildMap;

typedef GameObjectChildMap::iterator GameObjectChildMapIt;

typedef GameObjectChildMap::const_iterator GameObjectChildMapItConst;

/// <summary>
/// Typedef for List of Mesh GOCs
/// </summary>
typedef std::list<MeshMaterialGOC*> MeshMaterialsGOCList;

typedef MeshMaterialsGOCList::iterator MeshMaterialsGOCListIt;

typedef MeshMaterialsGOCList::const_iterator MeshMaterialsGOCListItConst;

/// <summary>
/// Typedef for List of Game Object Scripts GOCs
/// </summary>
typedef std::list<GameObjectScriptGOC*> GameObjectScriptGOCList;

typedef GameObjectScriptGOCList::iterator GameObjectScriptGOCtListIt;

typedef GameObjectScriptGOCList::const_iterator GameObjectScriptGOCListItConst;

/// <summary>
/// Typedef for List of Game Object Scripts GOCs
/// </summary>
typedef std::list<AudioSourceGOC*> AudioSourceGOCList;

typedef AudioSourceGOCList::iterator AudioSourceGOCListIt;

typedef AudioSourceGOCList::const_iterator AudioSourceGOCListItConst;

/*****************
*   Class Decl   *
******************/
class GameObject sealed : public Object3D
{
	friend class GameObjectManager;

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// ID of Game Object Manager that manages this Game Object
		/// </summary>
		uint64_t m_ManagerID = 0;

		/// <summary>
		/// Group that the Game Object belongs to
		/// </summary>
		std::wstring m_GroupName = L"";

		/// <summary>
		/// Parent Game Object for this Game Object
		/// </summary>
		GameObject* m_ParentGameObject = nullptr;

		/// <summary>
		/// Game Objects childs of this Object
		/// </summary>
		GameObjectChildMap m_GameObjectChilds;

		/// <summary>
		/// Mesh Game Objects Component associated with this Game Object
		/// </summary>
		MeshGOC* m_MeshGOC = nullptr;

		/// <summary>
		/// Mesh Materials Game Object Components Associated with this Game Object
		/// </summary>
		MeshMaterialsGOCList m_MeshMaterialsGOCList;

		/// <summary>
		/// Script Game Objects Components List associated with this Game Object
		/// </summary>
		GameObjectScriptGOCList m_GameObjectScriptGOCList;

		/// <summary>
		/// Callback Function to notify Manager that Game Object is becoming a child
		/// </summary>
		GameObjectMoveToChildCallback m_GameObjectMoveToChildCallback = nullptr;

		/// <summary>
		/// Light Game Objects Component associated with this Game Object
		/// </summary>
		LightGOC* m_LightGOC = nullptr;

		/// <summary>
		/// Mesh Animation Game Object Component associated with this Game Object
		/// </summary>
		MeshAnimationGOC* m_MeshAnimationGOC = nullptr;

		/// <summary>
		/// Camera Game Objects Component associated with this Game Object
		/// </summary>
		CameraGOC* m_CameraGOC = nullptr;

		/// <summary>
		/// Audio Listener Game Objects Component associated with this Game Object
		/// </summary>
		AudioListenerGOC* m_AudioListenerGOC = nullptr;

		AudioSourceGOCList m_AudioSourceGOCList;

		PhysicsGOC* m_PhysicsGOC = nullptr;

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods
		
		/// <summary>
		/// Generic validation the GameObjectComponent
		/// </summary>
		/// <param name="goc">Game Object Component to validate</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult ValidateGOC(GameObjectComponent* goc);

		/// <summary>
		/// Removes a Child to this Game Object
		/// </summary>
		/// <param name="childID">Game Object ID to be removed as a child of this Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult RemoveChild(uint64_t childID);

		/// <summary>
		/// Cleans up all the memory that the Game Object uses
		/// </summary>
		void CleanUp();

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// GameObject Constructor
		/// </summary>
		/// <param name="name">Name of Game Object</param>
		/// <param name="position">Position of Game Object</param>
		/// <param name="rotation">Rotation of Game Object in Degrees</param>
		/// <param name="scale">Scale of Game Object</param>
		GameObject(const std::wstring& name = L"", const glm::vec3& position = XEMathHelpers::Vec3fZero, const glm::vec3& rotation = XEMathHelpers::Vec3fZero, const glm::vec3& scale = XEMathHelpers::Vec3fOne);

		/// <summary>
		/// Default GameObject Destructor
		/// </summary>
		virtual ~GameObject();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Gets the Game Object Group Name that it belongs to
		/// </summary>
		/// <returns>Game Object Group Name that it belongs to</returns>
		inline const std::wstring& GetGroupName() const
		{ 
			return m_GroupName; 
		}

		/// <summary>
		/// Gets the number of Children this Game Object has
		/// </summary>
		/// <returns>Number of children this game object has</returns>
		inline uint32_t GetNumChildren() const 
		{
			return static_cast<uint32_t>(m_GameObjectChilds.size());
		}

		/// <summary>
		/// Gets the Mesh GOC associated with this Game Object has
		/// </summary>
		/// <returns>Mesh Game Object Component</returns>
		inline MeshGOC* GetMeshGOC() const
		{
			return m_MeshGOC;
		}

		/// <summary>
		/// Gets the Camera GOC associated with this Game Object has
		/// </summary>
		/// <returns>Camera Game Object Component</returns>
		inline CameraGOC* GetCameraGOC() const
		{
			return m_CameraGOC;
		}

		const GameObjectScriptGOCList& GetGameObjectScriptGOCList() const
		{
			return m_GameObjectScriptGOCList;
		}

		GameObjectScriptGOC* GetGameObjectScriptGOC(uint64_t id);

		const MeshMaterialsGOCList& GetMeshMaterialsGOCList() const
		{
			return m_MeshMaterialsGOCList;
		}

		inline uint64_t GetManagerID() const
		{
			return m_ManagerID;
		}

		inline GameObject* GetParentGameObject() const
		{
			return m_ParentGameObject;
		}

		/// <summary>
		/// Gets the Light GOC associated with this Game Object has
		/// </summary>
		/// <returns>Light Game Object Component</returns>
		inline LightGOC* GetLightGOC() const
		{
			return m_LightGOC;
		}

		/// <summary>
		/// Gets the Mesh Animation GOC associated with this Game Object has
		/// </summary>
		/// <returns>Mesh Animation Game Object Component</returns>
		inline MeshAnimationGOC* GetMeshAnimationGOC() const
		{
			return m_MeshAnimationGOC;
		}

		/// <summary>
		/// Returns if this Game Object currently has any Game Object Scripts Components
		/// </summary>
		/// <returns>true if it has Game Object Scripts Component, false otherwise</returns>
		inline bool HasGameObjectScriptGOCs() const
		{
			return (m_GameObjectScriptGOCList.size() != 0);
		}

		/// <summary>
		/// Returns if this Game Object currently has any Material Game Object Components assign to it
		/// </summary>
		/// <returns>true if it has Materials Game Object Component, false otherwise</returns>
		inline bool HasMaterialGOCs() const
		{
			return (m_MeshMaterialsGOCList.size() != 0);
		}

		/// <summary>
		/// Gets the Audio Listener GOC associated with this Game Object has
		/// </summary>
		/// <returns>Audio Listener Game Object Component</returns>
		inline AudioListenerGOC* GetAudioListenerGOC() const
		{
			return m_AudioListenerGOC;
		}

		const AudioSourceGOCList& GetAudioSourceGOCList() const
		{
			return m_AudioSourceGOCList;
		}

		AudioSourceGOC* GetAudioSourceGOC(const std::wstring& name);

		AudioSourceGOC* GetAudioSourceGOC(uint64_t id);

		inline PhysicsGOC* GetPhysicsGOC() const
		{
			return m_PhysicsGOC;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the Game Object Group Name that it belongs to
		/// </summary>
		/// <param name="groupName">New Game Object Group Name that it belongs to</param>
		inline void SetGroupName(const std::wstring& groupName) 
		{ 
			m_GroupName = groupName; 
		}

		/// <summary>
		/// Sets Mesh Game Object Component to the Game Object
		/// </summary>
		/// <param name="meshGOC">Mesh Game Object Component to add to Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult SetMeshGOC(MeshGOC* meshGOC);

		/// <summary>
		/// Sets Light Game Object Component to the Game Object
		/// </summary>
		/// <param name="lightGOC">Light Game Object Component to add to Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult SetLightGOC(LightGOC* lightGOC);

		/// <summary>
		/// Sets Mesh Animation Game Object Component to the Game Object
		/// </summary>
		/// <param name="meshAnimationGOC">Mesh Animation Game Object Component to add to Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult SetMeshAnimationGOC(MeshAnimationGOC* meshAnimationGOC);

		/// <summary>
		/// Sets Camera Game Object Component to the Game Object
		/// </summary>
		/// <param name="cameraGOC">Camera Game Object Component to add to Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult SetCameraGOC(CameraGOC* meshGOC);

		/// <summary>
		/// Sets Audio Listener Game Object Component to the Game Object
		/// </summary>
		/// <param name="audioListenerGOC">Audio Listener Game Object Component to add to Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult SetAudioListenerGOC(AudioListenerGOC* audioListenerGOC);

		/// <summary>
		/// Sets Physics Game Object Component to the Game Object
		/// </summary>
		/// <param name="physicsGOC">Physics Game Object Component to add to Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult SetPhysicsGOC(PhysicsGOC* physicsGOC);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Returns if this Game Object is a child of another Object
		/// </summary>
		/// <returns>true if this Game Object is a child, false otherwise</returns>
		inline bool IsChild() const
		{
			return (m_ParentGameObject != nullptr);
		}

		/// <summary>
		/// Returns if this Game Object currently has a Mesh Game Object Component assign to it
		/// </summary>
		/// <returns>true if a Mesh Game Object Component exists, false otherwise</returns>
		inline bool HasMeshGOC() const
		{
			return (m_MeshGOC != nullptr);
		}

		/// <summary>
		/// Returns if this Game Object currently has any Light Game Object Components assign to it
		/// </summary>
		/// <returns>true if it has Light Game Object Component, false otherwise</returns>
		inline bool HasLightGOC() const
		{
			return (m_LightGOC != nullptr);
		}

		/// <summary>
		/// Returns if this Game Object currently has any Mesh Animation Game Object Components assign to it
		/// </summary>
		/// <returns>true if it has Mesh Animation Game Object Component, false otherwise</returns>
		inline bool HasMeshAnimationGOC() const
		{
			return (m_MeshAnimationGOC != nullptr);
		}

		/// <summary>
		/// Returns if this Game Object currently has any Camera Game Object Components assign to it
		/// </summary>
		/// <returns>true if it has Camera Game Object Component, false otherwise</returns>
		inline bool HasCameraGOC() const
		{
			return (m_CameraGOC != nullptr);
		}

		/// <summary>
		/// Returns if this Game Object currently has any Audio Listener Game Object Components assign to it
		/// </summary>
		/// <returns>true if it has Audio Listener Game Object Component, false otherwise</returns>
		inline bool HasAudioListenerGOC() const
		{
			return (m_AudioListenerGOC != nullptr);
		}

		/// <summary>
		/// Returns if this Game Object currently has any Audio Source Components
		/// </summary>
		/// <returns>true if it has Audio Source Component, false otherwise</returns>
		inline bool HasAudioSourceGOCs() const
		{
			return (m_AudioSourceGOCList.size() != 0);
		}

		inline bool HasPhysicsGOC() const
		{
			return (m_PhysicsGOC != nullptr);
		}

		/// <summary>
		/// Initialize method for the Game Object called a constant interval
		/// Interval is Constant defined in Time of GameApp
		/// </summary>
		void Initialize();

		/// <summary>
		/// Constant Update method for the Game Object called a constant interval
		/// Interval is Constant defined in Time of GameApp
		/// </summary>
		/// <param name="timerParams">Game Timer Parameters</param>
		void ConstantUpdate(const TimerParams& timerParams);

		/// <summary>
		/// Update method for the Game Object called each frame
		/// </summary>
		/// <param name="timerParams">Game Timer Parameters</param>
		void Update(const TimerParams& timerParams);

		/// <summary>
		/// Post Update method for the Game Object called each frame after the Update Method
		/// </summary>
		/// <param name="timerParams">Game Timer Parameters</param>
		void PostUpdate(const TimerParams& timerParams);

		/// <summary>
		/// Add a Child to this Game Object
		/// </summary>
		/// <param name="child">Game Object to be added as a child of this Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult AddChild(GameObject* child);

		/// <summary>
		/// Returns if this Game Object is a child of this Object
		/// </summary>
		/// <param name="child">Game Object to check</param>
		/// <returns>true if this Game Object, has the object 'child' as one of its children</returns>
		bool HasChild(GameObject* child) const;

		/// <summary>
		/// Returns if this Game Object is a child of this Object
		/// </summary>
		/// <param name="childID">ID of Game Object to check</param>
		/// <returns>true if this Game Object, has the object with 'childID' as one of its children</returns>
		bool HasChild(uint64_t childID) const;

		/// <summary>
		/// Removes Mesh Game Object Component to the Game Object
		/// </summary>
		/// <param name="meshGOC">Mesh Game Object Component to remove from Game Object</param>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult RemoveMeshGOC();

		MeshMaterialGOC* GetMeshMaterialGOC(uint64_t id);

		XEResult AddMeshMaterialGOC(MeshMaterialGOC* meshMaterialGOC);

		XEResult RemoveMeshMaterialGOC(uint64_t id);

		/// <summary>
		/// Removes Light Game Object Component to the Game Object
		/// </summary>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult RemoveLightGOC();

		/// <summary>
		/// Removes Mesh Animation Game Object Component to the Game Object
		/// </summary>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult RemoveMeshAnimationGOC();

		/// <summary>
		/// Removes Camera Game Object Component to the Game Object
		/// </summary>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult RemoveCameraGOC();

		/// <summary>
		/// Removes Audio Listener Game Object Component to the Game Object
		/// </summary>
		/// <returns>XEResult::OK if it successful, otherwise an error code</returns>
		XEResult RemoveAudioListenerGOC();

		XEResult AddGameObjectScriptGOC(GameObjectScriptGOC* gameObjectScriptGOC);

		XEResult RemoveGameObjectScriptGOC(uint64_t id);

		XEResult AddAudioSourceGOC(AudioSourceGOC* audioSourceGOC);

		XEResult RemoveAudioSourceGOC(uint64_t id);

		XEResult RemovePhysicsGOC();

		GameObjectChildMapIt begin();

		GameObjectChildMapIt end();

		GameObjectChildMapItConst begin() const;

		GameObjectChildMapItConst end() const;

		GameObjectChildMapItConst cbegin() const;

		GameObjectChildMapItConst cend() const;

#pragma endregion

};

#endif
