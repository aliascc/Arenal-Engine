
#include <Structures.fx>

#include <ForwardPlusLights.fx>

//Forward Plus ends in t5
//start here in t6
Texture2D DiffuseTexture : register(t6);

SamplerState _XE_DiffuseTextureSampler : register(s1) = sampler_state
{
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

float4 main(VSOutputPosNorTexPosWSDepthV input) : SV_Target0
{
	float4 textureColor = DiffuseTexture.Sample(_XE_DiffuseTextureSampler, input.TexCoord);

	LightOutput lightOutput = ForwardPlusLightCalculation(input.PositionWS, input.Normal, 0, input.Position.xy, input.DepthView, input.PosVtx);

	float4 finalColor = float4(textureColor.xyz * lightOutput.Color.xyz, 1.0f);

	return finalColor;
}
