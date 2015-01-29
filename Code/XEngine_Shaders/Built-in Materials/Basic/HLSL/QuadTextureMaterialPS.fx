
#include "Structures.fx"

Texture2D _XE_QuadTexture : register(t0);

SamplerState _XE_QuadTextureSampler : register(s0) = sampler_state
{
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

float4 main(VSOutputPosTex input) : SV_Target0
{
	return _XE_QuadTexture.Sample(_XE_QuadTextureSampler, input.TexCoord);
}