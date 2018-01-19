/** \file 
 * Contains definitions for basic use of the AEngine 
 */

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

#ifndef _BASE_H
#define _BASE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdint.h>

/***************************
*   Game Engine Includes   *
****************************/

/**************
*   Defines   *
***************/

//Uncomment for verbose deleting and releasing
//or define in the CPP where delete or release is
//taking place
//#define AE_DEBUG_DEL_PRINT_VAR_NAME
//#define AE_DEBUG_REL_PRINT_VAR_NAME

#define _AEWSTR(x) L#x
#define _AESTR(x) #x 
#define AEWSTR(x) _AEWSTR(x) 
#define AESTR(x) _AESTR(x) 

/// <summary>
/// Add comments to the code so when it compiles a message is seen.
/// This lets know if we need to work on something 
/// </summary>
/// <remarks>
/// This should be kept to a minimum, and non should appear when 
/// a product is ready to be released
/// </remarks>
#define AETODO(x) __pragma(message(__FILE__ "(" AESTR(__LINE__) "): TODO: " _AESTR(x))) 

/// <summary>
/// Release AE Engine Game Resources
/// Even if it is identical to ReleaseCOM
/// we want to have our own if we need to
/// change it in the future
/// </summary>
#define AERelease(x)	if(x != nullptr)	\
						{					\
							x->Release();	\
							x = nullptr;	\
						}

/// <summary>
/// Releases an object created by Angel Script
/// </summary>
#define ReleaseAngel(x)	if(x != nullptr)	\
						{					\
							x->Release(); 	\
							x = nullptr;	\
												}

/// <summary>
/// Releases an object created by PhysX
/// </summary>
#define ReleasePhysX(x)	if(x != nullptr)	\
						{					\
							x->release(); 	\
							x = nullptr;	\
						}

/// <summary>
/// Release DirectX COM Interfaces
/// </summary>
#if defined(_DEBUG) && defined(AE_DEBUG_REL_PRINT_VAR_NAME)
#define ReleaseCOM(x)	if(x != nullptr)																				\
						{																								\
							uint32_t rel = x->Release();																\
							x = nullptr;																				\
							std::wstringstream sst;																		\
							sst << rel;																					\
							std::wstring file(__FILEW__);																\
							std::wstring line(AEWSTR(__LINE__));														\
							std::wstring mvName = std::wstring(L"Release DirectX Var: ") + std::wstring(AEWSTR(x));		\
							std::wstring rem = std::wstring(L", remaining: ") + sst.str();								\
							std::wstring output = file + L"(" + line + L"): " + mvName + rem + L"\n";					\
							OutputDebugString(output.c_str());															\
						}
#else
#define ReleaseCOM(x)	if(x != nullptr)	\
						{					\
							x->Release();	\
							x = nullptr;	\
						}
#endif


/// <summary>
/// Deletes a pointer. Checks that the pointer is not null
/// and if it is not deletes it and sets it to null
/// </summary>
/// <remarks>
/// This should be use instead of 'delete'
/// </remarks>
#if defined(_DEBUG) && defined(AE_DEBUG_DEL_PRINT_VAR_NAME)
#define DeleteMem(x)	if(x != nullptr)																			\
						{																							\
							delete x;																				\
							x = nullptr;																			\
							std::wstring file(__FILEW__);															\
							std::wstring line(AEWSTR(__LINE__));													\
							std::wstring vname = std::wstring(L"Deleting Pointer: ") + std::wstring(AEWSTR(x));		\
							std::wstring output = file + L"(" + line + L"): " + vname + L"\n";						\
							OutputDebugString(output.c_str());														\
						}
#else
#define DeleteMem(x)	if(x != nullptr)	\
						{					\
							delete x;		\
							x = nullptr;	\
						}
#endif

/// <summary>
/// Deletes a Array pointer. Checks that the pointer is not null
/// and if it is not deletes it and sets it to null
/// </summary>
/// <remarks>
/// This should be use instead of 'delete[]'
/// </remarks>
#if defined(_DEBUG) && defined(AE_DEBUG_DEL_PRINT_VAR_NAME)
#define DeleteMemArr(x)	if(x != nullptr)																				\
						{																								\
							delete[] x;																					\
							x = nullptr;																				\
							std::wstring file(__FILEW__);																\
							std::wstring line(AEWSTR(__LINE__));														\
							std::wstring vname = std::wstring(L"Deleting Pointer Array: ") + std::wstring(AEWSTR(x));	\
							std::wstring output = file + L"(" + line + L"): " + vname + L"\n";							\
							OutputDebugString(output.c_str());															\
						}
#else
#define DeleteMemArr(x)	if(x != nullptr)	\
						{					\
							delete[] x;		\
							x = nullptr;	\
						}
#endif

/// <summary>
/// Base Filesystem Path for files to use.
/// </summary>
#define AE_Base_FS_PATH		L"..\\"

/*************
*   Aligns   *
**************/

/// <summary>
/// Use to align a variable to 16 byte Boundaries
/// </summary>
#define AEFloat4Align __declspec(align(16))

/// <summary>
/// Use to align a variable to 4 byte Boundaries
/// </summary>
#define AEByte4Align __declspec(align(4))

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/// <summary>
/// AEngine Return Code Types
/// </summary>
enum class AEResult : int32_t
{
	/// <summary>
	/// Generic Failure
	/// </summary>
	Fail = -90000,

	/// <summary>
	/// Object is not in a Ready State
	/// </summary>
	NotReady,

	/// <summary>
	/// Graphic Device pointer is null
	/// </summary>
	GraphicDeviceNull, 

	/// <summary>
	/// An object that was pass as a parameter is null
	/// </summary>
	NullParameter, 

	/// <summary>
	/// No Filename was given
	/// </summary>
	EmptyFilename,
	
	/// <summary>
	/// Failed to create DirectX Device.
	/// </summary>
	CreateDXDeviceFail,
	
	/// <summary>
	/// Failed to initialize DirectX Config.
	/// </summary>
	CreateDXConfFail,
	
	/// <summary>
	/// Failed to load required feature level
	/// </summary>
	FeatureLvlUnsupported,
	
	/// <summary>
	/// Failed to create DirectX Swap Chain
	/// </summary>
	CreateDXSwapChainFail,
	
	/// <summary>
	/// Failed to create Default Render Target and Depth Stencil
	/// </summary>
	CreateDXDefaultRTDSFail,
	
	/// <summary>
	/// Failed to initialize Viewport
	/// </summary>
	InitViewportFail,

	/// <summary>
	/// Failed to Register Global Property for Script 
	/// </summary>
	RegGlobalPropFail,

	/// <summary>
	/// Failed to Register Global Function for Script 
	/// </summary>
	RegGlobalFuncFail,

	/// <summary>
	/// Failed to Register Object Type for Script 
	/// </summary>
	RegObjTypeFail,

	/// <summary>
	/// Failed to Register Enum Value for Script 
	/// </summary>
	RegEnumValFail,

	/// <summary>
	/// Failed to Register Enum for Script 
	/// </summary>
	RegEnumFail,

	/// <summary>
	/// Failed to Register Object Method for Script 
	/// </summary>
	RegObjMethodFail,

	/// <summary>
	/// Failed to Register Object Property for Script
	/// </summary> 
	RegObjPropFail,

	/// <summary>
	/// Failed to Register Object Behavior for Script 
	/// </summary>
	RegObjBehaviorFail,

	/// <summary>
	/// Failed to Register Interface for Script 
	/// </summary>
	RegInterfaceFail,

	/// <summary>
	/// Failed to Open File 
	/// </summary>
	OpenFileFail,

	/// <summary>
	/// Index or Key not found
	/// </summary>
	NotFound,

	/// <summary>
	/// XML Read Error
	/// </summary>
	XMLReadError,

	/// <summary>
	/// Failed to initialize Logger
	/// </summary>
	InitLoggerFail,

	/// <summary>
	/// Failed Log to File is Active
	/// </summary>
	Log2FileActive,

	/// <summary>
	/// Failed Load Angel Script Module
	/// </summary>
	ASModuleNotFound,

	/// <summary>
	/// Failed Angel Script Module is already present
	/// </summary>
	ASModuleAlreadyCreated,

	/// <summary>
	/// Failed to Create Angel Script Module 
	/// </summary>
	ASModuleCreateFail,

	/// <summary>
	/// Failed to Add new Section to Angel Script Module 
	/// </summary>
	ASAddSecModuleFail,

	/// <summary>
	/// Failed to Build Angel Script Module 
	/// </summary>
	ASBuildModuleFail,

	/// <summary>
	/// Failed to Discard Angel Script Module 
	/// </summary>
	ASDiscardModuleFail,

	/// <summary>
	/// Failed to Prepare Angel Script Context
	/// </summary>
	ASPrepareContextFailed,

	/// <summary>
	/// Failed to Setting Argument Object on Angel Script Failed
	/// </summary>
	ASSetArgObjectFailed,

	/// <summary>
	/// Failed to Lock Keyboard
	/// </summary>
	KeyboardLockFailed,

	/// <summary>
	/// Failed to UnLock Keyboard
	/// </summary>
	KeyboardUnLockFailed,

	/// <summary>
	/// Failed to Initialize Structured Buffer
	/// </summary>
	StructuredBufferInitFailed,

	/// <summary>
	/// Failed to Set Data to Structured Buffer
	/// </summary>
	StructuredBufferSetDateFailed,

	/// <summary>
	/// Failed to Initialize Simple Buffer
	/// </summary>
	SimpleBufferInitFailed,

	/// <summary>
	/// Failed to Set Data to Simple Buffer
	/// </summary>
	SimpleBufferSetDateFailed,

	/// <summary>
	/// Failed to Initialize Constant Buffer
	/// </summary>
	ConstantBufferInitFailed,

	/// <summary>
	/// Failed to Set Data to Constant Buffer
	/// </summary>
	ConstantBufferSetDataFailed,

	/// <summary>
	/// Failed to Load Vertex Shader
	/// </summary>
	VertexShaderLoadFailed,

	/// <summary>
	/// Failed to Load Pixel Shader
	/// </summary>
	PixelShaderLoadFailed,

	/// <summary>
	/// Failed to Load Hull Shader
	/// </summary>
	HullShaderLoadFailed,

	/// <summary>
	/// Failed to Load Domain Shader
	/// </summary>
	DomainShaderLoadFailed,

	/// <summary>
	/// Failed to Load Geometry Shader
	/// </summary>
	GeometryShaderLoadFailed,

	/// <summary>
	/// Failed to Load Compute Shader
	/// </summary>
	ComputeShaderLoadFailed,

	/// <summary>
	/// Failed Empty String
	/// </summary>
	EmptyString,

	/// <summary>
	/// Failed Object Exists
	/// </summary>
	ObjExists,

	/// <summary>
	/// Failed Invalid Effect Permutation
	/// </summary>
	InvalidPermutation,
	
	/// <summary>
	/// Failed to Set Texture
	/// </summary>
	SetTextureFailed,

	/// <summary>
	/// Failed nullptr object detected
	/// </summary>
	NullObj,

	/// <summary>
	/// Failed due to being outside of the range
	/// </summary>
	OutsideRange,

	/// <summary>
	/// Failed to Load File
	/// </summary>
	LoadFileFailed,

	/// <summary>
	/// Assimp Failed to Process Graph
	/// </summary>
	AssimpProcessGraph,

	/// <summary>
	/// Assimp Failed to Process Materials
	/// </summary>
	AssimpProcessMaterials,

	/// <summary>
	/// Assimp Failed to Process Embedded Textures
	/// </summary>
	AssimpProcessEmbeddedTextures,

	/// <summary>
	/// Assimp Failed to Process Meshes
	/// </summary>
	AssimpProcessMeshes,

	/// <summary>
	/// Assimp Failed to Process Skeleton
	/// </summary>
	AssimpProcessSkeleton,

	/// <summary>
	/// Assimp Failed to Process Animation
	/// </summary>
	AssimpProcessAnimation,

	/// <summary>
	/// Assimp Failed to Construct Model
	/// </summary>
	AssimpProcessConstructModel,

	/// <summary>
	/// Failed due to Skeleton having no bones
	/// </summary>
	EmptySkeleton,

	/// <summary>
	/// Failed due to Name of Object been empty string
	/// </summary>
	EmptyName,

	/// <summary>
	/// Game Resource Manager pointer is null
	/// </summary>
	GameResourceManagerNull,

	/// <summary>
	/// Game Resource is already Managed
	/// </summary>
	ResourceIsManaged,

	/// <summary>
	/// Game Resource has failed to be 
	/// added to Resource Manager
	/// </summary>
	ResourceManagedFailed,
	
	/// <summary>
	/// Game Resource Type does not match
	/// </summary>
	ResourceManagedTypeError,

	/// <summary>
	/// Failed to Create Shader Resource View
	/// </summary>
	CreateSRViewFailed,

	/// <summary>
	/// Failed to Create Render Target View
	/// </summary>
	CreateRTViewFailed,

	/// <summary>
	/// Failed to Create Texture
	/// </summary>
	CreateTextureFailed,

	/// <summary>
	/// Failed to Depth Stencil View
	/// </summary>
	CreateDSViewFailed,

	/// <summary>
	/// Failed to add due to maximum item type in collection
	/// </summary>
	MaximunItemType,

	/// <summary>
	/// Failed due to object type being invalid option
	/// </summary>
	InvalidObjType,

	/// <summary>
	/// Failed due to size being zero
	/// </summary>
	ZeroSize,

	/// <summary>
	/// Failed due to the Texture not matching the type expected
	/// </summary>
	InvalidTextureType,

	/// <summary>
	/// Failed due to the Shader not matching the type expected
	/// </summary>
	InvalidShaderType,

	/// <summary>
	/// Failed when trying to set the Constant Buffer to the Shader
	/// </summary>
	SetConstantBufferFailed,

	/// <summary>
	/// Failed when trying to set the Simple Buffer to the Shader
	/// </summary>
	SetSimpleBufferFailed,

	/// <summary>
	/// Failed when trying to set the Structured Buffer to the Shader
	/// </summary>
	SetStructuredBufferFailed,

	/// <summary>
	/// Failed when trying to set the Vertex Shader to the device
	/// </summary>
	SetVertexShaderFailed,

	/// <summary>
	/// Failed when trying to set the Pixel Shader to the device
	/// </summary>
	SetPixelShaderFailed,

	/// <summary>
	/// Failed when trying to set the Geometry Shader to the device
	/// </summary>
	SetGeometryShaderFailed,

	/// <summary>
	/// Failed when trying to set the Domain Shader to the device
	/// </summary>
	SetDomainShaderFailed,

	/// <summary>
	/// Failed when trying to set the Hull Shader to the device
	/// </summary>
	SetHullShaderFailed,

	/// <summary>
	/// Failed when trying to set the Compute Shader to the device
	/// </summary>
	SetComputeShaderFailed,

	/// <summary>
	/// Failed when trying to set the Vertex Shader Properties to the device
	/// </summary>
	SetVertexShaderPropsFailed,

	/// <summary>
	/// Failed when trying to set the Pixel Shader Properties to the device
	/// </summary>
	SetPixelShaderPropsFailed,

	/// <summary>
	/// Failed when trying to set the Geometry Shader Properties to the device
	/// </summary>
	SetGeometryShaderPropsFailed,

	/// <summary>
	/// Failed when trying to set the Domain Shader Properties to the device
	/// </summary>
	SetDomainShaderPropsFailed,

	/// <summary>
	/// Failed when trying to set the Hull Shader Properties to the device
	/// </summary>
	SetHullShaderPropsFailed,

	/// <summary>
	/// Failed when trying to set the Compute Shader Properties to the device
	/// </summary>
	SetComputeShaderPropsFailed,

	/// <summary>
	/// Failed when trying to create Shader Texture Binding
	/// </summary>
	ShaderTextureBindingInitFailed,

	/// <summary>
	/// Failed when trying to create Shader Structured Buffer
	/// </summary>
	ShaderStructuredBufferInitFailed,

	/// <summary>
	/// Failed when trying to create Shader Simple Buffer
	/// </summary>
	ShaderSimpleBufferInitFailed,

	/// <summary>
	/// Failed due to Unknown file extension
	/// </summary>
	UnknownFileExtension,

	/// <summary>
	/// Failed due to Empty file extension
	/// </summary>
	EmptyFileExtension,

	/// <summary>
	/// Failed due to Unknown Content Type
	/// </summary>
	UnknownContentType,

	/// <summary>
	/// Failed due to Game Object Component already associated with another Game Object
	/// </summary>
	GOCIsAssociated,

	/// <summary>
	/// Failed due to Game Object Component is not associated with any Game Object
	/// </summary>
	GOCIsNotAssociated,

	/// <summary>
	/// Game Object Manager pointer is null
	/// </summary>
	GameObjectManagerNull,

	/// <summary>
	/// Localization Manager pointer is null
	/// </summary>
	LocalizationManagerNull,

	/// <summary>
	/// Logger pointer is null
	/// </summary>
	LoggerNull,

	/// <summary>
	/// Failed to import file
	/// </summary>
	ImportFail,

	/// <summary>
	/// Failed due to invalid file header
	/// </summary>
	InvalidFileHeader,

	/// <summary>
	/// Failed due to invalid Shader Model
	/// </summary>
	InvalidShaderModel,

	/// <summary>
	/// Shader Compiled Failed
	/// </summary>
	ShaderCompiledFail,

	/// <summary>
	/// Failed when Extracting Shader Simple Buffers
	/// </summary>
	ShaderExtractSimpleBuffersFail,

	/// <summary>
	/// Failed when Extracting Shader Structured Buffers
	/// </summary>
	ShaderExtractStructuredBuffersFail,

	/// <summary>
	/// Failed when Extracting Shader Constant Buffers
	/// </summary>
	ShaderExtractConstantBuffersFail,

	/// <summary>
	/// Failed when Extracting Shader Texture Inputs
	/// </summary>
	ShaderExtractTextureInputFail,

	/// <summary>
	/// Failed due to Shader Type Mismatch
	/// </summary>
	ShaderTypeMismatch,
	
	/// <summary>
	/// Failed due to the File does not exists
	/// </summary>
	FileNotPresent,

	/// <summary>
	/// Game Asset Manager pointer is null
	/// </summary>
	GameAssetManagerNull,

	/// <summary>
	/// Game Asset has not been loaded
	/// </summary>
	GameAssetNotLoaded,

	/// <summary>
	/// Game Asset ID is already in use
	/// </summary>
	GameAssetIDInUse,

	/// <summary>
	/// Game Asset ID is invalid
	/// </summary>
	GameAssetInvalidID,

	/// <summary>
	/// Raw Game Asset does not exist
	/// </summary>
	RawGameAssetNotFound,

	/// <summary>
	/// Shader Object is null
	/// </summary>
	ShaderNull,

	/// <summary>
	/// Invalid or Incompatible Resource Map Type
	/// </summary>
	InvalidResourceMapType,

	/// <summary>
	/// Failed to Map the Resource to Memory
	/// </summary>
	ResourceMapFailed,

	/// <summary>
	/// Invalid Index Buffer Type
	/// </summary>
	InvalidIndexBufferType,

	/// <summary>
	/// Failed to update Index Buffer
	/// </summary>
	FailIndexBufferUpdate,

	/// <summary>
	/// Failed to initialize Index Buffer
	/// </summary>
	FailIndexBufferInit,

	/// <summary>
	/// Container is full
	/// </summary>
	FullContainer,

	/// <summary>
	/// Light Manager pointer is null
	/// </summary>
	LightManagerNull,

	/// <summary>
	/// No Camera Obj is null
	/// </summary>
	CameraNull,

	/// <summary>
	/// Writing information to a file was not successful
	/// </summary>
	WriteToFileFailed,

	/// <summary>
	/// Configuration Load Error
	/// </summary>
	ConfigLoadError,

    /// <summary>
    /// Vector Exception Handle Creation Failed
    /// </summary>
    ExceptionHandledFailed,

	/****************************************************
	* Add new error above this line, 
	* do not forget to include it in
	*   - Script Register Enum (AngelScriptManager.cpp)
	*   - GetAEResultString Function (BaseFunctions.cpp)
	*****************************************************/

	/// <summary>
	/// Everything went as expected
	/// </summary>
	Ok = 0
};

/*************
*   Struct   *
**************/

/***************
*   Typedefs   *
****************/

#endif