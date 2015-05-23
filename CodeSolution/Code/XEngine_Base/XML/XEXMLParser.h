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
		int8_t			GetInt8				(const std::wstring& propName, int8_t defaultValue = 0, bool warning = true);
		uint8_t			GetUInt8			(const std::wstring& propName, uint8_t defaultValue = 0, bool warning = true);
		int16_t			GetInt16			(const std::wstring& propName, int16_t defaultValue = 0, bool warning = true);
		uint16_t		GetUInt16			(const std::wstring& propName, uint16_t defaultValue = 0, bool warning = true);
		int32_t			GetInt				(const std::wstring& propName, int32_t defaultValue = 0, bool warning = true);
		uint32_t		GetUInt				(const std::wstring& propName, uint32_t defaultValue = 0, bool warning = true);
		int64_t			GetInt64			(const std::wstring& propName, int64_t defaultValue = 0, bool warning = true);
		uint64_t		GetUInt64			(const std::wstring& propName, uint64_t defaultValue = 0, bool warning = true);
		float			GetFloat			(const std::wstring& propName, float defaultValue = 0.0f, bool warning = true);
		double			GetDouble			(const std::wstring& propName, double defaultValue = 0.0, bool warning = true);
		bool			GetBool				(const std::wstring& propName, bool defaultValue = false, bool warning = true);
		std::wstring	GetString			(const std::wstring& propName, const std::wstring& defaultValue = L"", bool warning = true);
		glm::vec2		GetVect2f			(const std::wstring& propName, const glm::vec2& defaultValue = XEMathHelpers::Vec2fZero, bool warning = true);
		glm::vec3		GetVect3f			(const std::wstring& propName, const glm::vec3& defaultValue = XEMathHelpers::Vec3fZero, bool warning = true);
		glm::vec4		GetVect4f			(const std::wstring& propName, const glm::vec4& defaultValue = XEMathHelpers::Vec4fZero, bool warning = true);
		glm::dvec2		GetVect2d			(const std::wstring& propName, const glm::dvec2& defaultValue = XEMathHelpers::Vec2dZero, bool warning = true);
		glm::dvec3		GetVect3d			(const std::wstring& propName, const glm::dvec3& defaultValue = XEMathHelpers::Vec3dZero, bool warning = true);
		glm::dvec4		GetVect4d			(const std::wstring& propName, const glm::dvec4& defaultValue = XEMathHelpers::Vec4dZero, bool warning = true);
		glm::bvec2		GetVect2b			(const std::wstring& propName, const glm::bvec2& defaultValue = XEMathHelpers::Vec2bZero, bool warning = true);
		glm::bvec3		GetVect3b			(const std::wstring& propName, const glm::bvec3& defaultValue = XEMathHelpers::Vec3bZero, bool warning = true);
		glm::bvec4		GetVect4b			(const std::wstring& propName, const glm::bvec4& defaultValue = XEMathHelpers::Vec4bZero, bool warning = true);
		glm::ivec2		GetVect2i			(const std::wstring& propName, const glm::ivec2& defaultValue = XEMathHelpers::Vec2iZero, bool warning = true);
		glm::ivec3		GetVect3i			(const std::wstring& propName, const glm::ivec3& defaultValue = XEMathHelpers::Vec3iZero, bool warning = true);
		glm::ivec4		GetVect4i			(const std::wstring& propName, const glm::ivec4& defaultValue = XEMathHelpers::Vec4iZero, bool warning = true);
		glm::mat2		GetMat2f			(const std::wstring& propName, const glm::mat2& defaultValue = XEMathHelpers::Mat2Identity, bool warning = true);
		glm::mat3		GetMat3f			(const std::wstring& propName, const glm::mat3& defaultValue = XEMathHelpers::Mat3Identity, bool warning = true);
		glm::mat4		GetMat4f			(const std::wstring& propName, const glm::mat4& defaultValue = XEMathHelpers::Mat4Identity, bool warning = true);

		//Operator Overload
		XEXMLParser operator[] (const std::wstring& nodeName);

		XEXMLParser operator() (uint32_t index);
};

#endif