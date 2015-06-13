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
#include "Object3D.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Object3D::Object3D(const std::wstring& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
	: UniqueXEObjectNamed(name)
	, m_Position(position)
	, m_Rotation(rotation)
	, m_Scale(scale)
{
}

Object3D::~Object3D()
{
}

void Object3D::CallObject3DChangedEvents(Object3DChangeEventType changeType, uint64_t cbIdIgnore)
{
	for (auto callbackPair : m_Object3DChangedEventList)
	{
		if (cbIdIgnore != callbackPair.first)
		{
			Object3DChangedEvent changeEvent = callbackPair.second;

			changeEvent(changeType, this);
		}
	}
}

glm::quat Object3D::GetLocalRotationQuat() const
{
	return XEMathHelpers::CreateQuatRotation(m_Rotation);
}

glm::mat4 Object3D::GetLocalTransform() const
{
	return XEMathHelpers::CreateTransform(m_Position, m_Rotation, m_Scale);
}

glm::mat4 Object3D::GetLocalScaleTransform() const
{
	return glm::scale(m_Scale);
}

glm::vec3 Object3D::GetWorldPosition() const
{
	if (m_ParentObject3D != nullptr)
	{
		//////////////////////////////////////////////////
		// Get the Position vector
		glm::vec3 worldPosition = XEMathHelpers::GetPositionVectorFromMatrix(this->GetWorldTransform());

		return worldPosition;
	}
	else
	{
		return m_Position;
	}
}

glm::vec3 Object3D::GetWorldRotationAngles() const
{
	if (m_ParentObject3D != nullptr)
	{
		glm::quat worldRotationQuat = GetWorldRotationQuat();

		glm::vec3 worldRotationAngles = glm::degrees(XEMathHelpers::GetEulerAnglesFromQuaternionRotation(worldRotationQuat));

		return worldRotationAngles;
	}
	else
	{
		return m_Rotation;
	}
}

glm::quat Object3D::GetWorldRotationQuat() const
{
	glm::quat localRotationQuat = GetLocalRotationQuat();

	if (m_ParentObject3D != nullptr)
	{
		return m_ParentObject3D->GetWorldRotationQuat() * localRotationQuat;
	}
	else
	{
		return localRotationQuat;
	}
}

glm::mat4 Object3D::GetWorldTransform() const
{
	glm::mat4 localTransform = GetLocalTransform();

	if (m_ParentObject3D != nullptr)
	{
		return m_ParentObject3D->GetWorldTransform() * localTransform;
	}
	else
	{
		return localTransform;
	}
}

glm::mat4 Object3D::GetWorldScaleTransform() const
{
	glm::mat4 localScaleTransform = GetLocalScaleTransform();

	if (m_ParentObject3D != nullptr)
	{
		return m_ParentObject3D->GetWorldScaleTransform() * localScaleTransform;
	}
	else
	{
		return localScaleTransform;
	}
}

glm::vec3 Object3D::GetDirection() const
{
	return XEMathHelpers::CalculateDirectionFromQuaterionRotation(GetWorldRotationQuat());
}

void Object3D::UpdateTransformToNewParent(const glm::mat4& newParentTransform, const glm::quat& newParentQuaternionRot, const glm::mat4& newParentScaleTransform)
{
	glm::vec3 newPos		= XEMathHelpers::Vec3fZero;
	glm::vec3 newRot		= XEMathHelpers::Vec3fZero;;
	glm::vec3 newScale		= XEMathHelpers::Vec3fOne;;

	//////////////////////////////////////////////////
	// Sets the current transformations
	glm::quat currentWorldQuaternionRot		= this->GetWorldRotationQuat();
	glm::mat4 currentWorldTransform			= this->GetWorldTransform();
	glm::mat4 currentWorldScaleTransform	= this->GetWorldScaleTransform();

	//////////////////////////////////////////////////
	// Get new Transforms
	XEMathHelpers::GetTransformationFromRelativeToParent(newParentTransform, newParentQuaternionRot, newParentScaleTransform, currentWorldQuaternionRot, currentWorldTransform, currentWorldScaleTransform, newPos, newRot, newScale);

	//////////////////////////////////////////////////
	// Set new Transforms
	SetPosition(newPos);
	SetRotation(newRot);
	SetScale(newScale);
}

bool Object3D::ExistsObject3DChangeEventCallback(uint64_t id)
{
	for (auto callbackPair : m_Object3DChangedEventList)
	{
		if (id == callbackPair.first)
		{
			return true;
		}
	}

	return false;
}

XEResult Object3D::AddObject3DChangeEventCallback(uint64_t id, Object3DChangedEvent changeEventCallback)
{
	if (ExistsObject3DChangeEventCallback(id))
	{
		return XEResult::ObjExists;
	}

	Object3DChangedEventPair pair(id, changeEventCallback);
	m_Object3DChangedEventList.push_back(pair);

	return XEResult::Ok;
}

XEResult Object3D::RemoveObject3DChangeEventCallback(uint64_t id)
{
	auto it = m_Object3DChangedEventList.begin();
	auto itEnd = m_Object3DChangedEventList.end();
	for (; it != itEnd; it++)
	{
		if (id == it->first)
		{
			m_Object3DChangedEventList.erase(it);

			return XEResult::Ok;
		}
	}

	return XEResult::NotFound;
}
