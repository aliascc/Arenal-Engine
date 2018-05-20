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

#ifndef _LIGHT_SHAPE_H
#define _LIGHT_SHAPE_H

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
#include "ShapesDefs.h"
#include "Base\AEObject.h"

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
class GraphicDevice;
class IVertexBuffer;

/*****************
*   Class Decl   *
******************/
class LightShape abstract : public AEObject
{
    protected:
        //Variables
        bool m_IsReady = false;

        std::vector<IVertexBuffer*> m_VertexBufferVector;

        GraphicDevice* m_GraphicDevice = nullptr;

        void CleanUp();

    public:
        //Constructor Destructor.
        LightShape(GraphicDevice* graphicDevice);
        virtual ~LightShape();

        //Framework Methods
        virtual AEResult BuildLightShape() = 0;

        AEResult DrawLightShape();
};

#endif

