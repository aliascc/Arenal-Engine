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
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Utils\AEQTDefs.h"
#include "Base\BaseFunctions.h"
#include "CodeEditorTextEdit.h"
#include "CodeEditorMainWindow.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
CodeEditorMainWindow::CodeEditorMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    ////////////////////////////////////////
    //Initialize UI
    m_CodeEditorMainWindowQtUI.setupUi(this);
}

CodeEditorMainWindow::~CodeEditorMainWindow()
{
}

AEResult CodeEditorMainWindow::AddCodeTab(const QString& codeFilepath)
{
    std::wstring filePath = codeFilepath.toStdWString();

    QString fileName = QString::fromStdWString(AE_Base::GetFilename(filePath));

    return NewCodeTab(codeFilepath, fileName);
}

AEResult CodeEditorMainWindow::AddCodeTab(const std::wstring& codeFilepath)
{
    QString fileName = QString::fromStdWString(AE_Base::GetFilename(codeFilepath));

    QString qCodeFilepath = QString::fromStdWString(codeFilepath);

    return NewCodeTab(qCodeFilepath, fileName);
}

AEResult CodeEditorMainWindow::NewCodeTab(const QString& codeFilepath, const QString& fileName)
{
    if(m_TabFileMap.find(fileName) != m_TabFileMap.end())
    {
        int32_t index = m_TabFileMap[fileName];

        m_CodeEditorMainWindowQtUI.m_TabTextBoxs->setCurrentIndex(index);

        CodeEditorTextEdit* codeEditorTextEdit = reinterpret_cast<CodeEditorTextEdit*>(m_CodeEditorMainWindowQtUI.m_TabTextBoxs->widget(index));

        return AEResult::Ok;
    }
    else
    {
        CodeEditorTextEdit* codeEditorTextEdit = new CodeEditorTextEdit(codeFilepath);

        if(!codeEditorTextEdit->IsReady())
        {
            DeleteMem(codeEditorTextEdit);

            return AEResult::Fail;
        }

        int32_t index = m_CodeEditorMainWindowQtUI.m_TabTextBoxs->addTab(codeEditorTextEdit, QIcon(), fileName);

        m_TabFileMap[fileName] = index;

        return AEResult::Ok;
    }
}

void CodeEditorMainWindow::on_m_CloseFileAction_triggered()
{
    CloseCurrent();
}

AEResult CodeEditorMainWindow::CloseCurrent()
{
    CodeEditorTextEdit* codeEditorTextEdit = reinterpret_cast<CodeEditorTextEdit*>(m_CodeEditorMainWindowQtUI.m_TabTextBoxs->currentWidget());

    if(codeEditorTextEdit != nullptr)
    {
        if(codeEditorTextEdit->WasModified())
        {
            QMessageBox::StandardButton ret =  QMessageBox::information(    this,
                                                                            "File Modified",
                                                                            tr("File: %1 has been changed, do you want to save the changes?")
                                                                            .arg(codeEditorTextEdit->GetFilename()),
                                                                            QMessageBox::StandardButton::Save,
                                                                            QMessageBox::StandardButton::Cancel);

            if(ret == QMessageBox::StandardButton::Save)
            {
                codeEditorTextEdit->SaveFile();
            }
        }

        int32_t index = m_CodeEditorMainWindowQtUI.m_TabTextBoxs->currentIndex();

        m_TabFileMap.erase(codeEditorTextEdit->GetFilename());

        m_CodeEditorMainWindowQtUI.m_TabTextBoxs->removeTab(index);
    }

    return AEResult::Ok;
}

void CodeEditorMainWindow::closeEvent(QCloseEvent* cEvent)
{
    CloseAll();

    QMainWindow::closeEvent(cEvent);
}

AEResult CodeEditorMainWindow::CloseAll()
{
    int32_t tabCount = m_CodeEditorMainWindowQtUI.m_TabTextBoxs->count();

    for(int32_t i = 0; i < tabCount; i++)
    {
        CodeEditorTextEdit* codeEditorTextEdit = reinterpret_cast<CodeEditorTextEdit*>(m_CodeEditorMainWindowQtUI.m_TabTextBoxs->widget(i));

        if(codeEditorTextEdit != nullptr)
        {
            if(codeEditorTextEdit->WasModified())
            {
                QMessageBox::StandardButton ret =  QMessageBox::information(    this,
                                                                                "File Modified",
                                                                                tr("File: %1 has been changed, do you want to save the changes?")
                                                                                .arg(codeEditorTextEdit->GetFilename()),
                                                                                QMessageBox::StandardButton::Save,
                                                                                QMessageBox::StandardButton::Cancel);

                if(ret == QMessageBox::StandardButton::Save)
                {
                    codeEditorTextEdit->SaveFile();
                }
            }
        }
    }

    m_TabFileMap.clear();
    m_CodeEditorMainWindowQtUI.m_TabTextBoxs->clear();

    return AEResult::Ok;
}

void CodeEditorMainWindow::on_m_SaveTBAction_triggered()
{
    SaveCurrent();
}

void CodeEditorMainWindow::on_m_SaveFileAction_triggered()
{
    SaveCurrent();
}

AEResult CodeEditorMainWindow::SaveCurrent()
{
    CodeEditorTextEdit* codeEditorTextEdit = reinterpret_cast<CodeEditorTextEdit*>(m_CodeEditorMainWindowQtUI.m_TabTextBoxs->currentWidget());

    if(codeEditorTextEdit != nullptr)
    {
        codeEditorTextEdit->SaveFile();
    }

    return AEResult::Ok;
}

void CodeEditorMainWindow::on_m_SaveAllFileAction_triggered()
{
    SaveAll();
}

AEResult CodeEditorMainWindow::SaveAll()
{
    int32_t tabCount = m_CodeEditorMainWindowQtUI.m_TabTextBoxs->count();

    for(int32_t i = 0; i < tabCount; i++)
    {
        CodeEditorTextEdit* codeEditorTextEdit = reinterpret_cast<CodeEditorTextEdit*>(m_CodeEditorMainWindowQtUI.m_TabTextBoxs->widget(i));

        if(codeEditorTextEdit != nullptr)
        {
            codeEditorTextEdit->SaveFile();
        }
    }

    return AEResult::Ok;
}
