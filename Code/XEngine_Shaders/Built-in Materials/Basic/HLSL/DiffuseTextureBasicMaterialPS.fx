
#include <Structures.fx>

Texture2D DiffuseTexture : register(t0);

SamplerState _XE_DiffuseTextureSampler : register(s0);

float4 main(VSOutputPosTex input) : SV_Target0
{
	float4 textColor = DiffuseTexture.Sample(_XE_DiffuseTextureSampler, input.TexCoord);

	clip(textColor.a < 0.1f ? -1 : 1);

	return textColor;
}