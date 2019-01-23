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
#include "precomp_base.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AEXMLDefs.h"
#include "AEXMLParser.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AEXMLParser::AEXMLParser()
{
}

AEXMLParser::~AEXMLParser()
{
    DeleteMem(m_XMLDoc);
}

AEResult AEXMLParser::LoadFile(const std::string& file)
{
    if (m_Type != XMLType::Empty)
    {
        return AEResult::Fail;
    }

    AEAssert(!file.empty());
    if(file.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_XMLDoc = new tinyxml2::XMLDocument();

    tinyxml2::XMLError xmlRes = m_XMLDoc->LoadFile(file.c_str());
    if (xmlRes != tinyxml2::XMLError::XML_SUCCESS)
    {
        AELogHelpers::LogError(LogSystem::XML, "XML_PARSE_FILE_ERR_MSG");
        AELogHelpers::LogDebug(LogSystem::XML, "XML_API_ERR_DBG_MSG", m_XMLDoc->ErrorStr());

        return AEResult::Fail;
    }

    m_Type = XMLType::Document;

    return AEResult::Ok;
}

AEXMLParser AEXMLParser::GetChildElement(const std::string& elementName) const
{
    const tinyxml2::XMLElement* xmlElement = nullptr;

    switch (m_Type)
    {
        case XMLType::Element:
            xmlElement = m_XMLElement->FirstChildElement(elementName.c_str());
            break;

        case XMLType::Document:
            xmlElement = m_XMLDoc->FirstChildElement(elementName.c_str());
            break;

        default:
            break;
    }

    if (xmlElement == nullptr)
    {
        return AEXMLParser();
    }

    AEXMLParser element;

    element.m_Type          = XMLType::Element;
    element.m_XMLElement    = xmlElement;

    return element;
}

AEXMLParser AEXMLParser::GetFirstChildElement() const
{
    const tinyxml2::XMLElement* xmlElement = nullptr;

    switch (m_Type)
    {
        case XMLType::Element:
            xmlElement = m_XMLElement->FirstChildElement();
            break;

        case XMLType::Document:
            xmlElement = m_XMLDoc->FirstChildElement();
            break;

        default:
            break;
    }

    if (xmlElement == nullptr)
    {
        return AEXMLParser();
    }

    AEXMLParser element;

    element.m_Type = XMLType::Element;
    element.m_XMLElement = xmlElement;

    return element;
}

AEXMLParser AEXMLParser::GetNextSiblingElement() const
{
    const tinyxml2::XMLElement* xmlElement = nullptr;

    switch (m_Type)
    {
        case XMLType::Element:
            xmlElement = m_XMLElement->NextSiblingElement();
            break;

        case XMLType::Document:
            xmlElement = m_XMLDoc->NextSiblingElement();
            break;

        default:
            break;
    }

    if (xmlElement == nullptr)
    {
        return AEXMLParser();
    }

    AEXMLParser element;

    element.m_Type = XMLType::Element;
    element.m_XMLElement = xmlElement;

    return element;
}

AEXMLParser AEXMLParser::operator[](const std::string& nodeName) const
{
    return GetChildElement(nodeName);
}

const char* AEXMLParser::GetNodeAttribute(const std::string& attributeName) const
{
    if (m_Type != XMLType::Element)
    {
        return "";
    }

    return m_XMLElement->Attribute(attributeName.c_str());
}

int8_t AEXMLParser::GetInt8(const std::string& attributeName, int8_t defaultValue, bool warning) const
{
    int8_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        int32_t tmp = boost::lexical_cast<int32_t>(value);
        ret = static_cast<int8_t>(tmp);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, (int32_t)defaultValue, attributeName);
    }

    return ret;
}

uint8_t AEXMLParser::GetUInt8(const std::string& attributeName, uint8_t defaultValue, bool warning) const
{
    uint8_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        int32_t tmp = boost::lexical_cast<uint32_t>(value);
        ret = static_cast<uint8_t>(tmp);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, (int32_t)defaultValue, attributeName);
    }

    return ret;
}

int16_t AEXMLParser::GetInt16(const std::string& attributeName, int16_t defaultValue, bool warning) const
{
    int16_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<int16_t>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

uint16_t AEXMLParser::GetUInt16(const std::string& attributeName, uint16_t defaultValue, bool warning) const
{
    uint16_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<uint16_t>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

int32_t AEXMLParser::GetInt(const std::string& attributeName, int32_t defaultValue, bool warning) const
{
    int32_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<int32_t>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

uint32_t AEXMLParser::GetUInt(const std::string& attributeName, uint32_t defaultValue, bool warning) const
{
    uint32_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<uint32_t>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

int64_t AEXMLParser::GetInt64(const std::string& attributeName, int64_t defaultValue, bool warning) const
{
    int64_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<int64_t>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

uint64_t AEXMLParser::GetUInt64(const std::string& attributeName, uint64_t defaultValue, bool warning) const
{
    uint64_t ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<uint64_t>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

float AEXMLParser::GetFloat(const std::string& attributeName, float defaultValue, bool warning) const
{
    float ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<float>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

double AEXMLParser::GetDouble(const std::string& attributeName, double defaultValue, bool warning) const
{
    double ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = boost::lexical_cast<double>(value);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

bool AEXMLParser::GetBool(const std::string& attributeName, bool defaultValue, bool warning) const
{
    bool ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = (strcmp("true", value) == 0);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

std::string AEXMLParser::GetString(const std::string& attributeName, const std::string& defaultValue, bool warning) const
{
    std::string ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        ret = value;
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MSG", __FUNCTION__, defaultValue, attributeName);
    }

    return ret;
}

glm::vec2 AEXMLParser::GetVect2f(const std::string& attributeName, const glm::vec2& defaultValue, bool warning) const
{
    glm::vec2 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%f %f", &ret.x, &ret.y);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_2_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, attributeName);
    }

    return ret;
}

glm::vec3 AEXMLParser::GetVect3f(const std::string& attributeName, const glm::vec3& defaultValue, bool warning) const
{
    glm::vec3 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%f %f %f", &ret.x, &ret.y, &ret.z);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_3_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, attributeName);
    }

    return ret;
}

glm::vec4 AEXMLParser::GetVect4f(const std::string& attributeName, const glm::vec4& defaultValue, bool warning) const
{
    glm::vec4 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%f %f %f %f", &ret.x, &ret.y, &ret.z, &ret.w);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_4_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, attributeName);
    }

    return ret;
}

glm::dvec2 AEXMLParser::GetVect2d(const std::string& attributeName, const glm::dvec2& defaultValue, bool warning) const
{
    glm::dvec2 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%lf %lf", &ret.x, &ret.y);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_2_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, attributeName);
    }

    return ret;
}

glm::dvec3 AEXMLParser::GetVect3d(const std::string& attributeName, const glm::dvec3& defaultValue, bool warning) const
{
    glm::dvec3 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%lf %lf %lf", &ret.x, &ret.y, &ret.z);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_3_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, attributeName);
    }

    return ret;
}

glm::dvec4 AEXMLParser::GetVect4d(const std::string& attributeName, const glm::dvec4& defaultValue, bool warning) const
{
    glm::dvec4 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%lf %lf %lf %lf", &ret.x, &ret.y, &ret.z, &ret.w);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_4_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, attributeName);
    }

    return ret;
}

glm::bvec2 AEXMLParser::GetVect2b(const std::string& attributeName, const glm::bvec2& defaultValue, bool warning) const
{
    glm::bvec2 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        std::vector<std::string> strs;
        AE_Base::SplitString(value, strs, " ");

        for (uint32_t i = 0; i < 2 && i < strs.size(); i++)
        {
            ret[i] = (strs[i].compare("true") == 0);
        }
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_2_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, attributeName);
    }

    return ret;
}

glm::bvec3 AEXMLParser::GetVect3b(const std::string& attributeName, const glm::bvec3& defaultValue, bool warning) const
{
    glm::bvec3 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        std::vector<std::string> strs;
        AE_Base::SplitString(value, strs, " ");

        for (uint32_t i = 0; i < 3 && i < strs.size(); i++)
        {
            ret[i] = (strs[i].compare("true") == 0);
        }
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_3_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, attributeName);
    }

    return ret;
}

glm::bvec4 AEXMLParser::GetVect4b(const std::string& attributeName, const glm::bvec4& defaultValue, bool warning) const
{
    glm::bvec4 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        std::vector<std::string> strs;
        AE_Base::SplitString(value, strs, " ");

        for (uint32_t i = 0; i < 4 && i < strs.size(); i++)
        {
            ret[i] = (strs[i].compare("true") == 0);
        }
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_4_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, attributeName);
    }

    return ret;
}

glm::ivec2 AEXMLParser::GetVect2i(const std::string& attributeName, const glm::ivec2& defaultValue, bool warning) const
{
    glm::ivec2 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%i %i", &ret.x, &ret.y);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_2_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, attributeName);
    }

    return ret;
}

glm::ivec3 AEXMLParser::GetVect3i(const std::string& attributeName, const glm::ivec3& defaultValue, bool warning) const
{
    glm::ivec3 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%i %i %i", &ret.x, &ret.y, &ret.z);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_3_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, attributeName);
    }

    return ret;
}

glm::ivec4 AEXMLParser::GetVect4i(const std::string& attributeName, const glm::ivec4& defaultValue, bool warning) const
{
    glm::ivec4 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value, "%i %i %i %i", &ret.x, &ret.y, &ret.z, &ret.w);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_VECTOR_4_MSG", __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, attributeName);
    }

    return ret;
}

glm::mat2 AEXMLParser::GetMat2f(const std::string& attributeName, const glm::mat2& defaultValue, bool warning) const
{
    glm::mat2 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value,
            "%f %f "
            "%f %f",
            &ret[0].x, &ret[0].y,
            &ret[1].x, &ret[1].y);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MATRIX_2_MSG", __FUNCTION__,
            defaultValue[0].x, defaultValue[0].y,
            defaultValue[1].x, defaultValue[1].y, attributeName);
    }

    return ret;
}

glm::mat3 AEXMLParser::GetMat3f(const std::string& attributeName, const glm::mat3& defaultValue, bool warning) const
{
    glm::mat3 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value,
            "%f %f %f "
            "%f %f %f "
            "%f %f %f",
            &ret[0].x, &ret[0].y, &ret[0].z,
            &ret[1].x, &ret[1].y, &ret[1].z,
            &ret[2].x, &ret[2].y, &ret[2].z);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MATRIX_3_MSG", __FUNCTION__,
            defaultValue[0].x, defaultValue[0].y, defaultValue[0].z,
            defaultValue[1].x, defaultValue[1].y, defaultValue[1].z,
            defaultValue[2].x, defaultValue[2].y, defaultValue[2].z, attributeName);
    }

    return ret;
}

glm::mat4 AEXMLParser::GetMat4f(const std::string& attributeName, const glm::mat4& defaultValue, bool warning) const
{
    glm::mat4 ret = defaultValue;

    const char* value = GetNodeAttribute(attributeName);

    if (value)
    {
        sscanf_s(value,
            "%f %f %f %f "
            "%f %f %f %f "
            "%f %f %f %f "
            "%f %f %f %f",
            &ret[0].x, &ret[0].y, &ret[0].z, &ret[0].w,
            &ret[1].x, &ret[1].y, &ret[1].z, &ret[1].w,
            &ret[2].x, &ret[2].y, &ret[2].z, &ret[2].w,
            &ret[3].x, &ret[3].y, &ret[3].z, &ret[3].w);
    }
    else if (warning)
    {
        AELogHelpers::LogWarning(LogSystem::XML, "XML_DEFAULT_VALUE_USE_MATRIX_4_MSG", __FUNCTION__,
            defaultValue[0].x, defaultValue[0].y, defaultValue[0].z, defaultValue[0].w,
            defaultValue[1].x, defaultValue[1].y, defaultValue[1].z, defaultValue[1].w,
            defaultValue[2].x, defaultValue[2].y, defaultValue[2].z, defaultValue[2].w,
            defaultValue[3].x, defaultValue[3].y, defaultValue[3].z, defaultValue[3].w, attributeName);
    }

    return ret;
}
