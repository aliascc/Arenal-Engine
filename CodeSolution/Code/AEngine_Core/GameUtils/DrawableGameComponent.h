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

#ifndef _DRAWABLE_GAME_COMPONENT_H
#define _DRAWABLE_GAME_COMPONENT_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameComponent.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class GraphicDevice;

/*****************
*   Class Decl   *
******************/
class DrawableGameComponent abstract : public GameComponent
{
    //Friend class
    friend class GameComponentCollection;

    private:

        //Find out if we are a game component or drawable game component
        inline bool IsDrawableGameComponent() const override
        {
            return true;
        }

    protected:
        //Variable to check if we can draw or not
        bool m_Visible = true;
        GraphicDevice& m_GraphicDevice;

    public:
        //Constructor Destructor
        DrawableGameComponent(GameApp& gameApp, const std::string& name = "", uint32_t callOrder = 100);
        virtual ~DrawableGameComponent();

        //Gets
        inline bool GetVisible() const
        {
            return m_Visible;
        }

        //Sets
        inline void SetVisible(bool visible)
        {
            m_Visible = visible;
        }

        //Framework Methods

        /// <summary>
        /// Render method for the Drawable Game Component called each frame if the GC is active
        /// </summary>
        /// <param name="timerParams">Game Timer Parameters</param>
        virtual void Render(const TimerParams& timerParams) = 0;
};

#endif
