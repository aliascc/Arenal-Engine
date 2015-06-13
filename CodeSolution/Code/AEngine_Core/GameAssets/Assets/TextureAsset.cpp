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

/***************************
*   Game Engine Includes   *
****************************/
#include "TextureAsset.h"
#include "GraphicDevice.h"
#include "Textures\Texture.h"
#include "Base\BaseFunctions.h"
#include "Textures\Texture2D.h"
#include "Textures\TextureDefs.h"
#include "Textures\TextureCube.h"
#include "Resource\GameResourceManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AETODO("Check if this class needs a mutex");
TextureAsset::TextureAsset(const std::wstring& filePath, GameResourceManager* gameResourceManager, TextureType textureType, GraphicDevice* graphicDevice)
	: GameAsset(GameContentType::Texture, filePath, gameResourceManager)
	, m_TextureType(textureType)
	, m_GraphicDevice(graphicDevice)
{
	AEAssert(m_GraphicDevice != nullptr);
}

TextureAsset::~TextureAsset()
{
	AERelease(m_Texture);
}

Texture* TextureAsset::GetTextureReference()
{
	if(m_Texture == nullptr)
	{
		return nullptr;
	}

	return reinterpret_cast<Texture*>(m_Texture->AddRef());
}

AEResult TextureAsset::LoadAssetResource()
{
	AEAssert(m_GraphicDevice != nullptr);
	if (m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	AEAssert(m_GameResourceManager != nullptr);
	if (m_GameResourceManager == nullptr)
	{
		return AEResult::GameResourceManagerNull;
	}

	AEAssert(!m_FilePath.empty());
	if(m_FilePath.empty())
	{
		return AEResult::EmptyFilename;
	}

	AEResult ret = AEResult::Ok;

	AETODO("Look into what happens if file changes location");

	if(m_Texture != nullptr)
	{
		AETODO("Check return value");
		ret = m_Texture->Load();
	}
	else
	{
		GameResourceType gameResourceType = GameResourceType::Unknown;

		switch (m_TextureType)
		{
			case TextureType::Texture2D:
				gameResourceType = GameResourceType::Texture2D;
				break;
			case TextureType::TextureCube:
				gameResourceType = GameResourceType::TextureCube;
				break;
			default:
				AEAssert(false);
				return AEResult::InvalidTextureType;
				break;
		}

		/////////////////////////////////////////////
		//Check if Game Resources contains this Mesh
		m_Texture = (Texture*)m_GameResourceManager->AcquireGameResourceByStringID(m_FilePath, gameResourceType);

		if(m_Texture != nullptr)
		{
			AETODO("Check if we always need to reload");
			AETODO("Check return value");
			m_Texture->Load();

			return AEResult::Ok;
		}

		/////////////////////////////////////////////
		//Create Resource
		switch (m_TextureType)
		{
			case TextureType::Texture2D:
				{
					Texture2D* texture2D = new Texture2D(m_GraphicDevice, m_Name);

					ret = texture2D->CreateFromFile(m_FilePath);
					if(ret != AEResult::Ok)
					{
						AETODO("Add log");

						AERelease(texture2D);

						return AEResult::LoadFileFailed;
					}

					m_Texture = texture2D;
				}
				break;

			case TextureType::TextureCube:
				{
					TextureCube* textureCube = new TextureCube(m_GraphicDevice, m_Name);

					ret = textureCube->CreateFromFile(m_FilePath);
					if(ret != AEResult::Ok)
					{
						AETODO("Add log");

						AERelease(textureCube);

						return AEResult::LoadFileFailed;
					}

					m_Texture = textureCube;
				}
				break;

			default:
				AEAssert(false);
				return AEResult::InvalidShaderType;
				break;
		}

		/////////////////////////////////////////////
		//Add to Resource Manager
		ret = m_GameResourceManager->ManageGameResource(m_Texture, m_FilePath);
		if(ret != AEResult::Ok)
		{
			AETODO("Add log");

			AERelease(m_Texture);

			return AEResult::ResourceManagedFailed;
		}
	}

	return AEResult::Ok;
}