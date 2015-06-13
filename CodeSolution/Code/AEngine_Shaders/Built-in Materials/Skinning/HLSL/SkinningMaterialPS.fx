
#include "Structures.fx"

Texture2D _AE_DiffuseTexture : register(t0);

SamplerState _AE_DiffuseTextureSampler : register(s0) = sampler_state
{
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

float4 main(VSOutputPosNorTex input) : SV_Target0
{
	return _AE_DiffuseTexture.Sample(_AE_DiffuseTextureSampler, input.TexCoord);
}
