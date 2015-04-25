/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _BONE_H
#define _BONE_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\Named.h"
#include "Math\XEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class Bone sealed : public Named
{
	private:
		glm::mat4 m_LocalMatrix = XEMathHelpers::Mat4Identity;

		glm::mat4 m_WorldMatrix = XEMathHelpers::Mat4Identity;

		glm::mat4 m_OffSetMatrix = XEMathHelpers::Mat4Identity;

		int32_t m_ParentIndex = -1;

		int32_t m_Index = -1;

	public:
		Bone();
		~Bone();

		inline const glm::mat4& GetWorldMatrix() const 
		{
			return m_WorldMatrix;
		}

		inline const glm::mat4& GetLocalMatrix() const 
		{
			return m_LocalMatrix;
		}

		inline const glm::mat4& GetOffSetMatrix() const 
		{
			return m_OffSetMatrix;
		}

		inline int32_t GetParentIndex() const
		{
			return m_ParentIndex;
		}

		inline int32_t GetIndex() const
		{
			return m_Index;
		}

		inline void SetIndex(int32_t index)
		{
			m_Index = index;
		}

		inline void SetParentIndex(int32_t index)
		{
			m_ParentIndex = index;
		}

		inline void SetWorldMatrix(const glm::mat4& world)
		{
			m_WorldMatrix = world;
		}

		inline void SetLocalMatrix(const glm::mat4& local)
		{
			m_LocalMatrix = local;
		}

		inline void SetOffSetMatrix(const glm::mat4& offsetMatrix)
		{
			m_OffSetMatrix = offsetMatrix;
		}
};

#endif