/********************************************************
*
* Author: Carlos Chac�n N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _SPRITE_BATCH_XE_H
#define _SPRITE_BATCH_XE_H

/**********************
*   System Includes   *
***********************/
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "DirectXTK\SpriteBatch.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "SpriteDefs.h"
#include "Color\Color.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"
#include "Color\XEColorDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class Texture2D;

/*****************
*   Class Decl   *
******************/
class SpriteBatchXE sealed : public XEObject
{
	/*********************
	*   Friend Classes   *
	**********************/
#pragma region Private Structs

	/// <summary>
	/// Lets SpriteFontXE access private variables
	/// </summary>
	friend class SpriteFontXE;

#pragma endregion
	
	/***********************
	*   Private Structs    *
	************************/
#pragma region Private Structs

	/// <summary>
	/// Keeps a list of current states that change in order to set them back if XE_SSM_DISCARD 
	/// is pass in as we do not want the user to be changing states each time Sprite Batch
	/// is called. The only state it ignores is Sampler State as normally this state 
	/// is set in the shader or by code.
	/// </summary>
	struct SpriteStates sealed : public XEObject
	{
		/// <summary>
		/// Holder for Blend State
		/// </summary>
		ID3D11BlendState* m_BlendState = nullptr;

		/// <summary>
		/// Holder for Blend Factor
		/// </summary>
		glm::vec4 m_BlendFactor = XEMathHelpers::Vec4fZero;

		/// <summary>
		/// Holder for Sample Mask for Blend State
		/// </summary>
		uint32_t m_SampleMask = 0;

		/// <summary>
		/// Holder for Depth Stencil State
		/// </summary>
		ID3D11DepthStencilState* m_DepthStencilState = nullptr;

		/// <summary>
		/// Holder for Stencil Reference for Depth Stencil State
		/// </summary>
		uint32_t m_StencilRef = 0;

		/// <summary>
		/// Holder for Rasterizer State
		/// </summary>
		ID3D11RasterizerState* m_RasterizerState = nullptr;

		/// <summary>
		/// Default SpriteStates Constructor
		/// </summary>
		SpriteStates()
		{
		}
	};

#pragma endregion

	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// Lets SpriteBatchXE know how to handle state changes
		/// </summary>
		SpriteStates m_SpriteStates;
		
		/// <summary>
		/// Struct to keep a record of current states
		/// </summary>
		SpriteStateChange m_StateChange = SpriteStateChange::Discard;
		
		/// <summary>
		/// DirectXTK Sprite Batch Instance
		/// </summary>
		DirectX::SpriteBatch* m_SpriteBatch = nullptr;

		/// <summary>
		/// Lets know if the Sprite Batch is ready for use
		/// </summary>
		bool m_IsReady = false;

		/// <summary>
		/// Graphic Device associated with the object
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

		/// <summary>
		/// States if the Sprite Batch has the Begin method called
		/// </summary>
		bool m_HasBegun = false;

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default SpriteBatchXE Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		SpriteBatchXE(GraphicDevice* graphicDevice);

		/// <summary>
		/// Default SpriteBatchXE Destructor
		/// </summary>
		virtual ~SpriteBatchXE();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Returns if Sprite Batch has been initialized
		/// </summary>
		/// <returns>True if Sprite Batch has been Initialized</returns>
		inline bool IsReady() const
		{
			return m_IsReady;
		}

		/// <summary>
		/// Returns if Sprite Batch Begin methods has been called
		/// </summary>
		/// <returns>True if Sprite Batch Begin method was called</returns>
		inline bool HasBegun() const
		{
			return m_HasBegun;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods
		
		/// <summary>
		/// Initializes the Sprite Batch for use
		/// </summary>
		/// <returns>XEResult::Ok if Initialize was successful</returns>
		XEResult Initialize();

		/// <summary>
		/// Lets the Sprite Batch know it is time to start processing textures & font
		/// </summary>
		/// <param name="sortMode">How will the texture be drawn and at what time</param>
		/// <param name="stateMode">How will the current states behave with the new ones</param>
		/// <param name="blendState">Blend State to use, can be null</param>
		/// <param name="samplerState">Sampler State to use, can be null</param>
		/// <param name="depthStencilState">Depth Stencil State to use, can be null</param>
		/// <param name="rasterizerState">Rasterizer State to use, can be null</param>
		/// <param name="transformMatrix">Transformations applied to the textures to be drawn</param>
		/// <returns>XEResult::Ok if Begin was successful</returns>
		XEResult Begin(DirectX::SpriteSortMode sortMode = DirectX::SpriteSortMode_Deferred, SpriteStateChange stateMode = SpriteStateChange::Discard, ID3D11BlendState* blendState = nullptr, ID3D11SamplerState* samplerState = nullptr, ID3D11DepthStencilState* depthStencilState = nullptr, ID3D11RasterizerState* rasterizerState = nullptr, const glm::mat4& transformMatrix = XEMathHelpers::Mat4Identity);

		/// <summary>
		/// Draws a Texture.
		/// </summary>
		/// <param name="texture2D">Texture to be drawn</param>
		/// <param name="position">Position where to put the texture</param>
		/// <param name="color">Color to tint the texture</param>
		/// <returns>XEResult::Ok if Draw was successful</returns>
		XEResult Draw(Texture2D* texture2D, const glm::vec2& position, const Color& color = XEColors::White);

		/// <summary>
		/// Draws a Texture.
		/// </summary>
		/// <param name="texture2D">Texture to be drawn</param>
		/// <param name="position">Position where to put the texture</param>
		/// <param name="sourceRectangle">Rectangle coordinates to draw part of the texture</param>
		/// <param name="color">Color to tint the texture</param>
		/// <param name="rotation">Rotation of the texture in Degrees</param>
		/// <param name="origin">Origin point of the texture to determine position/scale/rotation. (Upper left is 0,0)</param>
		/// <param name="scale">Scale of the texture</param>
		/// <param name="SpriteEffects">Effects to be applied on the texture</param>
		/// <param name="layerDepth">Layer Depth for Sorting</param>
		/// <returns>XEResult::Ok if Draw was successful</returns>
		XEResult Draw(Texture2D* texture2D, const glm::vec2& position, const RECT* sourceRectangle, const Color& color = XEColors::White, float rotation = 0.0f, const glm::vec2& origin = XEMathHelpers::Vec2fZero, float scale = 1.0f, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

		/// <summary>
		/// Draws a Texture.
		/// </summary>
		/// <param name="texture2D">Texture to be drawn</param>
		/// <param name="position">Position where to put the texture</param>
		/// <param name="sourceRectangle">Rectangle coordinates to draw part of the texture</param>
		/// <param name="color">Color to tint the texture</param>
		/// <param name="rotation">Rotation of the texture in Degrees</param>
		/// <param name="origin">Origin point of the texture to determine position/scale/rotation. (Upper left is 0,0)</param>
		/// <param name="scale">Scale of the texture</param>
		/// <param name="SpriteEffects">Effects to be applied on the texture</param>
		/// <param name="layerDepth">Layer Depth for Sorting</param>
		/// <returns>XEResult::Ok if Draw was successful</returns>
		XEResult Draw(Texture2D* texture2D, const glm::vec2& position, const RECT* sourceRectangle, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

		/// <summary>
		/// Draws a Texture.
		/// </summary>
		/// <param name="texture2D">Texture to be drawn</param>
		/// <param name="destinationRectangle">Rectangle coordinates where to draw the texture</param>
		/// <param name="color">Color to tint the texture</param>
		/// <returns>XEResult::Ok if Draw was successful</returns>
		XEResult Draw(Texture2D* texture2D, const RECT& destinationRectangle, const Color& color = XEColors::White);

		/// <summary>
		/// Draws a Texture.
		/// </summary>
		/// <param name="texture2D">Texture to be drawn</param>
		/// <param name="destinationRectangle">Rectangle coordinates where to draw the texture</param>
		/// <param name="sourceRectangle">Rectangle coordinates to draw part of the texture</param>
		/// <param name="color">Color to tint the texture</param>
		/// <param name="rotation">Rotation of the texture in Degrees</param>
		/// <param name="origin">Origin point of the texture to determine position/scale/rotation. (Upper left is 0,0)</param>
		/// <param name="SpriteEffects">Effects to be applied on the texture</param>
		/// <param name="layerDepth">Layer Depth for Sorting</param>
		/// <returns>XEResult::Ok if Draw was successful</returns>
		XEResult Draw(Texture2D* texture2D, const RECT& destinationRectangle, const RECT* sourceRectangle, const Color& color = XEColors::White, float rotation = 0.0f, const glm::vec2& origin = XEMathHelpers::Vec2fZero, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

		/// <summary>
		/// Lets the Sprite Batch know it is done processing textures & font
		/// </summary>
		/// <returns>XEResult::Ok if End was successful</returns>
		XEResult End();

#pragma endregion

};

#endif
