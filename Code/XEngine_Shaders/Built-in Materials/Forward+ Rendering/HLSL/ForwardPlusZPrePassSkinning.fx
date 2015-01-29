
#include <Structures.fx>

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

VSOuputPos main(VSInputPosIdxWght input)
{
	VSOuputPos output = (VSOuputPos)0;

	float3 animatedPos = XE_CalculateSkinningPos(input.Position, input.BlendIndices, input.BlendWeight);

	matrix wvp = mul(_XE_Projection, mul(_XE_View, _XE_World));
	output.Position = mul(wvp, float4(animatedPos, 1));

	//output.Position		= mul(mul(_XE_Projection, _XE_View), float4(animatedPos, 1.0f));

	return output;
}
