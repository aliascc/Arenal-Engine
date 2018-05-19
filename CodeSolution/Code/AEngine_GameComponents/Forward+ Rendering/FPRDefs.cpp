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
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "FPRDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/************************
*   ForwardPlusHelper   *
*************************/
ForwardPlusHelper::ForwardPlusHelper()
{
}

glm::uvec2 ForwardPlusHelper::GetNumTiles(const glm::uvec2& screenSize)
{
    return GetNumTiles(screenSize.x, screenSize.y);
}

glm::uvec2 ForwardPlusHelper::GetNumTiles(uint32_t width, uint32_t height)
{
    glm::uvec2 numTiles;

    float numTilesX = ( (width + AE_TILE_RES - 1 ) / (float)AE_TILE_RES );
    float numTilesY = ( (height + AE_TILE_RES - 1 ) / (float)AE_TILE_RES );

    numTiles.x = static_cast<uint32_t>(numTilesX);
    numTiles.y = static_cast<uint32_t>(numTilesY);

    return numTiles;
}
