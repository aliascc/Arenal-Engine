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

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"

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
	AEAssert(graphicDevice != nullptr);
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
	AEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return;
	}

	AEAssert(asset->GetGameContentType() == GameContentType::Texture);
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

uint64_t ShaderProperties::GetTextureAssetID(const std::wstring& name)
{
	auto textureAssetIt = m_TextureAssetMap.find(name);
	if (textureAssetIt == m_TextureAssetMap.end())
	{
		return 0;
	}

	return textureAssetIt->second.m_AssetID;
}

AEResult ShaderProperties::AddTextureAssetBinding(const std::wstring& name, TextureAsset* asset)
{
	AEAssert(asset != nullptr);
	if(asset == nullptr)
	{
		return AEResult::NullParameter;
	}

	if(m_TextureAssetMap.find(name) == m_TextureAssetMap.end())
	{
		return AEResult::NotFound;
	}

	GameObjectAssetPair<Texture>& textureAssetPair = m_TextureAssetMap[name];

	if(textureAssetPair.m_ResourceAsset != nullptr)
	{
		AEResult ret = RemoveTextureAssetBinding(name);

		if(ret != AEResult::Ok)
		{
			AETODO("Log error");

			AETODO("Add better return code");
			return AEResult::Fail;
		}
	}

	uint64_t callerID = AE_Base::GetNextUniqueID();

	AEResult ret = asset->RegisterEventHandlers(callerID, std::bind(&ShaderProperties::TextureAssetDeletion, this, std::placeholders::_1));
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	textureAssetPair.m_AssetID						= asset->GetUniqueAssetID();
	textureAssetPair.m_CallerID						= callerID;
	textureAssetPair.m_ResourceAsset				= asset->GetTextureReference();
	textureAssetPair.m_OnListenerObjDeletionEvent	= asset->GetOnListenerObjDeletionEventHandler();
	textureAssetPair.m_GameAsset					= asset;

	return AEResult::Ok;
}

AEResult ShaderProperties::RemoveTextureAssetBinding(const std::wstring& name, bool informGameAsset)
{
	if(m_TextureAssetMap.find(name) == m_TextureAssetMap.end())
	{
		return AEResult::NotFound;
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

		AERelease(textureAssetPair.m_ResourceAsset);
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::SetTextureToBinding(ShaderTextureBinding* stb, TextureAsset* asset)
{
	AEAssert(stb != nullptr);
	if(stb == nullptr)
	{
		return AEResult::NullParameter;
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

	AEResult ret = stb->SetTexture(texture);
	if(ret != AEResult::Ok)
	{
		AERelease(texture);

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

AEResult ShaderProperties::SetTextureToBinding(ShaderTextureBinding* stb, Texture* texture)
{
	AEAssert(stb != nullptr);
	if(stb == nullptr)
	{
		return AEResult::NullParameter;
	}

	const std::wstring& textureName = stb->GetName();

	Texture* refTexture = reinterpret_cast<Texture*>(texture->AddRef());

	AEResult ret = stb->SetTexture(refTexture);
	if(ret != AEResult::Ok)
	{
		AERelease(refTexture);

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

	return AEResult::Ok;
}

AEResult ShaderProperties::SetTexture(const std::wstring& name, TextureAsset* asset)
{
	if(!ShaderTextureBindingExists(name))
	{
		return AEResult::NotFound;
	}

	ShaderTextureBinding* stb = m_TextureBindingMap[name];

	return SetTextureToBinding(stb, asset);
}

AEResult ShaderProperties::SetTexture(uint32_t index, TextureAsset* asset)
{
	if(index >= m_TextureBindingVector.size())
	{
		return AEResult::OutsideRange;
	}

	ShaderTextureBinding* stb = m_TextureBindingVector[index];

	return SetTextureToBinding(stb, asset);
}

AEResult ShaderProperties::SetTexture(const std::wstring& name, Texture* texture)
{
	if(!ShaderTextureBindingExists(name))
	{
		return AEResult::NotFound;
	}

	ShaderTextureBinding* stb = m_TextureBindingMap[name];

	return SetTextureToBinding(stb, texture);
}

AEResult ShaderProperties::CreateFromShaderSignatures(Shader* shader)
{
	/////////////////////////////////////////////////////
	//Pre-check variables
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEAssert(shader != nullptr);
	if(shader == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEResult ret = AEResult::Ok;

	/////////////////////////////////////////////////////
	//Clean Up Shader Properties for new Information
	CleanUp();

	/////////////////////////////////////////////////////
	//Add Constant Buffers from Signature List
	const ConstantBufferSignatureList& constBufferSigList = shader->GetConstantBufferSignatureList();
	ret = AddConstantBuffersFromSigList(constBufferSigList);
	if(ret != AEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_CB_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), AE_Base::GetAEResultString(ret));
		AELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Samplers from Signature List
	const SamplerSignatureList& samplerSignatureList = shader->GetSamplerSignatureList();
	ret = AddSamplersFromSigList(samplerSignatureList);
	if (ret != AEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_SAM_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), AE_Base::GetAEResultString(ret));
		AELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Simple Buffers from Signature List
	const SimpleBufferSignatureList& simpleBufferSignatureList = shader->GetSimpleBufferSignatureList();
	ret = AddSimpleBuffersFromSigList(simpleBufferSignatureList);
	if(ret != AEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_B_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), AE_Base::GetAEResultString(ret));
		AELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Structured Buffers from Signature List
	const StructuredBufferSignatureList& structuredBufferSignatureList = shader->GetStructuredBufferSignatureList();
	ret = AddStructuredBuffersFromSigList(structuredBufferSignatureList);
	if(ret != AEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_SB_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), AE_Base::GetAEResultString(ret));
		AELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Texture Inputs from Signature List
	const TextureInputSignatureList& textBindingSigList = shader->GetTextureInputSignatureList();
	ret = AddTextureBindingsFromSigList(textBindingSigList);
	if(ret != AEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_TB_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), AE_Base::GetAEResultString(ret));
		AELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Add Texture Arrays Inputs from Signature List
	const TextureArrayInputSignatureList& textArrayBindingSigList = shader->GetTextureArrayInputSignatureList();
	ret = AddTextureArraysFromSigList(textArrayBindingSigList);
	if (ret != AEResult::Ok)
	{
		CleanUp();

		std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_TA_ADD_SIG_LIST_ERR_MSG"), __FUNCTIONW__, shader->GetName(), AE_Base::GetAEResultString(ret));
		AELOGGER->AddNewLog(LogLevel::Error, msgerr);

		return ret;
	}

	/////////////////////////////////////////////////////
	//Finish
	return AEResult::Ok;
}

AEResult ShaderProperties::AddConstantBuffersFromSigList(const ConstantBufferSignatureList& constBufferSigList)
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for(const ConstantBufferSignature& constBufferSig : constBufferSigList)
	{
		ConstantBuffer* cb = new ConstantBuffer(m_GraphicDevice, constBufferSig.m_BindIndex, constBufferSig.m_Name);

		for(const ShaderCustomVariable& constSCVar : constBufferSig.m_ShaderCustomVariableList)
		{
			ShaderCustomVariable* scVar = new ShaderCustomVariable(constSCVar);

			ret = cb->AddVariable(scVar);
			if(ret != AEResult::Ok)
			{
				DeleteMem(scVar);

				std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_CB_SIG_ADD_VAR_DBG_MSG"), __FUNCTIONW__, constSCVar.m_Name, cb->GetName(), AE_Base::GetAEResultString(ret));
				AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

				break;
			}
		}

		if(ret != AEResult::Ok)
		{
			DeleteMem(cb);

			return ret;
		}

		ret = cb->Initialize();
		if(ret != AEResult::Ok)
		{
			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_CB_SIG_INIT_CB_DBG_MSG"), __FUNCTIONW__, cb->GetName(), AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret; 
		}

		ret = AddConstantBuffer(cb);
		if(ret != AEResult::Ok)
		{
			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_CB_SIG_ADD_CB_DBG_MSG"), __FUNCTIONW__, cb->GetName(), AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			DeleteMem(cb);

			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::AddSamplersFromSigList(const SamplerSignatureList& samplerSigList)
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for (const SamplerSignature& samplerSig : samplerSigList)
	{
		Sampler* sampler = new Sampler(m_GraphicDevice, samplerSig.m_Name, samplerSig.m_BindIndex);

		ret = sampler->Initialize();
		if (ret != AEResult::Ok)
		{
			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_SAM_SIG_INIT_CB_DBG_MSG"), __FUNCTIONW__, sampler->GetName(), AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}

		ret = AddSampler(sampler);
		if (ret != AEResult::Ok)
		{
			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_SAM_SIG_ADD_CB_DBG_MSG"), __FUNCTIONW__, sampler->GetName(), AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			DeleteMem(sampler);

			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::AddSimpleBuffersFromSigList(const SimpleBufferSignatureList& simpleBufferSignatureList)
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for(const SimpleBufferSignature& simpleBufferSig : simpleBufferSignatureList)
	{
		SimpleBuffer* simpleBuffer = new SimpleBuffer(simpleBufferSig.m_Name, simpleBufferSig.m_BindIndex, simpleBufferSig.m_IsRW, simpleBufferSig.m_VariableClass, simpleBufferSig.m_VariableType, simpleBufferSig.m_ElementCount, m_GraphicDevice);

		ret = AddSimpleBuffer(simpleBuffer);

		if(ret != AEResult::Ok)
		{
			DeleteMem(simpleBuffer);

			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_B_SIG_ADD_B_DBG_MSG"), __FUNCTIONW__, simpleBufferSig.m_Name, AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::AddStructuredBuffersFromSigList(const StructuredBufferSignatureList& structuredBufferSigList)
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for(const StructuredBufferSignature& structuredBufferSig : structuredBufferSigList)
	{
		StructuredBuffer* structuredBuffer = new StructuredBuffer(structuredBufferSig.m_Name, structuredBufferSig.m_BindIndex, structuredBufferSig.m_IsRW, m_GraphicDevice);

		ret = AddStructuredBuffer(structuredBuffer);

		if(ret != AEResult::Ok)
		{
			DeleteMem(structuredBuffer);

			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_SB_SIG_ADD_SB_DBG_MSG"), __FUNCTIONW__, structuredBufferSig.m_Name, AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::AddTextureBindingsFromSigList(const TextureInputSignatureList& textBindingSigList)
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for(const TextureInputSignature& textBindingSig : textBindingSigList)
	{
		ShaderTextureBinding* stb = new ShaderTextureBinding(textBindingSig.m_Name, textBindingSig.m_BindIndex, textBindingSig.m_TextureType, nullptr);

		ret = AddShaderTextureBinding(stb);

		if(ret != AEResult::Ok)
		{
			DeleteMem(stb);

			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_TB_SIG_ADD_TB_DBG_MSG"), __FUNCTIONW__, textBindingSig.m_Name, AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::AddTextureArraysFromSigList(const TextureArrayInputSignatureList& textArrayInputSigList)
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for (const TextureArrayInputSignature& textArrayInput : textArrayInputSigList)
	{
		TextureArray* textureArray = nullptr;

		switch (textArrayInput.m_TextureType)
		{
			case TextureType::Texture2D:
				textureArray = new Texture2DArray(m_GraphicDevice, textArrayInput.m_Name, textArrayInput.m_BindIndex);
				break;

			default:
				AEAssert(false);
				ret = AEResult::InvalidTextureType;
				break;
		}

		if (textureArray != nullptr)
		{
			ret = AddTextureArray(textureArray);
		}

		if (ret != AEResult::Ok)
		{
			DeleteMem(textureArray);

			std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_TA_SIG_ADD_SB_DBG_MSG"), __FUNCTIONW__, textArrayInput.m_Name, AE_Base::GetAEResultString(ret));
			AELOGGER->AddNewLog(LogLevel::Debug, msgerr);

			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::AddConstantBuffer(ConstantBuffer* cb)
{
	AEAssert(cb != nullptr);
	if(cb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!cb->GetName().empty());
	if(cb->GetName().empty())
	{
		return AEResult::EmptyName;
	}

	if(ConstantBufferExists(cb->GetName()))
	{
		return AEResult::ObjExists;
	}

	m_ConstantBufferMap[cb->GetName()] = cb;
	m_ConstantBufferVector.push_back(cb);

	return AEResult::Ok;
}

AEResult ShaderProperties::AddSampler(Sampler* sampler)
{
	AEAssert(sampler != nullptr);
	if (sampler == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!sampler->GetName().empty());
	if (sampler->GetName().empty())
	{
		return AEResult::EmptyName;
	}

	if (SamplerExists(sampler->GetName()))
	{
		return AEResult::ObjExists;
	}

	ShaderPropertyObject<Sampler>* spo = new ShaderPropertyObject<Sampler>();
	spo->m_Object = sampler;

	m_SamplerMap[sampler->GetName()] = spo;
	m_SamplerVector.push_back(spo);

	return AEResult::Ok;
}

AEResult ShaderProperties::RemoveSampler(const std::wstring& name)
{
	if (!SamplerExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::OverrideSampler(const std::wstring& name, Sampler* sampler, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	AEAssert(sampler != nullptr);
	if (sampler == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!name.empty());
	if (name.empty())
	{
		return AEResult::EmptyName;
	}

	if (!SamplerExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::AddSimpleBuffer(SimpleBuffer* simpleBuffer)
{
	AEAssert(simpleBuffer != nullptr);
	if(simpleBuffer == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!simpleBuffer->GetName().empty());
	if(simpleBuffer->GetName().empty())
	{
		return AEResult::EmptyName;
	}

	if(SimpleBufferExists(simpleBuffer->GetName()))
	{
		return AEResult::ObjExists;
	}

	ShaderPropertyObject<SimpleBuffer>* spo = new ShaderPropertyObject<SimpleBuffer>();
	spo->m_Object = simpleBuffer;

	m_SimpleBufferMap[simpleBuffer->GetName()] = spo;
	m_SimpleBufferVector.push_back(spo);

	return AEResult::Ok;
}

AEResult ShaderProperties::RemoveSimpleBuffer(const std::wstring& name)
{
	if(!SimpleBufferExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::OverrideSimpleBuffer(const std::wstring& name, SimpleBuffer* simpleBuffer, bool overwriteRW, bool setRW, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	AEAssert(simpleBuffer != nullptr);
	if(simpleBuffer == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!name.empty());
	if(name.empty())
	{
		return AEResult::EmptyName;
	}

	if(!SimpleBufferExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::AddStructuredBuffer(StructuredBuffer* structuredBuffer)
{
	AEAssert(structuredBuffer != nullptr);
	if(structuredBuffer == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!structuredBuffer->GetName().empty());
	if(structuredBuffer->GetName().empty())
	{
		return AEResult::EmptyName;
	}

	if(StructuredBufferExists(structuredBuffer->GetName()))
	{
		return AEResult::ObjExists;
	}

	ShaderPropertyObject<StructuredBuffer>* spo = new ShaderPropertyObject<StructuredBuffer>();
	spo->m_Object = structuredBuffer;

	m_StructuredBufferMap[structuredBuffer->GetName()] = spo;
	m_StructuredBufferVector.push_back(spo);

	return AEResult::Ok;
}

AEResult ShaderProperties::RemoveStructuredBuffer(const std::wstring& name)
{
	if(!StructuredBufferExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::OverrideStructuredBuffer(const std::wstring& name, StructuredBuffer* structuredBuffer, bool overwriteRW, bool setRW, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	AEAssert(structuredBuffer != nullptr);
	if(structuredBuffer == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!name.empty());
	if(name.empty())
	{
		return AEResult::EmptyName;
	}

	if(!StructuredBufferExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::AddTextureArray(TextureArray* textureArray)
{
	AEAssert(textureArray != nullptr);
	if (textureArray == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!textureArray->GetName().empty());
	if (textureArray->GetName().empty())
	{
		return AEResult::EmptyName;
	}

	if (TextureArrayExists(textureArray->GetName()))
	{
		return AEResult::ObjExists;
	}

	ShaderPropertyObject<TextureArray>* spo = new ShaderPropertyObject<TextureArray>();
	spo->m_Object = textureArray;

	m_TextureArrayMap[textureArray->GetName()] = spo;
	m_TextureArrayVector.push_back(spo);

	return AEResult::Ok;
}

AEResult ShaderProperties::RemoveTextureArray(const std::wstring& name)
{
	if (!StructuredBufferExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::OverrideTextureArray(const std::wstring& name, TextureArray* textureArray, bool overrideBindIndex, uint32_t bindIndex, bool deleteAtShaderDestroy)
{
	AEAssert(textureArray != nullptr);
	if (textureArray == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!name.empty());
	if (name.empty())
	{
		return AEResult::EmptyName;
	}

	if (!TextureArrayExists(name))
	{
		return AEResult::NotFound;
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

	return AEResult::Ok;
}

AEResult ShaderProperties::AddShaderTextureBinding(ShaderTextureBinding* stb)
{
	AEAssert(stb != nullptr);
	if(stb == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEAssert(!stb->GetName().empty());
	if(stb->GetName().empty())
	{
		return AEResult::EmptyName;
	}

	if(ShaderTextureBindingExists(stb->GetName()))
	{
		return AEResult::ObjExists;
	}

	m_TextureBindingMap[stb->GetName()] = stb;
	m_TextureBindingVector.push_back(stb);

	return AEResult::Ok;
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

AEResult ShaderProperties::CopyVariableData(const ShaderProperties* otherShaderProps)
{
	//////////////////////////////////////////////////
	//Check Parameters
	AEAssert(otherShaderProps != nullptr);
	if(otherShaderProps == nullptr)
	{
		return AEResult::NullParameter;
	}

	AEResult ret = AEResult::Ok;

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

					if(ret != AEResult::Ok)
					{
						std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_CB_SET_VALUE_ERR_MSG"), __FUNCTIONW__, thisShaderVar->m_Name, thisCB->GetName(), AE_Base::GetAEResultString(ret));
						AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
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

					if(ret != AEResult::Ok)
					{
						AERelease(reference);

						std::wstring msgerr = fmt::format(AELOCMAN->GetLiteral(L"SHADER_TEX_BINDING_SET_ERR_MSG"), __FUNCTIONW__, thisTextBinding->GetName(), AE_Base::GetAEResultString(ret));
						AELOGGER->AddNewLog(LogLevel::Warning, msgerr);

						continue;
					}

					//////////////////////////////////////////////////
					//If Texture comes from Texture Asset Copy information
					auto otherTexturePairIt = otherShaderProps->m_TextureAssetMap.find(thisTextBinding->GetName());
					if(otherTexturePairIt != otherShaderProps->m_TextureAssetMap.end())
					{
						const GameObjectAssetPair<Texture>& otherTexturePair = otherTexturePairIt->second;
						ret = CopyTexturePairData(thisTextBinding->GetName(), otherTexturePair);

						if(ret != AEResult::Ok)
						{
							thisTextBinding->SetTexture(nullptr);

							AETODO("Log error, can not copy object pair");
						}
					}
					else
					{
						AETODO("Log warning, texture did not contain object pair");
					}
				}
			}
		}
	}

	AETODO("Add sampler to copy");

	return AEResult::Ok;
}

AEResult ShaderProperties::CopyTexturePairData(const std::wstring& textureName, const GameObjectAssetPair<Texture>& otherTexturePair)
{
	if(m_TextureAssetMap.find(textureName) == m_TextureAssetMap.end())
	{
		m_TextureAssetMap[textureName] = GameObjectAssetPair<Texture>();
	}

	AEAssert(otherTexturePair.m_GameAsset != nullptr);
	AEAssert(otherTexturePair.m_ResourceAsset != nullptr);
	if(otherTexturePair.m_GameAsset == nullptr || otherTexturePair.m_ResourceAsset == nullptr)
	{
		return AEResult::NullObj;
	}

	GameObjectAssetPair<Texture>& textureAssetPair = m_TextureAssetMap[textureName];

	uint64_t callerID = AE_Base::GetNextUniqueID();

	AEResult ret = otherTexturePair.m_GameAsset->RegisterEventHandlers(callerID, std::bind(&ShaderProperties::TextureAssetDeletion, this, std::placeholders::_1));
	if(ret != AEResult::Ok)
	{
		AETODO("Log error");

		return ret;
	}

	textureAssetPair.m_AssetID						= otherTexturePair.m_AssetID;
	textureAssetPair.m_CallerID						= callerID;
	textureAssetPair.m_ResourceAsset				= reinterpret_cast<Texture*>(otherTexturePair.m_ResourceAsset->AddRef());
	textureAssetPair.m_OnListenerObjDeletionEvent	= otherTexturePair.m_GameAsset->GetOnListenerObjDeletionEventHandler();
	textureAssetPair.m_GameAsset					= otherTexturePair.m_GameAsset;

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplyAll()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	if(ApplyConstantBuffers() != AEResult::Ok)
	{
		return AEResult::SetConstantBufferFailed;
	}

	if (ApplySamplers() != AEResult::Ok)
	{
		AETODO("Change return value");
		return AEResult::Fail;
	}

	if(ApplyTextureBindings() != AEResult::Ok)
	{
		return AEResult::SetTextureFailed;
	}

	if (ApplyTextureArrays() != AEResult::Ok)
	{
		AETODO("Change return value");
		return AEResult::SetTextureFailed;
	}

	if(ApplySimpleBuffers() != AEResult::Ok)
	{
		return AEResult::SetSimpleBufferFailed;
	}

	if(ApplyStructuredBuffers() != AEResult::Ok)
	{
		return AEResult::SetStructuredBufferFailed;
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplyConstantBuffers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto cbIt : m_ConstantBufferMap)
	{
		ConstantBuffer* cb = cbIt.second;

		AEResult ret = AEResult::Ok;

		ret = cb->Apply();

		if(ret != AEResult::Ok)
		{
			AETODO("Set debug message");
			return ret;
		}

		ret = m_GraphicDevice->SetConstantBuffer(m_ShaderType, cb);

		if(ret != AEResult::Ok)
		{
			AETODO("Set debug message");
			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplySamplers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for (auto samplerIt : m_SamplerMap)
	{
		AEResult ret = AEResult::Ok;

		Sampler* sampler = samplerIt.second->m_Object;

		if (sampler->NeedsReinit())
		{
			ret = sampler->Reinitialize();

			if (ret != AEResult::Ok)
			{
				AETODO("Log error");
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

			if (ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplySimpleBuffers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto simpleBufferIt : m_SimpleBufferMap)
	{
		SimpleBuffer* simpleBuffer = simpleBufferIt.second->m_Object;

		if(simpleBuffer->IsReady())
		{
			AEResult ret = AEResult::Ok;

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
					AETODO("need to check if we would ever get here");
					AEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, simpleBuffer, overrideBindIndex, bindIndex);
			}

			if(ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplyStructuredBuffers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto structuredBufferIt : m_StructuredBufferMap)
	{
		StructuredBuffer* structuredBuffer = structuredBufferIt.second->m_Object;

		if(structuredBuffer->IsReady())
		{
			AEResult ret = AEResult::Ok;

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
					AETODO("need to check if we would ever get here");
					AEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, structuredBuffer, overrideBindIndex, bindIndex);
			}

			if(ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplyTextureBindings()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto stbIt : m_TextureBindingMap)
	{
		ShaderTextureBinding* stb = stbIt.second;

		Texture* textureBinding = stb->GetTexture();
		if (textureBinding == nullptr)
		{
			textureBinding = m_GraphicDevice->GetDefaultTexture2D();
		}

		AEResult ret = m_GraphicDevice->SetTexture(m_ShaderType, stb->GetBindIndex(), textureBinding);
		if (ret != AEResult::Ok)
		{
			AETODO("Set debug message");
			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::ApplyTextureArrays()
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for (auto textureArrayIt : m_TextureArrayMap)
	{
		TextureArray* textureArray = textureArrayIt.second->m_Object;

		if (textureArray->IsReady())
		{
			AEResult ret = AEResult::Ok;

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = textureArrayIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = textureArray->GetBindIndex();

			if (overrideBindIndex)
			{
				bindIndex = textureArrayIt.second->m_BindIndex;
			}

			ret = m_GraphicDevice->SetTextureArray(m_ShaderType, bindIndex, textureArray);

			if (ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplyAll()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	if(UnApplyConstantBuffers() != AEResult::Ok)
	{
		return AEResult::SetConstantBufferFailed;
	}

	if (UnApplySamplers() != AEResult::Ok)
	{
		AETODO("Change return value");
		return AEResult::Fail;
	}

	if(UnApplyTextureBindings() != AEResult::Ok)
	{
		return AEResult::SetTextureFailed;
	}

	if (UnApplyTextureArrays() != AEResult::Ok)
	{
		AETODO("Change return value");
		return AEResult::SetTextureFailed;
	}

	if(UnApplySimpleBuffers() != AEResult::Ok)
	{
		return AEResult::SetSimpleBufferFailed;
	}

	if(UnApplyStructuredBuffers() != AEResult::Ok)
	{
		return AEResult::SetStructuredBufferFailed;
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplyConstantBuffers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEResult ret = AEResult::Ok;

	for(auto cbIt : m_ConstantBufferMap)
	{
		ConstantBuffer* cb = cbIt.second;

		ret = m_GraphicDevice->SetConstantBuffer(m_ShaderType, nullptr, true, cb->GetBindIndex());

		if(ret != AEResult::Ok)
		{
			AETODO("Set debug message");
			return ret;
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplySamplers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for (auto samplerIt : m_SamplerMap)
	{
		AEResult ret = AEResult::Ok;

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

			if (ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplySimpleBuffers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto simpleBufferIt : m_SimpleBufferMap)
	{
		SimpleBuffer* simpleBuffer = simpleBufferIt.second->m_Object;

		if(simpleBuffer->IsReady())
		{
			AEResult ret = AEResult::Ok;

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
					AETODO("need to check if we would ever get here");
					AEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, nullptr, overrideBindIndex, bindIndex);
			}

			if(ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplyStructuredBuffers()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto structuredBufferIt : m_StructuredBufferMap)
	{
		StructuredBuffer* structuredBuffer = structuredBufferIt.second->m_Object;

		if(structuredBuffer->IsReady())
		{
			AEResult ret = AEResult::Ok;

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
					AETODO("need to check if we would ever get here");
					AEAssert(false);
				}
			}
			else
			{
				ret = m_GraphicDevice->SetShaderBuffer(m_ShaderType, nullptr, overrideBindIndex, bindIndex);
			}

			if(ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplyTextureBindings()
{
	AEAssert(m_GraphicDevice != nullptr);
	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for(auto stbIt : m_TextureBindingMap)
	{
		ShaderTextureBinding* stb = stbIt.second;

		if(stb->GetTexture() != nullptr)
		{
			AEResult ret = m_GraphicDevice->SetTexture(m_ShaderType, stb->GetBindIndex(), nullptr);

			if(ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}

AEResult ShaderProperties::UnApplyTextureArrays()
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	for (auto textureArrayIt : m_TextureArrayMap)
	{
		TextureArray* textureArray = textureArrayIt.second->m_Object;

		if (textureArray->IsReady())
		{
			AEResult ret = AEResult::Ok;

			//////////////////////////////////////////////////
			//Get Bind Index
			bool overrideBindIndex = textureArrayIt.second->m_IsOverrideBindIndex;
			uint32_t bindIndex = textureArray->GetBindIndex();

			if (overrideBindIndex)
			{
				bindIndex = textureArrayIt.second->m_BindIndex;
			}

			ret = m_GraphicDevice->SetTextureArray(m_ShaderType, bindIndex, nullptr);

			if (ret != AEResult::Ok)
			{
				AETODO("Set debug message");
				return ret;
			}
		}
	}

	return AEResult::Ok;
}