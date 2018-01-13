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

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "BaseAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
BaseAddOnAS::BaseAddOnAS()
	: AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Base)
{
}

BaseAddOnAS::~BaseAddOnAS()
{
}

AEResult BaseAddOnAS::Register(asIScriptEngine* engine)
{
	AEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return AEResult::NullParameter;
	}

	if (RegisterBasicEngineConst(engine) != AEResult::Ok)
	{
		AETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_CONSTS_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterAEResultEnum(engine) != AEResult::Ok)
	{
		std::wstring errMsg = L"";
		AETODO("Add Log and return");
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_ENUM_ERR_MSG"), __FUNCTIONW__, L"AEResult");
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	if (RegisterBasicEngineFuncs(engine) != AEResult::Ok)
	{
		std::wstring errMsg = L"";
		AETODO("Add Log and return");
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_FUNCTS_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return AEResult::Fail;
	}

	return AEResult::Ok;
}

AEResult BaseAddOnAS::RegisterBasicEngineConst(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterGlobalProperty("const wstring AE_VERSION", (void*)&AE_Base::AE_VERSION);
	if (ret < 0)
	{
		return AEResult::RegGlobalPropFail;
	}

	ret = engine->RegisterGlobalProperty("const wstring AE_CODENAME", (void*)&AE_Base::AE_CODENAME);
	if (ret < 0)
	{
		return AEResult::RegGlobalPropFail;
	}

	return AEResult::Ok;
}

AEResult BaseAddOnAS::RegisterAEResultEnum(asIScriptEngine* engine)
{
	int ret = 0;

	///////////////////////
	//Register AEResult
	ret = engine->RegisterEnum("AEResult");
	if (ret < 0)
	{
		return AEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "Ok", static_cast<int32_t>(AEResult::Ok));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "Fail", static_cast<int32_t>(AEResult::Fail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "NotReady", static_cast<int32_t>(AEResult::NotReady));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GraphicDeviceNull", static_cast<int32_t>(AEResult::GraphicDeviceNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "NullParameter", static_cast<int32_t>(AEResult::NullParameter));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "EmptyFilename", static_cast<int32_t>(AEResult::EmptyFilename));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateDXDeviceFail", static_cast<int32_t>(AEResult::CreateDXDeviceFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateDXConfFail", static_cast<int32_t>(AEResult::CreateDXConfFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "FeatureLvlUnsupported", static_cast<int32_t>(AEResult::FeatureLvlUnsupported));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateDXSwapChainFail", static_cast<int32_t>(AEResult::CreateDXSwapChainFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateDXDefaultRTDSFail", static_cast<int32_t>(AEResult::CreateDXDefaultRTDSFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InitViewportFail", static_cast<int32_t>(AEResult::InitViewportFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegGlobalPropFail", static_cast<int32_t>(AEResult::RegGlobalPropFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegGlobalFuncFail", static_cast<int32_t>(AEResult::RegGlobalFuncFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegObjTypeFail", static_cast<int32_t>(AEResult::RegObjTypeFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegEnumValFail", static_cast<int32_t>(AEResult::RegEnumValFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegEnumFail", static_cast<int32_t>(AEResult::RegEnumFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegObjMethodFail", static_cast<int32_t>(AEResult::RegObjMethodFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegObjPropFail", static_cast<int32_t>(AEResult::RegObjPropFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegObjBehaviorFail", static_cast<int32_t>(AEResult::RegObjBehaviorFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RegInterfaceFail", static_cast<int32_t>(AEResult::RegInterfaceFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "OpenFileFail", static_cast<int32_t>(AEResult::OpenFileFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "NotFound", static_cast<int32_t>(AEResult::NotFound));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "XMLReadError", static_cast<int32_t>(AEResult::XMLReadError));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InitLoggerFail", static_cast<int32_t>(AEResult::InitLoggerFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "Log2FileActive", static_cast<int32_t>(AEResult::Log2FileActive));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASModuleNotFound", static_cast<int32_t>(AEResult::ASModuleNotFound));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASModuleAlreadyCreated", static_cast<int32_t>(AEResult::ASModuleAlreadyCreated));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASModuleCreateFail", static_cast<int32_t>(AEResult::ASModuleCreateFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASAddSecModuleFail", static_cast<int32_t>(AEResult::ASAddSecModuleFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASBuildModuleFail", static_cast<int32_t>(AEResult::ASBuildModuleFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASDiscardModuleFail", static_cast<int32_t>(AEResult::ASDiscardModuleFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASPrepareContextFailed", static_cast<int32_t>(AEResult::ASPrepareContextFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ASSetArgObjectFailed", static_cast<int32_t>(AEResult::ASSetArgObjectFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "KeyboardLockFailed", static_cast<int32_t>(AEResult::KeyboardLockFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "KeyboardUnLockFailed", static_cast<int32_t>(AEResult::KeyboardUnLockFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "StructuredBufferInitFailed", static_cast<int32_t>(AEResult::StructuredBufferInitFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "StructuredBufferSetDateFailed", static_cast<int32_t>(AEResult::StructuredBufferSetDateFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SimpleBufferInitFailed", static_cast<int32_t>(AEResult::SimpleBufferInitFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SimpleBufferSetDateFailed", static_cast<int32_t>(AEResult::SimpleBufferSetDateFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ConstantBufferInitFailed", static_cast<int32_t>(AEResult::ConstantBufferInitFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ConstantBufferSetDataFailed", static_cast<int32_t>(AEResult::ConstantBufferSetDataFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "VertexShaderLoadFailed", static_cast<int32_t>(AEResult::VertexShaderLoadFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "PixelShaderLoadFailed", static_cast<int32_t>(AEResult::PixelShaderLoadFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "HullShaderLoadFailed", static_cast<int32_t>(AEResult::HullShaderLoadFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "DomainShaderLoadFailed", static_cast<int32_t>(AEResult::DomainShaderLoadFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GeometryShaderLoadFailed", static_cast<int32_t>(AEResult::GeometryShaderLoadFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ComputeShaderLoadFailed", static_cast<int32_t>(AEResult::ComputeShaderLoadFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "EmptyString", static_cast<int32_t>(AEResult::EmptyString));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ObjExists", static_cast<int32_t>(AEResult::ObjExists));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidPermutation", static_cast<int32_t>(AEResult::InvalidPermutation));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetTextureFailed", static_cast<int32_t>(AEResult::SetTextureFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "NullObj", static_cast<int32_t>(AEResult::NullObj));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "OutsideRange", static_cast<int32_t>(AEResult::OutsideRange));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "LoadFileFailed", static_cast<int32_t>(AEResult::LoadFileFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessGraph", static_cast<int32_t>(AEResult::AssimpProcessGraph));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessMaterials", static_cast<int32_t>(AEResult::AssimpProcessMaterials));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessEmbeddedTextures", static_cast<int32_t>(AEResult::AssimpProcessEmbeddedTextures));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessMeshes", static_cast<int32_t>(AEResult::AssimpProcessMeshes));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessSkeleton", static_cast<int32_t>(AEResult::AssimpProcessSkeleton));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessAnimation", static_cast<int32_t>(AEResult::AssimpProcessAnimation));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "AssimpProcessConstructModel", static_cast<int32_t>(AEResult::AssimpProcessConstructModel));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "EmptySkeleton", static_cast<int32_t>(AEResult::EmptySkeleton));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "EmptyName", static_cast<int32_t>(AEResult::EmptyName));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GameResourceManagerNull", static_cast<int32_t>(AEResult::GameResourceManagerNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ResourceIsManaged", static_cast<int32_t>(AEResult::ResourceIsManaged));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ResourceManagedFailed", static_cast<int32_t>(AEResult::ResourceManagedFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ResourceManagedTypeError", static_cast<int32_t>(AEResult::ResourceManagedTypeError));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateSRViewFailed", static_cast<int32_t>(AEResult::CreateSRViewFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateRTViewFailed", static_cast<int32_t>(AEResult::CreateRTViewFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateTextureFailed", static_cast<int32_t>(AEResult::CreateTextureFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CreateDSViewFailed", static_cast<int32_t>(AEResult::CreateDSViewFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "MaximunItemType", static_cast<int32_t>(AEResult::MaximunItemType));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidObjType", static_cast<int32_t>(AEResult::InvalidObjType));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ZeroSize", static_cast<int32_t>(AEResult::ZeroSize));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidTextureType", static_cast<int32_t>(AEResult::InvalidTextureType));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidShaderType", static_cast<int32_t>(AEResult::InvalidShaderType));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetConstantBufferFailed", static_cast<int32_t>(AEResult::SetConstantBufferFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetSimpleBufferFailed", static_cast<int32_t>(AEResult::SetSimpleBufferFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetStructuredBufferFailed", static_cast<int32_t>(AEResult::SetStructuredBufferFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetVertexShaderFailed", static_cast<int32_t>(AEResult::SetVertexShaderFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetPixelShaderFailed", static_cast<int32_t>(AEResult::SetPixelShaderFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetGeometryShaderFailed", static_cast<int32_t>(AEResult::SetGeometryShaderFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetDomainShaderFailed", static_cast<int32_t>(AEResult::SetDomainShaderFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetHullShaderFailed", static_cast<int32_t>(AEResult::SetHullShaderFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetComputeShaderFailed", static_cast<int32_t>(AEResult::SetComputeShaderFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetVertexShaderPropsFailed", static_cast<int32_t>(AEResult::SetVertexShaderPropsFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetPixelShaderPropsFailed", static_cast<int32_t>(AEResult::SetPixelShaderPropsFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetGeometryShaderPropsFailed", static_cast<int32_t>(AEResult::SetGeometryShaderPropsFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetDomainShaderPropsFailed", static_cast<int32_t>(AEResult::SetDomainShaderPropsFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetHullShaderPropsFailed", static_cast<int32_t>(AEResult::SetHullShaderPropsFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "SetComputeShaderPropsFailed", static_cast<int32_t>(AEResult::SetComputeShaderPropsFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderTextureBindingInitFailed", static_cast<int32_t>(AEResult::ShaderTextureBindingInitFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderStructuredBufferInitFailed", static_cast<int32_t>(AEResult::ShaderStructuredBufferInitFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderSimpleBufferInitFailed", static_cast<int32_t>(AEResult::ShaderSimpleBufferInitFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "UnknownFileExtension", static_cast<int32_t>(AEResult::UnknownFileExtension));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "EmptyFileExtension", static_cast<int32_t>(AEResult::EmptyFileExtension));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GOCIsAssociated", static_cast<int32_t>(AEResult::GOCIsAssociated));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GOCIsNotAssociated", static_cast<int32_t>(AEResult::GOCIsNotAssociated));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GameObjectManagerNull", static_cast<int32_t>(AEResult::GameObjectManagerNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "LocalizationManagerNull", static_cast<int32_t>(AEResult::LocalizationManagerNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "LoggerNull", static_cast<int32_t>(AEResult::LoggerNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ImportFail", static_cast<int32_t>(AEResult::ImportFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidFileHeader", static_cast<int32_t>(AEResult::InvalidFileHeader));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidShaderModel", static_cast<int32_t>(AEResult::InvalidShaderModel));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderCompiledFail", static_cast<int32_t>(AEResult::ShaderCompiledFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderExtractSimpleBuffersFail", static_cast<int32_t>(AEResult::ShaderExtractSimpleBuffersFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderExtractStructuredBuffersFail", static_cast<int32_t>(AEResult::ShaderExtractStructuredBuffersFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderExtractConstantBuffersFail", static_cast<int32_t>(AEResult::ShaderExtractConstantBuffersFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderExtractTextureInputFail", static_cast<int32_t>(AEResult::ShaderExtractTextureInputFail));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderTypeMismatch", static_cast<int32_t>(AEResult::ShaderTypeMismatch));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "FileNotPresent", static_cast<int32_t>(AEResult::FileNotPresent));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GameAssetManagerNull", static_cast<int32_t>(AEResult::GameAssetManagerNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GameAssetNotLoaded", static_cast<int32_t>(AEResult::GameAssetNotLoaded));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GameAssetIDInUse", static_cast<int32_t>(AEResult::GameAssetIDInUse));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "GameAssetInvalidID", static_cast<int32_t>(AEResult::GameAssetInvalidID));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "RawGameAssetNotFound", static_cast<int32_t>(AEResult::RawGameAssetNotFound));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ShaderNull", static_cast<int32_t>(AEResult::ShaderNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidResourceMapType", static_cast<int32_t>(AEResult::InvalidResourceMapType));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "ResourceMapFailed", static_cast<int32_t>(AEResult::ResourceMapFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "InvalidIndexBufferType", static_cast<int32_t>(AEResult::InvalidIndexBufferType));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "FailIndexBufferUpdate", static_cast<int32_t>(AEResult::FailIndexBufferUpdate));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "FailIndexBufferInit", static_cast<int32_t>(AEResult::FailIndexBufferInit));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "FullContainer", static_cast<int32_t>(AEResult::FullContainer));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "LightManagerNull", static_cast<int32_t>(AEResult::LightManagerNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "CameraNull", static_cast<int32_t>(AEResult::CameraNull));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("AEResult", "WriteToFileFailed", static_cast<int32_t>(AEResult::WriteToFileFailed));
	if (ret < 0)
	{
		return AEResult::RegEnumValFail;
	}

	return AEResult::Ok;
}

AEResult BaseAddOnAS::RegisterBasicEngineFuncs(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterGlobalFunction("void ConsolePrintLine(const wstring&in)", asFUNCTION(AE_Base::ConsolePrintLine), asCALL_CDECL);
	if (ret < 0)
	{
		return AEResult::RegGlobalFuncFail;
	}

	ret = engine->RegisterGlobalFunction("void ConsolePrint(const wstring&in)", asFUNCTION(AE_Base::ConsolePrint), asCALL_CDECL);
	if (ret < 0)
	{
		return AEResult::RegGlobalFuncFail;
	}

	return AEResult::Ok;
}