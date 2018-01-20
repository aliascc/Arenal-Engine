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

cbuffer _AE_CB_World_View_Proj : register(b0)
{
    matrix _AE_World        : packoffset(c0);
    matrix _AE_View         : packoffset(c4);
    matrix _AE_Projection   : packoffset(c8);
};

VSOutputPosNorTexPosWSDepthV main(VSInputPosNorTex input)
{
    VSOutputPosNorTexPosWSDepthV output = (VSOutputPosNorTexPosWSDepthV)0;

    matrix wvp = mul(_AE_Projection, mul(_AE_View, _AE_World));

    output.PositionWS    = mul(_AE_World, float4(input.Position, 1.0f)).xyz;

    output.PosVtx        = input.Position;
    output.Position      = mul(wvp, float4(input.Position, 1.0f));
    output.TexCoord      = input.TexCoord;
    output.Normal        = mul(_AE_World, float4(input.Normal, 0.0f)).xyz;
    output.DepthView     = mul(_AE_View, float4(output.PositionWS, 1.0f)).z;

    return output;
}
