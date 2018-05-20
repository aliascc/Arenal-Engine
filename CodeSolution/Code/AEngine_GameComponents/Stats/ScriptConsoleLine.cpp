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

/*************************
*   Precompiled Header   *
**************************/
#include "precomp_gamecomponents.h"

/**********************
*   System Includes   *
***********************/
#include <new>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ScriptConsoleLine.h"

/********************
*   Function Defs   *
*********************/
ScriptConsoleLine::ScriptConsoleLine()
{
}

ScriptConsoleLine::ScriptConsoleLine(const ScriptConsoleLine& other)
{
    *this = other;
}

ScriptConsoleLine::~ScriptConsoleLine()
{
    ReleaseAngel(m_SA_Strings);
    ReleaseAngel(m_SA_Colors);
}

ScriptConsoleLine& ScriptConsoleLine::operator=(const ScriptConsoleLine& other)
{
    ReleaseAngel(m_SA_Strings);
    ReleaseAngel(m_SA_Colors);

    if (other.m_SA_Strings != nullptr)
    {
        m_SA_Strings = other.m_SA_Strings;
        other.m_SA_Strings->AddRef();
    }

    if (other.m_SA_Colors != nullptr)
    {
        m_SA_Colors = other.m_SA_Colors;
        other.m_SA_Colors->AddRef();
    }

    return *this;
}

void ScriptConsoleLine::Constructor(ScriptConsoleLine* self)
{
    new(self) ScriptConsoleLine();
}

void ScriptConsoleLine::Destructor(ScriptConsoleLine* self)
{
    self->~ScriptConsoleLine();
}
