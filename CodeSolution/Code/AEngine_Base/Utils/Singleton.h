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

#ifndef _SINGLETON_H
#define _SINGLETON_H

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
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/

/*****************
*   Class Decl   *
******************/

template <class T>
class Singleton abstract : public AEObject
{
    private:

        /************************
        *   Private Variables   *
        *************************/
#pragma region Private Variables

        /// <summary>
        /// Mutex to Lock Thread
        /// </summary>
        static std::mutex m_SinglentonMutex;

#pragma endregion

    protected:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Private Variables

        //Singleton Pointer
        static T* m_Instance;

#pragma endregion

        /*****************************
        *   Protected Constructors   *
        ******************************/
#pragma region Protected Constructor & Destructor

        /// <summary>
        /// Protected constructor
        /// </summary>
        Singleton()
        {
        };

        /// <summary>
        /// Prevent copy-construction
        /// </summary>
        Singleton(const Singleton&) = delete;

        /// <summary>
        /// Prevent assignment
        /// </summary>
        Singleton& operator=(const Singleton&) = delete;

#pragma endregion

    public:

        /**************************
        *   Protected Variables   *
        ***************************/
#pragma region Private Variables

        static T* GetInstance()
        {
            if (m_Instance == nullptr)
            {
                std::lock_guard<std::mutex> lock(m_SinglentonMutex);

                if (m_Instance == nullptr)
                {
                    m_Instance = new T();
                }
            }

            return m_Instance;
        }

        static void DestroyInstance()
        {
            std::lock_guard<std::mutex> lock(m_SinglentonMutex);

            DeleteMem(m_Instance);
        }

#pragma endregion

};

template <class T>
T* Singleton<T>::m_Instance = nullptr;

template <class T>
std::mutex Singleton<T>::m_SinglentonMutex;

#endif
