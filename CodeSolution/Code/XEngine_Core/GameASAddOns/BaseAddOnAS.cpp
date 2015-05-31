/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

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

XEResult BaseAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (RegisterBasicEngineConst(engine) != XEResult::Ok)
	{
		XETODO("Add Log and return");
		std::wstring errMsg = L"";
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_CONSTS_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterXEResultEnum(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log and return");
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_ENUM_ERR_MSG"), __FUNCTIONW__, L"XEResult");
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterBasicEngineFuncs(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log and return");
		//fmt::format(m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_FUNCTS_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult BaseAddOnAS::RegisterBasicEngineConst(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterGlobalProperty("const wstring XE_VERSION", (void*)&XE_Base::XE_VERSION);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	ret = engine->RegisterGlobalProperty("const wstring XE_CODENAME", (void*)&XE_Base::XE_CODENAME);
	if (ret < 0)
	{
		return XEResult::RegGlobalPropFail;
	}

	return XEResult::Ok;
}

XEResult BaseAddOnAS::RegisterXEResultEnum(asIScriptEngine* engine)
{
	int ret = 0;

	///////////////////////
	//Register XEResult
	ret = engine->RegisterEnum("XEResult");
	if (ret < 0)
	{
		return XEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "Ok", static_cast<int32_t>(XEResult::Ok));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "Fail", static_cast<int32_t>(XEResult::Fail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "NotReady", static_cast<int32_t>(XEResult::NotReady));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GraphicDeviceNull", static_cast<int32_t>(XEResult::GraphicDeviceNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "NullParameter", static_cast<int32_t>(XEResult::NullParameter));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "EmptyFilename", static_cast<int32_t>(XEResult::EmptyFilename));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateDXDeviceFail", static_cast<int32_t>(XEResult::CreateDXDeviceFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateDXConfFail", static_cast<int32_t>(XEResult::CreateDXConfFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "FeatureLvlUnsupported", static_cast<int32_t>(XEResult::FeatureLvlUnsupported));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateDXSwapChainFail", static_cast<int32_t>(XEResult::CreateDXSwapChainFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateDXDefaultRTDSFail", static_cast<int32_t>(XEResult::CreateDXDefaultRTDSFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InitViewportFail", static_cast<int32_t>(XEResult::InitViewportFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegGlobalPropFail", static_cast<int32_t>(XEResult::RegGlobalPropFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegGlobalFuncFail", static_cast<int32_t>(XEResult::RegGlobalFuncFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegObjTypeFail", static_cast<int32_t>(XEResult::RegObjTypeFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegEnumValFail", static_cast<int32_t>(XEResult::RegEnumValFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegEnumFail", static_cast<int32_t>(XEResult::RegEnumFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegObjMethodFail", static_cast<int32_t>(XEResult::RegObjMethodFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegObjPropFail", static_cast<int32_t>(XEResult::RegObjPropFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegObjBehaviorFail", static_cast<int32_t>(XEResult::RegObjBehaviorFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RegInterfaceFail", static_cast<int32_t>(XEResult::RegInterfaceFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "OpenFileFail", static_cast<int32_t>(XEResult::OpenFileFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "NotFound", static_cast<int32_t>(XEResult::NotFound));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "XMLReadError", static_cast<int32_t>(XEResult::XMLReadError));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InitLoggerFail", static_cast<int32_t>(XEResult::InitLoggerFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "Log2FileActive", static_cast<int32_t>(XEResult::Log2FileActive));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASModuleNotFound", static_cast<int32_t>(XEResult::ASModuleNotFound));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASModuleAlreadyCreated", static_cast<int32_t>(XEResult::ASModuleAlreadyCreated));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASModuleCreateFail", static_cast<int32_t>(XEResult::ASModuleCreateFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASAddSecModuleFail", static_cast<int32_t>(XEResult::ASAddSecModuleFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASBuildModuleFail", static_cast<int32_t>(XEResult::ASBuildModuleFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASDiscardModuleFail", static_cast<int32_t>(XEResult::ASDiscardModuleFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASPrepareContextFailed", static_cast<int32_t>(XEResult::ASPrepareContextFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ASSetArgObjectFailed", static_cast<int32_t>(XEResult::ASSetArgObjectFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "KeyboardLockFailed", static_cast<int32_t>(XEResult::KeyboardLockFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "KeyboardUnLockFailed", static_cast<int32_t>(XEResult::KeyboardUnLockFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "StructuredBufferInitFailed", static_cast<int32_t>(XEResult::StructuredBufferInitFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "StructuredBufferSetDateFailed", static_cast<int32_t>(XEResult::StructuredBufferSetDateFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SimpleBufferInitFailed", static_cast<int32_t>(XEResult::SimpleBufferInitFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SimpleBufferSetDateFailed", static_cast<int32_t>(XEResult::SimpleBufferSetDateFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ConstantBufferInitFailed", static_cast<int32_t>(XEResult::ConstantBufferInitFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ConstantBufferSetDataFailed", static_cast<int32_t>(XEResult::ConstantBufferSetDataFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "VertexShaderLoadFailed", static_cast<int32_t>(XEResult::VertexShaderLoadFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "PixelShaderLoadFailed", static_cast<int32_t>(XEResult::PixelShaderLoadFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "HullShaderLoadFailed", static_cast<int32_t>(XEResult::HullShaderLoadFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "DomainShaderLoadFailed", static_cast<int32_t>(XEResult::DomainShaderLoadFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GeometryShaderLoadFailed", static_cast<int32_t>(XEResult::GeometryShaderLoadFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ComputeShaderLoadFailed", static_cast<int32_t>(XEResult::ComputeShaderLoadFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "EmptyString", static_cast<int32_t>(XEResult::EmptyString));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ObjExists", static_cast<int32_t>(XEResult::ObjExists));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidPermutation", static_cast<int32_t>(XEResult::InvalidPermutation));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetTextureFailed", static_cast<int32_t>(XEResult::SetTextureFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "NullObj", static_cast<int32_t>(XEResult::NullObj));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "OutsideRange", static_cast<int32_t>(XEResult::OutsideRange));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "LoadFileFailed", static_cast<int32_t>(XEResult::LoadFileFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessGraph", static_cast<int32_t>(XEResult::AssimpProcessGraph));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessMaterials", static_cast<int32_t>(XEResult::AssimpProcessMaterials));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessEmbeddedTextures", static_cast<int32_t>(XEResult::AssimpProcessEmbeddedTextures));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessMeshes", static_cast<int32_t>(XEResult::AssimpProcessMeshes));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessSkeleton", static_cast<int32_t>(XEResult::AssimpProcessSkeleton));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessAnimation", static_cast<int32_t>(XEResult::AssimpProcessAnimation));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "AssimpProcessConstructModel", static_cast<int32_t>(XEResult::AssimpProcessConstructModel));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "EmptySkeleton", static_cast<int32_t>(XEResult::EmptySkeleton));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "EmptyName", static_cast<int32_t>(XEResult::EmptyName));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GameResourceManagerNull", static_cast<int32_t>(XEResult::GameResourceManagerNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ResourceIsManaged", static_cast<int32_t>(XEResult::ResourceIsManaged));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ResourceManagedFailed", static_cast<int32_t>(XEResult::ResourceManagedFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ResourceManagedTypeError", static_cast<int32_t>(XEResult::ResourceManagedTypeError));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateSRViewFailed", static_cast<int32_t>(XEResult::CreateSRViewFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateRTViewFailed", static_cast<int32_t>(XEResult::CreateRTViewFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateTextureFailed", static_cast<int32_t>(XEResult::CreateTextureFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CreateDSViewFailed", static_cast<int32_t>(XEResult::CreateDSViewFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "MaximunItemType", static_cast<int32_t>(XEResult::MaximunItemType));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidObjType", static_cast<int32_t>(XEResult::InvalidObjType));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ZeroSize", static_cast<int32_t>(XEResult::ZeroSize));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidTextureType", static_cast<int32_t>(XEResult::InvalidTextureType));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidShaderType", static_cast<int32_t>(XEResult::InvalidShaderType));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetConstantBufferFailed", static_cast<int32_t>(XEResult::SetConstantBufferFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetSimpleBufferFailed", static_cast<int32_t>(XEResult::SetSimpleBufferFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetStructuredBufferFailed", static_cast<int32_t>(XEResult::SetStructuredBufferFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetVertexShaderFailed", static_cast<int32_t>(XEResult::SetVertexShaderFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetPixelShaderFailed", static_cast<int32_t>(XEResult::SetPixelShaderFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetGeometryShaderFailed", static_cast<int32_t>(XEResult::SetGeometryShaderFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetDomainShaderFailed", static_cast<int32_t>(XEResult::SetDomainShaderFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetHullShaderFailed", static_cast<int32_t>(XEResult::SetHullShaderFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetComputeShaderFailed", static_cast<int32_t>(XEResult::SetComputeShaderFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetVertexShaderPropsFailed", static_cast<int32_t>(XEResult::SetVertexShaderPropsFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetPixelShaderPropsFailed", static_cast<int32_t>(XEResult::SetPixelShaderPropsFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetGeometryShaderPropsFailed", static_cast<int32_t>(XEResult::SetGeometryShaderPropsFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetDomainShaderPropsFailed", static_cast<int32_t>(XEResult::SetDomainShaderPropsFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetHullShaderPropsFailed", static_cast<int32_t>(XEResult::SetHullShaderPropsFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "SetComputeShaderPropsFailed", static_cast<int32_t>(XEResult::SetComputeShaderPropsFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderTextureBindingInitFailed", static_cast<int32_t>(XEResult::ShaderTextureBindingInitFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderStructuredBufferInitFailed", static_cast<int32_t>(XEResult::ShaderStructuredBufferInitFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderSimpleBufferInitFailed", static_cast<int32_t>(XEResult::ShaderSimpleBufferInitFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "UnknownFileExtension", static_cast<int32_t>(XEResult::UnknownFileExtension));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "EmptyFileExtension", static_cast<int32_t>(XEResult::EmptyFileExtension));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GOCIsAssociated", static_cast<int32_t>(XEResult::GOCIsAssociated));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GOCIsNotAssociated", static_cast<int32_t>(XEResult::GOCIsNotAssociated));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GameObjectManagerNull", static_cast<int32_t>(XEResult::GameObjectManagerNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "LocalizationManagerNull", static_cast<int32_t>(XEResult::LocalizationManagerNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "LoggerNull", static_cast<int32_t>(XEResult::LoggerNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ImportFail", static_cast<int32_t>(XEResult::ImportFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidFileHeader", static_cast<int32_t>(XEResult::InvalidFileHeader));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidShaderModel", static_cast<int32_t>(XEResult::InvalidShaderModel));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderCompiledFail", static_cast<int32_t>(XEResult::ShaderCompiledFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderExtractSimpleBuffersFail", static_cast<int32_t>(XEResult::ShaderExtractSimpleBuffersFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderExtractStructuredBuffersFail", static_cast<int32_t>(XEResult::ShaderExtractStructuredBuffersFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderExtractConstantBuffersFail", static_cast<int32_t>(XEResult::ShaderExtractConstantBuffersFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderExtractTextureInputFail", static_cast<int32_t>(XEResult::ShaderExtractTextureInputFail));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderTypeMismatch", static_cast<int32_t>(XEResult::ShaderTypeMismatch));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "FileNotPresent", static_cast<int32_t>(XEResult::FileNotPresent));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GameAssetManagerNull", static_cast<int32_t>(XEResult::GameAssetManagerNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GameAssetNotLoaded", static_cast<int32_t>(XEResult::GameAssetNotLoaded));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GameAssetIDInUse", static_cast<int32_t>(XEResult::GameAssetIDInUse));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "GameAssetInvalidID", static_cast<int32_t>(XEResult::GameAssetInvalidID));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "RawGameAssetNotFound", static_cast<int32_t>(XEResult::RawGameAssetNotFound));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ShaderNull", static_cast<int32_t>(XEResult::ShaderNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidResourceMapType", static_cast<int32_t>(XEResult::InvalidResourceMapType));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "ResourceMapFailed", static_cast<int32_t>(XEResult::ResourceMapFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "InvalidIndexBufferType", static_cast<int32_t>(XEResult::InvalidIndexBufferType));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "FailIndexBufferUpdate", static_cast<int32_t>(XEResult::FailIndexBufferUpdate));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "FailIndexBufferInit", static_cast<int32_t>(XEResult::FailIndexBufferInit));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "FullContainer", static_cast<int32_t>(XEResult::FullContainer));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "LightManagerNull", static_cast<int32_t>(XEResult::LightManagerNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "CameraNull", static_cast<int32_t>(XEResult::CameraNull));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEResult", "WriteToFileFailed", static_cast<int32_t>(XEResult::WriteToFileFailed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	return XEResult::Ok;
}

XEResult BaseAddOnAS::RegisterBasicEngineFuncs(asIScriptEngine* engine)
{
	int ret = 0;

	ret = engine->RegisterGlobalFunction("void ConsolePrintLine(const wstring&in)", asFUNCTION(XE_Base::ConsolePrintLine), asCALL_CDECL);
	if (ret < 0)
	{
		return XEResult::RegGlobalFuncFail;
	}

	ret = engine->RegisterGlobalFunction("void ConsolePrint(const wstring&in)", asFUNCTION(XE_Base::ConsolePrint), asCALL_CDECL);
	if (ret < 0)
	{
		return XEResult::RegGlobalFuncFail;
	}

	return XEResult::Ok;
}