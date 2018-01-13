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
		bool m_IsReady = false;
		bool m_InMemory = false;
		xmlTextWriterPtr m_XMLWriter = nullptr;
		xmlBufferPtr m_XMLBuffer = nullptr;
		std::wstring m_Filename = L"";

		void CleanUp();

	public:
		AEXMLWriter();
		virtual ~AEXMLWriter();

		AEResult CreateXML(const std::wstring& file, bool inMemory);

		AEResult StartNode(const std::wstring& name);

		AEResult EndNode(); 

		AEResult FinalizeXML();

		AEResult WriteString(const std::wstring& propName, const std::wstring& value);

		AEResult WriteInt8(const std::wstring& propName, int8_t value);

		AEResult WriteUInt8(const std::wstring& propName, uint8_t value);

		AEResult WriteInt16(const std::wstring& propName, int16_t value);

		AEResult WriteUInt16(const std::wstring& propName, uint16_t value);

		AEResult WriteInt(const std::wstring& propName, int32_t value);

		AEResult WriteUInt(const std::wstring& propName, uint32_t value);

		AEResult WriteInt64(const std::wstring& propName, int64_t value);

		AEResult WriteUInt64(const std::wstring& propName, uint64_t value);

		AEResult WriteFloat(const std::wstring& propName, float value);

		AEResult WriteDouble(const std::wstring& propName, double value);

		AEResult WriteBool(const std::wstring& propName, bool value);

		AEResult WriteVect2f(const std::wstring& propName, const glm::vec2& value);

		AEResult WriteVect3f(const std::wstring& propName, const glm::vec3& value);

		AEResult WriteVect4f(const std::wstring& propName, const glm::vec4& value);

		AEResult WriteVect2d(const std::wstring& propName, const glm::dvec2& value);

		AEResult WriteVect3d(const std::wstring& propName, const glm::dvec3& value);

		AEResult WriteVect4d(const std::wstring& propName, const glm::dvec4& value);

		AEResult WriteVect2b(const std::wstring& propName, const glm::bvec2& value);

		AEResult WriteVect3b(const std::wstring& propName, const glm::bvec3& value);

		AEResult WriteVect4b(const std::wstring& propName, const glm::bvec4& value);

		AEResult WriteVect2i(const std::wstring& propName, const glm::ivec2& value);

		AEResult WriteVect3i(const std::wstring& propName, const glm::ivec3& value);

		AEResult WriteVect4i(const std::wstring& propName, const glm::ivec4& value);

		AEResult WriteMat2f(const std::wstring& propName, const glm::mat2& value);

		AEResult WriteMat3f(const std::wstring& propName, const glm::mat3& value);

		AEResult WriteMat4f(const std::wstring& propName, const glm::mat4& value);
};

#endif
