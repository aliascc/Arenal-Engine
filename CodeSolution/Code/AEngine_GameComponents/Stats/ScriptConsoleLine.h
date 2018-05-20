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

#ifndef _SCRIPT_CONSOLE_LINE_DEFS_H
#define _SCRIPT_CONSOLE_LINE_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/
struct ScriptConsoleLine sealed : public AEObject
{
    CScriptArray* m_SA_Strings = nullptr;
    CScriptArray* m_SA_Colors = nullptr;

    //Contructor
    ScriptConsoleLine();

    ScriptConsoleLine(const ScriptConsoleLine& other);

    virtual ~ScriptConsoleLine();

    ScriptConsoleLine& operator=(const ScriptConsoleLine& other);

    static void Constructor(ScriptConsoleLine* self);

    static void Destructor(ScriptConsoleLine* self);
};

#endif
