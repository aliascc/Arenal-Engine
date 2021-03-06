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

cbuffer _AE_CB_World_View_Proj : register(b0)
{
    matrix _AE_World         : packoffset(c0);
    matrix _AE_View          : packoffset(c4);
    matrix _AE_PROJection    : packoffset(c8);
};

VSOutputPosPosDepth main(VSInputPos input)
{
    VSOutputPosPosDepth output = (VSOutputPosPosDepth)0;

    matrix wvp = mul(_AE_PROJection, mul(_AE_View, _AE_World));

    output.Position = mul(wvp, float4(input.Position, 1));

    output.PosDepth.xy = output.Position.zw;

    return output;
}
