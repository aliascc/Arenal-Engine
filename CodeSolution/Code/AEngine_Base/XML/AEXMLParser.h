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

#pragma once

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "AEXMLDefs.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class AEXMLParser sealed : public AEObject
{
private:

    /// <summary>
    /// XML Parsed File
    /// </summary>
    tinyxml2::XMLDocument* m_XMLDoc = nullptr;

    /// <summary>
    /// Current XML Element
    /// </summary>
    const tinyxml2::XMLElement* m_XMLElement = nullptr;

    /// <summary>
    /// Defines if this is the Parent 
    /// </summary>
    XMLType m_Type = XMLType::Empty;

    /// <summary>
    /// Gets the Attribute from the Node
    /// </summary>
    /// <param name="attributeName">Name of the Attribute</param>
    /// <returns>Returns the Attribute</returns>
    const char* GetNodeAttribute(const std::string& attributeName) const;

public:

    /// <summary>
    /// AEXMLParser Constructor
    /// </summary>
    AEXMLParser();

    /// <summary>
    /// Default AEXMLParser Destructor
    /// </summary>
    virtual ~AEXMLParser();

    /// <summary>
    /// Gets if the Parser currently has  an Element
    /// </summary>
    inline bool HasElement() const
    {
        return (m_Type != XMLType::Empty);
    }

    /// <summary>
    /// Gets the Name of current Element
    /// </summary>
    inline std::string GetName() const
    {
        if (m_Type != XMLType::Element)
        {
            return "";
        }

        return m_XMLElement->Name();
    }

    /// <summary>
    /// Loads a XML File
    /// </summary>
    /// <param name="file">Name of the XML File</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult LoadFile(const std::string& file);

    /// <summary>
    /// Gets an Element from the Parser
    /// </summary>
    /// <param name="elementName">Name of the Element</param>
    /// <returns>XML Element</returns>
    AEXMLParser GetChildElement(const std::string& elementName) const;

    /// <summary>
    /// Gets an Element from the Parser
    /// </summary>
    /// <returns>XML Element</returns>
    AEXMLParser GetFirstChildElement() const;

    /// <summary>
    /// Gets an Element from the Parser
    /// </summary>
    /// <returns>XML Element</returns>
    AEXMLParser GetNextSiblingElement() const;

    /// <summary>
    /// [] Operator Overload
    /// </summary>
    /// <param name="elementName">Element Name</param>
    /// <returns>XML Element</returns>
    AEXMLParser operator[](const std::string& elementName) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    int8_t GetInt8(const std::string& attributeName, int8_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    uint8_t GetUInt8(const std::string& attributeName, uint8_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    int16_t GetInt16(const std::string& attributeName, int16_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    uint16_t GetUInt16(const std::string& attributeName, uint16_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    int32_t GetInt(const std::string& attributeName, int32_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    uint32_t GetUInt(const std::string& attributeName, uint32_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    int64_t GetInt64(const std::string& attributeName, int64_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    uint64_t GetUInt64(const std::string& attributeName, uint64_t defaultValue = 0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    float GetFloat(const std::string& attributeName, float defaultValue = 0.0f, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    double GetDouble(const std::string& attributeName, double defaultValue = 0.0, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    bool GetBool(const std::string& attributeName, bool defaultValue = false, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    std::string GetString(const std::string& attributeName, const std::string& defaultValue = "", bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::vec2 GetVect2f(const std::string& attributeName, const glm::vec2& defaultValue = AEMathHelpers::Vec2fZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::vec3 GetVect3f(const std::string& attributeName, const glm::vec3& defaultValue = AEMathHelpers::Vec3fZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::vec4 GetVect4f(const std::string& attributeName, const glm::vec4& defaultValue = AEMathHelpers::Vec4fZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::dvec2 GetVect2d(const std::string& attributeName, const glm::dvec2& defaultValue = AEMathHelpers::Vec2dZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::dvec3 GetVect3d(const std::string& attributeName, const glm::dvec3& defaultValue = AEMathHelpers::Vec3dZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::dvec4 GetVect4d(const std::string& attributeName, const glm::dvec4& defaultValue = AEMathHelpers::Vec4dZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::bvec2 GetVect2b(const std::string& attributeName, const glm::bvec2& defaultValue = AEMathHelpers::Vec2bZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::bvec3 GetVect3b(const std::string& attributeName, const glm::bvec3& defaultValue = AEMathHelpers::Vec3bZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::bvec4 GetVect4b(const std::string& attributeName, const glm::bvec4& defaultValue = AEMathHelpers::Vec4bZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::ivec2 GetVect2i(const std::string& attributeName, const glm::ivec2& defaultValue = AEMathHelpers::Vec2iZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::ivec3 GetVect3i(const std::string& attributeName, const glm::ivec3& defaultValue = AEMathHelpers::Vec3iZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::ivec4 GetVect4i(const std::string& attributeName, const glm::ivec4& defaultValue = AEMathHelpers::Vec4iZero, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::mat2 GetMat2f(const std::string& attributeName, const glm::mat2& defaultValue = AEMathHelpers::Mat2Identity, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::mat3 GetMat3f(const std::string& attributeName, const glm::mat3& defaultValue = AEMathHelpers::Mat3Identity, bool warning = true) const;

    /// <summary>
    /// Get the value of the property
    /// </summary>
    /// <param name="attributeName">Name of the property</param>
    /// <param name="defaultValue">Default value for the property if not found</param>
    /// <param name="warning">Writes a warning to the log if the property was not found</param>
    /// <returns>Value of the property</returns>
    glm::mat4 GetMat4f(const std::string& attributeName, const glm::mat4& defaultValue = AEMathHelpers::Mat4Identity, bool warning = true) const;

};