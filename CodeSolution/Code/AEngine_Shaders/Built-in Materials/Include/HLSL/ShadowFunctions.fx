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

#ifndef _SHADOW_FUNCTIONS_FX
#define _SHADOW_FUNCTIONS_FX

#include <ForwardPlusCommon.fx>

static const float		CascadeShadowMapOffset	= (1.0f / LIGHT_NUM_CASCADE_MAPS);

//
////--------------------------------------------------------------------------------------
//// Use PCF to sample the depth map and return a percent lit value.
////--------------------------------------------------------------------------------------
//void CalculateVarianceShadow(int cascadeIndex, float4 cascadeScale, Texture2DArray shadowTextureArray, SamplerState shadowTextureSampler, uint shadowTextureIndex, float4 shadowTexCoord, float4 shadowMapTextureCoordViewSpace, int cascade, out float percentLit)
//{
//	percentLit = 0.0f;
//
//	float2 mapDepth = 0;
//
//	// In orderto pull the derivative out of divergent flow control we calculate the 
//	// derivative off of the view space coordinates an then scale the deriviative.
//
//	float3 shadowTexCoordDDX = ddx(shadowMapTextureCoordViewSpace).xyz;
//	shadowTexCoordDDX *= cascadeScale.xyz;
//
//	float3 shadowTexCoordDDY = ddy(shadowMapTextureCoordViewSpace).xyz;
//	shadowTexCoordDDY *= cascadeScale.xyz;
//
//	////////////////////////////////////////////////////////////
//	/////////////////////////////////////////////////////////////
//
//	float offset = CascadeShadowMapOffset * cascadeIndex;
//	shadowTexCoord.x = (shadowTexCoord.x * CascadeShadowMapOffset) + offset;
//
//	/////////////////////////////////////////////////////////////
//	/////////////////////////////////////////////////////////////
//
//	mapDepth = shadowTextureArray.SampleLevel(shadowTextureSampler, float3(shadowTexCoord.xy, shadowTextureIndex), 0).rg;
//
//	//mapDepth += shadowTextureArray.SampleGrad(shadowTextureSampler, float3(shadowTexCoord.xy, shadowTextureIndex), shadowTexCoordDDX.xy, shadowTexCoordDDY.xy).rg;
//
//	float avgZ	= mapDepth.x; // Filtered z
//	float avgZ2	= mapDepth.y; // Filtered z-squared
//
//	if (shadowTexCoord.w <= avgZ) // We put the z value in w so that we can index the texture array with Z.
//	{
//		percentLit = 1;
//	}
//	else
//	{
//		float variance = (avgZ2) - (avgZ * avgZ);
//
//		variance = min(1.0f, max(0.0f, variance + 0.00001f));
//
//		float mean = avgZ;
//		float d = shadowTexCoord.w - mean; // We put the z value in w so that we can index the texture array with Z.
//
//		float p_max = variance / (variance + d*d);
//
//		// To combat light-bleeding, experiment with raising p_max to some power
//		// (Try values from 0.1 to 100.0, if you like.)
//		percentLit = pow(p_max, 4);
//	}
//}
//
//void ComputeCoordinatesTransform(float4 cascadeScale, float4 cascadeOffset, int cascadeIndex, in out float4 shadowTexCoord, in out float4 shadowTexCoordViewSpace)
//{
//	// Now that we know the correct map, we can transform the world space position of the current fragment
//	if (SELECT_CASCADE_BY_INTERVAL_FLAG)
//	{
//		shadowTexCoord = shadowTexCoordViewSpace * cascadeScale;
//		shadowTexCoord += cascadeOffset;
//	}
//
//	shadowTexCoord.w = shadowTexCoord.z; // We put the z value in w so that we can index the texture array with Z.
//	shadowTexCoord.z = cascadeIndex;
//}
//
//float CalcShadowVarianceCascadeDirLight(float3 posVtx, float depthView, uint shadowTextureIndex, StructuredBuffer<LightCascadeInfo> lightInfoShadow, Texture2DArray shadowTextureArray, SamplerState shadowTextureSampler)
//{
//	float4 shadowMapTextureCoord = 0.0f;
//	float4 shadowMapTextureCoordViewSpace = mul(lightInfoShadow[shadowTextureIndex].m_ShadowViewMatrix, float4(posVtx, 1.0f));
//
//	float percentLit = 0.0f;
//
//	int cascadeFound = 0;
//	int nextCascadeIndex = 0;
//	int currentCascadeIndex = 0;
//
//	// This for loop is not necessary when the frustum is uniformly divided and interval based selection is used.
//	// In this case fCurrentPixelDepth could be used as an array lookup into the correct frustum. 
//	float currentPixelDepth = depthView;
//
//	if (SELECT_CASCADE_BY_INTERVAL_FLAG)
//	{
//		currentCascadeIndex = 0;
//		if (LIGHT_NUM_CASCADE_MAPS > 1)
//		{
//			//float4 currentPixelDepthVec = depthView;
//			float3 currentPixelDepthVec = depthView;
//
//			//Fix this
//			float3 cascadeDepthsTemp = float3(lightInfoShadow[shadowTextureIndex].m_CascadeDepths[0], lightInfoShadow[shadowTextureIndex].m_CascadeDepths[1], lightInfoShadow[shadowTextureIndex].m_CascadeDepths[2]);
//
//			float3 comparison = (currentPixelDepthVec > lightInfoShadow[shadowTextureIndex].m_CascadeDepths[0]);
//			//float4 comparison2 = (currentPixelDepthVec > lightInfoShadow[shadowTextureIndex].m_CascadeDepths[1]);
//
//			float3 indexCascadeMapComp = float3(LIGHT_NUM_CASCADE_MAPS > 0, LIGHT_NUM_CASCADE_MAPS > 1, LIGHT_NUM_CASCADE_MAPS > 2);
//			//float4 indexCascadeMapComp2 = float4(LIGHT_NUM_CASCADE_MAPS > 4, LIGHT_NUM_CASCADE_MAPS > 5, LIGHT_NUM_CASCADE_MAPS > 6, LIGHT_NUM_CASCADE_MAPS > 7);
//
//			float compDot1 = dot(indexCascadeMapComp, comparison);
//			//float compDot2 = dot(indexCascadeMapComp2, comparison2);
//
//			float index = compDot1;// +compDot2;
//
//			index = min(index, LIGHT_NUM_CASCADE_MAPS - 1);
//			currentCascadeIndex = (int)index;
//		}
//	}
//
//	float4 color = 0;
//
//	//(float4 cascadeScale, float4 cascadeOffset, int cascadeIndex, float4 interpolatedPosition, in out float4 shadowTexCoord, in out float4 shadowTexCoordViewSpace)
//	ComputeCoordinatesTransform(lightInfoShadow[shadowTextureIndex].m_CascadeScale[currentCascadeIndex], lightInfoShadow[shadowTextureIndex].m_CascadeOffset[currentCascadeIndex], currentCascadeIndex, shadowMapTextureCoord, shadowMapTextureCoordViewSpace);
//
//	//Because the Z coordinate specifies the texture array,
//	//the derivative will be 0 when there is no divergence
//	//float fDivergence = abs( ddy( vShadowMapTextureCoord.z ) ) +  abs( ddx( vShadowMapTextureCoord.z ) );
//	CalculateVarianceShadow(currentCascadeIndex, lightInfoShadow[shadowTextureIndex].m_CascadeScale[currentCascadeIndex], shadowTextureArray, shadowTextureSampler, shadowTextureIndex, shadowMapTextureCoord, shadowMapTextureCoordViewSpace, currentCascadeIndex, percentLit);
//
//	return percentLit;
//}


static const float		VSMMinVariance = 0.000001f;
static const float2		FPBias = float2(0.0f, 0.0f);
static const bool		LBREnable = true;						// Enable/disable light bleeding reduction
static const float		LBRAmount = 0.18f;					// Aggressiveness of light bleeding reduction

float2 ComputeMoments(float depth)
{
	// Compute first few moments of depth
	float2 moments;

	moments.x = depth;
	moments.y = depth * depth;

	return moments;
}

float LBR(float p)
{
	return clamp((p - LBRAmount) / (1 - LBRAmount), 0, 1);
}

float ChebyshevUpperBound(float2 moments, float mean, float minVariance)
{
	// Standard shadow map comparison
	float p = (mean <= moments.x);

	// Compute variance
	float variance = moments.y - (moments.x * moments.x);
	variance = max(variance, minVariance);

	// Compute probabilistic upper bound
	float d = mean - moments.x;
	float p_max = variance / (variance + d*d);

	return max(p, p_max);
}

float CalcShadowVarianceCascadeDirLightSimple(float3 position, float depthView, uint shadowTextureIndex, StructuredBuffer<LightCascadeInfo> lightInfoShadow, Texture2DArray shadowTextureArray, SamplerState shadowTextureSampler)
{
	uint cascadeIndex = 0;

	if (abs(depthView) >= lightInfoShadow[shadowTextureIndex].m_CascadeDepths[1])
	{
		cascadeIndex = 2;
	}
	else if (abs(depthView) >= lightInfoShadow[shadowTextureIndex].m_CascadeDepths[0])
	{
		cascadeIndex = 1;
	}
	else
	{
		cascadeIndex = 0;
	}

	/////////////////////////////////////////////////////////////

	matrix lvp = lightInfoShadow[shadowTextureIndex].m_CascadeViewProjectionMatrix[cascadeIndex];// m_ShadowProjMatrix[cascadeIndex] * lightInfoShadow[shadowTextureIndex].m_ShadowViewMatrix;

	float4 shadowPos = mul(lvp, float4(position, 1.0f));

	////////////////////////////////////////////////////////////////////
	//Project the texture coords and scale/offset to [0, 1].
	float2 shadowTexC = shadowPos.xy / shadowPos.w;
	shadowTexC.x = 0.5f * shadowTexC.x + 0.5f;
	shadowTexC.y = -0.5f * shadowTexC.y + 0.5f;

	float rescaledDist = shadowPos.z / shadowPos.w;

	////////////////////////////////////////////////////////////////////

	float offset = CascadeShadowMapOffset * cascadeIndex;
	shadowTexC.x = (shadowTexC.x * CascadeShadowMapOffset) + offset;

	////////////////////////////////////////////////////////////////////

	float2 moments = shadowTextureArray.SampleLevel(shadowTextureSampler, float3(shadowTexC, shadowTextureIndex), 0).rg;
	moments = moments;

	float shadowContrib = ChebyshevUpperBound(moments, rescaledDist, VSMMinVariance);

	if (LBREnable)
	{
		shadowContrib = LBR(shadowContrib);
	}

	return shadowContrib;
}

float CalcShadowVarianceSpotLight(float3 position, uint shadowTextureIndex, StructuredBuffer<SpotLightShadowInfo> spotLightInfoShadow, Texture2DArray shadowTextureArray, SamplerState shadowTextureSampler)
{
	float4 shadowPos = mul(spotLightInfoShadow[shadowTextureIndex].ViewProjectionMatrix, float4(position, 1.0f));

	////////////////////////////////////////////////////////////////////
	//Project the texture coords and scale/offset to [0, 1].
	float2 shadowTexC = shadowPos.xy / shadowPos.w;
	shadowTexC.x = 0.5f * shadowTexC.x + 0.5f;
	shadowTexC.y = -0.5f * shadowTexC.y + 0.5f;

	float rescaledDist = shadowPos.z / shadowPos.w;

	float2 moments = shadowTextureArray.SampleLevel(shadowTextureSampler, float3(shadowTexC, shadowTextureIndex), 0).rg;
	moments = moments;

	float shadowContrib = ChebyshevUpperBound(moments, rescaledDist, VSMMinVariance);

	if (LBREnable)
	{
		shadowContrib = LBR(shadowContrib);
	}

	return shadowContrib;
}

#endif