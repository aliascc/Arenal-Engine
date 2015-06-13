
#include "Structures.fx"

cbuffer _AE_CB_World_View_Proj : register(b0)
{
	matrix _AE_World		: packoffset(c0);
	matrix _AE_View			: packoffset(c4);
	matrix _AE_PROJection	: packoffset(c8);
};

cbuffer _AE_CB_Bones : register(b1)
{
	matrix _AE_BoneTransforms[MAX_BONES] : packoffset(c0);
};

//Include Skinning Functions
#include "Skinning.fx"

VSOutputPosPosDepth main(VSInputPosIdxWght input)
{
	VSOutputPosPosDepth output = (VSOutputPosPosDepth)0;

	float3 animatedPos = AE_CalculateSkinningPos(input.Position, input.BlendIndices, input.BlendWeight);

	matrix wvp = mul(_AE_PROJection, mul(_AE_View, _AE_World));
	output.Position = mul(wvp, float4(animatedPos, 1));

	//output.Position = mul(mul(_AE_PROJection, _AE_View), float4(animatedPos, 1.0f));

	output.PosDepth.xy = output.Position.zw;

	return output;
}
