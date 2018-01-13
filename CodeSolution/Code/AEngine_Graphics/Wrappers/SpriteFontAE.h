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

#pragma once

#ifndef _SPRITE_FONT_AE_H
#define _SPRITE_FONT_AE_H

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
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"
#include "Color\AEColorDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class SpriteBatchAE;

/*****************
*   Class Decl   *
******************/
class SpriteFontAE sealed : public AEObject
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
		/// Default SpriteFontAE Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		/// <param name="spriteFontFile">Sprite Font file to open</param>
		SpriteFontAE(GraphicDevice* graphicDevice, const std::wstring spriteFontFile);

		/// <summary>
		/// Default SpriteFontAE Destructor
		/// </summary>
		virtual ~SpriteFontAE();

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
		/// <returns>AE_GEN_OK if Initialize was successful</returns>
		AEResult Initialize();

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
		/// <returns>AEResult::Ok if Begin was successful</returns>
		AEResult DrawString(const SpriteBatchAE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color = AEColors::White, float rotation = 0.0f, const glm::vec2& origin = AEMathHelpers::Vec2fZero, float scale = 1.0f, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

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
		/// <returns>AEResult::Ok if Begin was successful</returns>
		AEResult DrawString(const SpriteBatchAE* spriteBatch, const std::wstring& text, const glm::vec2& position, const Color& color, float rotation, const glm::vec2& origin, const glm::vec2& scale, DirectX::SpriteEffects effects = DirectX::SpriteEffects_None, float layerDepth = 0.0f);

		/// <summary>
		/// Measures a string
		/// </summary>
		/// <returns>Size that string will occupy</returns>
		glm::vec2 MeasureString(const std::wstring& text) const;

#pragma endregion

};

#endif
