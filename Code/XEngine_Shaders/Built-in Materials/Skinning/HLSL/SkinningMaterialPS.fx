
#include "Structures.fx"

Texture2D _XE_DiffuseTexture : register(t0);

SamplerState _XE_DiffuseTextureSampler : register(s0) = sampler_state
{
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

float4 main(VSOutputPosNorTex input) : SV_Target0
{
	return _XE_DiffuseTexture.Sample(_XE_DiffuseTextureSampler, input.TexCoord);
}
