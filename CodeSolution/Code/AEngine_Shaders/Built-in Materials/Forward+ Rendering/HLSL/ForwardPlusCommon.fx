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

//Some Functions are extracted from the exampled of AMD Forward+ 11 (Jason Stewart)
//http://developer.amd.com/wordpress/media/2013/01/ForwardPlus11_v1.0.zip

#ifndef _FORWARD_PLUS_COMMON_FX
#define _FORWARD_PLUS_COMMON_FX

/****************************************************************
* Forward Plus Light Constants
****************************************************************/

#define LIGHT_NUM_CASCADE_MAPS		3
#define TILE_RES					16
#define MAX_NUM_LIGHTS_PER_TILE		128
#define FLT_MAX						0x7F7FFFFF
#define UINT_MAX					0xFFFFFFFF

#define LIGHT_INDEX_BUFFER_SENTINEL	0x7FFFFFFF

#define LIGHT_INVALID				0
#define LIGHT_SPOT					1
#define LIGHT_OMNI					2
#define LIGHT_DIRECTIONAL			3

#define LIGHT_FLAG_ENABLE			0x01
#define LIGHT_FLAG_SHADOWENABLE		0x02

/****************************************************************
* Forward Plus Light Structures
****************************************************************/

//float4 float2 float2 float3 uint float3 float uint uint
struct Light
{
	float4 Color;
	float NearAttenuation;
	float FarAttenuation;
	float FallOffAngle;
	float Angle;
	float3 Position;
	uint LightType;
	float3 Direction;
	float Intensity;
	uint LightFlags;
	uint ShadowTextureIndex;
};

//float4 float4 float4 float4
struct SpotLightShadowInfo
{
	matrix ViewProjectionMatrix;
};

struct LightOutput
{
	float4 Color;
	float4 SpecularColor;
	float SpecularShine;
};

struct LightCascadeInfo
{
	matrix m_CascadeViewProjectionMatrix[LIGHT_NUM_CASCADE_MAPS];
	float m_CascadeDepths[LIGHT_NUM_CASCADE_MAPS + 1];
};

/****************************************************************
* Helper functions
****************************************************************/

uint GetTileIndex(float2 screenPos, uint windowWidth)
{
	float tileRes = (float)TILE_RES;

	uint numCellsX = (windowWidth + TILE_RES - 1) / TILE_RES;

	uint tileIndex = floor(screenPos.x / tileRes) + floor(screenPos.y / tileRes) * numCellsX;

	return tileIndex;
}

#endif