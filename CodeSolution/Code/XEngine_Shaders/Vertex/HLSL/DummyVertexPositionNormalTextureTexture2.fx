
/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 8/3/2012
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
	float2 TexCoord2	: TEXCOORD1;
};

//Functions
float4 main(VertexInput input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}