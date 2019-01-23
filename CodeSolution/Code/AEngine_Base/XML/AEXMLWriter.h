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

class AEXMLWriter sealed : public AEObject
{
private:

    /// <summary>
    /// Defines if the XML Writer
    /// is initialized and ready to use
    /// </summary>
    bool m_IsReady = false;

    /// <summary>
    /// XML Document
    /// </summary>
    tinyxml2::XMLDocument m_XMLDoc;

    /// <summary>
    /// Keeps Track of the Elements been created
    /// </summary>
    std::stack<tinyxml2::XMLElement*> m_XMLElementStack;

    /// <summary>
    /// Current Element
    /// </summary>
    tinyxml2::XMLElement* m_XMLElement = nullptr;

    /// <summary>
    /// File name of the XML
    /// </summary>
    std::string m_Filename = "";

public:

    /// <summary>
    /// AEXMLWriter Constructor
    /// </summary>
    AEXMLWriter();

    /// <summary>
    /// Default AEXMLWriter Destructor
    /// </summary>
    virtual ~AEXMLWriter();

    //Delete copy constructor/operator
    AEXMLWriter(const AEXMLWriter&) = delete;
    AEXMLWriter& operator=(const AEXMLWriter&) = delete;

    /// <summary>
    /// Creates a new XML File
    /// </summary>
    /// <param name="file">Name of the XML File</param>
    /// <param name="inMemory">Defines if the buffer is to be kept in memory</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult CreateXML(const std::string& file, bool inMemory);

    /// <summary>
    /// Starts a new Node
    /// </summary>
    /// <param name="name">Name of the Node</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult StartNode(const std::string& name);

    /// <summary>
    /// Finalizes the current Node
    /// </summary>
    /// <returns>AEResult::OK if successful</returns>
    AEResult EndNode(); 

    /// <summary>
    /// Finalizes the current XML File
    /// </summary>
    /// <returns>AEResult::OK if successful</returns>
    AEResult FinalizeXML();

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteString(const std::string& attributeName, const std::string& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteInt8(const std::string& attributeName, int8_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteUInt8(const std::string& attributeName, uint8_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteInt16(const std::string& attributeName, int16_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteUInt16(const std::string& attributeName, uint16_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteInt(const std::string& attributeName, int32_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteUInt(const std::string& attributeName, uint32_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteInt64(const std::string& attributeName, int64_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteUInt64(const std::string& attributeName, uint64_t value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteFloat(const std::string& attributeName, float value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteDouble(const std::string& attributeName, double value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteBool(const std::string& attributeName, bool value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect2f(const std::string& attributeName, const glm::vec2& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect3f(const std::string& attributeName, const glm::vec3& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect4f(const std::string& attributeName, const glm::vec4& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect2d(const std::string& attributeName, const glm::dvec2& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect3d(const std::string& attributeName, const glm::dvec3& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect4d(const std::string& attributeName, const glm::dvec4& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect2b(const std::string& attributeName, const glm::bvec2& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect3b(const std::string& attributeName, const glm::bvec3& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect4b(const std::string& attributeName, const glm::bvec4& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect2i(const std::string& attributeName, const glm::ivec2& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect3i(const std::string& attributeName, const glm::ivec3& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteVect4i(const std::string& attributeName, const glm::ivec4& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteMat2f(const std::string& attributeName, const glm::mat2& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteMat3f(const std::string& attributeName, const glm::mat3& value);

    /// <summary>
    /// Writes a property value to the XML
    /// </summary>
    /// <param name="name">Name of the property</param>
    /// <param name="value">Value of the property</param>
    /// <returns>AEResult::OK if successful</returns>
    AEResult WriteMat4f(const std::string& attributeName, const glm::mat4& value);
};