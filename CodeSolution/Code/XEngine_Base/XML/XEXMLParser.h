/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XE_XML_PARSER_H
#define _XE_XML_PARSER_H

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
class XEXMLParser sealed : public XEObject
{
	private:
		bool			m_IsReady = false;
		bool			m_ParentNode = false;
		xmlDocPtr		m_LibXMLDoc = nullptr;
		xmlNodePtr		m_LibXMLNode = nullptr;

		//Private Methods
		void			CleanUp				();
		XEResult		FindNode			(const xmlChar* nodeNameMB, xmlNodePtr xmlLibNode, XEXMLParser& node);
		xmlChar*		GetNodeProperty		(const std::wstring& propName);

	public:

		/***************************************
		*   Constructor & Destructor Methods   *
		****************************************/
#pragma region Constructor & Destructor Methods

		/// <summary>
		/// XEXMLParser Constructor
		/// </summary>
		XEXMLParser();

		/// <summary>
		/// Default XEXMLParser Destructor
		/// </summary>
		virtual ~XEXMLParser();

#pragma endregion

		//Gets
		bool IsReady() const
		{
			return m_IsReady;
		}

		//Framework Methods
		XEResult		LoadFile			(const std::wstring& file);
		XEResult		GetNode				(const std::wstring& nodeName, XEXMLParser& node);
		XEResult		GetChildNode		(uint32_t index, XEXMLParser& node);
		uint32_t		GetNumChildren		();
		std::wstring	GetName				();
		int32_t			GetInt				(const std::wstring& propName, int32_t defaultValue = 0, bool warning = true);
		uint32_t		GetUInt				(const std::wstring& propName, uint32_t defaultValue = 0, bool warning = true);
		int64_t			GetInt64			(const std::wstring& propName, int64_t defaultValue = 0, bool warning = true);
		uint64_t		GetUInt64			(const std::wstring& propName, uint64_t defaultValue = 0, bool warning = true);
		float			GetFloat			(const std::wstring& propName, float defaultValue = 0.0f, bool warning = true);
		bool			GetBool				(const std::wstring& propName, bool defaultValue = false, bool warning = true);
		std::wstring	GetString			(const std::wstring& propName, const std::wstring& defaultValue = L"", bool warning = true);
		glm::vec2		GetVect2f			(const std::wstring& propName, const glm::vec2& defaultValue = XEMathHelpers::Vec2fZero, bool warning = true);
		glm::vec3		GetVect3f			(const std::wstring& propName, const glm::vec3& defaultValue = XEMathHelpers::Vec3fZero, bool warning = true);
		glm::vec4		GetVect4f			(const std::wstring& propName, const glm::vec4& defaultValue = XEMathHelpers::Vec4fZero, bool warning = true);
		glm::ivec2		GetVect2i			(const std::wstring& propName, const glm::ivec2& defaultValue = XEMathHelpers::Vec2iZero, bool warning = true);
		glm::ivec3		GetVect3i			(const std::wstring& propName, const glm::ivec3& defaultValue = XEMathHelpers::Vec3iZero, bool warning = true);
		glm::ivec4		GetVect4i			(const std::wstring& propName, const glm::ivec4& defaultValue = XEMathHelpers::Vec4iZero, bool warning = true);

		//Operator Overload
		XEXMLParser operator[] (const std::wstring& nodeName);

		XEXMLParser operator() (uint32_t index);
};

#endif