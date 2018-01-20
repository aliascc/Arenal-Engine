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

#ifndef _STRUCTURES_FX
#define _STRUCTURES_FX

//Put this in a global include
//Max Bones Supported by this Vertex Shader
#define MAX_BONES 96

/***************************
*
* Vertex struct
*
****************************/

/***************************
* Vertex Input
****************************/
struct VSInputPosColor
{
    float3 Position     : POSITION0;
    float4 Color        : COLOR0;
};

struct VSInputPos
{
    float3 Position     : POSITION0;
};

struct VSInputPosIdxWght
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL0;
    float2 TexCoord     : TEXCOORD0;
    uint4 BlendIndices  : BLENDINDICES0;
    float4 BlendWeight  : BLENDWEIGHT0;
};

struct VSInputPosTex
{
    float3 Position     : POSITION0;
    float2 TexCoord     : TEXCOORD0;
};

struct VSInputPosNorTex
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL0;
    float2 TexCoord     : TEXCOORD0;
};

struct VSInputPosNorTexTex2
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL0;
    float2 TexCoord     : TEXCOORD0;
    float2 TexCoord2    : TEXCOORD1;
};

struct VSInputPosNorTexIdxWght
{
    float3 Position     : POSITION0;
    float3 Normal       : NORMAL0;
    float2 TexCoord     : TEXCOORD0;
    uint4 BlendIndices  : BLENDINDICES0;
    float4 BlendWeight  : BLENDWEIGHT0;
};

/********* End Vertex Input *********/

/***************************
* Vertex Output
****************************/
struct VSOutputPosColor
{
    float4 Position     : SV_POSITION0;
    float4 Color        : COLOR0;
};

struct VSOuputPos
{
    float4 Position     : SV_POSITION0;
};

struct VSOutputPosTex
{
    float4 Position     : SV_POSITION0;
    float2 TexCoord     : TEXCOORD0;
};

struct VSOutputPosNorTex
{
    float4 Position     : SV_POSITION0;
    float3 Normal       : NORMAL0;
    float2 TexCoord     : TEXCOORD0;
};

struct VSOutputPosNorTexTex2
{
    float4 Position     : SV_POSITION0;
    float3 Normal       : NORMAL0;
    float2 TexCoord     : TEXCOORD0;
    float2 TexCoord2    : TEXCOORD1;
};


struct VSOutputPosNorTexPosWSDepthV
{
    float4    Position      : SV_POSITION0;
    float3    Normal        : NORMAL0;
    float2    TexCoord      : TEXCOORD0;
    float3    PositionWS    : TEXCOORD1;
    float    DepthView      : TEXCOORD2;
    float3    PosVtx        : TEXCOORD3;
};

struct VSOutputPosNorTexTex2PosWSDepthV
{
    float4    Position      : SV_POSITION0;
    float3    Normal        : NORMAL0;
    float2    TexCoord      : TEXCOORD0;
    float2    TexCoord2     : TEXCOORD1;
    float3    PositionWS    : NORMAL1;
    float    DepthView      : NORMAL2;
    float3    PosVtx        : NORMAL3;
};

struct VSOutputPosPosDepth
{
    float4 Position        : SV_POSITION0;
    float2 PosDepth        : TEXCOORD0;
};

/********* End Vertex Output *********/

/***************************
*
* Pixel struct
*
****************************/

/***************************
* Pixel Input
****************************/

/********* End Pixel Input *********/

#endif
