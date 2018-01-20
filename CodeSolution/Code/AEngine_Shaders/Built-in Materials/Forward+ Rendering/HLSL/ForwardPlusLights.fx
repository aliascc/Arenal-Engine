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

#ifndef _FORWARD_PLUS_LIGHTS_FX
#define _FORWARD_PLUS_LIGHTS_FX

/******************
* Inputs
******************/

#include <ShadowFunctions.fx>
#include <ForwardPlusCommon.fx>

/******************
* Constant Buffers
******************/

cbuffer _AE_CB_FPR_Lights : register(b0)
{
    float3    _AE_CameraPos                : packoffset(c0);
    uint    _AE_WindowWidth                : packoffset(c0.w);
};

/******************
* Input Resources
******************/
SamplerState _AE_ShadowTextureSampler    : register(s0);

StructuredBuffer<Light>                 _AE_LightBuffer                 : register(t0);
Buffer<uint>                            _AE_PerTileLightIndexBuffer     : register(t1);
StructuredBuffer<SpotLightShadowInfo>   _AE_ShadowSpotLightInfoBuffer   : register(t2);
Texture2DArray                          _AE_SpotLightShadowTextureArray : register(t3);
Texture2DArray                          _AE_DirLightShadowTextureArray  : register(t4);
StructuredBuffer<LightCascadeInfo>      _AE_ShadowDirLightInfoBuffer    : register(t5);

/******************
* Light Functions
******************/

float CalculateAttenuation(float distance, float startAtt, float endAtt)
{
    return ( 1 - saturate( (distance - startAtt) / (endAtt - startAtt) ) );
}

float CalculateAttenuationFromAngle(float angle, float lightAngle, float lightFalloff)
{
    float startAngle = lightAngle / 2;
    float endAngle = lightFalloff / 2;

    return (1 - saturate( (angle - startAngle) / (endAngle - startAngle) ) );
}

float4 CalculateOmniLight(float3 normal, float3 position, Light light)
{
    //Get Light Vector
    float lightDistance = distance(light.Position, position);
    float3 lightVector = normalize(light.Position - position);

    float NDotL = max(0, dot(normal, lightVector));

    float omniAttenuation = CalculateAttenuation(lightDistance, light.NearAttenuation, light.FarAttenuation);

    float4 omniColorFinal = (float4(light.Color.rgb * light.Intensity , 1) * NDotL) * omniAttenuation;

    return omniColorFinal;
}

float4 CalculateDirectionLight(float3 normal, Light light)
{
    float3 lightVector = -normalize(light.Direction);

    float LdN = max(0, dot(lightVector, normal));
    
    float4 dirColor = float4(light.Color.rgb * light.Intensity , 1) * LdN;

    return dirColor;
}

float4 CalculateSpotLight(float3 normal, float3 position, Light light)
{
    float4 spotColorFinal = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 lightVector = normalize(light.Position - position);
    float angleBetweenDV = dot(light.Direction, -lightVector);

    float an = acos(angleBetweenDV);
    an = degrees(an);

    if(an <= (light.FallOffAngle / 2))
    {
        float NDotL = saturate(dot(lightVector, normal));

        float lightDistance = distance(light.Position, position);

        //float4 SpotColor = float4(1, 1, 1, 1);
        float4 spotColor = float4(light.Color.rgb * light.Intensity, 1.0f);

        float spotLightAttenuationDistance = CalculateAttenuation(lightDistance, light.NearAttenuation, light.FarAttenuation);

        float spotLightAttenuationAngle = CalculateAttenuationFromAngle(an, light.Angle, light.FallOffAngle);

        spotColor *= (spotLightAttenuationDistance * spotLightAttenuationAngle);
        spotColorFinal = (spotColor * NDotL);
    }

    return spotColorFinal;
}

float4 CalculateSpecular(float3 pos, float specularFactor, float3 eyePosition, float3 normal, Light light, out float specularShine)
{
    float4 specularColor = float4(light.Color.rgb * light.Intensity, 1);
    float3 lightVector = (float3)0;
    specularShine = 0;

    if(light.LightType == LIGHT_DIRECTIONAL)
    {
        lightVector = -normalize(light.Direction);
    }
    else
    {
        lightVector = normalize(light.Position - pos);
    }

    float NdL = saturate(dot(lightVector, normal));

    float3 Reflect = normalize(2.0f * NdL * normal - lightVector);
    
    specularShine = pow(saturate(dot(Reflect, eyePosition)), specularFactor);

    return (specularColor * specularShine);
}

LightOutput ForwardPlusLightCalculation(float3 positionWorldSpace, float3 normal, float specularFactor, float2 positionScreen, float depthView, float3 posVtx)
{
    LightOutput lightOutput = (LightOutput)0;
    Light currentLight = (Light)0;

    bool contLoopLights = true;

    uint tileIndex = GetTileIndex(positionScreen, _AE_WindowWidth);

    uint lightIndex = (MAX_NUM_LIGHTS_PER_TILE + 1) * tileIndex;

    uint lightBufferIndex = _AE_PerTileLightIndexBuffer[lightIndex];

    //loop over the lights
    [loop]
    while(contLoopLights && lightBufferIndex != LIGHT_INDEX_BUFFER_SENTINEL)
    {
        float4 loopColor = (float4)0;
        float shadowContrib = 1.0f;

        currentLight = _AE_LightBuffer[lightBufferIndex];

        switch(currentLight.LightType)
        {
            case LIGHT_SPOT:
                loopColor = CalculateSpotLight(normal, positionWorldSpace, currentLight);
                if ((currentLight.LightFlags & LIGHT_FLAG_SHADOWENABLE) != 0)
                {
                    shadowContrib = CalcShadowVarianceSpotLight(positionWorldSpace, currentLight.ShadowTextureIndex, _AE_ShadowSpotLightInfoBuffer, _AE_SpotLightShadowTextureArray, _AE_ShadowTextureSampler);
                }
                break;

            case LIGHT_OMNI:
                loopColor = CalculateOmniLight(normal, positionWorldSpace, currentLight);
                break;

            case LIGHT_DIRECTIONAL:
                loopColor = CalculateDirectionLight(normal, currentLight);
                if ((currentLight.LightFlags & LIGHT_FLAG_SHADOWENABLE) != 0)
                {
                    shadowContrib = CalcShadowVarianceCascadeDirLightSimple(positionWorldSpace, depthView, currentLight.ShadowTextureIndex, _AE_ShadowDirLightInfoBuffer, _AE_DirLightShadowTextureArray, _AE_ShadowTextureSampler);
                    //shadowContrib = CalcShadowVarianceCascadeDirLight(posVtx, depthView, currentLight.ShadowTextureIndex, _AE_ShadowDirLightInfoBuffer, _AE_DirLightShadowTextureArray, _AE_ShadowTextureSampler);
                }
                break;

            default:
                contLoopLights = false;
                break;
        }

        if(contLoopLights)
        {
            float speculatShineTemp = 0.0f;

            loopColor *= shadowContrib;

            lightOutput.Color += loopColor;

            lightOutput.SpecularColor += CalculateSpecular(positionWorldSpace, specularFactor, _AE_CameraPos, normal, currentLight, speculatShineTemp);
            lightOutput.SpecularShine += speculatShineTemp;

            lightIndex++;
            lightBufferIndex = _AE_PerTileLightIndexBuffer[lightIndex];
        }
    }

    lightOutput.Color.a = 1.0f;
    lightOutput.SpecularColor.a = 1.0f;

    //Fast Ambient
    lightOutput.Color.rgb += 0.1f;

    //This is a poor man's ambient cubemap (blend between an up color and a down color)
    //float ambientBlend = 0.5f * normal.y + 0.5;
    //float3 Ambient = g_MaterialAmbientColorUp.rgb * fAmbientBlend + g_MaterialAmbientColorDown.rgb * (1-fAmbientBlend);

    return lightOutput;
}

#endif
