
#include "Structures.fx"

Texture2D _AE_QuadTexture : register(t0);

SamplerState _AE_QuadTextureSampler : register(s0) = sampler_state
{
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

float4 main(VSOutputPosTex input) : SV_Target0
{
	return _AE_QuadTexture.Sample(_AE_QuadTextureSampler, input.TexCoord);
}