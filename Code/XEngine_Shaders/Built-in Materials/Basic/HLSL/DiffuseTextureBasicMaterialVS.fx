
#include "Structures.fx"

cbuffer _XE_CB_World_View_Proj : register(b0)
{
	matrix _XE_World		: packoffset(c0);
	matrix _XE_View			: packoffset(c4);
	matrix _XE_Projection	: packoffset(c8);
};

VSOutputPosTex main(VSInputPosTex input)
{
	VSOutputPosTex output = (VSOutputPosTex) 0;

	matrix wvp = mul(_XE_Projection, mul(_XE_View, _XE_World));

	output.Position = mul(wvp, float4(input.Position, 1.0f));
	output.TexCoord = input.TexCoord;

	return output;
}
