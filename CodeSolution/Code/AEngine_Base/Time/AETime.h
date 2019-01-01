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

#ifndef _AE_TIME_H
#define _AE_TIME_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "AETimer.h"
#include "AETimeDefs.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/
class AETime sealed : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Methods

        /// <summary>
        /// Flag to know if to sample for FPS
        /// </summary>
        bool m_IsSamplingFPS = false;

        /// <summary>
        /// Number of Sample to calculate FPS
        /// </summary>
        uint32_t m_NumOfSamples = 100;

        /// <summary>
        /// Position in the buffer of last frame sample added
        /// </summary>
        uint32_t m_SampleFramePos = 0;

        /// <summary>
        /// Ring Buffer of Frame Samples
        /// </summary>
        std::vector<double> m_FrameSamples;

        /// <summary>
        /// Current FPS
        /// </summary>
        float m_FPS = 0.0f;

        /// <summary>
        /// Current Milliseconds per frame
        /// </summary>
        float m_MilliPerFrame = 0.0f;

        /// <summary>
        /// Constant Update Step Size
        /// </summary>
        const double m_ConstantUpdateStep = 0.02;

        /// <summary>
        /// Timer for 'Frame'
        /// </summary>
        AETimer m_FrameTimer;

        /// <summary>
        /// Time Parameters for 'Constant Update'
        /// </summary>
        TimerParams m_ConstantUpdateTimerParams;

#pragma endregion

        /**********************
        *   Private Methods   *
        ***********************/
#pragma region Private Methods

        /// <summary>
        /// Samples to get FPS value
        /// </summary>
        void SampleFPS();

#pragma endregion

    public:

        /***************************************
        *   Constructor & Destructor Methods   *
        ****************************************/
#pragma region Constructor & Destructor Methods

        /// <summary>
        /// GameResource Constructor
        /// </summary>
        /// <param name="sample">If true, samples for FPS</param>
        /// <param name="numSamples">Number of Samples of Frame Time to calculate FPS</param>
        AETime(bool sample = false, uint32_t numSamples = 100);

        /// <summary>
        /// Default GameResource Destructor
        /// </summary>
        virtual ~AETime();

#pragma endregion

        /******************
        *   Get Methods   *
        *******************/
#pragma region Get Methods

        /// <summary>
        /// Gets if is Sampling for FPS
        /// </summary>
        /// <returns>True if it is sampling</returns>
        inline bool IsSamplingFPS() const
        {
            return m_IsSamplingFPS;
        }

        /// <summary>
        /// Gets FPS
        /// </summary>
        /// <returns>FPS</returns>
        inline float GetFPS() const
        {
            return m_FPS;
        }

        /// <summary>
        /// Gets millisecond second average for frames
        /// </summary>
        /// <returns>Millisecond average for a frame</returns>
        inline float GetMilliPerFrame() const
        {
            return m_MilliPerFrame;
        }

        /// <summary>
        /// Gets Timer Parameters for 'Frame'
        /// </summary>
        /// <returns>Frame Timer Params</returns>
        inline const TimerParams& GetFrameTimerParams() const
        {
            return m_FrameTimer.GetTimerParams();
        }

        /// <summary>
        /// Gets Timer Parameters for 'Constant Update'
        /// </summary>
        /// <returns>Frame Timer Params</returns>
        inline const TimerParams& GetConstantUpdateTimerParams() const
        {
            return m_ConstantUpdateTimerParams;
        }

#pragma endregion

        /******************
        *   Set Methods   *
        *******************/
#pragma region Set Methods

        /// <summary>
        /// Sets if it is sample for FPS
        /// </summary>
        /// <param name="sample">If true, samples for FPS</param>
        inline void IsSamplingFPS(bool sampleFPS)
        {
            m_IsSamplingFPS = sampleFPS;
        }

#pragma endregion

        /************************
        *   Framework Methods   *
        *************************/
#pragma region Framework Methods

        /// <summary>
        /// Updates Timers
        /// </summary>
        void Update();

        /// <summary>
        /// Updates Constant Update Time Parameters and
        /// checks if Constant Update needs to run
        /// </summary>
        /// <returns>True if constant update needs to run, false if not</returns>
        bool NeedToRunConstantUpdate();

#pragma endregion
};

#endif
