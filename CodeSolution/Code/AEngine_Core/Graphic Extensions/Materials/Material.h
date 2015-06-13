/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _MATERIAL_H
#define _MATERIAL_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <vector>
#include <d3d11_1.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\Named.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class HullShader;
class PixelShader;
class DomainShader;
class VertexShader;
class ComputeShader;
class GeometryShader;
class ShaderProperties;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class Material abstract : public Named
{
	protected:

		/**************************
		*   Protected Variables   *
		***************************/
#pragma region Protected Variables

		bool m_IsReady = false;

		GameResourceManager* m_GameResourceManager = nullptr;

		/// <summary>
		/// Graphic Device associated with the object
		/// </summary>
		GraphicDevice* m_GraphicDevice = nullptr;

		VertexShader* m_VertexShader = nullptr;
		ShaderProperties* m_VSProps = nullptr;

		PixelShader* m_PixelShader = nullptr;
		ShaderProperties* m_PSProps = nullptr;

		GeometryShader* m_GeometryShader = nullptr;
		ShaderProperties* m_GSProps = nullptr;

		DomainShader* m_DomainShader = nullptr;
		ShaderProperties* m_DSProps = nullptr;

		HullShader* m_HullShader = nullptr;
		ShaderProperties* m_HSProps = nullptr;

		ComputeShader* m_ComputeShader = nullptr;
		ShaderProperties* m_CSProps = nullptr;

#pragma endregion

		/************************
		*   Protected Methods   *
		*************************/
#pragma region Protected Methods
		
		void CleanUp();

#pragma endregion

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// Default Material Constructor
		/// </summary>
		/// <param name="graphicDevice">Graphic Device to be associated with</param>
		/// <param name="gameResourceManager">Game Resource Manager to get the manage resources</param>
		/// <param name="name">Name of the Material</param>
		Material(GraphicDevice* graphicDevice, GameResourceManager* gameResourceManager, const std::wstring& name = L"");

		/// <summary>
		/// Default Material Destructor
		/// </summary>
		virtual ~Material();

#pragma endregion
		
		/******************
		*   Get Methods   *
		*******************/
#pragma region Get Methods

		inline ShaderProperties* GetVSProps() 
		{
			return m_VSProps;
		}

		inline ShaderProperties* GetPSProps()
		{
			return m_PSProps;
		}
		
		inline ShaderProperties* GetGSProps() 
		{
			return m_GSProps;
		}
		
		inline ShaderProperties* GetDSProps() 
		{
			return m_DSProps;
		}
		
		inline ShaderProperties* GetHSProps() 
		{
			return m_HSProps;
		}
		
		inline ShaderProperties* GetCSProps() 
		{
			return m_CSProps;
		}

#pragma endregion
		
		/******************
		*   Set Methods   *
		*******************/
#pragma region Set Methods
		
		void SetVertexShader(VertexShader* shader, bool releaseOld = false);

		void SetPixelShader(PixelShader* shader, bool releaseOld = false);

		void SetGeometryShader(GeometryShader* shader, bool releaseOld = false);

		void SetDomainShader(DomainShader* shader, bool releaseOld = false);

		void SetHullShader(HullShader* shader, bool releaseOld = false);

		void SetComputeShader(ComputeShader* shader, bool releaseOld = false);

		void SetVSProps(ShaderProperties* shaderProps)
		{
			m_VSProps = shaderProps;
		}
		
		void SetPSProps(ShaderProperties* shaderProps)
		{
			m_PSProps = shaderProps;
		}
		
		void SetGSProps(ShaderProperties* shaderProps)
		{
			m_GSProps = shaderProps;
		}
		
		void SetDSProps(ShaderProperties* shaderProps)
		{
			m_DSProps = shaderProps;
		}
		
		void SetHSProps(ShaderProperties* shaderProps)
		{
			m_HSProps = shaderProps;
		}
		
		void SetCSProps(ShaderProperties* shaderProps)
		{
			m_CSProps = shaderProps;
		}

#pragma endregion

		/************************
		*   Framework Methods   *
		*************************/
#pragma region Framework Methods

		virtual AEResult LoadContent() = 0;

		AEResult Apply();

		AEResult UnApply();

#pragma endregion

		/**********************
		*   Deleted Methods   *
		***********************/
#pragma region Protected Methods

		Material(Material const&) = delete;

		Material& operator= (Material const&) = delete;

#pragma endregion

};

#endif