/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MESH_H
#define _MESH_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Utils\BoundingBox.h"
#include "Utils\BoundingSphere.h"
#include "Resource\GameResource.h"

/********************
*   Forward Decls   *
*********************/
class MeshPart;
class GraphicDevice;

/***************
*   Typedefs   *
****************/

/// <summary>
/// Typedef for Vector to contain MeshPart
/// </summary>
typedef std::vector<MeshPart*> MeshPartVector;

typedef MeshPartVector::iterator MeshPartVectorIt;

typedef MeshPartVector::const_iterator MeshPartVectorItConst;

/*****************
*   Class Decl   *
******************/
class Mesh : public GameResource
{
	protected:

		/// <summary>
		/// Vector containing the different Mesh Parts that make up the mesh
		/// </summary>
		MeshPartVector m_MeshPartVector;

		GraphicDevice* m_GraphicDevice = nullptr;

		BoundingBox m_BoundingBox;

		BoundingSphere m_BoundingSphere;

		void CleanUp();

		/************************************
		*   Protected Destructor Methods   *
		************************************/
#pragma region Protected Destructor Methods

		/// <summary>
		/// Default Mesh Destructor
		/// Protected so only Childs can access it
		/// </summary>
		virtual ~Mesh();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// Mesh Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device use to create Mesh</param>
		/// <param name="resourceName">Resource Name</param>
		Mesh(GraphicDevice* graphicDevice, const std::wstring& resourceName);

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		uint32_t GetNumberMeshParts() const
		{
			return (uint32_t)m_MeshPartVector.size();
		}

		MeshPart* GetMeshPart(uint32_t index);

		const BoundingBox& GetBoundingBox() const
		{
			return m_BoundingBox;
		}

		const BoundingSphere& GetBoundingSphere() const
		{
			return m_BoundingSphere;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		virtual XEResult Load() override;

		XEResult AddMeshPart(MeshPart* mp);

		MeshPartVectorIt begin();

		MeshPartVectorIt end();

		MeshPartVectorItConst begin() const;

		MeshPartVectorItConst end() const;

		MeshPartVectorItConst cbegin() const;

		MeshPartVectorItConst cend() const;

		MeshPart* operator[](uint32_t index);

#pragma endregion

		// Prevent copying.
		Mesh(Mesh const&) = delete;
		Mesh& operator= (Mesh const&) = delete;
};

#endif

