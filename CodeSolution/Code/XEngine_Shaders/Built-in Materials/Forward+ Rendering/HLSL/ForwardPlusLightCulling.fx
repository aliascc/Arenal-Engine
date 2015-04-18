
//////////////////////////////////////////////////////////////////////////////////////////////////
//Some Functions are extracted from the exampled of AMD Forward+ 11 (Jason Stewart)
//http://developer.amd.com/wordpress/media/2013/01/ForwardPlus11_v1.0.zip
//////////////////////////////////////////////////////////////////////////////////////////////////

/******************
* Constant Buffers
******************/

cbuffer _XE_CB_FPR_LightCulling : register(b0)
{
	matrix	_XE_View				: packoffset(c0);
	matrix	_XE_InvProjection		: packoffset(c4);

	uint	_XE_WindowHeight		: packoffset(c8.x);
	uint	_XE_WindowWidth			: packoffset(c8.y);
	uint	_XE_NumLights			: packoffset(c8.z);
};

/////////////////////////////////////
//Before include so _XE_WindowWidth
//is available to Functions in include
#include <ForwardPlusCommon.fx>

/******************
* Defines
******************/

#define NUM_THREADS_X TILE_RES
#define NUM_THREADS_Y TILE_RES
#define NUM_THREADS_PER_TILE (NUM_THREADS_X * NUM_THREADS_Y)

/******************
* Input Resources
******************/

StructuredBuffer<Light> _XE_LightBuffer : register(t0);

//MSAA
#ifdef _XE_MSAA

	Texture2DMS<float> _XE_DepthTexture : register(t1);

//Non-MSAA
#else

	Texture2D<float> _XE_DepthTexture : register(t1);

#endif

/******************
* Input Outputs
******************/

RWBuffer<uint> _XE_PerTileLightIndexBuffer : register(u0);

/******************************
* Group Shared Variables
*******************************/

groupshared uint _XE_TileZMax = 0;
groupshared uint _XE_TileZMin = 0;
groupshared uint _XE_TileLightIdxCounter = 0;
groupshared uint _XE_TileLightIdx[MAX_NUM_LIGHTS_PER_TILE];

/******************************
* Helper Functions
*******************************/

//this creates the standard Hessian-normal-form plane equation from three points, 
//except it is simplified for the case where the first point is the origin
float4 CreatePlaneEquation(float4 b, float4 c)
{
	float4 n;

	//normalize(cross( b.xyz-a.xyz, c.xyz-a.xyz )), except we know "a" is the origin
	n.xyz = normalize(cross(b.xyz, c.xyz));

	//-(n dot a), except we know "a" is the origin
	n.w = 0;

	return n;
}

// point-plane distance, simplified for the case where 
// the plane passes through the origin
float GetSignedDistanceFromPlane(float3 p, float4 eqn)
{
	// dot( eqn.xyz, p.xyz ) + eqn.w, , except we know eqn.w is zero 
	// (see CreatePlaneEquation above)
	return dot(eqn.xyz, p.xyz);
}

// calculate the number of tiles in the horizontal direction
uint GetNumTilesX()
{
	return (uint)((_XE_WindowWidth + TILE_RES - 1) / (float)TILE_RES);
}

// calculate the number of tiles in the vertical direction
uint GetNumTilesY()
{
	return (uint)((_XE_WindowHeight + TILE_RES - 1) / (float)TILE_RES);
}

// convert a point from post-projection space into view space
float4 ConvertProjToView(float4 p)
{
	p = mul(_XE_InvProjection, p);
	p /= p.w;
	return p;
}

// convert a depth value from post-projection space into view space
float ConvertProjDepthToView( float z )
{
	//z = 1.0f / (z * _XE_InvProjection._34 + _XE_InvProjection._44);
	z = 1.0f / (z * _XE_InvProjection._44 + _XE_InvProjection._34);

	//z = ((_XE_InvProjection._31 * z) + (_XE_InvProjection._32 * z) + (_XE_InvProjection._33 * z) + (_XE_InvProjection._34 * z));
	//float w = (_XE_InvProjection._41 + _XE_InvProjection._42 + _XE_InvProjection._43 + _XE_InvProjection._44);
	//z /= w;

	return z;
}

void CalculateMinMaxDepthInTile(uint3 globalThreadIdx, uint depthBufferSampleIdx)
{
//MSAA
#ifdef _XE_MSAA

	float depth = _XE_DepthTexture.Load(uint2(globalThreadIdx.x, globalThreadIdx.y), depthBufferSampleIdx).x;

//Non MSAA
#else

	float depth = _XE_DepthTexture.Load(uint3(globalThreadIdx.x, globalThreadIdx.y, 0)).x;

#endif

	float viewPosZ = ConvertProjDepthToView(depth);

	uint z = asuint(viewPosZ);

	if( depth != 0.0f )
	{
		InterlockedMax(_XE_TileZMax, z);
		InterlockedMin(_XE_TileZMin, z);
	}
}

bool PointLightIntersectsFrustum(uint lightIndex, float minZ, float maxZ, float4 frustum[4])
{
	bool intersects = false;

	Light pointLight = _XE_LightBuffer[lightIndex];

	float3 position = pointLight.Position;
	float radius = pointLight.FarAttenuation;

	position.xyz = mul(_XE_View, float4(position.xyz, 1.0f)).xyz;

	//test if sphere is intersecting or inside frustum
	if( (-position.z + minZ < radius) || 
		( position.z - maxZ < radius) )
	{
		if( GetSignedDistanceFromPlane(position, frustum[0]) < radius &&
			GetSignedDistanceFromPlane(position, frustum[1]) < radius &&
			GetSignedDistanceFromPlane(position, frustum[2]) < radius &&
			GetSignedDistanceFromPlane(position, frustum[3]) < radius )
		{
			intersects = true;
		}
	}

	return intersects;
}

bool SpotLightIntersectsFrustum(uint lightIndex, float minZ, float maxZ, float4 frustum[4])
{
	bool intersects = false;

	Light spotLight = _XE_LightBuffer[lightIndex];

	float3 position = spotLight.Position;
	float radius = spotLight.FarAttenuation;

	position = mul(_XE_View, float4(position, 1.0f)).xyz;

	//test if sphere is intersecting or inside frustum
	if( (-position.z + minZ < radius) || 
		( position.z - maxZ < radius) )
	{
		if( GetSignedDistanceFromPlane(position, frustum[0]) < radius &&
			GetSignedDistanceFromPlane(position, frustum[1]) < radius &&
			GetSignedDistanceFromPlane(position, frustum[2]) < radius &&
			GetSignedDistanceFromPlane(position, frustum[3]) < radius )
		{
			intersects = true;
		}
	}

	return intersects;
}

/******************
* Main Function
******************/

[numthreads(NUM_THREADS_X, NUM_THREADS_Y, 1)]
void main(uint3 globalIdx : SV_DispatchThreadID, uint3 localIdx : SV_GroupThreadID, uint3 groupIdx : SV_GroupID)
{
	uint localIdxFlattened = localIdx.x + localIdx.y * NUM_THREADS_X;
	uint tileIdxFlattened = groupIdx.x + groupIdx.y * GetNumTilesX();

	if( localIdxFlattened == 0 )
	{
		_XE_TileZMin = UINT_MAX;
		_XE_TileZMax = 0;

		_XE_TileLightIdxCounter = 0;
	}

	float4 frustumEqn[4] = { (float4)0, (float4)0, (float4)0, (float4)0 };

	{
		//construct frustum for this tile
		uint pxm = TILE_RES * groupIdx.x;
		uint pym = TILE_RES * groupIdx.y;
		uint pxp = TILE_RES * (groupIdx.x + 1);
		uint pyp = TILE_RES * (groupIdx.y + 1);

		uint uWindowWidthEvenlyDivisibleByTileRes = TILE_RES * GetNumTilesX();
		uint uWindowHeightEvenlyDivisibleByTileRes = TILE_RES * GetNumTilesY();

		// four corners of the tile, clockwise from top-left
		float4 frustum[4] = { (float4)0, (float4)0, (float4)0, (float4)0 };

		frustum[0] = ConvertProjToView(float4(pxm / (float)uWindowWidthEvenlyDivisibleByTileRes * 2.0f - 1.0f, (uWindowHeightEvenlyDivisibleByTileRes - pym) / (float)uWindowHeightEvenlyDivisibleByTileRes * 2.0f - 1.0f, 1.0f, 1.0f));

		frustum[1] = ConvertProjToView(float4(pxp / (float)uWindowWidthEvenlyDivisibleByTileRes * 2.0f - 1.0f, (uWindowHeightEvenlyDivisibleByTileRes - pym) / (float)uWindowHeightEvenlyDivisibleByTileRes * 2.0f - 1.0f, 1.0f, 1.0f));

		frustum[2] = ConvertProjToView(float4(pxp / (float)uWindowWidthEvenlyDivisibleByTileRes * 2.0f - 1.0f, (uWindowHeightEvenlyDivisibleByTileRes - pyp) / (float)uWindowHeightEvenlyDivisibleByTileRes * 2.0f - 1.0f, 1.0f, 1.f));

		frustum[3] = ConvertProjToView(float4(pxm / (float)uWindowWidthEvenlyDivisibleByTileRes * 2.0f - 1.0f, (uWindowHeightEvenlyDivisibleByTileRes - pyp) / (float)uWindowHeightEvenlyDivisibleByTileRes * 2.0f - 1.0f, 1.0f, 1.0f) );

		//create plane equations for the four sides of the frustum, 
		//with the positive half-space outside the frustum (and remember, 
		//view space is left handed, so use the left-hand rule to determine 
		//cross product direction)
		for(uint i = 0; i < 4; i++)
		{
			frustumEqn[i] = CreatePlaneEquation(frustum[i], frustum[(i + 1) & 3]);
		}
	}

	GroupMemoryBarrierWithGroupSync();

	//calculate the min and max depth for this tile, 
	//to form the front and back of the frustum
	float minZ = FLT_MAX;
	float maxZ = 0.0f;

//MSAA
#ifdef _XE_MSAA

	uint depthBufferWidth = 0;
	uint depthBufferHeight = 0;
	uint depthBufferNumSamples = 0;

	_XE_DepthTexture.GetDimensions(depthBufferWidth, depthBufferHeight, depthBufferNumSamples);

	for(uint sampleIdx = 0; sampleIdx < depthBufferNumSamples; sampleIdx++)
	{
		CalculateMinMaxDepthInTile(globalIdx, sampleIdx);
	}

//Non MSAA
#else

	CalculateMinMaxDepthInTile(globalIdx, 0);

#endif

	GroupMemoryBarrierWithGroupSync();
	maxZ = asfloat(_XE_TileZMax);
	minZ = asfloat(_XE_TileZMin);

	//loop over the lights and do an intersection test against the frustum
	for(uint i = 0; i < _XE_NumLights; i += NUM_THREADS_PER_TILE)
	{
		uint lightIndex = localIdxFlattened + i;

		if(lightIndex < _XE_NumLights)
		{
			bool lightIntersects = false;

			if ((_XE_LightBuffer[lightIndex].LightFlags & LIGHT_FLAG_ENABLE) != 0)
			{
				switch (_XE_LightBuffer[lightIndex].LightType)
				{
					case LIGHT_SPOT:
						lightIntersects = SpotLightIntersectsFrustum(lightIndex, minZ, maxZ, frustumEqn);
						break;

					case LIGHT_OMNI:
						lightIntersects = PointLightIntersectsFrustum(lightIndex, minZ, maxZ, frustumEqn);
						break;

					case LIGHT_DIRECTIONAL:
						lightIntersects = true;
						break;

					default:
						lightIntersects = false;
						break;
				}
			}

			if(lightIntersects && _XE_TileLightIdxCounter < MAX_NUM_LIGHTS_PER_TILE)
			{
				// do a thread-safe increment of the list counter 
				// and put the index of this light into the list
				uint dstIdx = 0;
				InterlockedAdd(_XE_TileLightIdxCounter, 1, dstIdx);
				_XE_TileLightIdx[dstIdx] = lightIndex;
			}
		}
	}

	GroupMemoryBarrierWithGroupSync();

	{
		//////////////////////////
		//Write to RW Buffer

		//////////////////////////////////////////////
		//Start offset is max lights + 1, so space for 
		//sentinel is available if we have full lights
		uint startOffset = (MAX_NUM_LIGHTS_PER_TILE + 1) * tileIdxFlattened;

		for(uint i = localIdxFlattened; i < _XE_TileLightIdxCounter; i += NUM_THREADS_PER_TILE)
		{
			// per-tile list of light indices
			_XE_PerTileLightIndexBuffer[startOffset + i] = _XE_TileLightIdx[i];
		}

		if( localIdxFlattened == 0 )
		{
			// mark the end of each per-tile list with a sentinel
			_XE_PerTileLightIndexBuffer[startOffset + _XE_TileLightIdxCounter] = LIGHT_INDEX_BUFFER_SENTINEL;
		}
	}
}
