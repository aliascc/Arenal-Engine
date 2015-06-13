
#include <Structures.fx>

Texture2D DiffuseTexture : register(t0);

SamplerState _AE_DiffuseTextureSampler : register(s0);

float4 main(VSOutputPosTex input) : SV_Target0
{
	float4 textColor = DiffuseTexture.Sample(_AE_DiffuseTextureSampler, input.TexCoord);

	clip(textColor.a < 0.1f ? -1 : 1);

	return textColor;
}