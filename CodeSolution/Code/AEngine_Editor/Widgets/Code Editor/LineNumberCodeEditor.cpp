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

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\BaseFunctions.h"
#include "CodeEditorTextEdit.h"
#include "LineNumberCodeEditor.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
LineNumberCodeEditor::LineNumberCodeEditor(CodeEditorTextEdit* codeEditorTextEdit)
    : QWidget(codeEditorTextEdit)
    , m_CodeEditorTextEdit(codeEditorTextEdit)
{
    AEAssert(codeEditorTextEdit != nullptr);
}

LineNumberCodeEditor::~LineNumberCodeEditor()
{
}

QSize LineNumberCodeEditor::sizeHint() const
{
    if(m_CodeEditorTextEdit != nullptr)
    {
        return QSize(m_CodeEditorTextEdit->GetLineNumberAreaWidth(), 0);
    }

    return QSize(0, 0);
}

void LineNumberCodeEditor::paintEvent(QPaintEvent* pEvent)
{
    if(m_CodeEditorTextEdit != nullptr)
    {
        m_CodeEditorTextEdit->LineNumberAreaPaintEvent(pEvent);
    }
}
