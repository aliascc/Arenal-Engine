
#ifndef _SKINNING_FX
#define _SKINNING_FX

#include <Structures.fx>

/***************************
*
* Skinning Functions 
*
****************************/

float3 AE_CalculateSkinningPos(float3 pos, uint4 indices, float4 weight)
{
	float3 retPos = (float3)0;
	float4 temp = float4(pos, 1);
	
	retPos  = mul(_AE_BoneTransforms[indices.x], temp).xyz * weight.x;
	retPos += mul(_AE_BoneTransforms[indices.y], temp).xyz * weight.y;
	retPos += mul(_AE_BoneTransforms[indices.z], temp).xyz * weight.z;
	retPos += mul(_AE_BoneTransforms[indices.w], temp).xyz * weight.w;
	
	return retPos;
}

float3 AE_CalculateSkinningNormal(float3 normal, uint4 indices, float4 weight)
{
	float3 retNormal	= (float3)0;
	float4 temp			= float4(normal, 0);
	float3 calNormal	= (float3)0;

	calNormal  = mul(_AE_BoneTransforms[indices.x], temp).xyz * weight.x;

	if(length(calNormal) != 0)
	{
		retNormal += normalize(calNormal);
	}

	calNormal = mul(_AE_BoneTransforms[indices.y], temp).xyz * weight.y;
	
	if(length(calNormal) != 0)
	{
		retNormal += normalize(calNormal);
	}

	calNormal = mul(_AE_BoneTransforms[indices.z], temp).xyz * weight.z;
	
	if(length(calNormal) != 0)
	{
		retNormal += normalize(calNormal);
	}

	calNormal = mul(_AE_BoneTransforms[indices.w], temp).xyz * weight.w;
	
	if(length(calNormal) != 0)
	{
		retNormal += normalize(calNormal);
	}

	if(length(retNormal) != 0)
	{
		retNormal = normalize(retNormal);
	}

	return retNormal;
}

#endif