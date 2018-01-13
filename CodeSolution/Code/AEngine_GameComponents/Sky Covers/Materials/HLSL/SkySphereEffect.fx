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

////////////////////////////////////
//	      Global Sky Sphere		 //
///////////////////////////////////

//////////////////////
//	Variables		//
//////////////////////

uniform extern float4x4		WorldViewProjection	: WORLDVIEWPROJECTION;
uniform extern TextureCube	SkyTexture			: SKY_TEXTURE;

sampler SkyTextureSampler = sampler_state 
{
	Texture		= < SkyTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	MipFilter	= Linear;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

struct VertexShaderInput
{
    float3 Position : POSITION0;
};

struct VertexShaderOutput
{
    float4 Position : POSITION0;
	float3 TexCoord : TEXCOORD0;
};

struct PixelShaderOuput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
};

////////////////////////////////////////
//  Vertex & Pixel Shader Functions   //
////////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output = (VertexShaderOutput) 0;

	output.Position = mul(float4(input.Position, 1.0f), WorldViewProjection).xyww;
	output.TexCoord = input.Position;

	return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	return texCUBE(SkyTextureSampler, input.TexCoord);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

PixelShaderOuput DeferredPixelShaderFunction(VertexShaderOutput input)
{
	PixelShaderOuput output = (PixelShaderOuput)0;

	
	output.DiffuseRT	= texCUBE(SkyTextureSampler, input.TexCoord);
	output.NormalRT		= float4(0.5f, 0.05f, 0.5f, 0.0f);
	output.DepthRT		= 1.0f;
	
	return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Techniques					 //
///////////////////////////////////////////////

technique SkySphereEffect
{
	pass Pass1
	{
		CullMode = None;
		//ZFunc = Always; // Always write sky to depth buffer
		//StencilEnable = true;
		//StencilFunc   = Always;
		//StencilPass   = Replace;
		//StencilRef    = 0; // clear to zero

		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 PixelShaderFunction();
	}
}

technique SkySphereDeferredEffect
{
	pass Pass1
	{
		CullMode = None;

		VertexShader = compile vs_3_0 VertexShaderFunction();
		PixelShader = compile ps_3_0 DeferredPixelShaderFunction();
	}
}