
#include "Structures.fx"

cbuffer _AE_CB_HalfPixel : register(b0)
{
	float2 _AE_HalfPixel	: packoffset(c0);
};

VSOutputPosTex main(VSInputPosTex input)
{
	VSOutputPosTex output = (VSOutputPosTex) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - _AE_HalfPixel;

	return output;
}