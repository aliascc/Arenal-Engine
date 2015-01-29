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

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "HDRPostProcessDefs.h"

/********************************
*		Variable Defs			*
*********************************/

#pragma region StarProp Variables

const StarProp StarProp::Disable		= StarProp("Disable",		0,	XMConvertToRadians(0.0f),	false,	0,	0.0f,	0.0f);
const StarProp StarProp::Cross			= StarProp("Cross",			4,	XMConvertToRadians(0.0f),	true,	3,	1.0f,	0.85f);
const StarProp StarProp::CrossFilter	= StarProp("Cross Filter",	4,	XMConvertToRadians(0.0f),	true,	3,	1.0f,	0.95f);
const StarProp StarProp::SnowCross		= StarProp("Snow Cross",	6,	XMConvertToRadians(20.0f),	true,	3,	1.0f,	0.96f);
const StarProp StarProp::Vertical		= StarProp("Vertical",		2,	XMConvertToRadians(0.0f),	false,	3,	1.0f,	0.96f);

const XMCOLOR StarProp::m_ChromaticAberrationColor[8] =
{
	XMCOLOR( 0.5f, 0.5f, 0.5f, 0.0f ), // w
	XMCOLOR( 0.8f, 0.3f, 0.3f, 0.0f ),
	XMCOLOR( 1.0f, 0.2f, 0.2f, 0.0f ), // r
	XMCOLOR( 0.5f, 0.2f, 0.6f, 0.0f ),
	XMCOLOR( 0.2f, 0.2f, 1.0f, 0.0f ), // b
	XMCOLOR( 0.2f, 0.3f, 0.7f, 0.0f ),
	XMCOLOR( 0.2f, 0.6f, 0.2f, 0.0f ), // g
	XMCOLOR( 0.3f, 0.5f, 0.3f, 0.0f )
};

#pragma endregion

#pragma region GlareProp Variables
														
const GlareProp GlareProp::Disable						= GlareProp("Disable",							0.0f,	0.0f,	0.0f,	0.01f,	0.0f,	StarProp::Disable,		XMConvertToRadians(0.00f),	0.5f,	0.00f,	0.00f,	0.0f);
const GlareProp GlareProp::Camera						= GlareProp("Camera",							1.5f,	1.2f,	1.0f,	0.00f,	1.0f,	StarProp::Cross,		XMConvertToRadians(0.00f),	0.5f,	0.25f,	0.90f,	1.0f);
const GlareProp GlareProp::Natural						= GlareProp("Natural Bloom",					1.5f,	1.2f,	0.0f,	0.00f,	0.0f,	StarProp::Disable,		XMConvertToRadians(00.0f),	0.0f,	0.40f,	0.85f,	0.5f);
const GlareProp GlareProp::Cheaplens					= GlareProp("Cheap Lens Camera",				1.25f,	2.0f,	1.5f,	0.05f,	2.0f,	StarProp::Cross,		XMConvertToRadians(00.0f),	0.5f,	0.18f,	0.95f,	1.0f);
const GlareProp GlareProp::AfterImage					= GlareProp("After Image",						1.5f,	1.2f,	0.5f,	0.00f,	0.7f,	StarProp::Cross,		XMConvertToRadians(00.0f),	0.5f,	0.10f,	0.98f,	2.0f);
const GlareProp GlareProp::FilterCrossScreen			= GlareProp("Filter Cross Screen",				1.0f,	2.0f,	1.7f,	0.00f,	1.5f,	StarProp::CrossFilter,	XMConvertToRadians(25.0f),	0.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::FilterCrossScreenSpectral	= GlareProp("Filter Cross Screen Spectral",		1.0f,	2.0f,	1.7f,	0.00f,	1.8f,	StarProp::CrossFilter,	XMConvertToRadians(70.0f),	1.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::FilterSnowCross				= GlareProp("Filter Snow Cross",				1.0f,	2.0f,	1.7f,	0.00f,	1.5f,	StarProp::SnowCross,	XMConvertToRadians(10.0f),	0.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::FilterSnowCrossSpectral		= GlareProp("Filter Snow Cross Spectral",		1.0f,	2.0f,	1.7f,	0.00f,	1.8f,	StarProp::SnowCross,	XMConvertToRadians(40.0f),	1.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::FilterSunnyCross				= GlareProp("Filter Sunny Cross",				1.0f,	2.0f,	1.7f,	0.00f,	1.5f,	StarPropSunny(),		XMConvertToRadians(00.0f),	0.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::FilterSunnyCrossSpectral		= GlareProp("Filter Sunny Cross Spectral",		1.0f,	2.0f,	1.7f,	0.00f,	1.8f,	StarPropSunny(),		XMConvertToRadians(45.0f),	1.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::CinemaVerticalLit			= GlareProp("Cinema Vertical Lit",				1.0f,	2.0f,	1.5f,	0.00f,	1.0f,	StarProp::Vertical,		XMConvertToRadians(90.0f),	0.5f,	0.20f,	0.93f,	1.0f);
const GlareProp GlareProp::CinemaHorizaontalLit			= GlareProp("Cinema Vertical Lit",				1.0f,	2.0f,	1.5f,	0.00f,	1.0f,	StarProp::Vertical,		XMConvertToRadians(00.0f),	0.5f,	0.20f,	0.93f,	1.0f);

#pragma endregion

/********************
*   Function Defs   *
*********************/

/*********************************
*		StarLines				 *
**********************************/
#pragma region struct StarLines

StarLines::StarLines()
	: m_Passes(0)
	, m_SampleLength(0.0f)
	, m_Attenuation(0.0f)
	, m_Inclination(0.0f)
{
}

#pragma endregion

/*********************************
*		StarProp				 *
**********************************/
#pragma region struct StarProp

StarProp::StarProp()
	: m_Name("") 
	, m_StarLineNum(0)
	, m_StarLines(nullptr)
	, m_Inclination(0.0f)
	, m_Rotation(false)
{
}

StarProp::StarProp(const StarProp& other)
{
	this->m_Name = other.m_Name;
	this->m_StarLineNum = other.m_StarLineNum;
	this->m_Inclination = other.m_Inclination;

	if(other.m_StarLines != nullptr)
	{
		this->m_StarLines = new StarLines[m_StarLineNum];
		memcpy(m_StarLines, other.m_StarLines, sizeof(StarLines) * m_StarLineNum);
	}
}

StarProp& StarProp::operator=(const StarProp& other)
{
	this->m_Name = other.m_Name;
	this->m_StarLineNum = other.m_StarLineNum;
	this->m_Inclination = other.m_Inclination;

	if(other.m_StarLines != nullptr)
	{
		this->m_StarLines = new StarLines[m_StarLineNum];
		memcpy(m_StarLines, other.m_StarLines, sizeof(StarLines) * m_StarLineNum);
	}

	return *this;
}

StarProp::StarProp(const std::string& name, uint32_t starLineNum, float inclination, bool rotation, uint32_t passes, float sampleLength, float attenuation)
{
	Initialize(name, starLineNum, inclination, rotation, passes, sampleLength, attenuation);
}

StarProp::~StarProp()
{
	CleanUp();
}

void StarProp::CleanUp()
{
	m_StarLineNum = 0;
	DeleteMemArr(m_StarLines);
}

XEResult StarProp::Initialize(const std::string& name, uint32_t starLineNum, float inclination, bool rotation, uint32_t passes, float sampleLength, float attenuation)
{
	if(starLineNum == 0)
	{
		return XEResult::Fail;
	}

	CleanUp();

	// Copy from parameters
	m_Name = name;
	m_StarLineNum = starLineNum;
	m_Inclination = inclination;
	m_Rotation = rotation;

	m_StarLines = new StarLines[m_StarLineNum];
	
	float inc = XMConvertToRadians(360.0f / (float) m_StarLineNum);
	
	for(uint32_t i = 0; i < m_StarLineNum; ++i)
	{
		m_StarLines[i].m_Passes = passes;
		m_StarLines[i].m_SampleLength = sampleLength;
		m_StarLines[i].m_Attenuation = attenuation;
		m_StarLines[i].m_Inclination = inc * (float)i;
	}

	return XEResult::Ok;
}

#pragma endregion

/*********************************
*		StarPropSunny			 *
**********************************/
#pragma region  struct StarPropSunny

StarPropSunny::StarPropSunny(const std::string& name, float sampleLength, float attenuation, float longAttenuation, float inclination)
{
	Initialize(name, sampleLength, attenuation, longAttenuation, inclination);
}

XEResult StarPropSunny::Initialize(const std::string& name, float sampleLength, float attenuation, float longAttenuation, float inclination)
{
	CleanUp();
	
    // Create parameters
    m_Name = name;

    m_StarLineNum = 8;
    m_Inclination = inclination;
    m_Rotation = false;

	m_StarLines = new StarLines[m_StarLineNum];

	float inc = XMConvertToRadians( 360.0f / (float)m_StarLineNum);

	for(uint32_t i = 0; i < m_StarLineNum; ++i)
    {
        m_StarLines[i].m_SampleLength = sampleLength;
		m_StarLines[i].m_Inclination = inc * (float)i + XMConvertToRadians( 0.0f );

        if(0 == (i % 2))
        {
            m_StarLines[i].m_Passes = 3;
            m_StarLines[i].m_Attenuation = longAttenuation;    // long
        }
        else
        {
            m_StarLines[i].m_Passes = 3;
            m_StarLines[i].m_Attenuation = attenuation;
        }
    }

	return XEResult::Ok;
}

#pragma endregion

/*********************************
*		GlareProp				 *
**********************************/
#pragma region struct GlareProp

GlareProp::GlareProp()
	: m_Name("")
	, m_GlareLuminance(0.0f)
	, m_BloomLuminance(0.0f)
	, m_GhostLuminance(0.0f)
	, m_GhostDistortion(0.0f)
	, m_StarLuminance(0.0f)
	, m_StarInclination(0.0f)
	, m_ChromaticAberration(0.0f)
	, m_AfterimageSensitivity(0.0f)
	, m_AfterimageRatio(0.0f)
	, m_AfterimageLuminance(0.0f)
{
}

GlareProp::~GlareProp()
{
}

GlareProp::GlareProp(const std::string& name, float glareLuminance, float bloomLuminance, float ghostLuminance, float ghostDistortion, float starLuminance, const StarProp& starProp, float starInclination, float chromaticAberration, float afterimageSensitivity, float afterimageRatio, float afterimageLuminance)
{
    m_Name					= name;
    m_GlareLuminance		= glareLuminance;
    m_BloomLuminance		= bloomLuminance;
    m_GhostLuminance		= ghostLuminance;
    m_GhostDistortion		= ghostDistortion;
    m_StarLuminance			= starLuminance;
    m_StarInclination		= starInclination;
    m_ChromaticAberration	= chromaticAberration;
    m_AfterimageSensitivity	= afterimageSensitivity;
    m_AfterimageRatio		= afterimageRatio;
    m_AfterimageLuminance	= afterimageLuminance;
	m_StarProp				= starProp;	
}

#pragma endregion