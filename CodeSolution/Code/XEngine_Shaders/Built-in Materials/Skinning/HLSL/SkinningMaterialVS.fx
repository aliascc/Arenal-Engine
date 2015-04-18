
//Include Vertex Buffers
#include "Structures.fx"

//Define Constant Buffers
cbuffer _XE_CB_World_View_Proj : register(b0)
{
	matrix _XE_World		: packoffset(c0);
	matrix _XE_View			: packoffset(c4);
	matrix _XE_Projection	: packoffset(c8);
};

cbuffer _XE_CB_Bones : register(b1)
{
	matrix _XE_BoneTransforms[MAX_BONES] : packoffset(c0);
};

//Include Skinning Functions
#include "Skinning.fx"

VSOutputPosNorTex main(VSInputPosNorTexIdxWght input)
{
	VSOutputPosNorTex output = (VSOutputPosNorTex) 0;

	float3 animatedPos = XE_CalculateSkinningPos(input.Position, input.BlendIndices, input.BlendWeight);
	float3 animatedNormal = XE_CalculateSkinningNormal(input.Normal, input.BlendIndices, input.BlendWeight);

	matrix vp = mul(_XE_Projection, _XE_View);

	output.Position = mul(vp, float4(animatedPos, 1));
	output.TexCoord = input.TexCoord;
	output.Normal = mul(_XE_World, float4(animatedNormal, 0.0f)).xyz;

	return output;
}
