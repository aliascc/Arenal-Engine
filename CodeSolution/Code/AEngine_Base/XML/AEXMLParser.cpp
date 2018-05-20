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
    if(m_ParentNode)
    {
        CleanUp();
    }
}

void AEXMLParser::CleanUp()
{
    AEXMLDocFree(m_LibXMLDoc);

    m_LibXMLNode = nullptr;
}

AEResult AEXMLParser::LoadFile(const std::string& file)
{
    AEAssert(!file.empty());
    if(file.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_IsReady = false;
    m_ParentNode = false;

    CleanUp();

    m_LibXMLDoc = xmlParseFile(file.c_str());
        
    if (m_LibXMLDoc)
    {
        m_LibXMLNode = xmlDocGetRootElement(m_LibXMLDoc);

        if (m_LibXMLNode)
        {
            m_IsReady = true;
            m_ParentNode = true;
            return AEResult::Ok;
        }
    }

    return AEResult::Fail;
}

AEResult AEXMLParser::FindNode(const xmlChar* nodeNameMB, xmlNodePtr mlNode, AEXMLParser& node)
{
    while (mlNode != nullptr)
    {
        if (xmlStrcmp(mlNode->name, nodeNameMB) != 0)
        {
            if (FindNode(nodeNameMB, mlNode->xmlChildrenNode, node) == AEResult::Ok)
            {
                return AEResult::Ok;
            }
        }
        else
        {
            node.m_LibXMLNode   = mlNode;
            node.m_LibXMLDoc    = m_LibXMLDoc;
            node.m_ParentNode   = false;
            node.m_IsReady      = true;

            return AEResult::Ok;
        }

        mlNode = mlNode->next;
    }

    return AEResult::Fail;
}

AEResult AEXMLParser::GetNode(const std::string& nodeName, AEXMLParser& node)
{
    if(!m_IsReady || m_LibXMLNode == nullptr)
    {
        return AEResult::Fail;
    }

    const xmlChar* xmlNodeName = reinterpret_cast<const xmlChar*>(nodeName.c_str());

    return FindNode(xmlNodeName, m_LibXMLNode, node);
}

AEXMLParser AEXMLParser::operator[](const std::string& nodeName)
{
    AEXMLParser node;

    GetNode(nodeName, node);

    return node;
}

AEResult AEXMLParser::GetChildNode(uint32_t index, AEXMLParser& node)
{
    if(!m_IsReady || m_LibXMLNode == nullptr)
    {
        return AEResult::Fail;
    }

    uint32_t count = 0;

    xmlNodePtr children = m_LibXMLNode->children;
    
    while (children != nullptr)
    {
        if (children->type != XML_TEXT_NODE)
        {
            if (index == count)
            {
                node.m_LibXMLNode = children;
                node.m_LibXMLDoc = m_LibXMLDoc;
                node.m_ParentNode = false;
                node.m_IsReady = true;

                return AEResult::Ok;
            }

            ++count;
        }

        children = children->next;
    }

    return AEResult::Fail;
}

AEXMLParser AEXMLParser::operator()(uint32_t index)
{
    AEXMLParser node;

    GetChildNode(index, node);

    return node;
}

uint32_t AEXMLParser::GetNumChildren()
{
    if(!m_IsReady || m_LibXMLNode == nullptr)
    {
        return 0;
    }

    uint32_t count = 0;

    xmlNodePtr children = m_LibXMLNode->children;

    while (children != nullptr)
    {
        if (children->type != XML_TEXT_NODE)
        {
            ++count;
        }

        children = children->next;
    }

    return count;
}

std::string AEXMLParser::GetName()
{
    if(!m_IsReady || m_LibXMLNode == nullptr)
    {
        return "";
    }

    const char* name = reinterpret_cast<const char*>(m_LibXMLNode->name);

    return name;
}

xmlChar* AEXMLParser::GetNodeProperty(const std::string& propName)
{
    xmlChar* prop = nullptr;

    if(!m_IsReady || m_LibXMLNode == nullptr)
    {
        return prop;
    }

    const xmlChar* xmlPropName = reinterpret_cast<const xmlChar*>(propName.c_str());

    prop = xmlGetProp(m_LibXMLNode, xmlPropName);

    return prop;
}

int8_t AEXMLParser::GetInt8(const std::string& propName, int8_t defaultValue, bool warning)
{
    int8_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        int32_t tmp = boost::lexical_cast<int32_t>(value);
        ret = static_cast<int8_t>(tmp);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, (int32_t)defaultValue, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

uint8_t AEXMLParser::GetUInt8(const std::string& propName, uint8_t defaultValue, bool warning)
{
    uint8_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        int32_t tmp = boost::lexical_cast<uint32_t>(value);
        ret = static_cast<uint8_t>(tmp);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, (int32_t)defaultValue, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

int16_t AEXMLParser::GetInt16(const std::string& propName, int16_t defaultValue, bool warning)
{
    int16_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<int16_t>(value);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

uint16_t AEXMLParser::GetUInt16(const std::string& propName, uint16_t defaultValue, bool warning)
{
    uint16_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<uint16_t>(value);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

int32_t AEXMLParser::GetInt(const std::string& propName, int32_t defaultValue, bool warning)
{
    int32_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<int32_t>(value);
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

uint32_t AEXMLParser::GetUInt(const std::string& propName, uint32_t defaultValue, bool warning)
{
    uint32_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<uint32_t>(value);
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

int64_t AEXMLParser::GetInt64(const std::string& propName, int64_t defaultValue, bool warning)
{
    int64_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<int64_t>(value);
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

uint64_t AEXMLParser::GetUInt64(const std::string& propName, uint64_t defaultValue, bool warning)
{
    uint64_t ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<uint64_t>(value);
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

float AEXMLParser::GetFloat(const std::string& propName, float defaultValue, bool warning)
{
    float ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<float>(value);
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

double AEXMLParser::GetDouble(const std::string& propName, double defaultValue, bool warning)
{
    double ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        ret = boost::lexical_cast<double>(value);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

bool AEXMLParser::GetBool(const std::string& propName, bool defaultValue, bool warning)
{
    bool ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        const char* pszValue = (const char*)value;
        
        ret = (strcmp("true", pszValue) == 0);
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

std::string AEXMLParser::GetString(const std::string& propName, const std::string& defaultValue, bool warning)
{
    std::string ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        const char* sValue = reinterpret_cast<const char*>(value);

        ret = sValue;
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MSG"), __FUNCTION__, defaultValue, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::vec2 AEXMLParser::GetVect2f(const std::string& propName, const glm::vec2& defaultValue, bool warning)
{
    glm::vec2 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%f %f", &ret.x, &ret.y);   
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::vec3 AEXMLParser::GetVect3f(const std::string& propName, const glm::vec3& defaultValue, bool warning)
{
    glm::vec3 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%f %f %f", &ret.x, &ret.y, &ret.z);   
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::vec4 AEXMLParser::GetVect4f(const std::string& propName, const glm::vec4& defaultValue, bool warning)
{
    glm::vec4 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%f %f %f %f", &ret.x, &ret.y, &ret.z, &ret.w);   
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::dvec2 AEXMLParser::GetVect2d(const std::string& propName, const glm::dvec2& defaultValue, bool warning)
{
    glm::dvec2 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%lf %lf", &ret.x, &ret.y);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::dvec3 AEXMLParser::GetVect3d(const std::string& propName, const glm::dvec3& defaultValue, bool warning)
{
    glm::dvec3 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%lf %lf %lf", &ret.x, &ret.y, &ret.z);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::dvec4 AEXMLParser::GetVect4d(const std::string& propName, const glm::dvec4& defaultValue, bool warning)
{
    glm::dvec4 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%lf %lf %lf %lf", &ret.x, &ret.y, &ret.z, &ret.w);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::bvec2 AEXMLParser::GetVect2b(const std::string& propName, const glm::bvec2& defaultValue, bool warning)
{
    glm::bvec2 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        std::string values((const char*)value);
        std::vector<std::string> strs;
        AE_Base::SplitString(values, strs, " ");

        for (uint32_t i = 0; i < 2 && i < strs.size(); i++)
        {
            ret[i] = (strs[i].compare("true") == 0);
        }
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::bvec3 AEXMLParser::GetVect3b(const std::string& propName, const glm::bvec3& defaultValue, bool warning)
{
    glm::bvec3 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        std::string values((const char*)value);
        std::vector<std::string> strs;
        AE_Base::SplitString(values, strs, " ");

        for (uint32_t i = 0; i < 3 && i < strs.size(); i++)
        {
            ret[i] = (strs[i].compare("true") == 0);
        }
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::bvec4 AEXMLParser::GetVect4b(const std::string& propName, const glm::bvec4& defaultValue, bool warning)
{
    glm::bvec4 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        std::string values((const char*)value);
        std::vector<std::string> strs;
        AE_Base::SplitString(values, strs, " ");

        for (uint32_t i = 0; i < 4 && i < strs.size(); i++)
        {
            ret[i] = (strs[i].compare("true") == 0);
        }
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::ivec2 AEXMLParser::GetVect2i(const std::string& propName, const glm::ivec2& defaultValue, bool warning)
{
    glm::ivec2 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%i %i", &ret.x, &ret.y);   
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::ivec3 AEXMLParser::GetVect3i(const std::string& propName, const glm::ivec3& defaultValue, bool warning)
{
    glm::ivec3 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%i %i %i", &ret.x, &ret.y, &ret.z);   
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::ivec4 AEXMLParser::GetVect4i(const std::string& propName, const glm::ivec4& defaultValue, bool warning)
{
    glm::ivec4 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value, "%i %i %i %i", &ret.x, &ret.y, &ret.z, &ret.w);   
    }
    else if(warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTION__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);
        
        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::mat2 AEXMLParser::GetMat2f(const std::string& propName, const glm::mat2& defaultValue, bool warning)
{
    glm::mat2 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value,
            "%f %f "
            "%f %f",
            &ret[0].x, &ret[0].y,
            &ret[1].x, &ret[1].y);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MATRIX_2_MSG"), __FUNCTION__,
                                                        defaultValue[0].x, defaultValue[0].y,
                                                        defaultValue[1].x, defaultValue[1].y, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::mat3 AEXMLParser::GetMat3f(const std::string& propName, const glm::mat3& defaultValue, bool warning)
{
    glm::mat3 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value,
            "%f %f %f "
            "%f %f %f "
            "%f %f %f",
            &ret[0].x, &ret[0].y, &ret[0].z,
            &ret[1].x, &ret[1].y, &ret[1].z,
            &ret[2].x, &ret[2].y, &ret[2].z);
    }
    else if (warning)
    {
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MATRIX_3_MSG"), __FUNCTION__,
                                                        defaultValue[0].x, defaultValue[0].y, defaultValue[0].z,
                                                        defaultValue[1].x, defaultValue[1].y, defaultValue[1].z, 
                                                        defaultValue[2].x, defaultValue[2].y, defaultValue[2].z, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}

glm::mat4 AEXMLParser::GetMat4f(const std::string& propName, const glm::mat4& defaultValue, bool warning)
{
    glm::mat4 ret = defaultValue;

    xmlChar* value = GetNodeProperty(propName);

    if (value)
    {
        sscanf_s((const char*)value,
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
        std::string msgerr = fmt::format(AELOCMAN->GetLiteral("XML_DEFAULT_VALUE_USE_MATRIX_4_MSG"), __FUNCTION__,
                                                        defaultValue[0].x, defaultValue[0].y, defaultValue[0].z, defaultValue[0].w,
                                                        defaultValue[1].x, defaultValue[1].y, defaultValue[1].z, defaultValue[1].w,
                                                        defaultValue[2].x, defaultValue[2].y, defaultValue[2].z, defaultValue[2].w,
                                                        defaultValue[3].x, defaultValue[3].y, defaultValue[3].z, defaultValue[3].w, propName);

        AELOGGER->AddNewLog(LogLevel::Warning, msgerr);
    }

    xmlFree(value);

    return ret;
}
