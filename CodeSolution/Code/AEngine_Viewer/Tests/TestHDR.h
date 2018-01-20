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

#ifndef TEST_HDR_H
#define TEST_HDR_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "GameUtils\DrawableGameComponent.h"
#include "Base\Base.h"
#include "Shapes\SphereShape.h"
#include "Effects\Materials\BasicColorMaterial.h"
#include "Effects\Materials\BasicDeferredRenderingDraw.h"
#include "Effects\Materials\BasicTextureMaterial.h"
#include "Camera\Camera.h"

/********************************
*        Using                    *
*********************************/
using namespace DirectX;
using namespace PackedVector;

/********************
*   Forward Decls   *
*********************/
struct TimerParams;
class GameApp;
class Model;

/*****************
*   Class Decl   *
******************/
class TestHDR : public DrawableGameComponent
{
    protected:
        SphereShape*        m_SphereShape;
        BasicColorMaterial*    m_BasicColorMaterial;
        BasicDeferredRenderingDraw* m_BasicDeferredRenderingDraw;
        BasicTextureMaterial* m_BasicTextureMaterial;
        Camera*                m_Camera;
        XMFLOAT4X4            m_World;
        XMFLOAT4X4            m_WVP;
        Model*                m_Room;
        Model*                m_Tea;

    public:
        //Constructor Destructor.
        TestHDR(GameApp* gameApp);
        virtual ~TestHDR();

        //Framework Methods
        void DrawModel(Model* model);

        //Drawable Game Component Override methods
        void                    Initialize                    ();
        void                    LoadContent                    ();
        void                    Update                        (const TimerParams& timerParams);
        void                    Render                        (const TimerParams& timerParams);
        void                    OnLostDevice                ();
        void                    OnResetDevice                ();
};

#endif
