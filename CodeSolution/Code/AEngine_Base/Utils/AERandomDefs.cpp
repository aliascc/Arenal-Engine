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
#include "AERandomDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

namespace AERandomHelpers
{
    /*********************
    *   Variables Defs   *
    **********************/
    static std::default_random_engine gRandomGenerator((std::random_device())());

    /********************
    *   Function Defs   *
    *********************/
    float AERandomHelpers::GetFloat(float min, float max)
    {
        std::uniform_real_distribution<float> disRand(min, max);

        return disRand(gRandomGenerator);
    }

    int32_t AERandomHelpers::GetInt(int32_t min, int32_t max)
    {
        std::uniform_int_distribution<int32_t> disRand(min, max);

        return disRand(gRandomGenerator);
    }

    uint32_t GetUInt(uint32_t min, uint32_t max)
    {
        std::uniform_int_distribution<uint32_t> disRand(min, max);

        return disRand(gRandomGenerator);
    }
}
