/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AE_MATH_DEFS_H
#define _AE_MATH_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <limits>
#include <stdint.h>
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "glm\gtx\norm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\quaternion.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\AEObject.h"
#include "Base\BaseFunctions.h"

/************
*   Using   *
*************/

/************
*   Enums   *
*************/

/// <summary>
/// Enum to define order of Rotation Order for Multiplying
/// Each rotation order can produce different end orientation
/// </summary>
enum class RotationOrder : uint32_t
{
	EulerXYZ = 0,
	EulerXZY,
	EulerYZX,
	EulerYXZ,
	EulerZXY,
	EulerZYX,
};

/**************
*   Defines   *
***************/
#define AE_DEFAULT_ROTATION_ORDER	RotationOrder::EulerZYX

/*************************************
*   Global Functions and Variables   *
**************************************/
namespace AEMathHelpers
{
	/***********************
	*   Global Variables   *
	************************/
	extern const glm::dvec2 Vec2dZero;

	extern const glm::bvec2 Vec2bZero;

	extern const glm::uvec2 Vec2uZero;

	extern const glm::ivec2 Vec2iZero;

	extern const glm::vec2 Vec2fZero;
	
	extern const glm::ivec3 Vec3iZero;

	extern const glm::vec3 Vec3fZero;
	extern const glm::vec3 Vec3fOne;
	extern const glm::vec3 Vec3fUp;
	extern const glm::vec3 Vec3fFwrZP;
	extern const glm::vec3 Vec3fFwrZN;
	extern const glm::vec3 Vec3fInfite;
	extern const glm::vec3 Vec3fMax;
	extern const glm::vec3 Vec3fMin;
	extern const glm::vec3 Vec3fHalf;

	extern const glm::dvec3 Vec3dZero;

	extern const glm::bvec3 Vec3bZero;

	extern const glm::ivec4 Vec4iZero;

	extern const glm::dvec4 Vec4dZero;

	extern const glm::bvec4 Vec4bZero;

	extern const glm::vec4 Vec4fZero;
	extern const glm::vec4 Vec4fOne;
	extern const glm::vec4 Vec4fInfite;
	extern const glm::vec4 Vec4fMax;
	extern const glm::vec4 Vec4fMin;
	extern const glm::vec4 Vec4fHalf;

	extern const glm::mat2 Mat2Identity;

	extern const glm::mat3 Mat3Identity;

	extern const glm::mat4 Mat4Identity;

	extern const glm::quat QuaternionIdentity;

	/***********************
	*   Global Functions   *
	************************/
	template<typename T>
	inline T Lerp(const T a, const T b, const T amount)
	{ 
		return a * ( (T)1 - amount ) + b * amount; 
	}

	inline bool EqualRECT(const RECT& a, const RECT& b)
	{
		return (a.top == b.top && a.left == b.left && a.right == b.right && a.bottom == b.bottom);
	}

	inline float GaussianDistribution(float x, float a, float theta)
	{
		float g = 1.0f / ( sqrt(2.0f * glm::pi<float>()) * theta );

		g *= exp( -( (x * x) + (a * a) + (-2 * x * a) ) / ( 2 * theta * theta) );

		return g;
	}

	inline glm::mat4 AffineTransformationMatrix(const glm::vec3& translation, const glm::vec3& rotationOrigin, const glm::quat& quaternionRotation, const glm::vec3& scale)
	{
		glm::mat4 translationMatrix		= glm::translate(translation);
		glm::mat4 rotationOriginMatrix	= glm::translate(rotationOrigin);
		glm::mat4 rotationMatrix		= glm::mat4_cast(quaternionRotation);
		glm::mat4 scaleMatrix			= glm::scale(scale);

		// M = MTranslation * MRotationOrigin * MRotation * Inverse(MRotationOrigin) * MScaling
		glm::mat4 affineMatrix = translationMatrix * rotationOriginMatrix * rotationMatrix * glm::inverse(rotationOriginMatrix) * scaleMatrix;

		return affineMatrix;
	}

	inline glm::mat4 CreateRotationX(float angle)
	{
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));

		return rotationMat;
	}

	inline glm::mat4 CreateRotationY(float angle)
	{
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

		return rotationMat;
	}

	inline glm::mat4 CreateRotationZ(float angle)
	{
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

		return rotationMat;
	}

	inline glm::mat4 CreateRotation(const glm::vec3& rotation, RotationOrder rotationOrder = AE_DEFAULT_ROTATION_ORDER)
	{
		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::mat4 rotationMat = Mat4Identity;

		switch (rotationOrder)
		{
			case RotationOrder::EulerXYZ:
				rotationMat = rotX * rotY * rotZ;
				break;
			case RotationOrder::EulerXZY:
				rotationMat = rotX * rotZ * rotY;
				break;
			case RotationOrder::EulerYZX:
				rotationMat = rotY * rotZ * rotX;
				break;
			case RotationOrder::EulerYXZ:
				rotationMat = rotY * rotX * rotZ;
				break;
			case RotationOrder::EulerZXY:
				rotationMat = rotZ * rotX * rotY;
				break;
			case RotationOrder::EulerZYX:
				rotationMat = rotZ * rotY * rotX;
				break;
			default:
				AEAssert(false);
				return rotationMat;
		}

		return rotationMat;
	}

	inline glm::mat4 CreateTransform(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale, RotationOrder rotationOrder = AE_DEFAULT_ROTATION_ORDER)
	{
		//Transform = Position * Rotation * Scale
		glm::mat4 transformMat = glm::translate(translation) * CreateRotation(rotation, rotationOrder) * glm::scale(scale);

		return transformMat;
	}

	inline glm::quat CreateQuatRotation(const glm::vec3& angles, RotationOrder rotationOrder = AE_DEFAULT_ROTATION_ORDER)
	{
		glm::quat rotX = glm::angleAxis(glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::quat rotY = glm::angleAxis(glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::quat rotZ = glm::angleAxis(glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));

		glm::quat rotationQuat = QuaternionIdentity;

		switch (rotationOrder)
		{
			case RotationOrder::EulerXYZ:
				rotationQuat = rotX * rotY * rotZ;
				break;
			case RotationOrder::EulerXZY:
				rotationQuat = rotX * rotZ * rotY;
				break;
			case RotationOrder::EulerYZX:
				rotationQuat = rotY * rotZ * rotX;
				break;
			case RotationOrder::EulerYXZ:
				rotationQuat = rotY * rotX * rotZ;
				break;
			case RotationOrder::EulerZXY:
				rotationQuat = rotZ * rotX * rotY;
				break;
			case RotationOrder::EulerZYX:
				rotationQuat = rotZ * rotY * rotX;
				break;
			default:
				AEAssert(false);
				return rotationQuat;
		}

		return rotationQuat;
	}

	inline glm::quat CreateQuatRotation(float pitch, float yaw, float roll, RotationOrder rotationOrder = AE_DEFAULT_ROTATION_ORDER)
	{
		return CreateQuatRotation(glm::vec3(pitch, yaw, roll), rotationOrder);
	}

	inline glm::vec3 CalculateDirectionFromQuaterionRotation(const glm::quat& quaternionRot, const glm::vec3& forwardVector = Vec3fFwrZP)
	{
		glm::vec3 vecDirection = (quaternionRot * forwardVector);

		if (glm::length2(vecDirection) != 0.0f)
		{
			vecDirection = glm::normalize(vecDirection);
		}

		return vecDirection;
	}

	inline glm::vec3 CalculateDirectionFromAngles(const glm::vec3& angles, const glm::vec3& forwardVector = Vec3fFwrZP, RotationOrder rotationOrder = AE_DEFAULT_ROTATION_ORDER)
	{
		glm::quat rotQuat = CreateQuatRotation(angles, rotationOrder);

		return CalculateDirectionFromQuaterionRotation(rotQuat, forwardVector);
	}

	inline glm::vec3 GetScaleVectorFromTransformationMatrix(const glm::mat4& transformationMatrix)
	{
		glm::vec3 scaleVector = Vec3fZero;

		float x = (transformationMatrix[0].x * transformationMatrix[0].x) + (transformationMatrix[0].y * transformationMatrix[0].y) + (transformationMatrix[0].z * transformationMatrix[0].z);
		float y = (transformationMatrix[1].x * transformationMatrix[1].x) + (transformationMatrix[1].y * transformationMatrix[1].y) + (transformationMatrix[1].z * transformationMatrix[1].z);
		float z = (transformationMatrix[2].x * transformationMatrix[2].x) + (transformationMatrix[2].y * transformationMatrix[2].y) + (transformationMatrix[2].z * transformationMatrix[2].z);

		scaleVector.x = (x != 0) ? glm::sqrt(x) : 0;
		scaleVector.y = (y != 0) ? glm::sqrt(y) : 0;
		scaleVector.z = (z != 0) ? glm::sqrt(z) : 0;

		return scaleVector;
	}

	inline glm::mat4 __AE_GetRotationMatrixFromTransformationMatrix(const glm::mat4& transformationMatrix, const glm::vec3& scaleVector)
	{
		glm::mat4 rotationMatrix = AEMathHelpers::Mat4Identity;

		AEAssert(scaleVector.x != 0);
		rotationMatrix[0] = (scaleVector.x != 0) ? (transformationMatrix[0] / scaleVector.x) : Vec4fZero;

		AEAssert(scaleVector.y != 0);
		rotationMatrix[1] = (scaleVector.y != 0) ? (transformationMatrix[1] / scaleVector.y) : Vec4fZero;

		AEAssert(scaleVector.z != 0);
		rotationMatrix[2] = (scaleVector.z != 0) ? (transformationMatrix[2] / scaleVector.z) : Vec4fZero;

		return rotationMatrix;
	}

	inline glm::mat4 GetRotationMatrixFromTransformationMatrix(const glm::mat4& transformationMatrix)
	{
		glm::vec3 scaleVector = GetScaleVectorFromTransformationMatrix(transformationMatrix);

		glm::mat4 rotationMatrix = __AE_GetRotationMatrixFromTransformationMatrix(transformationMatrix, scaleVector);

		return rotationMatrix;
	}

	inline glm::vec3 GetPositionVectorFromMatrix(const glm::mat4& transformationMatrix)
	{
		glm::vec3 positionVector = Vec3fZero;

		positionVector.x = transformationMatrix[3].x;
		positionVector.y = transformationMatrix[3].y;
		positionVector.z = transformationMatrix[3].z;

		return positionVector;
	}

	/// <summary>
	/// Gets the Euler Angles from a Quaternion Rotation
	/// </summary>
	/// <remarks>
	/// Formula from here:
	/// http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/Quaternions.pdf
	/// <remarks>
	/// <param name="quatRot">Quaternion Rotation to extract Angles From</param>
	/// <param name="rotationOrder">Rotation Order of the Quaternions that where multiplied</param>
	/// <returns>Returns Euler Angles in Radians</returns>
	inline glm::vec3 GetEulerAnglesFromQuaternionRotation(const glm::quat& quatRot, RotationOrder rotationOrder = AE_DEFAULT_ROTATION_ORDER)
	{
		//p0 = quatRot.w
		//p1 = quatRot.x
		//p2 = quatRot.y
		//p3 = quatRot.z

		glm::vec3 eulerAngleVector = Vec3fZero;

		float e = 0.0f;
		switch (rotationOrder)
		{
			case RotationOrder::EulerZYX:
				e = -1.0f;
				break;
			case RotationOrder::EulerYZX:
				e = 1.0f;
				break;
			case RotationOrder::EulerXZY:
				e = -1.0f;
				break;
			case RotationOrder::EulerZXY:
				e = 1.0f;
				break;
			case RotationOrder::EulerYXZ:
				e = -1.0f;
				break;
			case RotationOrder::EulerXYZ:
				e = 1.0f;
				break;
			default:
				break;
		}

		eulerAngleVector.y = glm::asin(2.0f * ((quatRot.w * quatRot.y) + (e * (quatRot.x * quatRot.z))));

		if (eulerAngleVector.y > glm::half_pi<float>() - 0.0001f && eulerAngleVector.y < glm::half_pi<float>() + 0.0001f)
		{
			eulerAngleVector.x = atan2f(quatRot.x, quatRot.w);
			eulerAngleVector.z = 0.0f;
		}
		else
		{
			eulerAngleVector.x = atan2f((2.0f * ((quatRot.w * quatRot.x) - (e * (quatRot.y * quatRot.z)))), (1.0f - (2.0f * ((quatRot.x * quatRot.x) + (quatRot.y * quatRot.y)))));
			eulerAngleVector.z = atan2f((2.0f * ((quatRot.w * quatRot.z) - (e * (quatRot.x * quatRot.y)))), (1.0f - (2.0f * ((quatRot.y * quatRot.y) + (quatRot.z * quatRot.z)))));
		}

		return eulerAngleVector;
	}

	/// <summary>
	/// Gets the Billboard Rotation Matrix from a Camera View Matrix
	/// </summary>
	/// <remarks>
	/// Formula from here:
	/// http://swiftcoder.wordpress.com/2008/11/25/constructing-a-billboard-matrix/
	/// <remarks>
	/// <param name="viewMatrx">Camera View Matrix</param>
	/// <returns>Billboard Rotation Matrix</returns>
	inline glm::mat4 GetBillboardRotationMatrixFromCameraView(const glm::mat4& viewMatrx)
	{
		glm::mat4 billBoardRotation = Mat4Identity;

		//Transpose Rotation of the View Matrix Rotation

		//Row 1
		billBoardRotation[0].x = viewMatrx[0].x;
		billBoardRotation[0].y = viewMatrx[1].x;
		billBoardRotation[0].z = viewMatrx[2].x;

		//Row 2
		billBoardRotation[1].x = viewMatrx[0].y;
		billBoardRotation[1].y = viewMatrx[1].y;
		billBoardRotation[1].z = viewMatrx[2].y;

		//Row 3
		billBoardRotation[2].x = viewMatrx[0].z;
		billBoardRotation[2].y = viewMatrx[1].z;
		billBoardRotation[2].z = viewMatrx[2].z;

		return billBoardRotation;
	}

	/// <summary>
	/// Normalizes a Plane
	/// </summary>
	/// <param name="plane">Plane to normalize</param>
	/// <returns>Normalized Plane if plane normal length is != 0, if not plane returned is infinite</returns>
	inline glm::vec4 NormalizePlane(const glm::vec4& plane)
	{
		float length = 0.0f;
		glm::vec4 planeNormalize = Vec4fZero;
		glm::vec3 planeNormal(plane.x, plane.y, plane.z);

		if (glm::length2(planeNormal) == 0.0f)
		{
			return Vec4fInfite;
		}

		length = glm::length(planeNormal);

		planeNormalize = plane / length;

		return planeNormalize;
	}

	/// <summary>
	/// Computes the Intersection Point of 3 Planes
	/// </summary>
	/// <param name="plane1">First Plane</param>
	/// <param name="plane2">Second Plane</param>
	/// <param name="plane3">Third Plane</param>
	/// <returns>Intersection Point of 3 Planes in space, returns infinite if no intersection is found</returns>
	inline glm::vec3 ComputePlaneIntersection(const glm::vec4& plane1, const glm::vec4& plane2, const glm::vec4& plane3)
	{
		// q = -D1(N2 X N3) + -D2(N3 X N1) + -D3(N1 X N2)
		//     ---------------------------------------
		//              N1.(N2 X N3)

		glm::vec3 normal1(plane1.x, plane1.y, plane1.z);
		glm::vec3 normal2(plane2.x, plane2.y, plane2.z);
		glm::vec3 normal3(plane3.x, plane3.y, plane3.z);

		glm::vec3 N2CN3		= Vec3fZero;
		glm::vec3 N3CN1		= Vec3fZero;
		glm::vec3 N1CN2		= Vec3fZero;
		glm::vec3 D1N2CN3	= Vec3fZero;
		glm::vec3 D2N3CN1	= Vec3fZero;
		glm::vec3 D3N1CN2	= Vec3fZero;
		glm::vec3 upSum		= Vec3fZero;
		glm::vec3 result	= Vec3fZero;

		float N1DN2CN3 = 0.0f;

		N2CN3 = glm::cross(normal2, normal3);
		N3CN1 = glm::cross(normal3, normal1);
		N1CN2 = glm::cross(normal1, normal2);

		N1DN2CN3 = glm::dot(normal1, N2CN3);

		if (N1DN2CN3 == 0.0f)
		{
			return Vec3fInfite;
		}

		D1N2CN3 = -plane1.w * N2CN3;
		D2N3CN1 = -plane2.w * N3CN1;
		D3N1CN2 = -plane3.w * N1CN2;

		upSum = D1N2CN3 + D2N3CN1 + D3N1CN2;

		result = upSum / N1DN2CN3;

		return result;
	}

	inline glm::vec4 VectorSelect(const glm::vec4& vec1, const glm::vec4& vec2, const glm::vec4& constrol)
	{
		glm::vec4 res = Vec4fZero;

		res[0] = static_cast<float>((static_cast<uint32_t>(vec1[0]) & ~static_cast<uint32_t>(constrol[0])) | (static_cast<uint32_t>(vec2[0]) & static_cast<uint32_t>(constrol[0])));
		res[1] = static_cast<float>((static_cast<uint32_t>(vec1[1]) & ~static_cast<uint32_t>(constrol[1])) | (static_cast<uint32_t>(vec2[1]) & static_cast<uint32_t>(constrol[1])));
		res[2] = static_cast<float>((static_cast<uint32_t>(vec1[2]) & ~static_cast<uint32_t>(constrol[2])) | (static_cast<uint32_t>(vec2[2]) & static_cast<uint32_t>(constrol[2])));
		res[3] = static_cast<float>((static_cast<uint32_t>(vec1[3]) & ~static_cast<uint32_t>(constrol[3])) | (static_cast<uint32_t>(vec2[3]) & static_cast<uint32_t>(constrol[3])));

		return res;
	}

	inline void GetTransformationFromRelativeToParent(const glm::mat4& parentTransform, const glm::quat& parentQuaternionRot, const glm::mat4& parentScaleTransform, const glm::quat& currentWorldQuaternionRot, const glm::mat4& currentWorldTransform, const glm::mat4& currentWorldScaleTransform, glm::vec3& localPosition, glm::vec3& localRotation, glm::vec3& localScale)
	{
		//////////////////////////////////////////////////
		// Gets the new parent's transformations inverse 
		glm::quat invNewParentWorldQuaternionRot = glm::inverse(parentQuaternionRot);
		glm::mat4 invNewParentWorldTransform = glm::inverse(parentTransform);
		glm::mat4 invNewParentScaleTransform = glm::inverse(parentScaleTransform);

		//////////////////////////////////////////////////
		// Get the new Local Transformations to be set
		glm::quat newLocalQuatRotation = invNewParentWorldQuaternionRot	* currentWorldQuaternionRot;
		glm::mat4 newLocalTransform = invNewParentWorldTransform		* currentWorldTransform;
		glm::mat4 newLocalScaleTransform = invNewParentScaleTransform		* currentWorldScaleTransform;

		//////////////////////////////////////////////////
		// Get the Position vector
		localPosition = AEMathHelpers::GetPositionVectorFromMatrix(newLocalTransform);

		//////////////////////////////////////////////////
		// Get the Scale vector
		localScale = AEMathHelpers::GetScaleVectorFromTransformationMatrix(newLocalScaleTransform);

		//////////////////////////////////////////////////
		// Get the Rotation vector
		localRotation = glm::degrees(AEMathHelpers::GetEulerAnglesFromQuaternionRotation(newLocalQuatRotation));
	}

	void CalculateTangentsAndBinormal(void* vertexData, const std::vector<uint16_t>& idxsData, uint32_t vtxCount, uint32_t vertexStride, uint32_t geometryStride, uint32_t normalStride, uint32_t tangentStride, uint32_t binormalStride, uint32_t textureCoordsStride);
}

/******************
*   Struct Decl   *
*******************/

#endif