/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _AE_XML_DEFS_H
#define _AE_XML_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/*************
*   Define   *
**************/
#define AE_XML_ENCODING			"ISO-8859-1"
#define AE_XML_ENCODING_STRW	L"ISO-8859-1"
#define AE_XML_HEADER			L"<?xml version = \"1.0\" encoding = \"" AE_XML_ENCODING_STRW L"\"?>\n"

#define AEXMLDocFree(x) if(x != nullptr) { xmlFreeDoc(x); x = nullptr; }
#define AEXMLTextWriterFree(x) if(x != nullptr) { xmlFreeTextWriter(x); x = nullptr; }
#define AEXMLBufferFree(x) if(x != nullptr) { xmlBufferFree(x); x = nullptr; }

/******************
*   Struct Decl   *
*******************/

#endif