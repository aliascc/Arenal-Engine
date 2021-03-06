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

//Structures
struct VertexInput
{
    float3 Position : POSITION0;
    float3 Normal    : NORMAL0;
    float2 TexCoord    : TEXCOORD0;
};

//Functions
float4 main(VertexInput input) : SV_POSITION
{
    return float4(0, 0, 0, 0);
}
