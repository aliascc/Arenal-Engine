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

#ifndef _WRITER_AE3D_H
#define _WRITER_AE3D_H

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

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/
struct ModelContent;
struct MeshContent;
struct SkeletonContent;
struct AnimationContent;

/*****************
*   Class Decl   *
******************/
class WriterAE3D sealed : public AEObject
{
    private:
        std::string m_FileName = "";

        std::string m_FilePath = "";

        std::string m_FullFileName = "";

        AEResult WriteMesh(const std::string& fileName, MeshContent* mesh);

        AEResult WriteSkeleton(const std::string& fileName, SkeletonContent* skeleton);

        AEResult WriteAnimation(const std::string& fileName, AnimationContent* animation);

        AEResult WriteModelBody();

    public:
        WriterAE3D();
        virtual ~WriterAE3D();

        const std::string& GetOutputFilePath() const 
        {
            return m_FullFileName;
        }

        //Framework
        AEResult WriteToFile(ModelContent* content, const std::string& fileName, const std::string& filePath);
};

#endif
