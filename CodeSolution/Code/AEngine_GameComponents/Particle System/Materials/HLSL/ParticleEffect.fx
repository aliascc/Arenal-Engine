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


////////////////////////////////////////

uniform extern float4x4		View				: CAMERA_VIEW;
uniform extern float4x4		Projection			: CAMERA_PROJECTION;
uniform extern float2		ViewportScale		: VIEWPORT_SCALE;

///////////////////////////////////////

// The current time, in seconds.
uniform extern float		CurrentTime			: CURRENT_TIME;

// Parameters describing how the particles animate.
uniform extern float		Duration			: DURATION;
uniform extern float		DurationRandomness	: DURATION_RANDOMNESS;
uniform extern float3		Gravity				: GRAVITY;
uniform extern float		EndVelocity			: END_VELOCITY;
uniform extern float4		MinColor			: MIN_COLOR;
uniform extern float4		MaxColor			: MAX_COLOR;

// These float2 parameters describe the min and max of a range.
// The actual value is chosen differently for each particle,
// interpolating between x and y by some random amount.
uniform extern float2		RotateSpeed			: ROTATE_SPEED;
uniform extern float2		StartSize			: START_SIZE;
uniform extern float2		EndSize				: END_SIZE;

// Particle texture and sampler.
uniform extern Texture2D	ParticleTexture		: PARTICLE_TEXTURE;
sampler ParticleTextureSampler = sampler_state
{
    Texture		= < ParticleTexture >;
    MinFilter	= Linear;
    MagFilter	= Linear;
    MipFilter	= Point;
    AddressU	= Clamp;
    AddressV	= Clamp;
};

// Vertex shader input structure describes the start position and
// velocity of the particle, and the time at which it was created,
// along with some random values that affect its size and rotation.
struct VertexShaderInput
{
    float2	Corner		: POSITION0;
    float3	Position	: POSITION1;
    float3	Velocity	: NORMAL0;
    float4	Random		: COLOR0;
    float	Time		: TEXCOORD0;
};

// Vertex shader output structure specifies the position and color of the particle.
struct VertexShaderOutput
{
    float4	Position			: POSITION0;
    float4	Color				: COLOR0;
    float2	TextureCoordinate	: COLOR1;
};

// Vertex shader helper for computing the position of a particle.
float4 ComputeParticlePosition(float3 position, float3 velocity,
                               float age, float normalizedAge)
{
    float startVelocity = length(velocity);

    // Work out how fast the particle should be moving at the end of its life,
    // by applying a constant scaling factor to its starting velocity.
    float endVelocity = startVelocity * EndVelocity;

    // Our particles have constant acceleration, so given a starting velocity
    // S and ending velocity E, at time T their velocity should be S + (E-S)*T.
    // The particle position is the sum of this velocity over the range 0 to T.
    // To compute the position directly, we must integrate the velocity
    // equation. Integrating S + (E-S)*T for T produces S*T + (E-S)*T*T/2.
    float velocityIntegral = startVelocity * normalizedAge +
                             (endVelocity - startVelocity) * normalizedAge *
                                                             normalizedAge / 2;

    position += normalize(velocity) * velocityIntegral * Duration;

    // Apply the gravitational force.
    position += Gravity * age * normalizedAge;

    // Apply the camera view and projection transforms.
    return mul(mul(float4(position, 1), View), Projection);
}

// Vertex shader helper for computing the size of a particle.
float ComputeParticleSize(float randomValue, float normalizedAge)
{
    // Apply a random factor to make each particle a slightly different size.
    float startSize = lerp(StartSize.x, StartSize.y, randomValue);
    float endSize = lerp(EndSize.x, EndSize.y, randomValue);

    // Compute the actual size based on the age of the particle.
    float size = lerp(startSize, endSize, normalizedAge);

    // Project the size into screen coordinates.
    return size * Projection._m11;
}



// Vertex shader helper for computing the color of a particle.
float4 ComputeParticleColor(float4 projectedPosition,
                            float randomValue, float normalizedAge)
{
    // Apply a random factor to make each particle a slightly different color.
    float4 color = lerp(MinColor, MaxColor, randomValue);

    // Fade the alpha based on the age of the particle. This curve is hard coded
    // to make the particle fade in fairly quickly, then fade out more slowly:
    // plot x*(1-x)*(1-x) for x=0:1 in a graphing program if you want to see what
    // this looks like. The 6.7 scaling factor normalizes the curve so the alpha
    // will reach all the way up to fully solid.
    color.a *= normalizedAge * (1-normalizedAge) * (1-normalizedAge) * 6.7;

    return color;
}

// Vertex shader helper for computing the rotation of a particle.
float2x2 ComputeParticleRotation(float randomValue, float age)
{
    // Apply a random factor to make each particle rotate at a different speed.
    float rotateSpeed = lerp(RotateSpeed.x, RotateSpeed.y, randomValue);
    float rotation = rotateSpeed * age;

    // Compute a 2x2 rotation matrix.
    float c = cos(rotation);
    float s = sin(rotation);

    return float2x2(c, -s, s, c);
}

// Custom vertex shader animates particles entirely on the GPU.
VertexShaderOutput ParticleVertexShader(VertexShaderInput input)
{
    VertexShaderOutput output = (VertexShaderOutput)0;

    // Compute the age of the particle.
    float age = CurrentTime - input.Time;

    // Apply a random factor to make different particles age at different rates.
    age *= 1 + input.Random.x * DurationRandomness;

    // Normalize the age into the range zero to one.
    float normalizedAge = saturate(age / Duration);

    // Compute the particle position, size, color, and rotation.
    output.Position = ComputeParticlePosition(input.Position, input.Velocity,
                                              age, normalizedAge);

    float size = ComputeParticleSize(input.Random.y, normalizedAge);

    float2x2 rotation = ComputeParticleRotation(input.Random.w, age);

    output.Position.xy += mul(input.Corner, rotation) * size * ViewportScale;

    output.Color = ComputeParticleColor(output.Position, input.Random.z, normalizedAge);

    output.TextureCoordinate = (input.Corner + 1) / 2;

    return output;
}

// Pixel shader for drawing particles.
float4 ParticlePixelShader(VertexShaderOutput input) : COLOR0
{
    return tex2D(ParticleTextureSampler, input.TextureCoordinate) * input.Color;
}

// Effect technique for drawing particles.
technique Particles
{
    pass Pass0
    {
        VertexShader = compile vs_3_0 ParticleVertexShader();
        PixelShader = compile ps_3_0 ParticlePixelShader();
    }
}
