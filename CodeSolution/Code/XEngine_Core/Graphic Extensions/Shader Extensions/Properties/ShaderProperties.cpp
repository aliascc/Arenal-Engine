
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/7/2012
*
* Last Major Update: 11/7/2012
* 
* Desc:
*
*
* Copyright 2012
*
*********************************************************/


/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicDevice.h"
#include "Logger\Logger.h"
#include "Shaders\Shader.h"
#include "ShaderProperties.h"
#include "Textures\Texture.h"
#include "Textures\Texture2D.h"
#include "Base\BaseFunctions.h"
#include "GameAssets\GameAsset.h"
#include "Shaders\Variables\Sampler.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Shaders\Variables\TextureArray.h"
#include "GameAssets\Assets\TextureAsset.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Localization\LocalizationManager.h"
#include "Shaders\Variables\Texture2DArray.h"
#include "Shaders\Bindings\ShaderTextureBinding.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
ShaderProperties::ShaderProperties(ShaderType shaderType, GraphicDevice* graphicDevice)
	: m_ShaderType(shaderType)
	, m_GraphicDevice(graphicDevice)
{
	XEAssert(graphicDevice != nullptr);
}

ShaderProperties::~ShaderProperties()
{
	CleanUp();
}

void ShaderProperties::CleanUp()
{
	/////////////////////////////////////
	//Delete Constant Buffers
	for (size_t i = 0; i < m_ConstantBufferVector.size(); i++)
	{
		DeleteMem(m_ConstantBufferVector[i]);
	}
	m_ConstantBufferVector.clear();
	m_ConstantBufferMap.clear();

	/////////////////////////////////////
	//Delete Samplers
	for (size_t i = 0; i < m_SamplerVector.size(); i++)
	{
		DeleteMem(m_SamplerVector[i]);
	}
	m_SamplerVector.clear();
	m_SamplerMap.clear();

	/////////////////////////////////////
	//Delete Simple Buffers
	for (size_t i = 0; i < m_SimpleBufferVector.size(); i++)
	{
		DeleteMem(m_SimpleBufferVector[i]);
	}
	m_SimpleBufferVector.clear();
	m_SimpleBufferMap.clear();

	/////////////////////////////////////
	//Delete Structured Buffers
	for (size_t i = 0; i < m_StructuredBufferVector.size(); i++)
	{
		DeleteMem(m_StructuredBufferVector[i]);
	}
	m_StructuredBufferVector.clear();
	m_StructuredBufferMap.clear();

	/////////////////////////////////////
	//Delete Texture Arrays
	for (size_t i = 0; i < m_TextureArrayVector.size(); i++)
	{
		DeleteMem(m_TextureArrayVector[i]);
	}
	m_TextureArrayVector.clear();
	m_TextureArrayMap.clear();

	/////////////////////////////////////
	//Delete Texture Bindings
	for (size_t i = 0; i < m_TextureBindingVector.size(); i++)
	{
		DeleteMem(m_TextureBindingVector[i]);
	}
	m_TextureBindingVector.clear();
	m_TextureBindingMap.clear();

	/////////////////////////////////////
	//Remove Texture Asset Binding
	for(auto texturePairIt : m_TextureAssetMap)
	{
		RemoveTextureAssetBinding(texturePairIt.first);
	}
	m_TextureAssetMap.clear();
}

void ShaderProperties::TextureAssetDeletion(GameAsset* asset)
{
	XEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}

	XEAssert(asset->GetGameContentType() == GameContentType::Texture);
	if(asset->GetGameContentType() != GameContentType::Texture)
	{
		return;
	}

	std::wstring textureName = L"";

	for(auto texturePairIt : m_TextureAssetMap)
	{
		if(texturePairIt.second.m_AssetID == asset->GetUniqueAssetID())
		{
			textureName = texturePairIt.first;

			break;
		}
	}

	if(!textureName.empty())
	{
		RemoveTextureAssetBinding(textureName, false);
	}
}

XEResult ShaderProperties::AddTextureAssetBinding(const std::wstring& name, TextureAsset* asset)
{
	XEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return XEResult::NullParameter;
	}

	if(m_TextureAssetMap.find(name) == m_TextureAssetMap.end())
	{
		return XEResult::NotFound;
	}

	GameObjectAssetPair<Texture>& textureAssetPair = m_TextureAssetMap[name];

	if(textureAssetPair.m_ResourceAsset != nullptr)
	{
		XEResult ret = RemoveTextureAssetBinding(name);

		if(ret != XEResult::Ok)
		{
			XETODO("Log error");

			XETODO("Add better return code");
			return XEResult::Fail;
		}
	}

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&ShaderProperties::TextureAssetDeletion, this, std::placeholders::_1));
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	textureAssetPair.m_AssetID						= asset->GetUniqueAssetID();
	textureAssetPair.m_CallerID						= callerID;
	textureAssetPair.m_ResourceAsset				= asset->GetTextureReference();
	textureAssetPair.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();
	textureAssetPair.m_GameAsset					= asset;

	return XEResult::Ok;
}

XEResult ShaderProperties::RemoveTextureAssetBinding(const std::wstring& name, bool informGameAsset)
{
	if(m_TextureAssetMap.find(name) == m_TextureAssetMap.end())
	{
		return XEResult::NotFound;
	}

	GameObjectAssetPair<Texture>& textureAssetPair = m_TextureAssetMap[name];

	if(textureAssetPair.m_ResourceAsset != nullptr)
	{
		if(textureAssetPair.m_OnListenerObjDeletionEvent != nullptr && informGameAsset)
		{
			textureAssetPair.m_OnListenerObjDeletionEvent(textureAssetPair.m_CallerID);
		}

		textureAssetPair.m_AssetID						= 0;
		textureAssetPair.m_CallerID						= 0;
		textureAssetPair.m_OnListenerObjDeletionEvent	= nullptr;
		textureAssetPair.m_GameAsset					= nullptr;

		XERelease(textureAssetPair.m_ResourceAsset);
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::SetTextureToBinding(ShaderTextureBinding* stb, TextureAsset* asset)
{
	XEAssert(stb != nullptr);
	if(stb == nullptr)
	{
		return XEResult::NullParameter;
	}

	const std::wstring& textureName = stb->GetName();

	if(m_TextureAssetMap.find(textureName) == m_TextureAssetMap.end())
	{
		m_TextureAssetMap[textureName] = GameObjectAssetPair<Texture>();
	}

	Texture* texture = nullptr;
	if(asset != nullptr)
	{
		texture = asset->GetTextureReference();
	}

	XEResult ret = stb->SetTexture(texture);
	if(ret != XEResult::Ok)
	{
		XERelease(texture);

		return ret;
	}

	if(asset == nullptr)
	{
		return RemoveTextureAssetBinding(textureName);
	}
	else
	{
		return AddTextureAssetBinding(textureName, asset);
	}
}

XEResult ShaderProperties::SetTextureToBinding(ShaderTextureBinding* stb, Texture* texture)
{
	XEAssert(stb != nullptr);
	if(stb == nullptr)
	{
		return XEResult::NullParameter;
	}

	const std::wstring& textureName = stb->GetName();

	Texture* refTexture = reinterpret_cast<Texture*>(texture->AddRef());

	XEResult ret = stb->SetTexture(refTexture);
	if(ret != XEResult::Ok)
	{
		XERelease(refTexture);

		return ret;
	}

	////////////////////////////////////////////
	//If Texture Asset Existed, remove it
	auto textureAssetMapIt = m_TextureAssetMap.find(textureName);
	if(textureAssetMapIt != m_TextureAssetMap.end())
	{
		RemoveTextureAssetBinding(textureName);

		m_TextureAssetMap.erase(textureAssetMapIt);
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::SetTexture(const std::wstring& name, TextureAsset* asset)
{
	if(!ShaderTextureBindingExists(name))
	{
		return XEResult::NotFound;
	}

	ShaderTextureBinding* stb = m_TextureBindingMap[name];

	return SetTextureToBinding(stb, asset);
}

XEResult ShaderProperties::SetTexture(uint32_t index, TextureAsset* asset)
{
	if(index >= m_TextureBindingVector.size())
	{
		return XEResult::OutsideRange;
	}

	ShaderTextureBinding* stb = m_TextureBindingVector[index];

	return SetTextureToBinding(stb, asset);
}

XEResult ShaderProperties::SetTexture(const std::wstring& name, Texture* texture)
{
	if(!ShaderTextureBindingExists(name))
	{
		return XEResult::NotFound;
	}

	ShaderTextureBinding* stb = m_TextureBindingMap[name];

	return SetTextureToBinding(stb, texture);
}

XEResult ShaderProperties::CreateFromShaderSignatures(Shader* shader)
{
	/////////////////////////////////////////////////////
	//Pre-check variables
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEAssert(shader != nullptr);
	if(shader == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	/////////////////////////////////////////////////////
	//Clean Up Shader Properties for new Information
	CleanUp();

	/////////////////////////////////////////////////////
	//Add Constant Buffers from Signature List
	const ConstantBufferSignatureList& constBufferSigList = shader->GetConstantBufferSignatureList();
	ret = AddConstantBuffersFromSigList(constBufferSigList);
	if(ret != XEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_CB_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), XE_Base::GetXEResultString(ret));
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Samplers from Signature List
	const SamplerSignatureList& samplerSignatureList = shader->GetSamplerSignatureList();
	ret = AddSamplersFromSigList(samplerSignatureList);
	if (ret != XEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_SAM_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), XE_Base::GetXEResultString(ret));
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Simple Buffers from Signature List
	const SimpleBufferSignatureList& simpleBufferSignatureList = shader->GetSimpleBufferSignatureList();
	ret = AddSimpleBuffersFromSigList(simpleBufferSignatureList);
	if(ret != XEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_B_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), XE_Base::GetXEResultString(ret));
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Structured Buffers from Signature List
	const StructuredBufferSignatureList& structuredBufferSignatureList = shader->GetStructuredBufferSignatureList();
	ret = AddStructuredBuffersFromSigList(structuredBufferSignatureList);
	if(ret != XEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_SB_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), XE_Base::GetXEResultString(ret));
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Texture Inputs from Signature List
	const TextureInputSignatureList& textBindingSigList = shader->GetTextureInputSignatureList();
	ret = AddTextureBindingsFromSigList(textBindingSigList);
	if(ret != XEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_TB_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), XE_Base::GetXEResultString(ret));
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Texture Arrays Inputs from Signature List
	const TextureArrayInputSignatureList& textArrayBindingSigList = shader->GetTextureArrayInputSignatureList();
	ret = AddTextureArraysFromSigList(textArrayBindingSigList);
	if (ret != XEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = L"";
		fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_TA_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), XE_Base::GetXEResultString(ret));
		XELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Finish
	return XEResult::Ok;
}

XEResult ShaderProperties::AddConstantBuffersFromSigList(const ConstantBufferSignatureList& constBufferSigList)
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for(const ConstantBufferSignature& constBufferSig : constBufferSigList)
	{
		ConstantBuffer* cb = new ConstantBuffer(m_GraphicDevice, constBufferSig.m_BindIndex, constBufferSig.m_Name);

		for(const ShaderCustomVariable& constSCVar : constBufferSig.m_ShaderCustomVariableList)
		{
			ShaderCustomVariable* scVar = new ShaderCustomVariable(constSCVar);

			ret = cb->AddVariable(scVar);
			if(ret != XEResult::Ok)
			{
				DeleteMem(scVar);

				std::wstring msgerr = L"";
				fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_CB_SIG_ADD_VAR_DBG_MSG"), __FUNCTIONW__, constSCVar.m_Name, cb->GetName(), XE_Base::GetXEResultString(ret));
				XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

				break;
			}
		}

		if(ret != XEResult::Ok)
		{
			DeleteMem(cb);

			return ret;
		}

		ret = cb->Initialize();
		if(ret != XEResult::Ok)
		{
			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_CB_SIG_INIT_CB_DBG_MSG"), __FUNCTIONW__, cb->GetName(), XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret; 
		}

		ret = AddConstantBuffer(cb);
		if(ret != XEResult::Ok)
		{
			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_CB_SIG_ADD_CB_DBG_MSG"), __FUNCTIONW__, cb->GetName(), XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			DeleteMem(cb);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::AddSamplersFromSigList(const SamplerSignatureList& samplerSigList)
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for (const SamplerSignature& samplerSig : samplerSigList)
	{
		Sampler* sampler = new Sampler(m_GraphicDevice, samplerSig.m_Name, samplerSig.m_BindIndex);

		ret = sampler->Initialize();
		if (ret != XEResult::Ok)
		{
			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_SAM_SIG_INIT_CB_DBG_MSG"), __FUNCTIONW__, sampler->GetName(), XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}

		ret = AddSampler(sampler);
		if (ret != XEResult::Ok)
		{
			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_SAM_SIG_ADD_CB_DBG_MSG"), __FUNCTIONW__, sampler->GetName(), XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			DeleteMem(sampler);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::AddSimpleBuffersFromSigList(const SimpleBufferSignatureList& simpleBufferSignatureList)
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for(const SimpleBufferSignature& simpleBufferSig : simpleBufferSignatureList)
	{
		SimpleBuffer* simpleBuffer = new SimpleBuffer(simpleBufferSig.m_Name, simpleBufferSig.m_BindIndex, simpleBufferSig.m_IsRW, simpleBufferSig.m_VariableClass, simpleBufferSig.m_VariableType, simpleBufferSig.m_ElementCount, m_GraphicDevice);

		ret = AddSimpleBuffer(simpleBuffer);

		if(ret != XEResult::Ok)
		{
			DeleteMem(simpleBuffer);

			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_B_SIG_ADD_B_DBG_MSG"), __FUNCTIONW__, simpleBufferSig.m_Name, XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::AddStructuredBuffersFromSigList(const StructuredBufferSignatureList& structuredBufferSigList)
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for(const StructuredBufferSignature& structuredBufferSig : structuredBufferSigList)
	{
		StructuredBuffer* structuredBuffer = new StructuredBuffer(structuredBufferSig.m_Name, structuredBufferSig.m_BindIndex, structuredBufferSig.m_IsRW, m_GraphicDevice);

		ret = AddStructuredBuffer(structuredBuffer);

		if(ret != XEResult::Ok)
		{
			DeleteMem(structuredBuffer);

			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_SB_SIG_ADD_SB_DBG_MSG"), __FUNCTIONW__, structuredBufferSig.m_Name, XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::AddTextureBindingsFromSigList(const TextureInputSignatureList& textBindingSigList)
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for(const TextureInputSignature& textBindingSig : textBindingSigList)
	{
		ShaderTextureBinding* stb = new ShaderTextureBinding(textBindingSig.m_Name, textBindingSig.m_BindIndex, textBindingSig.m_TextureType, nullptr);

		ret = AddShaderTextureBinding(stb);

		if(ret != XEResult::Ok)
		{
			DeleteMem(stb);

			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_TB_SIG_ADD_TB_DBG_MSG"), __FUNCTIONW__, textBindingSig.m_Name, XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::AddTextureArraysFromSigList(const TextureArrayInputSignatureList& textArrayInputSigList)
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for (const TextureArrayInputSignature& textArrayInput : textArrayInputSigList)
	{
		TextureArray* textureArray = nullptr;

		switch (textArrayInput.m_TextureType)
		{
			case TextureType::Texture2D:
				textureArray = new Texture2DArray(m_GraphicDevice, textArrayInput.m_Name, textArrayInput.m_BindIndex);
				break;

			default:
				XEAssert(false);
				ret = XEResult::InvalidTextureType;
				break;
		}

		if (textureArray != nullptr)
		{
			ret = AddTextureArray(textureArray);
		}

		if (ret != XEResult::Ok)
		{
			DeleteMem(textureArray);

			std::wstring msgerr = L"";
			fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_TA_SIG_ADD_SB_DBG_MSG"), __FUNCTIONW__, textArrayInput.m_Name, XE_Base::GetXEResultString(ret));
			XELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::AddConstantBuffer(ConstantBuffer* cb)
{
	XEAssert(cb != nullptr);
	if(cb == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!cb->GetName().empty());
	if(cb->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if(ConstantBufferExists(cb->GetName()))
	{
		return XEResult::ObjExists;
	}

	m_ConstantBufferMap[cb->GetName()] = cb;
	m_ConstantBufferVector.push_back(cb);

	return XEResult::Ok;
}

XEResult ShaderProperties::AddSampler(Sampler* sampler)
{
	XEAssert(sampler != nullptr);
	if (sampler == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!sampler->GetName().empty());
	if (sampler->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if (SamplerExists(sampler->GetName()))
	{
		return XEResult::ObjExists;
	}

	ShaderPropertyObject<Sampler>* spo = new ShaderPropertyObject<Sampler>();
	spo->m_Object = sampler;

	m_SamplerMap[sampler->GetName()] = spo;
	m_SamplerVector.push_back(spo);

	return XEResult::Ok;
}

XEResult ShaderProperties::RemoveSampler(const std::wstring& name)
{
	if (!SamplerExists(name))
	{
		return XEResult::NotFound;
	}

	auto samplerIt = m_SamplerMap.find(name);

	ShaderPropertyObject<Sampler>* currentSpo = samplerIt->second;

	auto samplerVecIt = m_SamplerVector.begin();
	auto samplerEndVecIt = m_SamplerVector.end();
	for (; samplerVecIt != samplerEndVecIt; samplerVecIt++)
	{
		ShaderPropertyObject<Sampler>* vecSpo = (*samplerVecIt);
		if (vecSpo == currentSpo)
		{
			m_SamplerVector.erase(samplerVecIt);
			break;
		}
	}

	m_SamplerMap.erase(samplerIt);

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::OverrideSampler(const std::wstring& name, Sampler* sampler, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	XEAssert(sampler != nullptr);
	if (sampler == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!name.empty());
	if (name.empty())
	{
		return XEResult::EmptyName;
	}

	if (!SamplerExists(name))
	{
		return XEResult::NotFound;
	}

	ShaderPropertyObject<Sampler>* spo = new ShaderPropertyObject<Sampler>();

	spo->m_Object				= sampler;
	spo->m_IsOverwriten			= true;
	spo->m_IsOwn				= deleteAtShaderDestroy;
	spo->m_IsOverrideBindIndex	= overrideBindIndex;
	spo->m_BindIndex			= bindIndex;

	ShaderPropertyObject<Sampler>* currentSpo = m_SamplerMap[name];

	for (size_t i = 0; i < m_SamplerVector.size(); i++)
	{
		if (m_SamplerVector[i] == currentSpo)
		{
			m_SamplerVector[i] = spo;
			i = m_SamplerVector.size();
		}
	}

	m_SamplerMap[name] = spo;

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::AddSimpleBuffer(SimpleBuffer* simpleBuffer)
{
	XEAssert(simpleBuffer != nullptr);
	if(simpleBuffer == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!simpleBuffer->GetName().empty());
	if(simpleBuffer->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if(SimpleBufferExists(simpleBuffer->GetName()))
	{
		return XEResult::ObjExists;
	}

	ShaderPropertyObject<SimpleBuffer>* spo = new ShaderPropertyObject<SimpleBuffer>();
	spo->m_Object = simpleBuffer;

	m_SimpleBufferMap[simpleBuffer->GetName()] = spo;
	m_SimpleBufferVector.push_back(spo);

	return XEResult::Ok;
}

XEResult ShaderProperties::RemoveSimpleBuffer(const std::wstring& name)
{
	if(!SimpleBufferExists(name))
	{
		return XEResult::NotFound;
	}

	auto simpleBufferIt = m_SimpleBufferMap.find(name);

	ShaderPropertyObject<SimpleBuffer>* currentSpo = simpleBufferIt->second;

	auto bufferIt = m_SimpleBufferVector.begin();
	auto bufferEndIt = m_SimpleBufferVector.end();
	for (; bufferIt != bufferEndIt; bufferIt++)
	{
		ShaderPropertyObject<SimpleBuffer>* vecSpo = (*bufferIt);
		if(vecSpo == currentSpo)
		{
			m_SimpleBufferVector.erase(bufferIt);
			break;
		}
	}

	m_SimpleBufferMap.erase(simpleBufferIt);

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::OverrideSimpleBuffer(const std::wstring& name, SimpleBuffer* simpleBuffer, bool overwriteRW, bool setRW, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	XEAssert(simpleBuffer != nullptr);
	if(simpleBuffer == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!name.empty());
	if(name.empty())
	{
		return XEResult::EmptyName;
	}

	if(!SimpleBufferExists(name))
	{
		return XEResult::NotFound;
	}

	ShaderPropertyObject<SimpleBuffer>* spo = new ShaderPropertyObject<SimpleBuffer>();

	spo->m_Object					= simpleBuffer;
	spo->m_IsOverwriten				= true;
	spo->m_IsOverrideRW				= overwriteRW;
	spo->m_IsOwn					= deleteAtShaderDestroy;
	spo->m_SetRW					= setRW;
	spo->m_IsOverrideBindIndex		= overrideBindIndex;
	spo->m_BindIndex				= bindIndex;

	ShaderPropertyObject<SimpleBuffer>* currentSpo = m_SimpleBufferMap[name];

	uint32_t numSB = static_cast<uint32_t>(m_SimpleBufferVector.size());
	for (uint32_t i = 0; i < numSB; i++)
	{
		if(m_SimpleBufferVector[i] == currentSpo)
		{
			m_SimpleBufferVector[i] = spo;
			i = numSB;
		}
	}

	m_SimpleBufferMap[name] = spo;

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::AddStructuredBuffer(StructuredBuffer* structuredBuffer)
{
	XEAssert(structuredBuffer != nullptr);
	if(structuredBuffer == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!structuredBuffer->GetName().empty());
	if(structuredBuffer->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if(StructuredBufferExists(structuredBuffer->GetName()))
	{
		return XEResult::ObjExists;
	}

	ShaderPropertyObject<StructuredBuffer>* spo = new ShaderPropertyObject<StructuredBuffer>();
	spo->m_Object = structuredBuffer;

	m_StructuredBufferMap[structuredBuffer->GetName()] = spo;
	m_StructuredBufferVector.push_back(spo);

	return XEResult::Ok;
}

XEResult ShaderProperties::RemoveStructuredBuffer(const std::wstring& name)
{
	if(!StructuredBufferExists(name))
	{
		return XEResult::NotFound;
	}

	auto structuredBufferIt = m_StructuredBufferMap.find(name);

	ShaderPropertyObject<StructuredBuffer>* currentSpo = structuredBufferIt->second;

	auto bufferIt = m_StructuredBufferVector.begin();
	auto bufferEndIt = m_StructuredBufferVector.end();
	for (; bufferIt != bufferEndIt; bufferIt++)
	{
		ShaderPropertyObject<StructuredBuffer>* vecSpo = (*bufferIt);
		if(vecSpo == currentSpo)
		{
			m_StructuredBufferVector.erase(bufferIt);
			break;
		}

	}

	m_StructuredBufferMap.erase(structuredBufferIt);

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::OverrideStructuredBuffer(const std::wstring& name, StructuredBuffer* structuredBuffer, bool overwriteRW, bool setRW, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	XEAssert(structuredBuffer != nullptr);
	if(structuredBuffer == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!name.empty());
	if(name.empty())
	{
		return XEResult::EmptyName;
	}

	if(!StructuredBufferExists(name))
	{
		return XEResult::NotFound;
	}

	ShaderPropertyObject<StructuredBuffer>* spo = new ShaderPropertyObject<StructuredBuffer>();
	spo->m_Object = structuredBuffer;

	spo->m_IsOverwriten				= true;
	spo->m_IsOverrideRW				= overwriteRW;
	spo->m_IsOwn					= deleteAtShaderDestroy;
	spo->m_SetRW					= setRW;
	spo->m_IsOverrideBindIndex		= overrideBindIndex;
	spo->m_BindIndex				= bindIndex;

	ShaderPropertyObject<StructuredBuffer>* currentSpo = m_StructuredBufferMap[name];

	uint32_t numSB = static_cast<uint32_t>(m_StructuredBufferVector.size());
	for (uint32_t i = 0; i < numSB; i++)
	{
		if(m_StructuredBufferVector[i] == currentSpo)
		{
			m_StructuredBufferVector[i] = spo;
			i = numSB;
		}
	}

	m_StructuredBufferMap[name] = spo;

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::AddTextureArray(TextureArray* textureArray)
{
	XEAssert(textureArray != nullptr);
	if (textureArray == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!textureArray->GetName().empty());
	if (textureArray->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if (TextureArrayExists(textureArray->GetName()))
	{
		return XEResult::ObjExists;
	}

	ShaderPropertyObject<TextureArray>* spo = new ShaderPropertyObject<TextureArray>();
	spo->m_Object = textureArray;

	m_TextureArrayMap[textureArray->GetName()] = spo;
	m_TextureArrayVector.push_back(spo);

	return XEResult::Ok;
}

XEResult ShaderProperties::RemoveTextureArray(const std::wstring& name)
{
	if (!StructuredBufferExists(name))
	{
		return XEResult::NotFound;
	}

	auto textureArrayIt = m_TextureArrayMap.find(name);

	ShaderPropertyObject<TextureArray>* currentSpo = textureArrayIt->second;

	auto taIt = m_TextureArrayVector.begin();
	auto taEndIt = m_TextureArrayVector.end();
	for (; taIt != taEndIt; taIt++)
	{
		ShaderPropertyObject<TextureArray>* vecSpo = (*taIt);
		if (vecSpo == currentSpo)
		{
			m_TextureArrayVector.erase(taIt);
			break;
		}

	}

	m_TextureArrayMap.erase(textureArrayIt);

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::OverrideTextureArray(const std::wstring& name, TextureArray* textureArray, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	XEAssert(textureArray != nullptr);
	if (textureArray == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!name.empty());
	if (name.empty())
	{
		return XEResult::EmptyName;
	}

	if (!TextureArrayExists(name))
	{
		return XEResult::NotFound;
	}

	ShaderPropertyObject<TextureArray>* spo = new ShaderPropertyObject<TextureArray>();
	spo->m_Object = textureArray;

	spo->m_IsOverwriten = true;
	spo->m_IsOwn = deleteAtShaderDestroy;
	spo->m_IsOverrideBindIndex = overrideBindIndex;
	spo->m_BindIndex = bindIndex;

	ShaderPropertyObject<TextureArray>* currentSpo = m_TextureArrayMap[name];

	for (size_t i = 0; i < m_TextureArrayVector.size(); i++)
	{
		if (m_TextureArrayVector[i] == currentSpo)
		{
			m_TextureArrayVector[i] = spo;
			i = m_TextureArrayVector.size();
		}
	}

	m_TextureArrayMap[name] = spo;

	DeleteMem(currentSpo);

	return XEResult::Ok;
}

XEResult ShaderProperties::AddShaderTextureBinding(ShaderTextureBinding* stb)
{
	XEAssert(stb != nullptr);
	if(stb == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEAssert(!stb->GetName().empty());
	if(stb->GetName().empty())
	{
		return XEResult::EmptyName;
	}

	if(ShaderTextureBindingExists(stb->GetName()))
	{
		return XEResult::ObjExists;
	}

	m_TextureBindingMap[stb->GetName()] = stb;
	m_TextureBindingVector.push_back(stb);

	return XEResult::Ok;
}

bool ShaderProperties::ConstantBufferExists(const std::wstring& name) const
{
	return (m_ConstantBufferMap.find(name) != m_ConstantBufferMap.end());
}

bool ShaderProperties::SamplerExists(const std::wstring& name) const
{
	return (m_SamplerMap.find(name) != m_SamplerMap.end());
}

bool ShaderProperties::SimpleBufferExists(const std::wstring& name) const
{
	return (m_SimpleBufferMap.find(name) != m_SimpleBufferMap.end());
}

bool ShaderProperties::StructuredBufferExists(const std::wstring& name) const
{
	return (m_StructuredBufferMap.find(name) != m_StructuredBufferMap.end());
}

bool ShaderProperties::ShaderTextureBindingExists(const std::wstring& name) const
{
	return (m_TextureBindingMap.find(name) != m_TextureBindingMap.end());
}

bool ShaderProperties::TextureArrayExists(const std::wstring& name) const
{
	return (m_TextureArrayMap.find(name) != m_TextureArrayMap.end());
}

ConstantBuffer* ShaderProperties::GetConstantBuffer(const std::wstring& name)
{
	if(!ConstantBufferExists(name))
	{
		return nullptr;
	}

	return m_ConstantBufferMap[name];
}

ConstantBuffer* ShaderProperties::GetConstantBuffer(uint32_t index)
{
	if(index >= static_cast<uint32_t>(m_ConstantBufferVector.size()))
	{
		return nullptr;
	}

	return m_ConstantBufferVector[index];
}

Sampler* ShaderProperties::GetSampler(const std::wstring& name)
{
	if (!SamplerExists(name))
	{
		return nullptr;
	}

	return m_SamplerMap[name]->m_Object;
}

Sampler* ShaderProperties::GetSampler(uint32_t index)
{
	if (index >= static_cast<uint32_t>(m_SamplerVector.size()))
	{
		return nullptr;
	}

	return m_SamplerVector[index]->m_Object;
}

SimpleBuffer* ShaderProperties::GetSimpleBuffer(const std::wstring& name)
{
	if(!SimpleBufferExists(name))
	{
		return nullptr;
	}

	return m_SimpleBufferMap[name]->m_Object;
}

SimpleBuffer* ShaderProperties::GetSimpleBuffer(uint32_t index)
{
	if(index >= static_cast<uint32_t>(m_SimpleBufferVector.size()))
	{
		return nullptr;
	}

	return m_SimpleBufferVector[index]->m_Object;
}

StructuredBuffer* ShaderProperties::GetStructuredBuffer(const std::wstring& name)
{
	if(!StructuredBufferExists(name))
	{
		return nullptr;
	}

	return m_StructuredBufferMap[name]->m_Object;
}

StructuredBuffer* ShaderProperties::GetStructuredBuffer(uint32_t index)
{
	if(index >= static_cast<uint32_t>(m_StructuredBufferVector.size()))
	{
		return nullptr;
	}

	return m_StructuredBufferVector[index]->m_Object;
}

const ShaderTextureBinding* ShaderProperties::GetShaderTextureBinding(const std::wstring& name) const
{
	if(!ShaderTextureBindingExists(name))
	{
		return nullptr;
	}

	return m_TextureBindingMap.find(name)->second;
}

const ShaderTextureBinding* ShaderProperties::GetShaderTextureBinding(uint32_t index) const
{
	if(index >= static_cast<uint32_t>(m_TextureBindingVector.size()))
	{
		return nullptr;
	}

	return m_TextureBindingVector[index];
}

TextureArray* ShaderProperties::GetTextureArray(const std::wstring& name)
{
	if (!TextureArrayExists(name))
	{
		return nullptr;
	}

	return m_TextureArrayMap[name]->m_Object;
}

TextureArray* ShaderProperties::GetTextureArray(uint32_t index)
{
	if (index >= static_cast<uint32_t>(m_TextureArrayVector.size()))
	{
		return nullptr;
	}

	return m_TextureArrayVector[index]->m_Object;
}

XEResult ShaderProperties::CopyVariableData(const ShaderProperties* otherShaderProps)
{
	//////////////////////////////////////////////////
	//Check Parameters
	XEAssert(otherShaderProps != nullptr);
	if(otherShaderProps == nullptr)
	{
		return XEResult::NullParameter;
	}

	XEResult ret = XEResult::Ok;

	//////////////////////////////////////////////////
	//Copy Constant Buffers from incoming Shader Props
	for(auto cbIt : otherShaderProps->m_ConstantBufferMap)
	{
		//////////////////////////////////////////////////
		//Get Constant Buffer from Other Property
		const ConstantBuffer* otherCB = cbIt.second;

		//////////////////////////////////////////////////
		//Verify that the old CB exists in this Property
		if(ConstantBufferExists(otherCB->GetName()))
		{
			//////////////////////////////////////////////////
			//Get Constant Buffer
			ConstantBuffer* thisCB = m_ConstantBufferMap[otherCB->GetName()];

			//////////////////////////////////////////////////
			//Copy variable data to this Prop CB
			uint32_t otherVarCount = otherCB->GetNumberOfVariables();
			for(uint32_t i = 0; i < otherVarCount; i++)
			{
				const ShaderCustomVariable* otherShaderVar = otherCB->GetVariable(i);
				
				const ShaderCustomVariable* thisShaderVar = thisCB->GetVariable(otherShaderVar->m_Name);

				//////////////////////////////////////////////////
				//Compare Variables and copy data if they are the same
				if(thisShaderVar->Compare(otherShaderVar))
				{
					ret = thisCB->SetValue(thisShaderVar->m_Name, otherShaderVar->m_Memory, thisShaderVar->m_Size);

					if(ret != XEResult::Ok)
					{
						std::wstring msgerr = L"";
						fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_CB_SET_VALUE_ERR_MSG"), __FUNCTIONW__, thisShaderVar->m_Name, thisCB->GetName(), XE_Base::GetXEResultString(ret));
						XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////
	//Copy Textures Binding Data
	for(auto tbIt : otherShaderProps->m_TextureBindingMap)
	{
		//////////////////////////////////////////////////
		//If Texture Binding Exists copy data
		const ShaderTextureBinding* otherTextBinding = tbIt.second;
		if(ShaderTextureBindingExists(otherTextBinding->GetName()))
		{
			//////////////////////////////////////////////////
			//If Texture Binding is the same copy data
			ShaderTextureBinding* thisTextBinding = m_TextureBindingMap[otherTextBinding->GetName()];
			if(thisTextBinding->Compare(otherTextBinding))
			{
				//////////////////////////////////////////////////
				//If Texture exists copy data to this Prop
				if(otherTextBinding->GetTexture() != nullptr)
				{
					//////////////////////////////////////////////////
					//If Texture exists copy data to this Prop
					Texture* reference = reinterpret_cast<Texture*>(otherTextBinding->GetTexture()->AddRef());
					ret = thisTextBinding->SetTexture(reference);

					if(ret != XEResult::Ok)
					{
						XERelease(reference);

						std::wstring msgerr = L"";
						fastformat::fmt(msgerr, XELOCMAN->GetLiteral(L"SHADER_TEX_BINDING_SET_ERR_MSG"), __FUNCTIONW__, thisTextBinding->GetName(), XE_Base::GetXEResultString(ret));
						XELOGGER->AddNewLog(LogLevel::Warning, msgerr);

						continue;
					}

					//////////////////////////////////////////////////
					//If Texture comes from Texture Asset Copy information
					auto otherTexturePairIt = otherShaderProps->m_TextureAssetMap.find(thisTextBinding->GetName());
					if(otherTexturePairIt != otherShaderProps->m_TextureAssetMap.end())
					{
						const GameObjectAssetPair<Texture>& otherTexturePair = otherTexturePairIt->second;
						ret = CopyTexturePairData(thisTextBinding->GetName(), otherTexturePair);

						if(ret != XEResult::Ok)
						{
							thisTextBinding->SetTexture(nullptr);

							XETODO("Log error, can not copy object pair");
						}
					}
					else
					{
						XETODO("Log warning, texture did not contain object pair");
					}
				}
			}
		}
	}

	XETODO("Add sampler to copy");

	return XEResult::Ok;
}

XEResult ShaderProperties::CopyTexturePairData(const std::wstring& textureName, const GameObjectAssetPair<Texture>& otherTexturePair)
{
	if(m_TextureAssetMap.find(textureName) == m_TextureAssetMap.end())
	{
		m_TextureAssetMap[textureName] = GameObjectAssetPair<Texture>();
	}

	XEAssert(otherTexturePair.m_GameAsset != nullptr);
	XEAssert(otherTexturePair.m_ResourceAsset != nullptr);
	if(otherTexturePair.m_GameAsset == nullptr || otherTexturePair.m_ResourceAsset == nullptr)
	{
		return XEResult::NullObj;
	}

	GameObjectAssetPair<Texture>& textureAssetPair = m_TextureAssetMap[textureName];

	uint64_t callerID = XE_Base::GetNextUniqueID();

	XEResult ret = otherTexturePair.m_GameAsset->RegisterEventHandlers(callerID, std::bind(&ShaderProperties::TextureAssetDeletion, this, std::placeholders::_1));
	if(ret != XEResult::Ok)
	{
		XETODO("Log error");

		return ret;
	}

	textureAssetPair.m_AssetID						= otherTexturePair.m_AssetID;
	textureAssetPair.m_CallerID						= callerID;
	textureAssetPair.m_ResourceAsset				= reinterpret_cast<Texture*>(otherTexturePair.m_ResourceAsset->AddRef());
	textureAssetPair.m_OnListenerObjDeletionEvent	= otherTexturePair.m_GameAsset->GetOnListenerObjDeletionEventHandler();
	textureAssetPair.m_GameAsset					= otherTexturePair.m_GameAsset;

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplyAll()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	if(ApplyConstantBuffers() != XEResult::Ok)
	{
		return XEResult::SetConstantBufferFailed;
	}

	if (ApplySamplers() != XEResult::Ok)
	{
		XETODO("Change return value");
		return XEResult::Fail;
	}

	if(ApplyTextureBindings() != XEResult::Ok)
	{
		return XEResult::SetTextureFailed;
	}

	if (ApplyTextureArrays() != XEResult::Ok)
	{
		XETODO("Change return value");
		return XEResult::SetTextureFailed;
	}

	if(ApplySimpleBuffers() != XEResult::Ok)
	{
		return XEResult::SetSimpleBufferFailed;
	}

	if(ApplyStructuredBuffers() != XEResult::Ok)
	{
		return XEResult::SetStructuredBufferFailed;
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplyConstantBuffers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto cbIt : m_ConstantBufferMap)
	{
		ConstantBuffer* cb = cbIt.second;

		XEResult ret = XEResult::Ok;

		ret = cb->Apply();

		if(ret != XEResult::Ok)
		{
			XETODO("Set debug message");
			return ret;
		}

		ret = m_GraphicDevice->SetConstantBuffer(m_ShaderType, cb);

		if(ret != XEResult::Ok)
		{
			XETODO("Set debug message");
			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplySamplers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for (auto samplerIt : m_SamplerMap)
	{
		XEResult ret = XEResult::Ok;

		Sampler* sampler = samplerIt.second->m_Object;

		if (sampler->NeedsReinit())
		{
			ret = sampler->Reinitialize();

			if (ret != XEResult::Ok)
			{
				XETODO("Log error");
			}
		}

		if (sampler->IsReady())
		{
			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = samplerIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = sampler->GetBindIndex();

			if (overrideBindIndex)
			{
				bindIndex = samplerIt.second->m_BindIndex;
			}

			ret = m_GraphicDevice->SetSampler(m_ShaderType, sampler, overrideBindIndex, bindIndex);

			if (ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplySimpleBuffers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto simpleBufferIt : m_SimpleBufferMap)
	{
		SimpleBuffer* simpleBuffer = simpleBufferIt.second->m_Object;

		if(simpleBuffer->IsReady())
		{
			XEResult ret = XEResult::Ok;

			//////////////////////////////////////////////////
			//Get RW Enabled
			bool overrideRW = simpleBufferIt.second->m_IsOverrideRW;
			bool rwEnabled = simpleBuffer->IsRWEnabled();

			if(overrideRW)
			{
				rwEnabled = simpleBufferIt.second->m_SetRW;
			}

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = simpleBufferIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = simpleBuffer->GetBindIndex();

			if(overrideBindIndex)
			{
				bindIndex = simpleBufferIt.second->m_BindIndex;
			}

			//////////////////////////////////////////////////
			//Set Shader Buffer
			if(rwEnabled)
			{
				if(m_ShaderType == ShaderType::ComputeShader)
				{
					ret = m_GraphicDevice->SetShaderRWBufferToCS(simpleBuffer, overrideBindIndex, bindIndex);
				}
				else
				{
					XETODO("need to check if we would ever get here");
					XEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, simpleBuffer, overrideBindIndex, bindIndex);
			}

			if(ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplyStructuredBuffers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto structuredBufferIt : m_StructuredBufferMap)
	{
		StructuredBuffer* structuredBuffer = structuredBufferIt.second->m_Object;

		if(structuredBuffer->IsReady())
		{
			XEResult ret = XEResult::Ok;

			//////////////////////////////////////////////////
			//Get RW Enabled
			bool overrideRW = structuredBufferIt.second->m_IsOverrideRW;
			bool rwEnabled = structuredBuffer->IsRWEnabled();

			if(overrideRW)
			{
				rwEnabled = structuredBufferIt.second->m_SetRW;
			}

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = structuredBufferIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = structuredBuffer->GetBindIndex();

			if(overrideBindIndex)
			{
				bindIndex = structuredBufferIt.second->m_BindIndex;
			}

			//////////////////////////////////////////////////
			//Set Shader Buffer
			if(rwEnabled)
			{
				if(m_ShaderType == ShaderType::ComputeShader)
				{
					ret = m_GraphicDevice->SetShaderRWBufferToCS(structuredBuffer, overrideBindIndex, bindIndex);
				}
				else
				{
					XETODO("need to check if we would ever get here");
					XEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, structuredBuffer, overrideBindIndex, bindIndex);
			}

			if(ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplyTextureBindings()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto stbIt : m_TextureBindingMap)
	{
		ShaderTextureBinding* stb = stbIt.second;

		Texture* textureBinding = stb->GetTexture();
		if (textureBinding == nullptr)
		{
			textureBinding = m_GraphicDevice->GetDefaultTexture2D();
		}

		XEResult ret = m_GraphicDevice->SetTexture(m_ShaderType, stb->GetBindIndex(), textureBinding);
		if (ret != XEResult::Ok)
		{
			XETODO("Set debug message");
			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::ApplyTextureArrays()
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for (auto textureArrayIt : m_TextureArrayMap)
	{
		TextureArray* textureArray = textureArrayIt.second->m_Object;

		if (textureArray->IsReady())
		{
			XEResult ret = XEResult::Ok;

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = textureArrayIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = textureArray->GetBindIndex();

			if (overrideBindIndex)
			{
				bindIndex = textureArrayIt.second->m_BindIndex;
			}

			ret = m_GraphicDevice->SetTextureArray(m_ShaderType, bindIndex, textureArray);

			if (ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplyAll()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	if(UnApplyConstantBuffers() != XEResult::Ok)
	{
		return XEResult::SetConstantBufferFailed;
	}

	if (UnApplySamplers() != XEResult::Ok)
	{
		XETODO("Change return value");
		return XEResult::Fail;
	}

	if(UnApplyTextureBindings() != XEResult::Ok)
	{
		return XEResult::SetTextureFailed;
	}

	if (UnApplyTextureArrays() != XEResult::Ok)
	{
		XETODO("Change return value");
		return XEResult::SetTextureFailed;
	}

	if(UnApplySimpleBuffers() != XEResult::Ok)
	{
		return XEResult::SetSimpleBufferFailed;
	}

	if(UnApplyStructuredBuffers() != XEResult::Ok)
	{
		return XEResult::SetStructuredBufferFailed;
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplyConstantBuffers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	XEResult ret = XEResult::Ok;

	for(auto cbIt : m_ConstantBufferMap)
	{
		ConstantBuffer* cb = cbIt.second;

		ret = m_GraphicDevice->SetConstantBuffer(m_ShaderType, nullptr, true, cb->GetBindIndex());

		if(ret != XEResult::Ok)
		{
			XETODO("Set debug message");
			return ret;
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplySamplers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for (auto samplerIt : m_SamplerMap)
	{
		XEResult ret = XEResult::Ok;

		Sampler* sampler = samplerIt.second->m_Object;

		if (sampler->IsReady())
		{
			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = samplerIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = sampler->GetBindIndex();

			if (overrideBindIndex)
			{
				bindIndex = samplerIt.second->m_BindIndex;
			}

			ret = m_GraphicDevice->SetSampler(m_ShaderType, nullptr, overrideBindIndex, bindIndex);

			if (ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplySimpleBuffers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto simpleBufferIt : m_SimpleBufferMap)
	{
		SimpleBuffer* simpleBuffer = simpleBufferIt.second->m_Object;

		if(simpleBuffer->IsReady())
		{
			XEResult ret = XEResult::Ok;

			//////////////////////////////////////////////////
			//Get RW Enabled
			bool overrideRW = simpleBufferIt.second->m_IsOverrideRW;
			bool rwEnabled = simpleBuffer->IsRWEnabled();

			if(overrideRW)
			{
				rwEnabled = simpleBufferIt.second->m_SetRW;
			}

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = simpleBufferIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = simpleBuffer->GetBindIndex();

			if(overrideBindIndex)
			{
				bindIndex = simpleBufferIt.second->m_BindIndex;
			}

			//////////////////////////////////////////////////
			//Set Shader Buffer
			if(rwEnabled)
			{
				if(m_ShaderType == ShaderType::ComputeShader)
				{
					ret = m_GraphicDevice->SetShaderRWBufferToCS(nullptr, overrideBindIndex, bindIndex);
				}
				else
				{
					XETODO("need to check if we would ever get here");
					XEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, nullptr, overrideBindIndex, bindIndex);
			}

			if(ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplyStructuredBuffers()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto structuredBufferIt : m_StructuredBufferMap)
	{
		StructuredBuffer* structuredBuffer = structuredBufferIt.second->m_Object;

		if(structuredBuffer->IsReady())
		{
			XEResult ret = XEResult::Ok;

			//////////////////////////////////////////////////
			//Get RW Enabled
			bool overrideRW = structuredBufferIt.second->m_IsOverrideRW;
			bool rwEnabled = structuredBuffer->IsRWEnabled();

			if(overrideRW)
			{
				rwEnabled = structuredBufferIt.second->m_SetRW;
			}

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = structuredBufferIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = structuredBuffer->GetBindIndex();

			if(overrideBindIndex)
			{
				bindIndex = structuredBufferIt.second->m_BindIndex;
			}

			//////////////////////////////////////////////////
			//Set Shader Buffer
			if(rwEnabled)
			{
				if(m_ShaderType == ShaderType::ComputeShader)
				{
					ret = m_GraphicDevice->SetShaderRWBufferToCS(nullptr, overrideBindIndex, bindIndex);
				}
				else
				{
					XETODO("need to check if we would ever get here");
					XEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, nullptr, overrideBindIndex, bindIndex);
			}

			if(ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplyTextureBindings()
{
	XEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for(auto stbIt : m_TextureBindingMap)
	{
		ShaderTextureBinding* stb = stbIt.second;

		if(stb->GetTexture() != nullptr)
		{
			XEResult ret = m_GraphicDevice->SetTexture(m_ShaderType, stb->GetBindIndex(), nullptr);

			if(ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}

XEResult ShaderProperties::UnApplyTextureArrays()
{
	XEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	for (auto textureArrayIt : m_TextureArrayMap)
	{
		TextureArray* textureArray = textureArrayIt.second->m_Object;

		if (textureArray->IsReady())
		{
			XEResult ret = XEResult::Ok;

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = textureArrayIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = textureArray->GetBindIndex();

			if (overrideBindIndex)
			{
				bindIndex = textureArrayIt.second->m_BindIndex;
			}

			ret = m_GraphicDevice->SetTextureArray(m_ShaderType, bindIndex, nullptr);

			if (ret != XEResult::Ok)
			{
				XETODO("Set debug message");
				return ret;
			}
		}
	}

	return XEResult::Ok;
}