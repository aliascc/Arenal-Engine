/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#define AE_XML_ENCODING         "ISO-8859-1"
#define AE_XML_ENCODING_STRW    "ISO-8859-1"
#define AE_XML_HEADER           "<?xml version = \"1.0\" encoding = \"" AE_XML_ENCODING_STRW "\"?>\n"

#define AEXMLTextWriterFree(x)  if(x != nullptr)            \
                                {                           \
                                    xmlFreeTextWriter(x);   \
                                    x = nullptr;            \
                                }

#define AEXMLBufferFree(x)  if(x != nullptr)        \
                            {                       \
                                xmlBufferFree(x);   \
                                x = nullptr;        \
                            }

/***********
*   Enum   *
************/

enum class XMLType : uint32_t
{
    Document,
    Element,
    Empty
};

/******************
*   Struct Decl   *
*******************/


#endif
