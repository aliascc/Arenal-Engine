
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


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicDevice.h"
#include "Shaders\ShaderDefs.h"
#include "Base\BaseFunctions.h"
#include "BuiltInMaterialsDefs.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Models\Skinning\SkinningDefs.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/********************************
*   XEBuiltInMaterialsHelpers   *
*********************************/
namespace XEBuiltInMaterialsHelpers
{

	XEResult BuildCBWVP(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_World_View_Proj
		//
		//	cbuffer _XE_CB_World_View_Proj : register(b0)
		//	{
		//		matrix _XE_World		: packoffset(c0);
		//		matrix _XE_View			: packoffset(c4);
		//		matrix _XE_Projection	: packoffset(c8);
		//	};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_World_View_Proj
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, XE_CB_WORLD_VIEW_PROJ_NAME);

		/**************************************
		*Add Var 1: _XE_World
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_WORLD_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 2: _XE_View
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_VIEW_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 3: _XE_Projection
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_PROJECTION_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/

		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildCBVP(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_View_Proj
		//
		//	cbuffer _XE_CB_View_Proj : register(b0)
		//	{
		//		matrix _XE_View			: packoffset(c0);
		//		matrix _XE_Projection	: packoffset(c4);
		//	};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_World_View_Proj
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, XE_CB_WORLD_VIEW_PROJ_NAME);

		/**************************************
		*Add Var 1: _XE_View
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_VIEW_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 2: _XE_Projection
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_PROJECTION_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/
		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildCBBones(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_Bones
		//
		//	cbuffer _XE_CB_Bones : register(b1)
		//	{
		//		matrix _XE_BoneTransforms[MAX_BONES] : packoffset(c0);
		//	};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_Bones
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 1, XE_CB_BONES_NAME);

		/**************************************
		*Add Var 1: _XE_World
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_BONE_TRANSFORMS_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = XE_MAX_BONES;
		scv->m_IsArray = true;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4) * XE_MAX_BONES;
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/

		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(cb);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildCBColor(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_Color
		//
		//	cbuffer _XE_CB_Color : register(b0)
		//	{
		//		float4 u_Color : packoffset(c0);
		//	};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_Color
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, XE_CB_COLOR_NAME);

		/**************************************
		*Add Var 1: _XE_World
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_COLOR_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::vec4);
		scv->m_Offset = 0;
		scv->m_UserVariable = true;
		scv->m_Size = sizeof(glm::vec4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Vector;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/
		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(cb);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildCBHalfPixel(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_HalfPixel
		//
		//	cbuffer _XE_CB_HalfPixel : register(b0)
		//	{
		//		float2 _XE_HalfPixel	: packoffset(c0);
		//	};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_HalfPixel
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, XE_CB_HALF_PIXEL_NAME);

		/**************************************
		*Add Var 1: _XE_HalfPixel
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_HALF_PIXEL_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 2;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::vec2);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::vec2);
		scv->m_ShaderVariableClass = ShaderVariableClass::Vector;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/

		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(cb);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildCBFPRLightCulling(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_FPR_LightCulling
		//
		//		cbuffer _XE_CB_FPR_LightCulling : register(b0)
		//		{
		//			matrix	_XE_View				: packoffset(c0);
		//			matrix	_XE_InvProjection		: packoffset(c4);
		//		
		//			uint	_XE_WindowHeight		: packoffset(c8.x);
		//			uint	_XE_WindowWidth			: packoffset(c8.y);
		//			uint	_XE_NumLights			: packoffset(c8.z);
		//		};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_FPR_LightCulling
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, XE_CB_FPR_LIGHT_CULLING_NAME);

		/**************************************
		*Add Var 1: _XE_View
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_VIEW_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 2: _XE_InvProjection
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_INV_PROJECTION_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 4;
		scv->m_Rows = 4;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::mat4);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::mat4);
		scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 3: _XE_WindowHeight
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_WINDOW_HEIGHT_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 1;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(uint32_t);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(uint32_t);
		scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
		scv->m_ShaderVariableType = ShaderVariableType::UInt;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 4: _XE_WindowWidth
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_WINDOW_WIDTH_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 1;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(uint32_t);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(uint32_t);
		scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
		scv->m_ShaderVariableType = ShaderVariableType::UInt;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 5: _XE_NumLights
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_NUM_LIGHTS_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 1;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(uint32_t);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(uint32_t);
		scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
		scv->m_ShaderVariableType = ShaderVariableType::UInt;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/

		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(cb);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildCBFPRLights(GraphicDevice* graphicDevice, ConstantBuffer** cb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(cb != nullptr);
		if (cb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;
		uint32_t offset = 0;

		/////////////////////////////////////////////////////
		//Create Constant Buffer _XE_CB_FPR_LightCulling
		//
		//		cbuffer _XE_CB_FPR_Lights : register(b0)
		//		{
		//			float3	_XE_CameraPos			: packoffset(c0);
		//			uint	_XE_WindowWidth			: packoffset(c0.w);
		//		};
		//

		/****************************************************************************
		*Constant Buffer: _XE_CB_HalfPixel
		****************************************************************************/
		ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, XE_CB_FPR_LIGHTS_NAME);

		/**************************************
		*Add Var 1: _XE_CameraPos
		***************************************/
		ShaderCustomVariable* scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_CAMERA_POS_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 3;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(glm::vec3);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(glm::vec3);
		scv->m_ShaderVariableClass = ShaderVariableClass::Vector;
		scv->m_ShaderVariableType = ShaderVariableType::Float;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Add Var 2: _XE_WindowWidth
		***************************************/
		scv = new ShaderCustomVariable();
		scv->m_Name = XE_CB_WINDOW_WIDTH_VAR_NAME;
		scv->m_StartOffset = offset;
		scv->m_Columns = 1;
		scv->m_Rows = 1;
		scv->m_Elements = 0;
		scv->m_IsArray = false;
		scv->m_ElementSize = sizeof(uint32_t);
		scv->m_Offset = 0;
		scv->m_UserVariable = false;
		scv->m_Size = sizeof(uint32_t);
		scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
		scv->m_ShaderVariableType = ShaderVariableType::UInt;

		ret = tempCB->AddVariable(scv);

		offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempCB);

			DeleteMem(scv);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Initialize
		***************************************/

		ret = tempCB->Initialize();

		if (ret != XEResult::Ok)
		{
			DeleteMem(cb);

			return XEResult::ConstantBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*cb = tempCB;

		return XEResult::Ok;
	}

	XEResult BuildBufferLightBuffer(GraphicDevice* graphicDevice, StructuredBuffer** sb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(sb != nullptr);
		if (sb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;

		/////////////////////////////////////////////////////
		//Create Structured Buffer
		//as in Shader:
		// StructuredBuffer<Light> _XE_LightBuffer : register(t0);
		//

		/****************************************************************************
		*Structured Buffer: _XE_LightBuffer
		****************************************************************************/
		StructuredBuffer* tempSB = new StructuredBuffer(XE_BF_LIGHT_BUFFER_NAME, XE_BI_LIGHT_BUFFER_CULL_BIND_IDX, false, graphicDevice);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempSB);

			return XEResult::ShaderStructuredBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*sb = tempSB;

		return XEResult::Ok;
	}

	XEResult BuildBufferPerTileLightIndexBuffer(GraphicDevice* graphicDevice, SimpleBuffer** sb)
	{
		XEAssert(graphicDevice != nullptr);
		if (graphicDevice == nullptr)
		{
			return XEResult::GraphicDeviceNull;
		}

		XEAssert(sb != nullptr);
		if (sb == nullptr)
		{
			return XEResult::NullParameter;
		}

		XEResult ret = XEResult::Ok;

		/////////////////////////////////////////////////////
		//Create Structured Buffer
		//as in Shader:
		// RWBuffer<uint> _XE_PerTileLightIndexBuffer
		//

		/****************************************************************************
		*Simple Buffer: _XE_PerTileLightIndexBuffer
		****************************************************************************/
		SimpleBuffer* tempSB = new SimpleBuffer(XE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME, XE_BI_PER_TILE_LIGHT_INDEX_BUFFER_CULL_BIND_IDX, true, ShaderVariableClass::Scalar, ShaderVariableType::UInt, 1, graphicDevice);

		if (ret != XEResult::Ok)
		{
			DeleteMem(tempSB);

			return XEResult::ShaderSimpleBufferInitFailed;
		}

		/**************************************
		*Constant Buffer Finish
		***************************************/
		*sb = tempSB;

		return XEResult::Ok;
	}

}
