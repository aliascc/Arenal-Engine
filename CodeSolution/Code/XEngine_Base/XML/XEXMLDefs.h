/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _XE_XML_DEFS_H
#define _XE_XML_DEFS_H

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
#define XE_XML_ENCODING "ISO-8859-1"

#define XEXMLDocFree(x) if(x != nullptr) { xmlFreeDoc(x); x = nullptr; }
#define XEXMLTextWriterFree(x) if(x != nullptr) { xmlFreeTextWriter(x); x = nullptr; }
#define XEXMLBufferFree(x) if(x != nullptr) { xmlBufferFree(x); x = nullptr; }

/******************
*   Struct Decl   *
*******************/

#endif