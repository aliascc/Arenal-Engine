
///////////////////////////////////////////////
//				Global HDR					 //
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

//////////////////////
// Texture samplers //
//////////////////////
sampler s0 : register(s0);
sampler s1 : register(s1);
sampler s2 : register(s2);
sampler s3 : register(s3);
sampler s4 : register(s4);
sampler s5 : register(s5);
sampler s6 : register(s6);
sampler s7 : register(s7);

//////////////////////
//	Variables		//
//////////////////////
static const int			MAX_SAMPLES										= 16;    // Maximum texture grabs

static const float			BRIGHT_PASS_THRESHOLD							= 5.0f;  // Threshold for BrightPass filter
static const float			BRIGHT_PASS_OFFSET								= 10.0f; // Offset for BrightPass filter

// The per-color weighting to be used for luminance calculations in RGB order.
static const float3			LUMINANCE_VECTOR								= float3(0.2125f, 0.7154f, 0.0721f);

// The per-color weighting to be used for blue shift under low light.
static const float3			BLUE_SHIFT_VECTOR								= float3(1.05f, 0.97f, 1.27f); 

// Contains sampling offsets used by the techniques
uniform extern float2		SampleOffsets[MAX_SAMPLES]	: SAMPLE_OFFSETS;
uniform extern float4		SampleWeights[MAX_SAMPLES]	: SAMPLE_WEIGHTS;

uniform extern float2		HalfPixel					: HALF_PIXEL		= (float2)0;

uniform extern float		ElapsedTime					: ELAPSED_TIME		= 0;			// Time in seconds since the last calculation
uniform extern float		MiddleGray					: MIDDLE_GRAY		= 0;			// The middle gray key value

uniform extern bool			EnableToneMap				: ENABLE_TONE_MAP	= true;
uniform extern bool			EnableBlueShift				: ENABLE_BLUE_SHIFT	= false;

uniform extern float		BloomScale					: BLOOM_SCALE		= 0.0f;			//Bloom process multiplier
uniform extern float		StarScale					: STAR_SCALE		= 0.0f;			//Star process multiplier

//////////////////////
//	Functions		//
//////////////////////
VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
{
	VertexShaderOutput output=(VertexShaderOutput) 0;
	
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord - HalfPixel;

	return output;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: DownScale4x4PS
// Type: Pixel shader                                      
// Desc: Scale the source texture down to 1/16 scale
//-----------------------------------------------------------------------------
float4 DownScale4x4PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
   float4 dsSample = 0.0f;

	for( int i=0; i < 16; i++ )
	{
		dsSample += tex2D(s0, texCoord + SampleOffsets[i] );
	}
    
	return dsSample / 16;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: DownScale2x2PS
// Type: Pixel shader                                      
// Desc: Scale the source texture down to 1/4 scale
//-----------------------------------------------------------------------------
float4 DownScale2x2PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 sample = 0.0f;

	for( int i=0; i < 4; i++ )
	{
		sample += tex2D(s0, texCoord + SampleOffsets[i] );
	}
    
	return sample / 4;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: SampleLumInitial
// Type: Pixel shader                                      
// Desc: Sample the luminance of the source image using a kernal of sample
//       points, and return a scaled image containing the log() of averages
//-----------------------------------------------------------------------------
float4 SampleLumInitialPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float3 lumSample = 0.0f;
    float  fLogLumSum = 0.0f;

    for(int iSample = 0; iSample < 9; iSample++)
    {
        // Compute the sum of log(luminance) throughout the sample points
        lumSample = tex2D(s0, texCoord + SampleOffsets[iSample]);
        fLogLumSum += log(dot(lumSample, LUMINANCE_VECTOR)+0.0001f);
    }
    
    // Divide the sum to complete the average
    fLogLumSum /= 9;

    return float4(fLogLumSum, fLogLumSum, fLogLumSum, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: SampleLumIterative
// Type: Pixel shader                                      
// Desc: Scale down the luminance texture by blending sample points
//-----------------------------------------------------------------------------
float4 SampleLumIterativePixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float fResampleSum = 0.0f; 
    
    for(int iSample = 0; iSample < 16; iSample++)
    {
        // Compute the sum of luminance throughout the sample points
        fResampleSum += tex2D(s0, texCoord + SampleOffsets[iSample]);
    }
    
    // Divide the sum to complete the average
    fResampleSum /= 16;

    return float4(fResampleSum, fResampleSum, fResampleSum, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: SampleLumFinal
// Type: Pixel shader                                      
// Desc: Extract the average luminance of the image by completing the averaging
//       and taking the exp() of the result
//-----------------------------------------------------------------------------
float4 SampleLumFinalPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float fResampleSum = 0.0f;
    
    for(int iSample = 0; iSample < 16; iSample++)
    {
        // Compute the sum of luminance throughout the sample points
        fResampleSum += tex2D(s0, texCoord + SampleOffsets[iSample]);
    }
    
    // Divide the sum to complete the average, and perform an exp() to complete
    // the average luminance calculation
    fResampleSum = exp(fResampleSum/16);
    
    return float4(fResampleSum, fResampleSum, fResampleSum, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: CalculateAdaptedLumPixelShaderFunction
// Type: Pixel shader                                      
// Desc: Calculate the luminance that the camera is current adapted to, using
//       the most recented adaptation level, the current scene luminance, and
//       the time elapsed since last calculated
//-----------------------------------------------------------------------------
float4 CalculateAdaptedLumPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float fAdaptedLum = tex2D(s0, float2(0.5f, 0.5f));
    float fCurrentLum = tex2D(s1, float2(0.5f, 0.5f));
    
    // The user's adapted luminance level is simulated by closing the gap between
    // adapted luminance and current luminance by 2% every frame, based on a
    // 30 fps rate. This is not an accurate model of human adaptation, which can
    // take longer than half an hour.
    float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * ( 1 - pow( 0.98f, 30 * ElapsedTime ) );
    return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: BrightPassFilterPixelShaderFunction
// Type: Pixel shader                                      
// Desc: Perform a high-pass filter on the source texture
//-----------------------------------------------------------------------------
float4 BrightPassFilterPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vSample = tex2D(s0, texCoord);
	float  fAdaptedLum = tex2D(s1, float2(0.5f, 0.5f));
	
	// Determine what the pixel's value will be after tone-mapping occurs
	vSample.rgb *= MiddleGray/(fAdaptedLum + 0.001f);
	
	// Subtract out dark pixels
	vSample.rgb -= BRIGHT_PASS_THRESHOLD;
	
	// Clamp to 0
	vSample = max(vSample, 0.0f);
	
	// Map the resulting value into the 0 to 1 range. Higher values for
	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene 
	// objects.
	vSample.rgb /= (BRIGHT_PASS_OFFSET+vSample);
    
	return vSample;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: GaussBlur5x5PixelShaderFunction
// Type: Pixel shader                                      
// Desc: Simulate a 5x5 kernel gaussian blur by sampling the 12 points closest
//       to the center point.
//-----------------------------------------------------------------------------
float4 GaussBlur5x5PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 sample = 0.0f;

	for( int i=0; i < 12; i++ )
	{
		sample += SampleWeights[i] * tex2D(s0, texCoord + SampleOffsets[i]);
	}

	return sample;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: BloomPixelShaderFunction
// Type: Pixel shader                                      
// Desc: Blur the source image along one axis using a gaussian
//       distribution. Since gaussian blurs are separable, this shader is called 
//       twice; first along the horizontal axis, then along the vertical axis.
//-----------------------------------------------------------------------------
float4 BloomPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
        
    float2 vSamplePosition;
    
    // Perform a one-directional gaussian blur
    for(int iSample = 0; iSample < 15; iSample++)
    {
        vSamplePosition = texCoord + SampleOffsets[iSample];
        vColor = tex2D(s0, vSamplePosition);
        vSample += SampleWeights[iSample]*vColor;
    }
    
    return vSample;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: StarPS
// Type: Pixel shader                                      
// Desc: Each star is composed of up to 8 lines, and each line is created by
//       up to three passes of this shader, which samples from 8 points along
//       the current line.
//-----------------------------------------------------------------------------
float4 StarPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 vSample = 0.0f;
    float4 vColor = 0.0f;
        
    float2 vSamplePosition;
    
    // Sample from eight points along the star line
    for(int iSample = 0; iSample < 8; iSample++)
    {
        vSamplePosition = texCoord + SampleOffsets[iSample];
        vSample = tex2D(s0, vSamplePosition);
        vColor += SampleWeights[iSample] * vSample;
    }
    	
    return vColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_1PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_2PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_3PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
	vColor += SampleWeights[2] * tex2D(s2, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_4PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
	vColor += SampleWeights[2] * tex2D(s2, texCoord);
	vColor += SampleWeights[3] * tex2D(s3, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_5PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
	vColor += SampleWeights[2] * tex2D(s2, texCoord);
	vColor += SampleWeights[3] * tex2D(s3, texCoord);
	vColor += SampleWeights[4] * tex2D(s4, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_6PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
	vColor += SampleWeights[2] * tex2D(s2, texCoord);
	vColor += SampleWeights[3] * tex2D(s3, texCoord);
	vColor += SampleWeights[4] * tex2D(s4, texCoord);
	vColor += SampleWeights[5] * tex2D(s5, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_7PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
	vColor += SampleWeights[2] * tex2D(s2, texCoord);
	vColor += SampleWeights[3] * tex2D(s3, texCoord);
	vColor += SampleWeights[4] * tex2D(s4, texCoord);
	vColor += SampleWeights[5] * tex2D(s5, texCoord);
	vColor += SampleWeights[6] * tex2D(s6, texCoord);
		
	return vColor;
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_NPS
// Type: Pixel shader                                      
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
float4 MergeTextures_8PixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
	float4 vColor = 0.0f;
	
	vColor += SampleWeights[0] * tex2D(s0, texCoord);
	vColor += SampleWeights[1] * tex2D(s1, texCoord);
	vColor += SampleWeights[2] * tex2D(s2, texCoord);
	vColor += SampleWeights[3] * tex2D(s3, texCoord);
	vColor += SampleWeights[4] * tex2D(s4, texCoord);
	vColor += SampleWeights[5] * tex2D(s5, texCoord);
	vColor += SampleWeights[6] * tex2D(s6, texCoord);
	vColor += SampleWeights[7] * tex2D(s7, texCoord);
		
	return vColor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: FinalScenePassPS
// Type: Pixel shader                                      
// Desc: Perform blue shift, tone map the scene, and add post-processed light
//       effects
//-----------------------------------------------------------------------------
float4 FinalScenePassPixelShaderFunction(float2 texCoord : TEXCOORD0) : COLOR
{
    float4 vSample = tex2D(s0, texCoord);
    float4 vBloom = tex2D(s1, texCoord);
    float4 vStar = tex2D(s2, texCoord);
	float fAdaptedLum = tex2D(s3, float2(0.5f, 0.5f));

	// For very low light conditions, the rods will dominate the perception
    // of light, and therefore color will be desaturated and shifted
    // towards blue.
    if(EnableBlueShift)
    {
		// Define a linear blending from -1.5 to 2.6 (log scale) which
		// determines the lerp amount for blue shift
        float fBlueShiftCoefficient = 1.0f - (fAdaptedLum + 1.5)/4.1;
        fBlueShiftCoefficient = saturate(fBlueShiftCoefficient);

		// Lerp between current color and blue, desaturated copy
        float3 vRodColor = dot( (float3)vSample, LUMINANCE_VECTOR ) * BLUE_SHIFT_VECTOR;
        vSample.rgb = lerp( (float3)vSample, vRodColor, fBlueShiftCoefficient );
    }
    
	
    // Map the high range of color values into a range appropriate for
    // display, taking into account the user's adaptation level, and selected
    // values for for middle gray and white cutoff.
    if(EnableToneMap)
    {
		vSample.rgb *= MiddleGray/(fAdaptedLum + 0.001f);
		vSample.rgb /= (1.0f+vSample);
    }  
    
    // Add the star and bloom post processing effects
    vSample += StarScale * vStar;
    vSample += BloomScale * vBloom;
    
    return vSample;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////
//				Techniques					 //
///////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Name: DownScale4x4
// Type: Technique                                     
// Desc: Scale the source texture down to 1/16 scale
//-----------------------------------------------------------------------------
technique DownScale4x4
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 DownScale4x4PixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: SampleAvgLum
// Type: Technique                                     
// Desc: Takes the HDR Scene texture as input and starts the process of 
//       determining the average luminance by converting to grayscale, taking
//       the log(), and scaling the image to a single pixel by averaging sample 
//       points.
//-----------------------------------------------------------------------------
technique SampleAvgLum
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 SampleLumInitialPixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: ResampleAvgLum
// Type: Technique                                     
// Desc: Continue to scale down the luminance texture
//-----------------------------------------------------------------------------
technique ResampleAvgLum
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 SampleLumIterativePixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: ResampleAvgLumExp
// Type: Technique                                     
// Desc: Sample the texture to a single pixel and perform an exp() to complete
//       the evalutation
//-----------------------------------------------------------------------------
technique ResampleAvgLumExp
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 SampleLumFinalPixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: CalculateAdaptedLum
// Type: Technique                                     
// Desc: Determines the level of the user's simulated light adaptation level
//       using the last adapted level, the current scene luminance, and the
//       time since last calculation
//-----------------------------------------------------------------------------
technique CalculateAdaptedLum
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 CalculateAdaptedLumPixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: BrightPassFilter
// Type: Technique                                     
// Desc: Perform a high-pass filter on the source texture
//-----------------------------------------------------------------------------
technique BrightPassFilter
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 BrightPassFilterPixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: GaussBlur5x5
// Type: Technique                                     
// Desc: Simulate a 5x5 kernel gaussian blur by sampling the 12 points closest
//       to the center point.
//-----------------------------------------------------------------------------
technique GaussBlur5x5
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 GaussBlur5x5PixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: DownScale2x2
// Type: Technique                                     
// Desc: Scale the source texture down to 1/4 scale
//-----------------------------------------------------------------------------
technique DownScale2x2
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 DownScale2x2PixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: Bloom
// Type: Technique                                     
// Desc: Performs a single horizontal or vertical pass of the blooming filter
//-----------------------------------------------------------------------------
technique Bloom
{
    pass Pass1
    {        
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 BloomPixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: Star
// Type: Technique                                     
// Desc: Perform one of up to three passes composing the current star line
//-----------------------------------------------------------------------------
technique Star
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 StarPixelShaderFunction();
    }

}


//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_1
{
    pass Pass1
    {        
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 MergeTextures_1PixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_2
{
    pass Pass1
    {        
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 MergeTextures_2PixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_3
{
    pass Pass1
    {        
		VertexShader = compile vs_3_0 VertexShaderFunction();
        PixelShader  = compile ps_3_0 MergeTextures_3PixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_4
{
    pass Pass1
    {       
		VertexShader = compile vs_3_0 VertexShaderFunction(); 
        PixelShader  = compile ps_3_0 MergeTextures_4PixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_5
{
    pass Pass1
    {    
		VertexShader = compile vs_3_0 VertexShaderFunction();    
        PixelShader  = compile ps_3_0 MergeTextures_5PixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_6
{
    pass Pass1
    {    
		VertexShader = compile vs_3_0 VertexShaderFunction();    
        PixelShader  = compile ps_3_0 MergeTextures_6PixelShaderFunction();
    }

}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_7
{
    pass Pass1
    {    
		VertexShader = compile vs_3_0 VertexShaderFunction();    
        PixelShader  = compile ps_3_0 MergeTextures_7PixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: MergeTextures_N
// Type: Technique                                     
// Desc: Return the average of N input textures
//-----------------------------------------------------------------------------
technique MergeTextures_8
{
    pass Pass1
    {     
		VertexShader = compile vs_3_0 VertexShaderFunction();   
        PixelShader  = compile ps_3_0 MergeTextures_8PixelShaderFunction();
    }
}

//-----------------------------------------------------------------------------
// Name: FinalScenePass
// Type: Technique                                     
// Desc: Minimally transform and texture the incoming geometry
//-----------------------------------------------------------------------------
technique FinalScenePass
{
    pass Pass1
    {
		VertexShader = compile vs_3_0 VertexShaderFunction();  
        PixelShader  = compile ps_3_0 FinalScenePassPixelShaderFunction();
    }
}