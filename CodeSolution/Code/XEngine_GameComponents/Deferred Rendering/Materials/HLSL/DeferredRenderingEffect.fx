
///////////////////////////////////////////////
//	      Global Deferred Rendering			 //
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

struct PixelShaderOutput
{
	float4 DiffuseRT	: COLOR0;
	float4 NormalRT		: COLOR1;
	float4 DepthRT		: COLOR2;
};

//////////////////////
//	Variables		//
//////////////////////
uniform extern float2		HalfPixel				: HALF_PIXEL = (float2)0;

uniform extern float4x4		InvertViewProjection	: INVERT_VIEW_PROJECTION;

uniform extern Texture2D	NormalTexture			: NORMAL_TEXTURE;
sampler2D NormalTextureSampler = sampler_state
{
    Texture		= < NormalTexture >;
	MinFilter	= Linear;
	MagFilter	= Linear;
	MipFilter	= Linear;   
	AddressU	= Clamp;
	AddressV	= Clamp;
};

uniform extern Texture2D	DepthTexture	: DEPTH_TEXTURE;
sampler2D DepthTextureSampler = sampler_state
{
    Texture		= < DepthTexture >;
	MagFilter	= POINT;
	MinFilter	= POINT;
	Mipfilter	= POINT;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

uniform extern Texture2D	LightTexture	: LIGHT_TEXTURE;
sampler2D LightTextureSampler = sampler_state
{
    Texture		= < LightTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	Mipfilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

uniform extern Texture2D	DiffuseTexture	: DIFFUSE_TEXTURE;
sampler2D DiffuseTextureSampler = sampler_state
{
    Texture		= < DiffuseTexture >;
	MagFilter	= Linear;
	MinFilter	= Linear;
	Mipfilter	= Linear;
	AddressU	= Clamp;
	AddressV	= Clamp;
};

uniform extern float		AmbientLightIntensity	: AMBIENT_LIGHT_INTENSITY	= 0.2f;
uniform extern float4		AmbientLightColor		: AMBIENT_LIGHT_COLOR		= {1.0f, 1.0f, 1.0f, 1.0f};

uniform extern float		LightStartAtt			: LIGHT_START_ATTENUATION;
uniform extern float		LightEndAtt				: LIGHT_END_ATTENUATION;
uniform extern float3		LightPosition			: LIGHT_POSITION;
uniform extern float4		LightColor				: LIGHT_COLOR;
uniform extern float3		LightDirection			: LIGHT_DIRECTION;
uniform extern float		LightFalloff			: LIGHT_FALL_OFF;
uniform extern float		LightAngle				: LIGHT_ANGLE;

//////////////////////
//	Functions		//
//////////////////////

float3 PackNormal(float3 normal)
{
    float3 packNormal = 0.5f * (normal + 1.0f);

    return packNormal;
}

float3 UnpackNormal(float3 normal)
{
    float3 unpackNormal = (2.0f * normal) - 1.0f;
    
	return unpackNormal;
}

float4 GetPositionFromDepth(float2 TexCoord, float depthVal)
{
    float4 position;
    
	position.x = TexCoord.x * 2.0f - 1.0f;
    position.y = -(TexCoord.y * 2.0f - 1.0f);
	//position.y = (1 - TexCoord.y) * 2.0f - 1.0f;
    
	position.z = depthVal;
    position.w = 1.0f;
    
	//transform to world space
    position = mul(position, InvertViewProjection);
    position /= position.w;
    
	return position;
}

float CalculateAttenuation(float distance, float startAtt, float endAtt)
{
	return ( 1 - saturate( (distance - startAtt) / (endAtt - startAtt) ) );
}

float CalculateAttenuationFromAngle(float angle, float ltAngle, float ltFalloff)
{
	float startAngle = ltAngle / 2;
	float endAngle = ltFalloff / 2;
	
	return (1 - saturate( (angle - startAngle) / (endAngle - startAngle) ) );
}

float4 CalculateOmniLight(float4 position, float3 normal)
{
	//Get Light Vector
	float lightDistance = distance(LightPosition, position.xyz);
	float3 lightVector = normalize(LightPosition - position.xyz);
	
	float NDotL = max(0, dot(normal, lightVector));
	
	float OmniAttenuation = CalculateAttenuation(lightDistance, LightStartAtt, LightEndAtt);
	
	float4 OmniColorFinal = (LightColor * NDotL) * OmniAttenuation;
	
	return OmniColorFinal;
}

float4 CalculateDirectionLight(float3 normal, float4 position, int lightNum)
{
    float3 lightVector = -normalize(LightDirection);
	
	float LdN = max(0, dot(lightVector, normal));
	
	float4 DirColor = LightColor * LdN;
	
	return (DirColor);
}

float4 CalculateSpotLight(float3 normal, float4 position, int lightNum)
{	
	float4 SpotColorFinal = (float4)0.0f;
	
	//Get Light Vector
	float lightDistance = distance(LightPosition, position.xyz);
	float3 lightVector = normalize(LightPosition - position.xyz);
	lightVector = normalize(lightVector);
	
	float SdL = dot(lightVector, -LightDirection);
	float ldM = length(lightVector);
	float sdM = length(LightDirection);
	float Rs = ( SdL / (ldM * sdM) );
	float an = acos(Rs);
	an = degrees(an);
	
	if(an <= (LightFalloff / 2))
	{
		float NDotL = saturate(dot(lightVector, normal));
				
		float SpotLightAttenuationDistance = CalculateAttenuation(lightDistance, LightStartAtt, LightEndAtt);
		
		float SpotLightAttenuationAngle = CalculateAttenuationFromAngle(an, LightAngle, LightFalloff);
		
		float4 SpotColor =  LightColor * (SpotLightAttenuationDistance * SpotLightAttenuationAngle);
		SpotColorFinal = (SpotColor * NDotL);
	}
	
	return ( SpotColorFinal );
}

////////////////////////////////////////
//  Vertex & Pixel Shader Functions   //
////////////////////////////////////////

VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output=(VertexShaderOutput) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - HalfPixel;

	return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 DRSkyPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR0
{
	float4 FinalPixelColor = (float4)0;

	float depthVal = tex2D(DepthTextureSampler, texCoord).r;

	if(depthVal == 1.0f)
	{
		FinalPixelColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	return FinalPixelColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 DRLightsPixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	float4 FinalPixelColor = (float4)0;
	
	//Get Normal From Map and unpack
	float3 normal = tex2D(NormalTextureSampler, input.TexCoord).xyz;
	normal = normalize(UnpackNormal(normal));

    //Get Depth from Map
    float depthVal = tex2D(DepthTextureSampler, input.TexCoord).r;

	//compute screen-space position
	float4 position = GetPositionFromDepth(input.TexCoord, depthVal);
		
	//if(lightType[0] == OMNI)
	//{
		//FinalPixelColor = CalculateOmniLight(normal, position, 0);
	//}
	//else if(lightType[0] == DIRECTIONAL)
	//{
		//FinalPixelColor = CalculateDirectionLight(normal, position, 0);
	//}
	//else if(lightType[0] == SPOT)
	//{
		//FinalPixelColor = CalculateSpotLight(normal, position, 0);
	//}
	
	return FinalPixelColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 DRCombinePixelShaderFunction(VertexShaderOutput input) : COLOR0
{
	float4 diffuseColor = tex2D(DiffuseTextureSampler, input.TexCoord);
    float4 light = tex2D(LightTextureSampler, input.TexCoord);

	float4 PixColor = diffuseColor * ( light + (AmbientLightIntensity * AmbientLightColor) );
	
	return PixColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Techniques					 //
///////////////////////////////////////////////

technique DFCombine
{
    pass Pass0
    {	
		AlphaBlendEnable	= false;

		VertexShader		= compile vs_3_0 VertexShaderFunction();
        PixelShader			= compile ps_3_0 DRCombinePixelShaderFunction();
    }
}

technique DFLights
{
    pass Pass0
    {
		AlphaBlendEnable 	= true;
		BlendOp				= Add;
		SrcBlend			= One;
		DestBlend			= InvSrcAlpha;
		
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 DRLightsPixelShaderFunction();
    }
}

technique DFSkyTechnique
{
    pass Pass0
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader = compile ps_3_0 DRSkyPixelShaderFunction();
    }
}