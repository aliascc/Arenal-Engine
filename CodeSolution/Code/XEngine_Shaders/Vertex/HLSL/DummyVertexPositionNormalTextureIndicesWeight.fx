
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/30/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

//Structures
struct VertexInput
{
	float3 Position		: POSITION0;
	float3 Normal		: NORMAL0;
	float2 TexCoord		: TEXCOORD0;
	uint4 BlendIndices	: BLENDINDICES0;
	float4 BlendWeight	: BLENDWEIGHT0;
};

//Functions
float4 main(VertexInput input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}