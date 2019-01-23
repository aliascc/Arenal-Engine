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
#include "AEXMLWriter.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AEXMLWriter::AEXMLWriter()
{
}

AEXMLWriter::~AEXMLWriter()
{
}

AEResult AEXMLWriter::CreateXML(const std::string& file, bool inMemory)
{
    if (m_IsReady)
    {
        AETODO("Better return code");
        return AEResult::Fail;
    }

    if (file.empty())
    {
        return AEResult::EmptyFilename;
    }

    m_Filename = file;
    m_IsReady = true;

    return AEResult::Ok;
}

AEResult AEXMLWriter::StartNode(const std::string& name)
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }


    tinyxml2::XMLElement* newXMLElement = m_XMLDoc.NewElement(name.c_str());

    if (m_XMLElement != nullptr)
    {
        m_XMLElement->InsertEndChild(newXMLElement);
    }
    else
    {
        m_XMLDoc.InsertEndChild(newXMLElement);
    }

    m_XMLElement = newXMLElement;
    m_XMLElementStack.push(m_XMLElement);

    return AEResult::Ok;
}

AEResult AEXMLWriter::EndNode()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    m_XMLElementStack.pop();

    if (m_XMLElementStack.empty())
    {
        m_XMLElement = nullptr;
    }
    else
    {
        m_XMLElement = m_XMLElementStack.top();
    }

    return AEResult::Ok;
}

AEResult AEXMLWriter::FinalizeXML()
{
    if (!m_IsReady)
    {
        return AEResult::NotReady;
    }

    AEAssert(m_XMLElementStack.empty());

    tinyxml2::XMLError xmlRes = m_XMLDoc.SaveFile(m_Filename.c_str());
    if (xmlRes != tinyxml2::XMLError::XML_SUCCESS)
    {
        AELogHelpers::LogError(LogSystem::XML, "XML_WRITER_SAVE_FILE_ERR_MSG", m_Filename);
        AELogHelpers::LogDebug(LogSystem::XML, "XML_API_ERR_DBG_MSG", m_XMLDoc.ErrorStr());

        return AEResult::WriteToFileFailed;
    }

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteString(const std::string& attributeName, const std::string& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt8(const std::string& attributeName, int8_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt8(const std::string& attributeName, uint8_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt16(const std::string& attributeName, int16_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt16(const std::string& attributeName, uint16_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt(const std::string& attributeName, int32_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt(const std::string& attributeName, uint32_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteInt64(const std::string& attributeName, int64_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteUInt64(const std::string& attributeName, uint64_t value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), std::to_string(value).c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteFloat(const std::string& attributeName, float value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteDouble(const std::string& attributeName, double value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    m_XMLElement->SetAttribute(attributeName.c_str(), value);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteBool(const std::string& attributeName, bool value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    const char* valueStr = (value ? "true" : "false");

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr);

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2f(const std::string& attributeName, const glm::vec2& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.4g} {:.4g}", value.x, value.y);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3f(const std::string& attributeName, const glm::vec3& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.4g} {:.4g} {:.4g}", value.x, value.y, value.z);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4f(const std::string& attributeName, const glm::vec4& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.4g} {:.4g} {:.4g} {:.4g}", value.x, value.y, value.z, value.w);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2d(const std::string& attributeName, const glm::dvec2& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.6g} {:.6g}", value.x, value.y);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3d(const std::string& attributeName, const glm::dvec3& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.6g} {:.6g} {:.6g}", value.x, value.y, value.z);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4d(const std::string& attributeName, const glm::dvec4& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.6g} {:.6g} {:.6g} {:.6g}", value.x, value.y, value.z, value.w);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2b(const std::string& attributeName, const glm::bvec2& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{} {}",
                                       (value.x ? "true" : "false"),
                                       (value.y ? "true" : "false"));

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3b(const std::string& attributeName, const glm::bvec3& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{} {} {}",
                                       (value.x ? "true" : "false"),
                                       (value.y ? "true" : "false"),
                                       (value.z ? "true" : "false"));

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4b(const std::string& attributeName, const glm::bvec4& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{} {} {} {}",
                                       (value.x ? "true" : "false"),
                                       (value.y ? "true" : "false"),
                                       (value.z ? "true" : "false"),
                                       (value.w ? "true" : "false"));

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect2i(const std::string& attributeName, const glm::ivec2& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{} {}", value.x, value.y);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect3i(const std::string& attributeName, const glm::ivec3& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{} {} {}", value.x, value.y, value.z);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteVect4i(const std::string& attributeName, const glm::ivec4& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{} {} {} {}", value.x, value.y, value.z, value.w);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteMat2f(const std::string& attributeName, const glm::mat2& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.4g} {:.4g} "
                                       "{:.4g} {:.4g}"
                                       , value[0].x, value[0].y
                                       , value[1].x, value[1].y);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteMat3f(const std::string& attributeName, const glm::mat3& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.4g} {:.4g} {:.4g} "
                                       "{:.4g} {:.4g} {:.4g} "
                                       "{:.4g} {:.4g} {:.4g}"
                                       , value[0].x, value[0].y, value[0].z
                                       , value[1].x, value[1].y, value[1].z
                                       , value[2].x, value[2].y, value[2].z);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}

AEResult AEXMLWriter::WriteMat4f(const std::string& attributeName, const glm::mat4& value)
{
    if (!m_IsReady || m_XMLElement == nullptr)
    {
        return AEResult::NotReady;
    }

    std::string valueStr = fmt::format("{:.4g} {:.4g} {:.4g} {:.4g} "
                                       "{:.4g} {:.4g} {:.4g} {:.4g} "
                                       "{:.4g} {:.4g} {:.4g} {:.4g} "
                                       "{:.4g} {:.4g} {:.4g} {:.4g}"
                                       , value[0].x, value[0].y, value[0].z, value[0].w
                                       , value[1].x, value[1].y, value[1].z, value[1].w
                                       , value[2].x, value[2].y, value[2].z, value[2].w
                                       , value[3].x, value[3].y, value[3].z, value[3].w);

    m_XMLElement->SetAttribute(attributeName.c_str(), valueStr.c_str());

    return AEResult::Ok;
}
