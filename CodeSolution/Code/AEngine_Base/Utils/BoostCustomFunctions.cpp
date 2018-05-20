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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_base.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "BoostCustomFunctions.h"

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
        path make_relative(boost::filesystem::path from, path to)
        {
            // Start at the root path and while they are the same then do nothing then when they first
            // diverge take the remainder of the two path and replace the entire from path with ".."
            // segments.
            path::const_iterator fromIter = from.begin();
            path::const_iterator toIter = to.begin();

            // Loop through both
            while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
            {
                ++toIter;
                ++fromIter;
            }

            path finalPath;
            while (fromIter != from.end())
            {
                finalPath /= "..";
                ++fromIter;
            }

            while (toIter != to.end())
            {
                finalPath /= *toIter;
                ++toIter;
            }

            return finalPath;
        }
    } 
}
