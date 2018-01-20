/*
* Copyright (c) 2018 <Carlos Chac�n>
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

#include "Structures.fx"

Texture2D _AE_DiffuseTexture : register(t0);

SamplerState _AE_DiffuseTextureSampler : register(s0) = sampler_state
{
    MagFilter    = Linear;
    MinFilter    = Linear;
    MipFilter    = Linear;
    AddressU     = Wrap;
    AddressV     = Wrap;
};

float4 main(VSOutputPosNorTex input) : SV_Target0
{
    return _AE_DiffuseTexture.Sample(_AE_DiffuseTextureSampler, input.TexCoord);
}
