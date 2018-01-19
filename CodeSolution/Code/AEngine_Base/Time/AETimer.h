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

#ifndef _AE_TIMER_H
#define _AE_TIMER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "AETimeDefs.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class AETimer sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Time Parameter to keep information about the timer
        /// </summary>
        TimerParams m_TimerParams;

        /// <summary>
        /// Keeps previous Time Stamp, to measure time between calls
        /// </summary>
        __int64 m_PrevTime = 0;

        /// <summary>
        /// Stores the Freq of Counts per Second of the system
        /// </summary>
        double m_SecondPerCount = 0.0;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Gets the time Since last call. Works like a lap in a chronometer
        /// </summary>
        /// <param name="updatePrevTime">If true, updates Previous time with current time</param>
        /// <returns>Returns time since last call and Previous time updated</returns>
        double GetLapTime(bool updatePrevTime);

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// Default AETimer Constructor
        /// </summary>
        AETimer();

        /// <summary>
        /// Default AETimer Destructor
        /// </summary>
        virtual ~AETimer();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets current Time Parameters
        /// </summary>
        /// <returns>Returns Time Parameters</returns>
        inline const TimerParams& GetTimerParams() const
        { 
            return m_TimerParams;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Tells the Timer to update it self and all the properties
        /// </summary>
        void Update();

        /// <summary>
        /// Updates Time Parameter 'Previous Call Time'
        /// </summary>
        void PostUpdate();

#pragma endregion
};

#endif
