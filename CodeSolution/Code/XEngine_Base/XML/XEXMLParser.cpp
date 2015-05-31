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
#include <vector>
#include <fstream>
#include <iostream>

/*************************
*   3rd Party Includes   *
**************************/
#include "cppformat\format.h"
#include "boost\lexical_cast.hpp"
#include "boost\algorithm\string.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "XEXMLDefs.h"
#include "XEXMLParser.h"
#include "Logger\Logger.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
XEXMLParser::XEXMLParser()
{
}

XEXMLParser::~XEXMLParser()
{
	if(m_ParentNode)
	{
		CleanUp();
	}
}

void XEXMLParser::CleanUp()
{
	XEXMLDocFree(m_LibXMLDoc);

	m_LibXMLNode = nullptr;
}

XEResult XEXMLParser::LoadFile(const std::wstring& file)
{
	XEAssert(!file.empty());
	if(file.empty())
	{
		return XEResult::EmptyFilename;
	}

	m_IsReady = false;
	m_ParentNode = false;

	CleanUp();

	std::string sFileName = XE_Base::WideStr2String(file);

	m_LibXMLDoc = xmlParseFile(sFileName.c_str());
		
	if (m_LibXMLDoc)
	{
		m_LibXMLNode = xmlDocGetRootElement(m_LibXMLDoc);

		if (m_LibXMLNode)
		{
			m_IsReady = true;
			m_ParentNode = true;
			return XEResult::Ok;
		}
	}

	return XEResult::Fail;
}

XEResult XEXMLParser::FindNode(const xmlChar* nodeNameMB, xmlNodePtr mlNode, XEXMLParser& node)
{
	while (mlNode != nullptr)
	{
		if (xmlStrcmp(mlNode->name, nodeNameMB) != 0)
		{
			if (FindNode(nodeNameMB, mlNode->xmlChildrenNode, node) == XEResult::Ok)
			{
				return XEResult::Ok;
			}
		}
		else
		{
			node.m_LibXMLNode = mlNode;
			node.m_LibXMLDoc = m_LibXMLDoc;
			node.m_ParentNode = false;
			node.m_IsReady = true;

			return XEResult::Ok;
		}

		mlNode = mlNode->next;
	}

	return XEResult::Fail;
}

XEResult XEXMLParser::GetNode(const std::wstring& nodeName, XEXMLParser& node)
{
	if(!m_IsReady || m_LibXMLNode == nullptr)
	{
		return XEResult::Fail;
	}

	std::string sNodeName = XE_Base::WideStr2String(nodeName);
	const xmlChar* xmlNodeName = reinterpret_cast<const xmlChar*>(sNodeName.c_str());

	return FindNode(xmlNodeName, m_LibXMLNode, node);
}

XEXMLParser XEXMLParser::operator[](const std::wstring& nodeName)
{
	XEXMLParser node;

	GetNode(nodeName, node);

	return node;
}

XEResult XEXMLParser::GetChildNode(uint32_t index, XEXMLParser& node)
{
	if(!m_IsReady || m_LibXMLNode == nullptr)
	{
		return XEResult::Fail;
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

				return XEResult::Ok;
			}

			++count;
		}

		children = children->next;
	}

	return XEResult::Fail;
}

XEXMLParser XEXMLParser::operator()(uint32_t index)
{
	XEXMLParser node;

	GetChildNode(index, node);

	return node;
}

uint32_t XEXMLParser::GetNumChildren()
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

std::wstring XEXMLParser::GetName()
{
	if(!m_IsReady || m_LibXMLNode == nullptr)
	{
		return L"";
	}

	const char* name = reinterpret_cast<const char*>(m_LibXMLNode->name);

	std::wstring wName = XE_Base::String2WideStr(std::string(name));

	return wName;
}

xmlChar* XEXMLParser::GetNodeProperty(const std::wstring& propName)
{
	xmlChar* prop = nullptr;

	if(!m_IsReady || m_LibXMLNode == nullptr)
	{
		return prop;
	}

	std::string sPropName = XE_Base::WideStr2String(propName);
	const xmlChar* xmlPropName = reinterpret_cast<const xmlChar*>(sPropName.c_str());

	prop = xmlGetProp(m_LibXMLNode, xmlPropName);

	return prop;
}

int8_t XEXMLParser::GetInt8(const std::wstring& propName, int8_t defaultValue, bool warning)
{
	int8_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<int8_t>(value);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, (int32_t)defaultValue, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

uint8_t XEXMLParser::GetUInt8(const std::wstring& propName, uint8_t defaultValue, bool warning)
{
	uint8_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<uint8_t>(value);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, (int32_t)defaultValue, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

int16_t XEXMLParser::GetInt16(const std::wstring& propName, int16_t defaultValue, bool warning)
{
	int16_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<int16_t>(value);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

uint16_t XEXMLParser::GetUInt16(const std::wstring& propName, uint16_t defaultValue, bool warning)
{
	uint16_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<uint16_t>(value);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

int32_t XEXMLParser::GetInt(const std::wstring& propName, int32_t defaultValue, bool warning)
{
	int32_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<int32_t>(value);
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

uint32_t XEXMLParser::GetUInt(const std::wstring& propName, uint32_t defaultValue, bool warning)
{
	uint32_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<uint32_t>(value);
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

int64_t XEXMLParser::GetInt64(const std::wstring& propName, int64_t defaultValue, bool warning)
{
	int64_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<int64_t>(value);
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

uint64_t XEXMLParser::GetUInt64(const std::wstring& propName, uint64_t defaultValue, bool warning)
{
	uint64_t ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<uint64_t>(value);
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

float XEXMLParser::GetFloat(const std::wstring& propName, float defaultValue, bool warning)
{
	float ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<float>(value);
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

double XEXMLParser::GetDouble(const std::wstring& propName, double defaultValue, bool warning)
{
	double ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		ret = boost::lexical_cast<double>(value);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

bool XEXMLParser::GetBool(const std::wstring& propName, bool defaultValue, bool warning)
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
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

std::wstring XEXMLParser::GetString(const std::wstring& propName, const std::wstring& defaultValue, bool warning)
{
	std::wstring ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		const char* sValue = reinterpret_cast<const char*>(value);

		ret = XE_Base::String2WideStr(sValue);
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MSG"), __FUNCTIONW__, defaultValue, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::vec2 XEXMLParser::GetVect2f(const std::wstring& propName, const glm::vec2& defaultValue, bool warning)
{
	glm::vec2 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%f %f", &ret.x, &ret.y);   
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::vec3 XEXMLParser::GetVect3f(const std::wstring& propName, const glm::vec3& defaultValue, bool warning)
{
	glm::vec3 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%f %f %f", &ret.x, &ret.y, &ret.z);   
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::vec4 XEXMLParser::GetVect4f(const std::wstring& propName, const glm::vec4& defaultValue, bool warning)
{
	glm::vec4 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%f %f %f %f", &ret.x, &ret.y, &ret.z, &ret.w);   
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::dvec2 XEXMLParser::GetVect2d(const std::wstring& propName, const glm::dvec2& defaultValue, bool warning)
{
	glm::dvec2 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%f %f", &ret.x, &ret.y);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::dvec3 XEXMLParser::GetVect3d(const std::wstring& propName, const glm::dvec3& defaultValue, bool warning)
{
	glm::dvec3 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%f %f %f", &ret.x, &ret.y, &ret.z);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::dvec4 XEXMLParser::GetVect4d(const std::wstring& propName, const glm::dvec4& defaultValue, bool warning)
{
	glm::dvec4 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%f %f %f %f", &ret.x, &ret.y, &ret.z, &ret.w);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::bvec2 XEXMLParser::GetVect2b(const std::wstring& propName, const glm::bvec2& defaultValue, bool warning)
{
	glm::bvec2 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		std::string values((const char*)value);
		std::vector<std::string> strs = XE_Base::SplitString(values, ' ');

		for (uint32_t i = 0; i < 2 && i < strs.size(); i++)
		{
			ret[i] = (strs[i].compare("true") == 0);
		}
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::bvec3 XEXMLParser::GetVect3b(const std::wstring& propName, const glm::bvec3& defaultValue, bool warning)
{
	glm::bvec3 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		std::string values((const char*)value);
		std::vector<std::string> strs = XE_Base::SplitString(values, ' ');

		for (uint32_t i = 0; i < 3 && i < strs.size(); i++)
		{
			ret[i] = (strs[i].compare("true") == 0);
		}
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::bvec4 XEXMLParser::GetVect4b(const std::wstring& propName, const glm::bvec4& defaultValue, bool warning)
{
	glm::bvec4 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		std::string values((const char*)value);
		std::vector<std::string> strs = XE_Base::SplitString(values, ' ');

		for (uint32_t i = 0; i < 4 && i < strs.size(); i++)
		{
			ret[i] = (strs[i].compare("true") == 0);
		}
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::ivec2 XEXMLParser::GetVect2i(const std::wstring& propName, const glm::ivec2& defaultValue, bool warning)
{
	glm::ivec2 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%i %i", &ret.x, &ret.y);   
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_2_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::ivec3 XEXMLParser::GetVect3i(const std::wstring& propName, const glm::ivec3& defaultValue, bool warning)
{
	glm::ivec3 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%i %i %i", &ret.x, &ret.y, &ret.z);   
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_3_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::ivec4 XEXMLParser::GetVect4i(const std::wstring& propName, const glm::ivec4& defaultValue, bool warning)
{
	glm::ivec4 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value, "%i %i %i %i", &ret.x, &ret.y, &ret.z, &ret.w);   
	}
	else if(warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_VECTOR_4_MSG"), __FUNCTIONW__, defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w, propName);
		
		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::mat2 XEXMLParser::GetMat2f(const std::wstring& propName, const glm::mat2& defaultValue, bool warning)
{
	glm::mat2 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value,
			"%f %f ",
			"%f %f",
			&ret[0].x, &ret[0].y,
			&ret[1].x, &ret[1].y);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MATRIX_2_MSG"), __FUNCTIONW__,
														defaultValue[0].x, defaultValue[0].y,
														defaultValue[1].x, defaultValue[1].y, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::mat3 XEXMLParser::GetMat3f(const std::wstring& propName, const glm::mat3& defaultValue, bool warning)
{
	glm::mat3 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value,
			"%f %f %f ",
			"%f %f %f ",
			"%f %f %f",
			&ret[0].x, &ret[0].y, &ret[0].z,
			&ret[1].x, &ret[1].y, &ret[1].z,
			&ret[2].x, &ret[2].y, &ret[2].z);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MATRIX_3_MSG"), __FUNCTIONW__,
														defaultValue[0].x, defaultValue[0].y, defaultValue[0].z,
														defaultValue[1].x, defaultValue[1].y, defaultValue[1].z, 
														defaultValue[2].x, defaultValue[2].y, defaultValue[2].z, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}

glm::mat4 XEXMLParser::GetMat4f(const std::wstring& propName, const glm::mat4& defaultValue, bool warning)
{
	glm::mat4 ret = defaultValue;

	xmlChar* value = GetNodeProperty(propName);

	if (value)
	{
		sscanf_s((const char*)value,
					"%f %f %f %f ",
					"%f %f %f %f ", 
					"%f %f %f %f ", 
					"%f %f %f %f",
					&ret[0].x, &ret[0].y, &ret[0].z, &ret[0].w,
					&ret[1].x, &ret[1].y, &ret[1].z, &ret[1].w,
					&ret[2].x, &ret[2].y, &ret[2].z, &ret[2].w,
					&ret[3].x, &ret[3].y, &ret[3].z, &ret[3].w);
	}
	else if (warning)
	{
		std::wstring msgerr = fmt::format(XELOCMAN->GetLiteral(L"XML_DEFAULT_VALUE_USE_MATRIX_4_MSG"), __FUNCTIONW__,
														defaultValue[0].x, defaultValue[0].y, defaultValue[0].z, defaultValue[0].w,
														defaultValue[1].x, defaultValue[1].y, defaultValue[1].z, defaultValue[1].w,
														defaultValue[2].x, defaultValue[2].y, defaultValue[2].z, defaultValue[2].w,
														defaultValue[3].x, defaultValue[3].y, defaultValue[3].z, defaultValue[3].w, propName);

		XELOGGER->AddNewLog(LogLevel::Warning, msgerr);
	}

	xmlFree(value);

	return ret;
}
