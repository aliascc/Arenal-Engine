
#include <Structures.fx>

cbuffer _AE_CB_World_View_Proj : register(b0)
{
	matrix _AE_World		: packoffset(c0);
	matrix _AE_View			: packoffset(c4);
	matrix _AE_Projection	: packoffset(c8);
};

VSOutputPosNorTexPosWSDepthV main(VSInputPosNorTex input)
{
	VSOutputPosNorTexPosWSDepthV output = (VSOutputPosNorTexPosWSDepthV)0;

	matrix wvp = mul(_AE_Projection, mul(_AE_View, _AE_World));

	output.PositionWS	= mul(_AE_World, float4(input.Position, 1.0f)).xyz;

	output.PosVtx		= input.Position;
	output.Position		= mul(wvp, float4(input.Position, 1.0f));
	output.TexCoord		= input.TexCoord;
	output.Normal		= mul(_AE_World, float4(input.Normal, 0.0f)).xyz;
	output.DepthView	= mul(_AE_View, float4(output.PositionWS, 1.0f)).z;

	return output;
}
