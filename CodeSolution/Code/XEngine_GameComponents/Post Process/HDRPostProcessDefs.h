/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/25/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/
#pragma once

#ifndef _HDR_POST_PROCESS_DEFS_H
#define _HDR_POST_PROCESS_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\XEObject.h"
#include "Base\Base.h"

/********************************
*		Using					*
*********************************/
using namespace DirectX;
using namespace PackedVector;

/**************
*   Defines   *
***************/
#define XE_HDR_MAX_TONE_MAP_TEXS	4
#define XE_HDR_MAX_BLOOM_TEXS		3
#define XE_HDR_MAX_START_TEXS		12
#define XE_HDR_MAX_SAMPLES			16

/****************
*   Constants   *
*****************/

/************
*   Enums   *
*************/

/*************
*   Struct   *
**************/
struct StarLines sealed : public XEObject
{
	uint32_t	m_Passes;
	float		m_SampleLength;
	float		m_Attenuation;
	float		m_Inclination;

	StarLines();
};


/********************************
*		Classes					*
*********************************/
struct StarProp : public XEObject
{
	//Static Ready Star Props
	static const StarProp	Disable;
	static const StarProp	Cross;
	static const StarProp	CrossFilter;
	static const StarProp	SnowCross;
	static const StarProp	Vertical;

	static const XMCOLOR	m_ChromaticAberrationColor[8];

	std::string			m_Name;
	uint32_t			m_StarLineNum;
	StarLines*			m_StarLines;		// [m_nStarLines]
	float				m_Inclination;
	bool				m_Rotation;			// Is Rotation is available from outside

	//Constructor & Destructor
	StarProp();	
	StarProp(const StarProp& other);	
	StarProp(const std::string& name, uint32_t starLineNum, float inclination, bool rotation, uint32_t passes, float sampleLength, float attenuation);	
	virtual ~StarProp();

	//Methods
	StarProp&			operator=					(const StarProp& other);
	void				CleanUp						();
	XEResult			Initialize					(const std::string& name, uint32_t starLineNum, float inclination, bool rotation, uint32_t passes, float sampleLength, float attenuation);
};

struct StarPropSunny sealed : public StarProp
{
	//Constructor
	StarPropSunny(const std::string& name = "Sunny Cross Filter", float sampleLength = 1.0f, float attenuation = 0.88f, float longAttenuation = 0.95f, float inclination = XMConvertToRadians(0.0f));
	
	//Methods
	XEResult				Initialize					(const std::string& name, float sampleLength, float attenuation, float longAttenuation, float inclination);
};

struct GlareProp sealed : public XEObject
{
	//Static Ready Glare Prop
	static const GlareProp Disable;	
	static const GlareProp Camera;
	static const GlareProp Natural;
	static const GlareProp Cheaplens;
	static const GlareProp AfterImage;
	static const GlareProp FilterCrossScreen;
	static const GlareProp FilterCrossScreenSpectral;
	static const GlareProp FilterSnowCross;
	static const GlareProp FilterSnowCrossSpectral;
	static const GlareProp FilterSunnyCross;
	static const GlareProp FilterSunnyCrossSpectral;
	static const GlareProp CinemaVerticalLit;
	static const GlareProp CinemaHorizaontalLit;

	std::string		m_Name;
	float			m_GlareLuminance;			// Total glare intensity (not effect to "after image")
	float			m_BloomLuminance;
	float			m_GhostLuminance;
	float			m_GhostDistortion;
	float			m_StarLuminance;
	float			m_StarInclination;
	float			m_ChromaticAberration;
	float			m_AfterimageSensitivity;	// Current weight
	float			m_AfterimageRatio;			// Afterimage weight
	float			m_AfterimageLuminance;

	StarProp		m_StarProp;
		
	public:
		//Constructor & Destructor
		GlareProp();
		GlareProp(const std::string& name, float glareLuminance, float bloomLuminance, float ghostLuminance, float ghostDistortion, float starLuminance, const StarProp& starProp, float starInclination, float chromaticAberration, float afterimageSensitivity, float afterimageRatio, float afterimageLuminance);
		virtual ~GlareProp();

		//Methods
};		
													 		
#endif