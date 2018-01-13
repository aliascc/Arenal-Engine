/*
* Copyright (c) 2018 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

///////////////////////////////////////////////
//				Global Bloom				 //
///////////////////////////////////////////////

//////////////////////
//	Structs			//
//////////////////////
struct VertexShaderInput
{
    float3 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float2 TexCoord : TEXCOORD0;
};

//////////////////////
//	Variables		//
//////////////////////
uniform extern float2		HalfPixel		: HALF_PIXEL;

uniform extern Texture2D	SceneTexture	: SCENE_TEXTURE;
sampler SceneTextureSampler = sampler_state 
{
	Texture		= < SceneTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////
//	Functions		//
//////////////////////
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output=(VertexShaderOutput) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - HalfPixel;

	return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Extract Bloom				 //
///////////////////////////////////////////////

//////////////////////
//	Variables		//
//////////////////////
uniform extern float		BloomThreshold	: BLOOM_THRESHOLD;

//////////////////////
//	Functions		//
//////////////////////
float4 BloomExtractPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Look up the original image color.
    float4 c = tex2D(SceneTextureSampler, texCoord);

    // Adjust it to keep only values brighter than the specified threshold.
    return saturate((c - BloomThreshold) / (1 - BloomThreshold));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Gaussian Blur				 //
///////////////////////////////////////////////

#define SAMPLE_COUNT 15

uniform extern float2		SampleOffsets[SAMPLE_COUNT]		: SAMPLE_OFFSETS;
uniform extern float		SampleWeights[SAMPLE_COUNT]		: SAMPLE_WEIGHTS;

uniform extern Texture2D	GaussianBlurTexture				: GAUSSIAN_TEXTURE;
sampler GaussianBlurTextureSampler = sampler_state 
{
	Texture		= < GaussianBlurTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////
//	Functions		//
//////////////////////

float4 GaussianBlurPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR0
{
    float4 c = (float4)0;
    
    // Combine a number of weighted image filter taps.
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        c += (tex2D(GaussianBlurTextureSampler, texCoord + SampleOffsets[i]) * SampleWeights[i]);
    }
    
    return c;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Combine Bloom				 //
///////////////////////////////////////////////

// Pixel shader combines the bloom image with the original
// scene, using tweakable intensity levels and saturation.
// This is the final step in applying a bloom postprocess.

//////////////////////
//	Variables		//
//////////////////////
uniform extern float		BloomIntensity		: BLOOM_INTENSITY;
uniform extern float		BaseIntensity		: BASE_INTENSITY;

uniform extern float		BloomSaturation		: BLOOM_SATURATION;
uniform extern float		BaseSaturation		: BASE_SATURATION;

uniform extern Texture2D	PostBloomTexture	: POST_BLOOM_TEXTURE;
sampler PostBloomTextureSampler = sampler_state 
{
	Texture		= < PostBloomTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

//////////////////////
//	Functions		//
//////////////////////

// Helper for modifying the saturation of a color.
float4 AdjustSaturation(float4 color, float saturation)
{
    // The constants 0.3, 0.59, and 0.11 are chosen because the
    // human eye is more sensitive to green light, and less to blue.
    float grey = dot(color, float3(0.3, 0.59, 0.11));

    return lerp(grey, color, saturation);
}

float4 BloomCombinePixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR0
{
    // Look up the bloom and original base image colors.
    float4 bloom = tex2D(PostBloomTextureSampler, texCoord);
    float4 base = tex2D(SceneTextureSampler, texCoord);
    
    // Adjust color saturation and intensity.
    bloom = AdjustSaturation(bloom, BloomSaturation) * BloomIntensity;
    base = AdjustSaturation(base, BaseSaturation) * BaseIntensity;
    
    // Darken down the base image in areas where there is a lot of bloom,
    // to prevent things looking excessively burned-out.
    base *= (1 - saturate(bloom));
    
    // Combine the two images.
    return base + bloom;
}

///////////////////////////////////////////////
//				Techniques					 //
///////////////////////////////////////////////
technique BloomExtract
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 BloomExtractPixelShaderFunction();
    }
}

technique GaussianBlur
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 GaussianBlurPixelShaderFunction();
    }
}

technique BloomCombine
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 BloomCombinePixelShaderFunction();
    }
}