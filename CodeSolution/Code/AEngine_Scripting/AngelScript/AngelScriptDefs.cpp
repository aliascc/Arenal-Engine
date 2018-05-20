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
#include "precomp_scripting.h"

/**********************
*   System Includes   *
***********************/
#include <new>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AngelScriptDefs.h"

/********************
*   Function Defs   *
*********************/

/***************************
*   AEAngelScriptHelpers   *
****************************/
namespace AEAngelScriptHelpers
{
    uint32_t GetSizeOfType(asETypeIdFlags typeID)
    {
        switch (typeID)
        {
            case asTYPEID_BOOL:
                return sizeof(bool);

            case asTYPEID_INT8:
                return sizeof(int8_t);

            case asTYPEID_INT16:
                return sizeof(int16_t);

            case asTYPEID_INT32:
                return sizeof(int32_t);

            case asTYPEID_INT64:
                return sizeof(int64_t);

            case asTYPEID_UINT8:
                return sizeof(uint8_t);

            case asTYPEID_UINT16:
                return sizeof(uint16_t);

            case asTYPEID_UINT32:
                return sizeof(uint32_t);

            case asTYPEID_UINT64:
                return sizeof(uint64_t);

            case asTYPEID_FLOAT:
                return sizeof(float);

            case asTYPEID_DOUBLE:
                return sizeof(double);

            default:
                return 0;
        }
    }

    bool TypeIsBasic(asETypeIdFlags typeID)
    {
        switch (typeID)
        {
            case asTYPEID_BOOL:
            case asTYPEID_INT8:
            case asTYPEID_INT16:
            case asTYPEID_INT32:
            case asTYPEID_INT64:
            case asTYPEID_UINT8:
            case asTYPEID_UINT16:
            case asTYPEID_UINT32:
            case asTYPEID_UINT64:
            case asTYPEID_FLOAT:
            case asTYPEID_DOUBLE:
                return true;

            default:
                return false;
        }
    }
}
