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

#ifndef _AE_XML_PARSER_H
#define _AE_XML_PARSER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "libxml\parser.h"

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
class AEXMLParser sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Defines if the XML Parser 
        /// is initialized and ready to use
        /// </summary>
        bool m_IsReady = false;

        /// <summary>
        /// Defines if the current
        /// node is the parent node
        /// </summary>
        bool m_ParentNode = false;

        /// <summary>
        /// XML Parsed File
        /// </summary>
        xmlDocPtr m_LibXMLDoc = nullptr;

        /// <summary>
        /// Current XML Node
        /// </summary>
        xmlNodePtr m_LibXMLNode = nullptr;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Clean up the current memory used
        /// by the XML Parsed File
        /// </summary>
        void CleanUp();

        /// <summary>
        /// Finds a Node in the XML
        /// </summary>
        /// <param name="nodeNameMB">XML Node Name</param>
        /// <param name="xmlLibNode">Current XML Node from where to search</param>
        /// <param name="node">Current Parser of the Node</param>
        AEResult FindNode(const xmlChar* nodeNameMB, xmlNodePtr xmlLibNode, AEXMLParser& node);

        /// <summary>
        /// Gets the Property value of the Node
        /// </summary>
        /// <param name="propName">Property Name</param>
        xmlChar* GetNodeProperty(const std::wstring& propName);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// AEXMLParser Constructor
        /// </summary>
        AEXMLParser();

        /// <summary>
        /// Default AEXMLParser Destructor
        /// </summary>
        virtual ~AEXMLParser();

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Defines if the XML Parser is ready
        /// </summary>
        inline bool IsReady() const
        {
            return m_IsReady;
        }

        /// <summary>
        /// Loads a XML File
        /// </summary>
        /// <param name="file">Name of the XML File</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult LoadFile(const std::wstring& file);

        /// <summary>
        /// Gets a node from the XML
        /// </summary>
        /// <param name="nodeName">Name of Node</param>
        /// <param name="node">Node where to set the result</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult GetNode(const std::wstring& nodeName, AEXMLParser& node);

        /// <summary>
        /// Loads a XML File
        /// </summary>
        /// <param name="index">Index of the node</param>
        /// <param name="node">Node where to set the result</param>
        /// <returns>AEResult::OK if successful</returns>
        AEResult GetChildNode(uint32_t index, AEXMLParser& node);

        /// <summary>
        /// Gets the number of child nodes
        /// </summary>
        uint32_t GetNumChildren();

        /// <summary>
        /// Gets the Name of current node
        /// </summary>
        std::wstring GetName();

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        int8_t GetInt8(const std::wstring& propName, int8_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        uint8_t GetUInt8(const std::wstring& propName, uint8_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        int16_t GetInt16(const std::wstring& propName, int16_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        uint16_t GetUInt16(const std::wstring& propName, uint16_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        int32_t GetInt(const std::wstring& propName, int32_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        uint32_t GetUInt(const std::wstring& propName, uint32_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        int64_t GetInt64(const std::wstring& propName, int64_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        uint64_t GetUInt64(const std::wstring& propName, uint64_t defaultValue = 0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        float GetFloat(const std::wstring& propName, float defaultValue = 0.0f, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        double GetDouble(const std::wstring& propName, double defaultValue = 0.0, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        bool GetBool(const std::wstring& propName, bool defaultValue = false, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        std::wstring GetString(const std::wstring& propName, const std::wstring& defaultValue = L"", bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::vec2 GetVect2f(const std::wstring& propName, const glm::vec2& defaultValue = AEMathHelpers::Vec2fZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::vec3 GetVect3f(const std::wstring& propName, const glm::vec3& defaultValue = AEMathHelpers::Vec3fZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::vec4 GetVect4f(const std::wstring& propName, const glm::vec4& defaultValue = AEMathHelpers::Vec4fZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::dvec2 GetVect2d(const std::wstring& propName, const glm::dvec2& defaultValue = AEMathHelpers::Vec2dZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::dvec3 GetVect3d(const std::wstring& propName, const glm::dvec3& defaultValue = AEMathHelpers::Vec3dZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::dvec4 GetVect4d(const std::wstring& propName, const glm::dvec4& defaultValue = AEMathHelpers::Vec4dZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::bvec2 GetVect2b(const std::wstring& propName, const glm::bvec2& defaultValue = AEMathHelpers::Vec2bZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::bvec3 GetVect3b(const std::wstring& propName, const glm::bvec3& defaultValue = AEMathHelpers::Vec3bZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::bvec4 GetVect4b(const std::wstring& propName, const glm::bvec4& defaultValue = AEMathHelpers::Vec4bZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::ivec2 GetVect2i(const std::wstring& propName, const glm::ivec2& defaultValue = AEMathHelpers::Vec2iZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::ivec3 GetVect3i(const std::wstring& propName, const glm::ivec3& defaultValue = AEMathHelpers::Vec3iZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::ivec4 GetVect4i(const std::wstring& propName, const glm::ivec4& defaultValue = AEMathHelpers::Vec4iZero, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::mat2 GetMat2f(const std::wstring& propName, const glm::mat2& defaultValue = AEMathHelpers::Mat2Identity, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::mat3 GetMat3f(const std::wstring& propName, const glm::mat3& defaultValue = AEMathHelpers::Mat3Identity, bool warning = true);

        /// <summary>
        /// Get the value of the property
        /// </summary>
        /// <param name="propName">Name of the property</param>
        /// <param name="defaultValue">Default value for the property if not found</param>
        /// <param name="warning">Writes a warning to the log if the property was not found</param>
        /// <returns>Value of the property</returns>
        glm::mat4 GetMat4f(const std::wstring& propName, const glm::mat4& defaultValue = AEMathHelpers::Mat4Identity, bool warning = true);

        /// <summary>
        /// Gets a node from the XML using the name 
        /// of the index
        /// </summary>
        AEXMLParser operator[] (const std::wstring& nodeName);

        /// <summary>
        /// Gets a node from the XML an index
        /// </summary>
        AEXMLParser operator() (uint32_t index);

#pragma endregion

};

#endif
