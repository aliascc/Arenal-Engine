
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

#pragma once

#ifndef _SPRITE_FONT_XE_H
#define _SPRITE_FONT_XE_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "DirectXTK\SpriteFont.h"
#include "glm\glm.hpp"

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
class SpriteBatchXE;

/*****************
*   Class Decl   *
******************/
class SpriteFontXE sealed : public XEObject
{
	private:

		/************************
		*   Private Variables   *
		*************************/
#pragma region Private Variables

		/// <summary>
		/// DirectXTK Sprite Font Instance
		/// </summary>
		DirectX::SpriteFont* m_SpriteFont = nullptr;

		/// <summary>
		/// Lets know if the Sprite Batch is ready for use
		/// </summary>
		bool m_IsReady = false;

		/// <summary>
		/// Graphic Device associated with the object
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

		/// <summary>
		/// Sprite Font File to get font from 
		/// </summary>
		std::wstring m_SpriteFontFile = L"";

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default SpriteFontXE Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		/// <param name="spriteFontFile">Sprite Font file to open</param>
		SpriteFontXE(GraphicDevice* graphicDevice, const std::wstring spriteFontFile);

		/// <summary>
		/// Default SpriteFontXE Destructor
		/// </summary>
		virtual ~SpriteFontXE();

#pragma endregion

		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		/// <summary>
		/// Returns if Sprite Font has been initialized
		/// </summary>
		/// <returns>True if Sprite Font has been Initialized</returns>
		inline bool IsReady() const
		{
			return m_IsReady;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		/// <summary>
		/// Initializes the Sprite Font for use
		/// </summary>
		/// <returns>XE_GEN_OK if Initialize was successful</returns>
		XEResult Initialize();

		/// <summary>
		/// Lets the Sprite Batch know it is time to start processing textures & font
		/// </summary>
		/// <param name="spriteBatch">Sprite Batch that will be use to render text</param>
		/// <param name="text">Text to render</param>
		/// <param name="position">Position where to draw the text</param>
		/// <param name="color">Color of the texture</param>
		/// <param name="rotation">Rotation to apply</param>
		/// <param name="origin">Origin point of the texture to determine position/scale/rotation. (Upper left is 0,0)</param>
		/// <param name="scale">Scale of the texture</param>
		/// <param name="SpriteEffects">Effects to be applied on the texture</param>
		/// <param name="layerDepth">Layer Depth for Sorting</param>
		/// <returns>XEResult::Ok if Begin was successful</returns>
		XEResult DrawString(const SpriteBatchXE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color = XEColors::White, float rotation = 0.0f, const glm::vec2& origin = XEMathHelpers::Vec2fZero, float scale = 1.0f, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

		/// <summary>
		/// Lets the Sprite Batch know it is time to start processing textures & font
		/// </summary>
		/// <param name="spriteBatch">Sprite Batch that will be use to render text</param>
		/// <param name="text">Text to render</param>
		/// <param name="position">Position where to draw the text</param>
		/// <param name="color">Color of the texture</param>
		/// <param name="rotation">Rotation to apply</param>
		/// <param name="origin">Origin point of the texture to determine position/scale/rotation. (Upper left is 0,0)</param>
		/// <param name="scale">Scale of the texture</param>
		/// <param name="SpriteEffects">Effects to be applied on the texture</param>
		/// <param name="layerDepth">Layer Depth for Sorting</param>
		/// <returns>XEResult::Ok if Begin was successful</returns>
		XEResult DrawString(const SpriteBatchXE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

		/// <summary>
		/// Measures a string
		/// </summary>
		/// <returns>Size that string will occupy</returns>
		glm::vec2 MeasureString(const std::wstring& text) const;

#pragma endregion

};

#endif
