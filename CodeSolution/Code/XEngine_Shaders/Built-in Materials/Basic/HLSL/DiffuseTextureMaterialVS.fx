
#include <Structures.fx>

cbuffer _XE_CB_World_View_Proj : register(b0)
{
	matrix _XE_World		: packoffset(c0);
	matrix _XE_View			: packoffset(c4);
	matrix _XE_Projection	: packoffset(c8);
};

VSOutputPosNorTexPosWSDepthV main(VSInputPosNorTex input)
{
	VSOutputPosNorTexPosWSDepthV output = (VSOutputPosNorTexPosWSDepthV)0;

	matrix wvp = mul(_XE_Projection, mul(_XE_View, _XE_World));

	output.PositionWS	= mul(_XE_World, float4(input.Position, 1.0f)).xyz;

	output.PosVtx		= input.Position;
	output.Position		= mul(wvp, float4(input.Position, 1.0f));
	output.TexCoord		= input.TexCoord;
	output.Normal		= mul(_XE_World, float4(input.Normal, 0.0f)).xyz;
	output.DepthView	= mul(_XE_View, float4(output.PositionWS, 1.0f)).z;

	return output;
}
