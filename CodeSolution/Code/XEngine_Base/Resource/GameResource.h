/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _GAME_RESOURCE_H
#define _GAME_RESOURCE_H

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <stdint.h>
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "GameResourceDefs.h"
#include "Base\UniqueXEObjectNamed.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class GameResource abstract : public UniqueXEObjectNamed
{
	friend class GameResourceManager;

	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		/// <summary>
		/// Reference count to the Game Resource
		/// </summary>
		uint32_t m_RefCount = 0;

		/// <summary>
		/// If resource is to be keep alive
		/// even when ref count reaches 0
		/// </summary>
		bool m_KeepAlive = false;

		/// <summary>
		/// Lets know the rest of the threads
		/// deleting this object
		/// </summary>
		bool m_Deleting = false;

		/// <summary>
		/// If Game Resource is assigned to a 
		/// Resource manager this flags is true
		/// </summary>
		bool m_Managed = false;

		/// <summary>
		/// Game Resource Type
		/// </summary>
		GameResourceType m_ResourceType = GameResourceType::Unknown;

		/// <summary>
		/// Boost Mutex to Lock Thread to
		/// perform atomic operations
		/// </summary>
		std::mutex m_GameResourceMutex;

		/// <summary>
		/// Callback to notify Manager of 
		/// Game Resource Release
		/// </summary>
		GameResourceReleaseCallback m_ReleaseCallback = nullptr;

		/// <summary>
		/// FileName from where the Resource was read
		/// </summary>
		std::wstring m_FileName = L"";

		/// <summary>
		/// String that Identifies the Resource in the Resource Manager
		/// </summary>
		std::wstring m_StringIdentifier = L"";

#pragma endregion

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Protected Methods

		/// <summary>
		/// Sets the name of the File that the
		/// the Resource was read from
		/// (if it was loaded from a file)
		/// </summary>
		/// <remarks>
		/// This function will not be thread safe and it is meant to be use when 
		/// a Thread safe function of a Game Resource needs to change the Filename
		/// </remarks>
		/// <param name="fileName">File from where the resource was read</param>
		void SetFileNameWithoutLock(const std::wstring& fileName);

		/// <summary>
		/// Sets the Name of the Resource
		/// </summary>
		/// <remarks>
		/// This function will not be thread safe and it is meant to be use when 
		/// a Thread safe function of a Game Resource needs to change the Name
		/// </remarks>
		/// <param name="name">Name of the Resource</param>
		void SetNameWithoutLock(const std::wstring name);

#pragma endregion

		/***********************************
		*   Protected Destructor Methods   *
		************************************/
#pragma region Protected Destructor Methods

		/// <summary>
		/// Default GameResource Destructor
		/// Protected so only Childs can access it
		/// </summary>
		virtual ~GameResource();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// GameResource Constructor
		/// </summary>
		/// <param name="resourceName">Resource Name</param>
		/// <param name="resourceType">Game Resource Type</param>
		GameResource(const std::wstring& resourceName, GameResourceType resourceType);

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Returns Reference Count of the 
		/// Game Resource
		/// </summary>
		/// <returns>Reference Count</returns>
		inline uint32_t GetRefCount() const
		{
			return m_RefCount;
		}

		/// <summary>
		/// Returns if Game Resource is to
		/// keep alive when Reference Count
		/// reaches 0
		/// </summary>
		/// <returns>Keep Alive Resource</returns>
		inline bool GetKeepAlive() const
		{
			return m_KeepAlive;
		}

		/// <summary>
		/// Returns what type of Game Resource
		/// the object is
		/// </summary>
		/// <returns>Return Game Resource Type</returns>
		inline GameResourceType GetGameResourceType() const
		{
			return m_ResourceType;
		}

		/// <summary>
		/// Returns if Game Resource is managed
		/// by a Resource Manager
		/// </summary>
		/// <returns>True if resource is managed</returns>
		inline bool IsManaged() const
		{
			return m_Managed;
		}

		/// <summary>
		/// Returns name of the File that the
		/// the Resource was read from
		/// (if it was loaded from a file)
		/// </summary>
		/// <returns>File Name of the Resource</returns>
		inline const std::wstring& GetFileName() const
		{
			return m_FileName;
		}

		/// <summary>
		/// Returns the String Identifier use in the Game Resource Manager
		/// </summary>
		/// <returns>String Identifier use in Game Resource Manager, empty if none is set</returns>
		inline const std::wstring& GetStringIdentifier() const
		{
			return m_StringIdentifier;
		}

#pragma endregion

		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods

		/// <summary>
		/// Sets the name of the File that the
		/// the Resource was read from
		/// (if it was loaded from a file)
		/// </summary>
		/// <param name="fileName">File from where the resource was read</param>
		void SetFileName(const std::wstring& fileName);
		
		/// <summary>
		/// Sets the Name of the Resource
		/// </summary>
		/// <param name="name">Name of the Resource</param>
		void SetName(const std::wstring name) override;

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Increases the reference count and returns a pointer to itself
		/// </summary>
		/// <returns>An exact reference to this Game Resource</returns>
		virtual GameResource* AddRef();

		/// <summary>
		/// Decreases the reference count to this resource, if it reaches 0 and Keep Alive is false, it will delete it self
		/// </summary>
		/// <returns>Number of references remaining</returns>
		virtual uint32_t Release();

		/// <summary>
		/// Load the Game Resource
		/// </summary>
		/// <returns>XEResult::Ok if it was successful</returns>
		virtual XEResult Load() = 0;

#pragma endregion

};

#endif

