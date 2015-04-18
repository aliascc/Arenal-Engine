
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 6/29/2013
*
* Desc:
*
*
* Copyright 2013
*
*********************************************************/

#pragma once

#ifndef _INCLUDE_INTERFACE_HLSL_H
#define _INCLUDE_INTERFACE_HLSL_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <D3Dcommon.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
struct IncludeInterfaceHLSL : public ID3DInclude
{
	std::wstring m_ShaderCurrentDir = L"";

	IncludeInterfaceHLSL(const std::wstring& currentDirectory);

	HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes) override;

	HRESULT __stdcall Close(LPCVOID pData) override;
};

#endif