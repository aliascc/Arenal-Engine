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

#ifndef _OMNI_LIGHT_SHAPE_H
#define _OMNI_LIGHT_SHAPE_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "LightShape.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class OmniLightShape sealed : public LightShape
{
    private:

        uint32_t m_NumEdges = 0;

        float m_Size = AE_LIGHT_SHAPE_OMNI_DEFAULT_RING_SIZE;

    public:
        //Constructor Destructor.
        OmniLightShape(GraphicDevice* graphicDevice, uint32_t numEdges = AE_LIGHT_SHAPE_DEFAULT_NUM_EDGES);
        virtual ~OmniLightShape();

        AEResult BuildLightShape() override;
};

#endif

