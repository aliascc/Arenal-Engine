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

#ifndef _NULL_EXCEPTION_H
#define _NULL_EXCEPTION_H

/**********************
*   System Includes   *
***********************/
#include <stdexcept>

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
class Keyboard;
class XBoxGamepadManager;

/*****************
*   Class Decl   *
******************/

class NullException sealed : public std::exception
{
    private:
        std::string m_Msg;

    public:

        explicit NullException(const char* message) :
            m_Msg(message)
        {
        }

        explicit NullException(const std::string& message) :
            m_Msg(message)
        {
        }

        virtual ~NullException() throw ()
        {
        }

        const char* what() const throw () override
        {
            return m_Msg.c_str();
        }

};

#endif
