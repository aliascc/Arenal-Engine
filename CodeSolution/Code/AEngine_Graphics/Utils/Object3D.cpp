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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_graphics.h"

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
Object3D::Object3D(const std::string& name, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
    : UniqueAEObjectNamed(name)
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
    return AEMathHelpers::CreateQuatRotation(m_Rotation);
}

glm::mat4 Object3D::GetLocalTransform() const
{
    return AEMathHelpers::CreateTransform(m_Position, m_Rotation, m_Scale);
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
        glm::vec3 worldPosition = AEMathHelpers::GetPositionVectorFromMatrix(this->GetWorldTransform());

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

        glm::vec3 worldRotationAngles = glm::degrees(AEMathHelpers::GetEulerAnglesFromQuaternionRotation(worldRotationQuat));

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
    return AEMathHelpers::CalculateDirectionFromQuaterionRotation(GetWorldRotationQuat());
}

void Object3D::UpdateTransformToNewParent(const glm::mat4& newParentTransform, const glm::quat& newParentQuaternionRot, const glm::mat4& newParentScaleTransform)
{
    glm::vec3 newPos        = AEMathHelpers::Vec3fZero;
    glm::vec3 newRot        = AEMathHelpers::Vec3fZero;;
    glm::vec3 newScale      = AEMathHelpers::Vec3fOne;;

    //////////////////////////////////////////////////
    // Sets the current transformations
    glm::quat currentWorldQuaternionRot     = this->GetWorldRotationQuat();
    glm::mat4 currentWorldTransform         = this->GetWorldTransform();
    glm::mat4 currentWorldScaleTransform    = this->GetWorldScaleTransform();

    //////////////////////////////////////////////////
    // Get new Transforms
    AEMathHelpers::GetTransformationFromRelativeToParent(newParentTransform, newParentQuaternionRot, newParentScaleTransform, currentWorldQuaternionRot, currentWorldTransform, currentWorldScaleTransform, newPos, newRot, newScale);

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

AEResult Object3D::AddObject3DChangeEventCallback(uint64_t id, Object3DChangedEvent changeEventCallback)
{
    if (ExistsObject3DChangeEventCallback(id))
    {
        return AEResult::ObjExists;
    }

    Object3DChangedEventPair pair(id, changeEventCallback);
    m_Object3DChangedEventList.push_back(pair);

    return AEResult::Ok;
}

AEResult Object3D::RemoveObject3DChangeEventCallback(uint64_t id)
{
    auto it = m_Object3DChangedEventList.begin();
    auto itEnd = m_Object3DChangedEventList.end();
    for (; it != itEnd; it++)
    {
        if (id == it->first)
        {
            m_Object3DChangedEventList.erase(it);

            return AEResult::Ok;
        }
    }

    return AEResult::NotFound;
}
