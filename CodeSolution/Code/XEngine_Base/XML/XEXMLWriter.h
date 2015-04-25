/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XE_XML_WRITER_H
#define _XE_XML_WRITER_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"
#include "libxml\xmlwriter.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\XEObject.h"
#include "Math\XEMathDefs.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

class XEXMLWriter sealed : public XEObject
{
	private:
		bool m_IsReady = false;
		bool m_InMemory = false;
		xmlTextWriterPtr m_XMLWriter = nullptr;
		xmlBufferPtr m_XMLBuffer = nullptr;
		std::wstring m_Filename = L"";

		void CleanUp();

	public:
		XEXMLWriter();
		virtual ~XEXMLWriter();

		XEResult CreateXML(const std::wstring& file, bool inMemory);

		XEResult StartNode(const std::wstring& name);

		XEResult EndNode(); 

		XEResult FinalizeXML();

		XEResult WriteString(const std::wstring& propName, const std::wstring& value);

		XEResult WriteInt(const std::wstring& propName, int32_t value);

		XEResult WriteUInt(const std::wstring& propName, uint32_t value);

		XEResult WriteInt64(const std::wstring& propName, int64_t value);

		XEResult WriteUInt64(const std::wstring& propName, uint64_t value);

		XEResult WriteFloat(const std::wstring& propName, float value);

		XEResult WriteBool(const std::wstring& propName, bool value);

		XEResult WriteVect3f(const std::wstring& propName, const glm::vec3& value);

		XEResult WriteVect4f(const std::wstring& propName, const glm::vec4& value);

		XEResult WriteVect2i(const std::wstring& propName, const glm::ivec2& value);

		XEResult WriteVect3i(const std::wstring& propName, const glm::ivec3& value);

		XEResult WriteVect4i(const std::wstring& propName, const glm::ivec4& value);
};

#endif
