/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _WRITER_AE3D_H
#define _WRITER_AE3D_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <fstream>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ModelContent;
struct MeshContent;
struct SkeletonContent;
struct AnimationContent;

/*****************
*   Class Decl   *
******************/
class WriterAE3D sealed : public AEObject
{
	private:
		std::wstring m_FileName = L"";

		std::wstring m_FilePath = L"";

		std::wstring m_FullFileName = L"";

		AEResult WriteMesh(const std::wstring& fileName, MeshContent* mesh);

		AEResult WriteSkeleton(const std::wstring& fileName, SkeletonContent* skeleton);

		AEResult WriteAnimation(const std::wstring& fileName, AnimationContent* animation);

		AEResult WriteModelBody();

	public:
		WriterAE3D();
		virtual ~WriterAE3D();

		const std::wstring& GetOutputFilePath() const 
		{
			return m_FullFileName;
		}

		//Framework
		AEResult WriteToFile(ModelContent* content, const std::wstring& fileName, const std::wstring& filePath);
};

#endif