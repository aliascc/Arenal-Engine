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

#include <Structures.fx>

#include <ForwardPlusLights.fx>

//Forward Plus ends in t5
//start here in t6
Texture2D DiffuseTexture : register(t6);

SamplerState _AE_DiffuseTextureSampler : register(s1) = sampler_state
{
    MagFilter   = Linear;
    MinFilter   = Linear;
    MipFilter   = Linear;
    AddressU    = Wrap;
    AddressV    = Wrap;
};

float4 main(VSOutputPosNorTexPosWSDepthV input) : SV_Target0
{
    float4 textureColor = DiffuseTexture.Sample(_AE_DiffuseTextureSampler, input.TexCoord);

    LightOutput lightOutput = ForwardPlusLightCalculation(input.PositionWS, input.Normal, 0, input.Position.xy, input.DepthView, input.PosVtx);

    float4 finalColor = float4(textureColor.xyz * lightOutput.Color.xyz, 1.0f);

    return finalColor;
}
