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
	float4 Color	: COLOR0;
};

//Functions
float4 main(VertexInput input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}