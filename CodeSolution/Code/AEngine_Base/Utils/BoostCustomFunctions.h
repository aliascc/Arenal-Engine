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

#ifndef _BOOST_CUSTOM_FUNCTIONS_H
#define _BOOST_CUSTOM_FUNCTIONS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "boost/filesystem.hpp" 

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/

namespace boost 
{ 
    namespace filesystem
    {
        /// <summary>
        /// Creates a relative file path
        /// <remarks>
        /// This function was created in: http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path/29221546#29221546
        /// </remarks>
        /// <returns>Relative Path</returns>
        path make_relative(path from, path to);
    } 
}

#endif