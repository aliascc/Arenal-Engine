/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Frustum.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Frustum::Frustum()
{
	std::fill(m_Planes.begin(), m_Planes.end(), XEMathHelpers::Vec4fZero);
	std::fill(m_Corners.begin(), m_Corners.end(), XEMathHelpers::Vec3fZero);
}

Frustum::Frustum(const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	UpdateFrustum(viewMatrix, projMatrix);
}

Frustum::~Frustum()
{
}

void Frustum::UpdateFrustum(const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	glm::mat4 viewProjection = projMatrix * viewMatrix;

	//Compute Slopes
	CreateSlopes(projMatrix);

	// Near plane
	m_Planes[XE_FRUSTUM_NEAR_PLANE].x = viewProjection[0].w + viewProjection[0].z;
	m_Planes[XE_FRUSTUM_NEAR_PLANE].y = viewProjection[1].w + viewProjection[1].z;
	m_Planes[XE_FRUSTUM_NEAR_PLANE].z = viewProjection[2].w + viewProjection[2].z;
	m_Planes[XE_FRUSTUM_NEAR_PLANE].w = viewProjection[3].w + viewProjection[3].z;

	// Far plane
	m_Planes[XE_FRUSTUM_FAR_PLANE].x = viewProjection[0].w - viewProjection[0].z;
	m_Planes[XE_FRUSTUM_FAR_PLANE].y = viewProjection[1].w - viewProjection[1].z;
	m_Planes[XE_FRUSTUM_FAR_PLANE].z = viewProjection[2].w - viewProjection[2].z;
	m_Planes[XE_FRUSTUM_FAR_PLANE].w = viewProjection[3].w - viewProjection[3].z;

	// Left plane
	m_Planes[XE_FRUSTUM_LEFT_PLANE].x = viewProjection[0].w + viewProjection[0].x;
	m_Planes[XE_FRUSTUM_LEFT_PLANE].y = viewProjection[1].w + viewProjection[1].x;
	m_Planes[XE_FRUSTUM_LEFT_PLANE].z = viewProjection[2].w + viewProjection[2].x;
	m_Planes[XE_FRUSTUM_LEFT_PLANE].w = viewProjection[3].w + viewProjection[3].x;

	// Right plane
	m_Planes[XE_FRUSTUM_RIGHT_PLANE].x = viewProjection[0].w - viewProjection[0].x;
	m_Planes[XE_FRUSTUM_RIGHT_PLANE].y = viewProjection[1].w - viewProjection[1].x;
	m_Planes[XE_FRUSTUM_RIGHT_PLANE].z = viewProjection[2].w - viewProjection[2].x;
	m_Planes[XE_FRUSTUM_RIGHT_PLANE].w = viewProjection[3].w - viewProjection[3].x;

	// Top plane
	m_Planes[XE_FRUSTUM_TOP_PLANE].x = viewProjection[0].w - viewProjection[0].y;
	m_Planes[XE_FRUSTUM_TOP_PLANE].y = viewProjection[1].w - viewProjection[1].y;
	m_Planes[XE_FRUSTUM_TOP_PLANE].z = viewProjection[2].w - viewProjection[2].y;
	m_Planes[XE_FRUSTUM_TOP_PLANE].w = viewProjection[3].w - viewProjection[3].y;

	// Bottom plane
	m_Planes[XE_FRUSTUM_BOTTOM_PLANE].x = viewProjection[0].w + viewProjection[0].y;
	m_Planes[XE_FRUSTUM_BOTTOM_PLANE].y = viewProjection[1].w + viewProjection[1].y;
	m_Planes[XE_FRUSTUM_BOTTOM_PLANE].z = viewProjection[2].w + viewProjection[2].y;
	m_Planes[XE_FRUSTUM_BOTTOM_PLANE].w = viewProjection[3].w + viewProjection[3].y;

	// Normalize planes
	for (uint32_t i = 0; i < 6; i++)
	{
		m_Planes[i] = XEMathHelpers::NormalizePlane(m_Planes[i]);
	}

	//Compute Corners
	m_Corners[XE_FRUSTUM_NEAR_BOTTOM_RIGHT]		= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_NEAR_PLANE],	m_Planes[XE_FRUSTUM_BOTTOM_PLANE],	m_Planes[XE_FRUSTUM_RIGHT_PLANE]);
	m_Corners[XE_FRUSTUM_NEAR_TOP_RIGHT]		= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_NEAR_PLANE],	m_Planes[XE_FRUSTUM_TOP_PLANE],		m_Planes[XE_FRUSTUM_RIGHT_PLANE]);
	m_Corners[XE_FRUSTUM_NEAR_TOP_LEFT]			= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_NEAR_PLANE],	m_Planes[XE_FRUSTUM_TOP_PLANE],		m_Planes[XE_FRUSTUM_LEFT_PLANE]);
	m_Corners[XE_FRUSTUM_NEAR_BOTTOM_LEFT]		= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_NEAR_PLANE],	m_Planes[XE_FRUSTUM_BOTTOM_PLANE],	m_Planes[XE_FRUSTUM_LEFT_PLANE]);

	m_Corners[XE_FRUSTUM_FAR_BOTTOM_RIGHT]		= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_FAR_PLANE],	m_Planes[XE_FRUSTUM_BOTTOM_PLANE],	m_Planes[XE_FRUSTUM_RIGHT_PLANE]);
	m_Corners[XE_FRUSTUM_FAR_TOP_RIGHT]			= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_FAR_PLANE],	m_Planes[XE_FRUSTUM_TOP_PLANE],		m_Planes[XE_FRUSTUM_RIGHT_PLANE]);
	m_Corners[XE_FRUSTUM_FAR_TOP_LEFT]			= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_FAR_PLANE],	m_Planes[XE_FRUSTUM_TOP_PLANE],		m_Planes[XE_FRUSTUM_LEFT_PLANE]);
	m_Corners[XE_FRUSTUM_FAR_BOTTOM_LEFT]		= XEMathHelpers::ComputePlaneIntersection(m_Planes[XE_FRUSTUM_FAR_PLANE],	m_Planes[XE_FRUSTUM_BOTTOM_PLANE],	m_Planes[XE_FRUSTUM_LEFT_PLANE]);
}

void Frustum::CreateSlopes(const glm::mat4& projection)
{
	// Corners of the projection frustum in homogeneous space.
	XETODO("set this in a better place");
	static glm::vec4 HomogenousPoints[6] =
	{
		{ -1.0f,  0.0f, 1.0f, 1.0f },   // right (at far plane)
		{  1.0f,  0.0f, 1.0f, 1.0f },   // left
		{  0.0f, -1.0f, 1.0f, 1.0f },   // top
		{  0.0f,  1.0f, 1.0f, 1.0f },   // bottom

		{  0.0f,  0.0f, 0.0f, 1.0f },     // near
		{  0.0f,  0.0f, 1.0f, 1.0f }      // far
	};

	glm::mat4 matInverse = glm::inverse(projection);

	// Compute the frustum corners in world space.
	glm::vec4 points[6];

	for (size_t i = 0; i < 6; ++i)
	{
		// Transform point.
		points[i] = (matInverse * HomogenousPoints[i]);
	}

	// Compute the slopes.
	points[0] = points[0] * (XEMathHelpers::Vec4fOne / glm::vec4(points[0].z, points[0].z, points[0].z, points[0].z));
	points[1] = points[1] * (XEMathHelpers::Vec4fOne / glm::vec4(points[1].z, points[1].z, points[1].z, points[1].z));
	points[2] = points[2] * (XEMathHelpers::Vec4fOne / glm::vec4(points[2].z, points[2].z, points[2].z, points[2].z));
	points[3] = points[3] * (XEMathHelpers::Vec4fOne / glm::vec4(points[3].z, points[3].z, points[3].z, points[3].z));

	m_RightSlope = points[0].x;
	m_LeftSlope = points[1].x;
	m_TopSlope = points[2].y;
	m_BottomSlope = points[3].y;

	// Compute near and far.
	points[4] = points[4] * (XEMathHelpers::Vec4fOne / glm::vec4(points[4].w, points[4].w, points[4].w, points[4].w));
	points[5] = points[5] * (XEMathHelpers::Vec4fOne / glm::vec4(points[5].w, points[5].w, points[5].w, points[5].w));

	XETODO("This is wrong calculate better");
	m_Near = points[4].z;
	m_Far = points[5].z;
}