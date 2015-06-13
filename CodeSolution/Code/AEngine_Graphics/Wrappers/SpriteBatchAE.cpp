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
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "SpriteBatchAE.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Textures\Texture2D.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SpriteBatchAE::SpriteBatchAE(GraphicDevice* graphicDevice)
	: m_GraphicDevice(graphicDevice)
{
	AEAssert(graphicDevice != nullptr);
}

SpriteBatchAE::~SpriteBatchAE()
{
	DeleteMem(m_SpriteBatch);
}

AEResult SpriteBatchAE::Initialize()
{
	if(m_IsReady)
	{
		return AEResult::Ok;
	}

	if(m_GraphicDevice == nullptr)
	{
		return AEResult::GraphicDeviceNull;
	}

	try
	{
		m_SpriteBatch = new DirectX::SpriteBatch(m_GraphicDevice->GetDeviceContextDX());
	}
	AETODO("Get correct Exception")
	catch(...)
	{
		return AEResult::Fail;
	}

	m_IsReady = true;

	return AEResult::Ok;
}

AEResult SpriteBatchAE::Begin(DirectX::SpriteSortMode sortMode, SpriteStateChange stateMode, ID3D11BlendState* blendState, ID3D11SamplerState* samplerState, ID3D11DepthStencilState* depthStencilState, ID3D11RasterizerState* rasterizerState, const glm::mat4& transformMatrix)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(m_HasBegun)
	{
		return AEResult::Fail;
	}

	m_StateChange = stateMode;

	if(m_StateChange == SpriteStateChange::Discard)
	{
		ID3D11DeviceContext* dxContext = m_GraphicDevice->GetDeviceContextDX();
		
		dxContext->OMGetBlendState(&m_SpriteStates.m_BlendState, (float*)&m_SpriteStates.m_BlendFactor, &m_SpriteStates.m_SampleMask);
		dxContext->OMGetDepthStencilState(&m_SpriteStates.m_DepthStencilState, &m_SpriteStates.m_StencilRef);
		dxContext->RSGetState(&m_SpriteStates.m_RasterizerState);
	}

	DirectX::CXMMATRIX cxmTransformMatrix = DirectX::XMMatrixSet(	transformMatrix[0][0], transformMatrix[0][1], transformMatrix[0][2], transformMatrix[0][3],
																	transformMatrix[1][0], transformMatrix[1][1], transformMatrix[1][2], transformMatrix[1][3],
																	transformMatrix[2][0], transformMatrix[2][1], transformMatrix[2][2], transformMatrix[2][3],
																	transformMatrix[3][0], transformMatrix[3][1], transformMatrix[3][2], transformMatrix[3][3]);
		
	m_SpriteBatch->Begin(sortMode, blendState, samplerState, depthStencilState, rasterizerState, nullptr, cxmTransformMatrix);

	m_HasBegun = true;

	return AEResult::Ok;
}

AEResult SpriteBatchAE::End()
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return AEResult::Fail;
	}

	m_SpriteBatch->End();

	if(m_StateChange == SpriteStateChange::Discard)
	{
		ID3D11DeviceContext* dxContext = m_GraphicDevice->GetDeviceContextDX();
		
		dxContext->OMSetBlendState(m_SpriteStates.m_BlendState, (float*)&m_SpriteStates.m_BlendFactor, m_SpriteStates.m_SampleMask);
		dxContext->OMSetDepthStencilState(m_SpriteStates.m_DepthStencilState, m_SpriteStates.m_StencilRef);
		dxContext->RSSetState(m_SpriteStates.m_RasterizerState);
	}

	m_HasBegun = false;

	return AEResult::Ok;
}

 // Draw overloads specifying position, origin and scale as XMFLOAT2.
AEResult SpriteBatchAE::Draw(Texture2D* texture2D, const glm::vec2& position, const Color& color)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return AEResult::Fail;
	}

	AEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return AEResult::NullParameter;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), xmPosition, xmColor);

	return AEResult::Ok;
}

AEResult SpriteBatchAE::Draw(Texture2D* texture2D, const glm::vec2& position, const RECT* sourceRectangle, const Color& color, float rotation, const glm::vec2& origin, float scale, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return AEResult::Fail;
	}

	AEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return AEResult::NullParameter;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), xmPosition, sourceRectangle, xmColor, glm::radians(rotation), xmOrigin, scale, effects, layerDepth);

	return AEResult::Ok;
}

AEResult SpriteBatchAE::Draw(Texture2D* texture2D, const glm::vec2& position, const RECT* sourceRectangle, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return AEResult::Fail;
	}

	AEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return AEResult::NullParameter;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);
	DirectX::XMFLOAT2 xmScale(scale.x, scale.y);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), xmPosition, sourceRectangle, xmColor, glm::radians(rotation), xmOrigin, xmScale, effects, layerDepth);

	return AEResult::Ok;
}

// Draw overloads specifying position as a RECT.
AEResult SpriteBatchAE::Draw(Texture2D* texture2D, const RECT& destinationRectangle, const Color& color)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return AEResult::Fail;
	}

	AEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return AEResult::NullParameter;
	}

	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), destinationRectangle, xmColor);

	return AEResult::Ok;
}

AEResult SpriteBatchAE::Draw(Texture2D* texture2D, const RECT& destinationRectangle, const RECT* sourceRectangle, const Color& color, float rotation, glm::vec2 const& origin, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return AEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return AEResult::Fail;
	}

	AEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return AEResult::NullParameter;
	}

	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), destinationRectangle, sourceRectangle, xmColor, glm::radians(rotation), xmOrigin, effects, layerDepth);
	
	return AEResult::Ok;
}