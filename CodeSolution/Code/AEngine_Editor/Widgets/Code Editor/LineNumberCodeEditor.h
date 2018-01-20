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

#ifndef _LINE_NUMBER_CODE_EDITOR_H
#define _LINE_NUMBER_CODE_EDITOR_H

/**********************
*   System Includes   *
***********************/
#include <string>
#include <stdint.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qevent.h"
#include "QtWidgets\qwidget.h"

/***************************
*   Game Engine Includes   *
****************************/

/********************
*   Forward Decls   *
*********************/
class CodeEditorTextEdit;

/*****************
*   Class Decl   *
******************/
class LineNumberCodeEditor sealed : public QWidget
{
    private:
        CodeEditorTextEdit* m_CodeEditorTextEdit;

    protected:
        void paintEvent(QPaintEvent* pEvent) override;

    public:
        LineNumberCodeEditor(CodeEditorTextEdit* codeEditorTextEdit);
        ~LineNumberCodeEditor();

        QSize sizeHint() const override;
};

#endif
