/********************************************************
*
* Author: Carlos Chacón N.
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "XEMathDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/*********************************
*   Function and Variable Defs   *
**********************************/
namespace XEMathHelpers
{
	/********************
	*   Variable Defs   *
	*********************/
	const glm::uvec2 Vec2uZero				= glm::uvec2(0, 0);

	const glm::ivec2 Vec2iZero				= glm::ivec2(0, 0);
	
	const glm::ivec3 Vec3iZero				= glm::ivec3(0, 0, 0);
	
	const glm::ivec4 Vec4iZero				= glm::ivec4(0, 0, 0, 0);

	const glm::vec2 Vec2fZero				= glm::vec2(0.0f, 0.0f);

	const glm::vec3 Vec3fZero				= glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 Vec3fOne				= glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 Vec3fUp					= glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 Vec3fFwrZP				= glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 Vec3fFwrZN				= glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3 Vec3fInfite				= glm::vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	const glm::vec3 Vec3fMax				= glm::vec3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	const glm::vec3 Vec3fMin				= glm::vec3(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	const glm::vec3 Vec3fHalf				= glm::vec3(0.5f, 0.5f, 0.5f);

	const glm::vec4 Vec4fZero				= glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	const glm::vec4 Vec4fOne				= glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const glm::vec4 Vec4fInfite				= glm::vec4(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
	const glm::vec4 Vec4fMax				= glm::vec4(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	const glm::vec4 Vec4fMin				= glm::vec4(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
	const glm::vec4 Vec4fHalf				= glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);

	const glm::mat4 Mat4Identity			= glm::mat4(1.0f, 0.0f, 0.0f, 0.0f,
														0.0f, 1.0f, 0.0f, 0.0f,
														0.0f, 0.0f, 1.0f, 0.0f,
														0.0f, 0.0f, 0.0f, 1.0f);

	const glm::quat QuaternionIdentity		= glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	/********************
	*   Function Defs   *
	*********************/
	void CalculateTangentsAndBinormal(void* vertexData, const std::vector<uint16_t>& idxsData, uint32_t vtxCount, uint32_t vertexStride, uint32_t geometryStride, uint32_t normalStride, uint32_t tangentStride, uint32_t binormalStride, uint32_t textureCoordsStride)
	{
		glm::vec3* tangent1 = new glm::vec3[vtxCount * 2];
		glm::vec3* tangent2 = tangent1 + vtxCount;

		//Clean memory
		memset(tangent1, 0, sizeof(glm::vec3) * vtxCount * 2);

		unsigned char *vtxAddress=(unsigned char *)vertexData;

		uint32_t idxCount = static_cast<uint32_t>(idxsData.size());
		for(uint32_t b = 0; b < idxCount; b += 3)
		{
			unsigned short i1 = idxsData[b];
			unsigned short i2 = idxsData[b + 1];
			unsigned short i3 = idxsData[b + 2];

			glm::vec3 *v1 = (glm::vec3*)&vtxAddress[i1 * vertexStride + geometryStride];
			glm::vec3 *v2 = (glm::vec3*)&vtxAddress[i2 * vertexStride + geometryStride];
			glm::vec3 *v3 = (glm::vec3*)&vtxAddress[i3 * vertexStride + geometryStride];

			glm::vec2 *w1 = (glm::vec2*)&vtxAddress[i1 * vertexStride + textureCoordsStride];
			glm::vec2 *w2 = (glm::vec2*)&vtxAddress[i2 * vertexStride + textureCoordsStride];
			glm::vec2 *w3 = (glm::vec2*)&vtxAddress[i3 * vertexStride + textureCoordsStride];

			float x1 = v2->x - v1->x;
			float x2 = v3->x - v1->x;

			float y1 = v2->y - v1->y;
			float y2 = v3->y - v1->y;

			float z1 = v2->z - v1->z;
			float z2 = v3->z - v1->z;

			float s1 = w2->x - w1->x;
			float s2 = w3->x - w1->x;

			float t1 = w2->y - w1->y;
			float t2 = w3->y - w1->y;

			float r = 1.0F / (s1 * t2 - s2 * t1);

			glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
			glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

			XEAssert(i1<vtxCount);
			XEAssert(i2<vtxCount);
			XEAssert(i3<vtxCount);

			tangent1[i1] = tangent1[i1] + sdir;
			tangent1[i2] = tangent1[i2] + sdir;
			tangent1[i3] = tangent1[i3] + sdir;

			tangent1[i1] = tangent1[i1] + tdir;
			tangent1[i2] = tangent1[i2] + tdir;
			tangent1[i3] = tangent1[i3] + tdir;
		}

		for (uint32_t b = 0; b < vtxCount; ++b)
		{
			glm::vec3* normalVtx = (glm::vec3 *)&vtxAddress[b * vertexStride + binormalStride];

			glm::vec3* tangentVtx = (glm::vec3 *)&vtxAddress[b * vertexStride + binormalStride];

			glm::vec4* tangentVtx4 = (glm::vec4 *)&vtxAddress[b * vertexStride + binormalStride];

			glm::vec3* binormalVtx = (glm::vec3 *)&vtxAddress[b * vertexStride + binormalStride];

			const glm::vec3& t = tangent1[b];

			// Gram-Schmidt orthogonalize
			glm::vec3 val = XEMathHelpers::Vec3fZero;
			float nDt = glm::dot(*normalVtx, t);
			val = *normalVtx * nDt;
			val = t - val;
			glm::normalize(val);
			//tangent[a] = (t - n * Dot(n, t)).Normalize();
			
			// Calculate handedness
			glm::vec3 cross = XEMathHelpers::Vec3fZero;
			cross = glm::cross(*normalVtx, *tangentVtx);
			tangentVtx4->w = (glm::dot(cross, tangent2[b]) < 0.0f ? -1.0f : 1.0f);
			//tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;

			*binormalVtx = glm::cross(*normalVtx, *tangentVtx);
		}

		DeleteMemArr(tangent1);
	}
}
