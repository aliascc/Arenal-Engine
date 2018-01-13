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

//Include Vertex Buffers
#include "Structures.fx"

//Define Constant Buffers
cbuffer _AE_CB_World_View_Proj : register(b0)
{
	matrix _AE_World		: packoffset(c0);
	matrix _AE_View			: packoffset(c4);
	matrix _AE_PROJection	: packoffset(c8);
};

cbuffer _AE_CB_Bones : register(b1)
{
	matrix _AE_BoneTransforms[MAX_BONES] : packoffset(c0);
};

//Include Skinning Functions
#include "Skinning.fx"

VSOutputPosNorTex main(VSInputPosNorTexIdxWght input)
{
	VSOutputPosNorTex output = (VSOutputPosNorTex) 0;

	float3 animatedPos = AE_CalculateSkinningPos(input.Position, input.BlendIndices, input.BlendWeight);
	float3 animatedNormal = AE_CalculateSkinningNormal(input.Normal, input.BlendIndices, input.BlendWeight);

	matrix vp = mul(_AE_PROJection, _AE_View);

	output.Position = mul(vp, float4(animatedPos, 1));
	output.TexCoord = input.TexCoord;
	output.Normal = mul(_AE_World, float4(animatedNormal, 0.0f)).xyz;

	return output;
}
