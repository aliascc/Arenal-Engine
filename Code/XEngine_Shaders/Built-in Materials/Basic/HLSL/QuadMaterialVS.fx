
#include "Structures.fx"

cbuffer _XE_CB_HalfPixel : register(b0)
{
	float2 _XE_HalfPixel	: packoffset(c0);
};

VSOutputPosTex main(VSInputPosTex input)
{
	VSOutputPosTex output = (VSOutputPosTex) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - _XE_HalfPixel;

	return output;
}