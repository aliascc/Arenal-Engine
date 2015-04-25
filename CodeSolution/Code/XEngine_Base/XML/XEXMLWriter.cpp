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

/*************************
*   3rd Party Includes   *
**************************/
#include "boost\lexical_cast.hpp"
#include "fastformat\fastformat.hpp"
#include "fastformat\sinks\ostream.hpp"
#include "fastformat\shims\conversion\filter_type\bool.hpp"
#include "fastformat\shims\conversion\filter_type\reals.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "XEXMLDefs.h"
#include "XEXMLWriter.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XEXMLWriter::XEXMLWriter()
{
}

XEXMLWriter::~XEXMLWriter()
{
	CleanUp();
}

void XEXMLWriter::CleanUp()
{
	m_InMemory = false;
	m_IsReady = false;

	XEXMLTextWriterFree(m_XMLWriter);
	XEXMLBufferFree(m_XMLBuffer);
}

XEResult XEXMLWriter::CreateXML(const std::wstring& file, bool inMemory)
{
	if (m_IsReady)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (file.empty())
	{
		return XEResult::EmptyFilename;
	}

	if (inMemory)
	{
		m_XMLBuffer = xmlBufferCreate();
		if (xmlBufferCreate == nullptr) 
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}

		m_XMLWriter = xmlNewTextWriterMemory(m_XMLBuffer, 0);
		if (m_XMLWriter == NULL)
		{
			CleanUp();

			XETODO("Better return code");
			return XEResult::Fail;
		}

		m_InMemory = true;
	}
	else
	{
		std::string fileStr = XE_Base::WideStr2String(file);

		m_XMLWriter = xmlNewTextWriterFilename(fileStr.c_str(), 0);
		if (m_XMLWriter == NULL)
		{
			XETODO("Better return code");
			return XEResult::Fail;
		}
	}

	int ret = xmlTextWriterStartDocument(m_XMLWriter, NULL, XE_XML_ENCODING, NULL);
	if (ret < 0)
	{
		CleanUp();

		XETODO("Better return code");
		return XEResult::Fail;
	}

	m_Filename = file;

	m_IsReady = true;

	return XEResult::Ok;
}

XEResult XEXMLWriter::StartNode(const std::wstring& name)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string nameStr = XE_Base::WideStr2String(name);

	int ret = xmlTextWriterStartElement(m_XMLWriter, BAD_CAST nameStr.c_str());
	if (ret < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::EndNode()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	int ret = xmlTextWriterEndElement(m_XMLWriter);
	if (ret < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::FinalizeXML()
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	XEResult ret = XEResult::Ok;

	int rsc = xmlTextWriterEndDocument(m_XMLWriter);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	if (m_InMemory)
	{
		std::ofstream xmlFile(m_Filename);
		if (xmlFile.is_open())
		{
			xmlFile << (const char *)m_XMLBuffer->content;
			xmlFile.close();
		}
		else
		{
			ret = XEResult::OpenFileFail;
		}
	}

	CleanUp();

	return ret;
}

XEResult XEXMLWriter::WriteString(const std::wstring& propName, const std::wstring& value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);
	std::string valueStr = XE_Base::WideStr2String(value);

	int rsc = xmlTextWriterWriteAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), BAD_CAST valueStr.c_str());
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteInt(const std::wstring& propName, int32_t value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d", value);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteUInt(const std::wstring& propName, uint32_t value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%u", value);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteInt64(const std::wstring& propName, int64_t value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%ll", value);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteUInt64(const std::wstring& propName, uint64_t value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%llu", value);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteFloat(const std::wstring& propName, float value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f", value);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteBool(const std::wstring& propName, bool value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);
	std::string valueStr = (value ? "true" : "false");

	int rsc = xmlTextWriterWriteAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), BAD_CAST valueStr.c_str());
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteVect3f(const std::wstring& propName, const glm::vec3& value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f %.4f %.4f", value.x, value.y, value.z);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteVect4f(const std::wstring& propName, const glm::vec4& value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%.4f %.4f %.4f %.4f", value.x, value.y, value.z, value.w);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteVect2i(const std::wstring& propName, const glm::ivec2& value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d %d", value.x, value.y);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteVect3i(const std::wstring& propName, const glm::ivec3& value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d %d %d", value.x, value.y, value.z);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}

XEResult XEXMLWriter::WriteVect4i(const std::wstring& propName, const glm::ivec4& value)
{
	if (!m_IsReady)
	{
		return XEResult::NotReady;
	}

	std::string propNameStr = XE_Base::WideStr2String(propName);

	int rsc = xmlTextWriterWriteFormatAttribute(m_XMLWriter, BAD_CAST propNameStr.c_str(), "%d %d %d %d", value.x, value.y, value.z, value.w);
	if (rsc < 0)
	{
		XETODO("Better return code");
		return XEResult::Fail;
	}

	return XEResult::Ok;
}
