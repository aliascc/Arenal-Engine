/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

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