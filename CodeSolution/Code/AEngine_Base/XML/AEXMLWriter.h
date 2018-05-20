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

#ifndef _AE_XML_WRITER_H
#define _AE_XML_WRITER_H

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

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Defines if the XML Writer
        /// is initialized and ready to use
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Defines if the XML Writer has
        /// not written its data to disk
        /// </summary>
        bool m_InMemory = false;

        /// <summary>
        /// XML Writter Object
        /// </summary>
        xmlTextWriterPtr m_XMLWriter = nullptr;

        /// <summary>
        /// XML Buffer
        /// </summary>
        xmlBufferPtr m_XMLBuffer = nullptr;

        /// <summary>
        /// File name of the XML
        /// </summary>
        std::string m_Filename = "";

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Clean up any memory of the
        /// XML Writer
        /// </summary>
        void CleanUp();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// AEXMLWriter Constructor
        /// </summary>
        AEXMLWriter();

        /// <summary>
        /// Default AEXMLWriter Destructor
        /// </summary>
        virtual ~AEXMLWriter();

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

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
        AEResult WriteString(const std::string& propName, const std::string& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteInt8(const std::string& propName, int8_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteUInt8(const std::string& propName, uint8_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteInt16(const std::string& propName, int16_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteUInt16(const std::string& propName, uint16_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteInt(const std::string& propName, int32_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteUInt(const std::string& propName, uint32_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteInt64(const std::string& propName, int64_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteUInt64(const std::string& propName, uint64_t value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteFloat(const std::string& propName, float value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteDouble(const std::string& propName, double value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteBool(const std::string& propName, bool value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect2f(const std::string& propName, const glm::vec2& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect3f(const std::string& propName, const glm::vec3& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect4f(const std::string& propName, const glm::vec4& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect2d(const std::string& propName, const glm::dvec2& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect3d(const std::string& propName, const glm::dvec3& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect4d(const std::string& propName, const glm::dvec4& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect2b(const std::string& propName, const glm::bvec2& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect3b(const std::string& propName, const glm::bvec3& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect4b(const std::string& propName, const glm::bvec4& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect2i(const std::string& propName, const glm::ivec2& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect3i(const std::string& propName, const glm::ivec3& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteVect4i(const std::string& propName, const glm::ivec4& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteMat2f(const std::string& propName, const glm::mat2& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteMat3f(const std::string& propName, const glm::mat3& value);

        /// <summary>
        /// Writes a property value to the XML
        /// </summary>
        /// <param name="name">Name of the property</param>
        /// <param name="value">Value of the property</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult WriteMat4f(const std::string& propName, const glm::mat4& value);

#pragma endregion
};

#endif
