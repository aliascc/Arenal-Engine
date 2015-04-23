
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

#ifndef _ICO_SPHERE_MESH_H
#define _ICO_SPHERE_MESH_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <stdint.h>
#include <unordered_map>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Models\Mesh.h"
#include "Resource\GameResource.h"
#include "Vertex\Types\VertexPositionNormalTexture.h"

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;

/**************
*   Defines   *
***************/

/// <summary>
/// ICO Sphere Mesh Name
/// </summary>
#define XE_ICO_SPHERE_MESH_NAME				L"IcoSphere Mesh"

/***************
*   Typedefs   *
****************/

/*****************
*   Class Decl   *
******************/
class IcoSphereMesh sealed : public Mesh
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		bool m_IsLoaded = false;

		uint32_t m_RecursionLevels = 6;

		std::vector<VertexPositionNormalTexture> m_VtxBuff;

		std::vector<uint16_t> m_IdxBuff;

		std::unordered_map<std::wstring, uint16_t> m_VtxMap;

		void AddFace(const VertexPositionNormalTexture& vtx1, const VertexPositionNormalTexture& vtx2, const VertexPositionNormalTexture& vtx3);

		uint16_t AddVtx(const VertexPositionNormalTexture& vtx);

		void AddIdx(uint16_t idx);

		uint16_t GetMiddlePoint(uint32_t vtxIndex1, uint32_t vtxIndex2);

#pragma endregion

		/**********************
		*   Private Methods   *
		***********************/
#pragma region Private Methods

#pragma endregion

		/*********************************
		*   Private Destructor Methods   *
		**********************************/
#pragma region Private Destructor Methods

		/// <summary>
		/// Default IcoSphereMesh Destructor
		/// </summary>
		virtual ~IcoSphereMesh();

#pragma endregion

	public:

		/**************************
		*   Constructor Methods   *
		***************************/
#pragma region Constructor Methods

		/// <summary>
		/// IcoSphereMesh Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device use to create Mesh</param>
		/// <param name="resourceName">Resource Name</param>
		IcoSphereMesh(GraphicDevice* graphicDevice, const std::wstring& resourceName);

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		XEResult Load() override;

#pragma endregion

		// Prevent copying.
		IcoSphereMesh(IcoSphereMesh const&) = delete;
		IcoSphereMesh& operator= (IcoSphereMesh const&) = delete;
};

#endif

