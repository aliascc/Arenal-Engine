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

#ifndef _GAME_COMPONENT_H
#define _GAME_COMPONENT_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>
#include <functional>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameService.h"
#include "GameUtilsDefs.h"

/********************
*   Forward Decls   *
*********************/
class GameApp;
struct TimerParams;
class GameResourceManager;

/*****************
*   Class Decl   *
******************/
class GameComponent abstract : public GameService
{
    //Friend class to access private methods
    friend class GameComponentCollection;

    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Callback NeedSortChange from GameCollection to let it know if we change the draw/update order and we need a sort.
        /// It is Private so derive classes can not change its function
        /// </summary>
        NeedSortChangeCallback m_NeedSortChangeCallback = nullptr;

        /// <summary>
        /// Order in which game component is called to be updated and drawn. Private so we force the derive classes to use the get and set methods
        /// </summary>
        uint32_t m_CallOrder = 0;
        
#pragma endregion

        /************************
         *   Private Methods    *
         ************************/
#pragma region Private Methods

        /// <summary>
        /// Find out if we are a game component or drawable game component
        /// </summary>
        /// <returns>Returns true if it a drawable game component, false otherwise</returns>
        virtual inline bool IsDrawableGameComponent    () const
        { 
            return false; 
        }

#pragma endregion

    protected:

        /****************************
         *   Protected Variables    *
         ****************************/
#pragma region Protected Variables
        
        /// <summary>
        /// Parent GameApp pointer
        /// </summary>
        GameApp* m_GameApp = nullptr;

        /// <summary>
        /// Enable/Disable Updates for this Game Component
        /// </summary>
        bool m_Enable = true;

        /// <summary>
        /// Game Resource Manager to handle resources
        /// </summary>
        GameResourceManager* m_GameResourceManager = nullptr;

#pragma endregion

    public:

        /****************************************
         *   Constructor & Destructor Methods   *
         ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameComponent Constructor
        /// </summary>
        GameComponent(GameApp* gameApp, const std::wstring& name = L"", uint32_t callOrder = AE_DEFAULT_GAME_COMPONENT_CALL_ORDER);

        /// <summary>
        /// Default GameComponent Destructor
        /// </summary>
        virtual ~GameComponent();

#pragma endregion
        
        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets the GameApp that associated with this component
        /// </summary>
        /// <returns>GameApp</returns>
        inline const GameApp* GetGameApp() const
        { 
            return m_GameApp; 
        }

        /// <summary>
        /// Gets if the Game Component is Active and Update is been called
        /// </summary>
        /// <returns>True if GC is Active</returns>
        inline bool GetEnable() const
        { 
            return m_Enable; 
        }

        /// <summary>
        /// Gets the Game Component call order number
        /// </summary>
        /// <returns>Call Order Number of GC</returns>
        inline uint32_t GetCallOrder() const
        { 
            return m_CallOrder; 
        }

#pragma endregion
        
        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        /// <summary>
        /// Sets the Game Component is active state and if Update should be called
        /// </summary>
        inline void SetEnable(bool enable)
        { 
            m_Enable = enable; 
        }

        /// <summary>
        /// Sets the Game Component's Call Order Number
        /// </summary>
        void SetCallOrder(uint32_t callOrder);

#pragma endregion
        
        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Initialize Methods of Game Component
        /// Here the GC's variables are initiated and GC is prepare to run
        /// </summary>
        virtual void Initialize();

        /// <summary>
        /// Load's the Content of the Game Component to memory
        /// </summary>
        virtual void LoadContent();

        /// <summary>
        /// Unloads the Content of the Game Component that are in memory
        /// </summary>
        virtual void UnLoadContent();

        /// <summary>
        /// Constant Update method for the Game Component called a constant interval if the GC is active
        /// Interval is Constant defined in Time of GameApp
        /// </summary>
        /// <param name="timerParams">Game Timer Parameters</param>
        virtual void ConstantUpdate(const TimerParams& timerParams);

        /// <summary>
        /// Update method for the Game Component called each frame if the GC is active
        /// </summary>
        /// <param name="timerParams">Game Timer Parameters</param>
        virtual void Update(const TimerParams& timerParams);

        /// <summary>
        /// Post Update method for the Game Component called each frame after the Update Method if the GC is active
        /// </summary>
        /// <param name="timerParams">Game Timer Parameters</param>
        virtual void PostUpdate(const TimerParams& timerParams);

        /// <summary>
        /// If the Graphic Device is Lost this method will be called and GC should handle decide what to do 
        /// </summary>
        virtual void OnLostDevice();

        /// <summary>
        /// If the Graphic Device is Lost and it is recover this methods will be called and GC should handle decide what to do 
        /// </summary>
        virtual void OnResetDevice();

#pragma endregion

};

#endif
