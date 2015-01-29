
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
	float3 Position : POSITION0;
};

//Functions
float4 main(VertexInput input) : SV_POSITION
{
	return float4(0, 0, 0, 0);
}