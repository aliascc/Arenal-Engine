/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

//Structures
struct VertexInput
{
	float3 Position : POSITION0;
	float3 Normal	: NORMAL0;
	float4 Tangent	: TANGENT0;
	float4 Binormal	: BINORMAL0;
	float2 TexCoord	: TEXCOORD0;
};

//Functions
float4 main(VertexInput input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}