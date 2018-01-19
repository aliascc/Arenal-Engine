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
#include <stdint.h>

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
        /// Number of Frames to be sampled
        /// </summary>
        uint32_t m_FrameCount = 0;

        /// <summary>
        /// Current FPS
        /// </summary>
        float m_FPS = 0.0f;

        /// <summary>
        /// Current Milliseconds per frame
        /// </summary>
        float m_MilliPerFrame = 0.0f;

        /// <summary>
        /// Last time FPS was sampled
        /// </summary>
        double m_LastFPSTime = 0.0;

        /// <summary>
        /// Timer for 'Constant Update'
        /// </summary>
        AETimer m_ConstantUpdateTimer;

        /// <summary>
        /// Timer for 'Update'
        /// </summary>
        AETimer m_UpdateTimer;

        /// <summary>
        /// Timer for 'Post Update'
        /// </summary>
        AETimer m_PostUpdateTimer;

        /// <summary>
        /// Timer for 'Frame'
        /// </summary>
        AETimer m_RenderTimer;

        /// <summary>
        /// Timer for 'Frame'
        /// </summary>
        AETimer m_FrameTimer;

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
        AETime(bool sample = false);

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
        /// Gets Timer Parameters for 'Constant Update'
        /// </summary>
        /// <returns>Constant Update Timer Params</returns>
        inline const TimerParams& GetConstantUpdateTimerParams() const
        {
            return m_ConstantUpdateTimer.GetTimerParams();
        }

        /// <summary>
        /// Gets Timer Parameters for 'Update'
        /// </summary>
        /// <returns>Update Timer Params</returns>
        inline const TimerParams& GetUpdateTimerParams() const{
            return m_UpdateTimer.GetTimerParams();
        }

        /// <summary>
        /// Gets Timer Parameters for 'Post Update'
        /// </summary>
        /// <returns>Post Update Timer Params</returns>
        inline const TimerParams& GetPostUpdateTimerParams() const{
            return m_PostUpdateTimer.GetTimerParams();
        }

        /// <summary>
        /// Gets Timer Parameters for 'Render Update'
        /// </summary>
        /// <returns>Constant Render Timer Params</returns>
        inline const TimerParams& GetRenderTimerParams() const{
            return m_RenderTimer.GetTimerParams();
        }

        /// <summary>
        /// Gets Timer Parameters for 'Frame'
        /// </summary>
        /// <returns>Frame Timer Params</returns>
        inline const TimerParams& GetFrameTimerParams() const
        {
            return m_FrameTimer.GetTimerParams();
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
        /// Updates All Timers
        /// </summary>
        void UpdateAllTimers();

        /// <summary>
        /// Updates 'Constant Update' Timers
        /// </summary>
        void UpdateConstantUpdateTimer();

        /// <summary>
        /// Updates 'Update' Timers
        /// </summary>
        void UpdateUpdateTimer();

        /// <summary>
        /// Updates 'Post Update' Timers
        /// </summary>
        void UpdatePostUpdateTimer();

        /// <summary>
        /// Updates 'Render' Timers
        /// </summary>
        void UpdateRenderTimer();

        /// <summary>
        /// Updates 'Frame' Timers
        /// </summary>
        void UpdateFrameTimer();

        /// <summary>
        /// Does a Post Update on 'Constant Update' Timers
        /// </summary>
        void PostUpdateConstantUpdateTimer();

        /// <summary>
        /// Does a Post Update on 'Update' Timers
        /// </summary>
        void PostUpdateUpdateTimer();

        /// <summary>
        /// Does a Post Update on 'Post Update' Timers
        /// </summary>
        void PostUpdatePostUpdateTimer();

        /// <summary>
        /// Does a Post Update on 'Render' Timers
        /// </summary>
        void PostUpdateRenderTimer();

        /// <summary>
        /// Does a Post Update on 'Frame' Timers
        /// </summary>
        void PostUpdateFrameTimer();

#pragma endregion
};

#endif
