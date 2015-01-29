/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 11/28/2012
*
* Last Major Update: 11/28/2012
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
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "SpriteBatchXE.h"
#include "GraphicDevice.h"
#include "Base\BaseFunctions.h"
#include "Textures\Texture2D.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
SpriteBatchXE::SpriteBatchXE(GraphicDevice* graphicDevice)
	: m_GraphicDevice(graphicDevice)
{
	XEAssert(graphicDevice != nullptr);
}

SpriteBatchXE::~SpriteBatchXE()
{
	DeleteMem(m_SpriteBatch);
}

XEResult SpriteBatchXE::Initialize()
{
	if(m_IsReady)
	{
		return XEResult::Ok;
	}

	if(m_GraphicDevice == nullptr)
	{
		return XEResult::GraphicDeviceNull;
	}

	try
	{
		m_SpriteBatch = new DirectX::SpriteBatch(m_GraphicDevice->GetDeviceContextDX());
	}
	XETODO("Get correct Exception")
	catch(...)
	{
		return XEResult::Fail;
	}

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult SpriteBatchXE::Begin(DirectX::SpriteSortMode sortMode, SpriteStateChange stateMode, ID3D11BlendState* blendState, ID3D11SamplerState* samplerState, ID3D11DepthStencilState* depthStencilState, ID3D11RasterizerState* rasterizerState, const glm::mat4& transformMatrix)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(m_HasBegun)
	{
		return XEResult::Fail;
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

	return XEResult::Ok;
}

XEResult SpriteBatchXE::End()
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return XEResult::Fail;
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

	return XEResult::Ok;
}

 // Draw overloads specifying position, origin and scale as XMFLOAT2.
XEResult SpriteBatchXE::Draw(Texture2D* texture2D, const glm::vec2& position, const Color& color)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return XEResult::Fail;
	}

	XEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return XEResult::NullParameter;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), xmPosition, xmColor);

	return XEResult::Ok;
}

XEResult SpriteBatchXE::Draw(Texture2D* texture2D, const glm::vec2& position, const RECT* sourceRectangle, const Color& color, float rotation, const glm::vec2& origin, float scale, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return XEResult::Fail;
	}

	XEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return XEResult::NullParameter;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), xmPosition, sourceRectangle, xmColor, glm::radians(rotation), xmOrigin, scale, effects, layerDepth);

	return XEResult::Ok;
}

XEResult SpriteBatchXE::Draw(Texture2D* texture2D, const glm::vec2& position, const RECT* sourceRectangle, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return XEResult::Fail;
	}

	XEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return XEResult::NullParameter;
	}

	DirectX::XMFLOAT2 xmPosition(position.x, position.y);
	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);
	DirectX::XMFLOAT2 xmScale(scale.x, scale.y);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), xmPosition, sourceRectangle, xmColor, glm::radians(rotation), xmOrigin, xmScale, effects, layerDepth);

	return XEResult::Ok;
}

// Draw overloads specifying position as a RECT.
XEResult SpriteBatchXE::Draw(Texture2D* texture2D, const RECT& destinationRectangle, const Color& color)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return XEResult::Fail;
	}

	XEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return XEResult::NullParameter;
	}

	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), destinationRectangle, xmColor);

	return XEResult::Ok;
}

XEResult SpriteBatchXE::Draw(Texture2D* texture2D, const RECT& destinationRectangle, const RECT* sourceRectangle, const Color& color, float rotation, glm::vec2 const& origin, DirectX::SpriteEffects effects, float layerDepth)
{
	if(!m_IsReady)
	{
		return XEResult::NotReady;
	}

	if(!m_HasBegun)
	{
		return XEResult::Fail;
	}

	XEAssert(texture2D != nullptr);

	if(texture2D == nullptr)
	{
		return XEResult::NullParameter;
	}

	DirectX::XMVECTOR xmColor = DirectX::XMVectorSet(color.r, color.g, color.b, color.a);
	DirectX::XMFLOAT2 xmOrigin(origin.x, origin.y);

	m_SpriteBatch->Draw(texture2D->GetDXShaderResourceView(), destinationRectangle, sourceRectangle, xmColor, glm::radians(rotation), xmOrigin, effects, layerDepth);
	
	return XEResult::Ok;
}