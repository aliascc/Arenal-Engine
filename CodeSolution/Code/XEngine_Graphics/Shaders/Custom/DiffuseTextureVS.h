/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _DIFFUSE_TEXTURE_VS_H
#define _DIFFUSE_TEXTURE_VS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Shaders\VertexShader.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IConstantBuffer;

/**************
*   Defines   *
***************/

/// <summary>
/// Vertex Shader PNT Name
/// </summary>
#define XE_DIFFUSE_TEXTURE_VS_NAME			L"Diffuse Texture Vertex Shader"

/*****************
*   Class Decl   *
******************/
class DiffuseTextureVS sealed : public VertexShader
{
private:

	/************************
	*   Private Variables   *
	*************************/
#pragma region Private Variables

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
	/// Default DiffuseTextureVS Destructor
	/// </summary>
	virtual ~DiffuseTextureVS();

#pragma endregion

public:

	/**************************
	*   Constructor Methods   *
	***************************/
#pragma region Constructor Methods

	/// <summary>
	/// Default DiffuseTextureVS Constructor
	/// </summary>
	/// <param name="graphicDevice">Graphic Device to be associated with this Shader</param>
	/// <param name="name">Name of Shader</param>
	DiffuseTextureVS(GraphicDevice* graphicDevice, const std::wstring& resourceName);

#pragma endregion

	/******************
	*   Get Methods   *
	*******************/
#pragma region Get Methods

#pragma endregion

	/************************
	*   Framework Methods   *
	*************************/
#pragma region Framework Methods

	XEResult Load() override;

#pragma endregion

};

#endif