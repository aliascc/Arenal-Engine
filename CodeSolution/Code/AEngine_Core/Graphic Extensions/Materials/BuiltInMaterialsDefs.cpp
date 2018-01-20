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
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "GraphicDevice.h"
#include "Shaders\ShaderDefs.h"
#include "Base\BaseFunctions.h"
#include "BuiltInMaterialsDefs.h"
#include "Shaders\Buffers\SimpleBuffer.h"
#include "Models\Skinning\SkinningDefs.h"
#include "Shaders\Buffers\ConstantBuffer.h"
#include "Shaders\Buffers\StructuredBuffer.h"
#include "Shaders\Variables\ShaderCustomVariable.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/********************************
*   AEBuiltInMaterialsHelpers   *
*********************************/
namespace AEBuiltInMaterialsHelpers
{

    AEResult BuildCBWVP(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_World_View_Proj
        //
        //    cbuffer _AE_CB_World_View_Proj : register(b0)
        //    {
        //        matrix _AE_World        : packoffset(c0);
        //        matrix _AE_View            : packoffset(c4);
        //        matrix _AE_PROJection    : packoffset(c8);
        //    };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_World_View_Proj
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, AE_CB_WORLD_VIEW_PROJ_NAME);

        /**************************************
        *Add Var 1: _AE_World
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_WORLD_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 2: _AE_View
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_VIEW_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 3: _AE_PROJection
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_PROJECTION_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/

        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildCBVP(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_View_Proj
        //
        //    cbuffer _AE_CB_View_Proj : register(b0)
        //    {
        //        matrix _AE_View            : packoffset(c0);
        //        matrix _AE_PROJection    : packoffset(c4);
        //    };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_World_View_Proj
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, AE_CB_WORLD_VIEW_PROJ_NAME);

        /**************************************
        *Add Var 1: _AE_View
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_VIEW_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 2: _AE_PROJection
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_PROJECTION_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/
        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildCBBones(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_Bones
        //
        //    cbuffer _AE_CB_Bones : register(b1)
        //    {
        //        matrix _AE_BoneTransforms[MAX_BONES] : packoffset(c0);
        //    };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_Bones
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 1, AE_CB_BONES_NAME);

        /**************************************
        *Add Var 1: _AE_World
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_BONE_TRANSFORMS_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = AE_MAX_BONES;
        scv->m_IsArray = true;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4) * AE_MAX_BONES;
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/

        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(cb);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildCBColor(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_Color
        //
        //    cbuffer _AE_CB_Color : register(b0)
        //    {
        //        float4 u_Color : packoffset(c0);
        //    };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_Color
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, AE_CB_COLOR_NAME);

        /**************************************
        *Add Var 1: _AE_World
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_COLOR_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::vec4);
        scv->m_Offset = 0;
        scv->m_UserVariable = true;
        scv->m_Size = sizeof(glm::vec4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Vector;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/
        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(cb);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildCBHalfPixel(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_HalfPixel
        //
        //    cbuffer _AE_CB_HalfPixel : register(b0)
        //    {
        //        float2 _AE_HalfPixel    : packoffset(c0);
        //    };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_HalfPixel
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, AE_CB_HALF_PIXEL_NAME);

        /**************************************
        *Add Var 1: _AE_HalfPixel
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_HALF_PIXEL_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 2;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::vec2);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::vec2);
        scv->m_ShaderVariableClass = ShaderVariableClass::Vector;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/

        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(cb);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildCBFPRLightCulling(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_FPR_LightCulling
        //
        //        cbuffer _AE_CB_FPR_LightCulling : register(b0)
        //        {
        //            matrix    _AE_View                : packoffset(c0);
        //            matrix    _AE_InvProjection        : packoffset(c4);
        //        
        //            uint    _AE_WindowHeight        : packoffset(c8.x);
        //            uint    _AE_WindowWidth            : packoffset(c8.y);
        //            uint    _AE_NumLights            : packoffset(c8.z);
        //        };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_FPR_LightCulling
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, AE_CB_FPR_LIGHT_CULLING_NAME);

        /**************************************
        *Add Var 1: _AE_View
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_VIEW_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 2: _AE_InvProjection
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_INV_PROJECTION_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 4;
        scv->m_Rows = 4;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::mat4);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::mat4);
        scv->m_ShaderVariableClass = ShaderVariableClass::Matrix;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 3: _AE_WindowHeight
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_WINDOW_HEIGHT_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 1;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(uint32_t);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(uint32_t);
        scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
        scv->m_ShaderVariableType = ShaderVariableType::UInt;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 4: _AE_WindowWidth
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_WINDOW_WIDTH_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 1;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(uint32_t);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(uint32_t);
        scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
        scv->m_ShaderVariableType = ShaderVariableType::UInt;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 5: _AE_NumLights
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_NUM_LIGHTS_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 1;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(uint32_t);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(uint32_t);
        scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
        scv->m_ShaderVariableType = ShaderVariableType::UInt;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/

        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(cb);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildCBFPRLights(GraphicDevice* graphicDevice, ConstantBuffer** cb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(cb != nullptr);
        if (cb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;
        uint32_t offset = 0;

        /////////////////////////////////////////////////////
        //Create Constant Buffer _AE_CB_FPR_LightCulling
        //
        //        cbuffer _AE_CB_FPR_Lights : register(b0)
        //        {
        //            float3    _AE_CameraPos            : packoffset(c0);
        //            uint    _AE_WindowWidth            : packoffset(c0.w);
        //        };
        //

        /****************************************************************************
        *Constant Buffer: _AE_CB_HalfPixel
        ****************************************************************************/
        ConstantBuffer* tempCB = new ConstantBuffer(graphicDevice, 0, AE_CB_FPR_LIGHTS_NAME);

        /**************************************
        *Add Var 1: _AE_CameraPos
        ***************************************/
        ShaderCustomVariable* scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_CAMERA_POS_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 3;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(glm::vec3);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(glm::vec3);
        scv->m_ShaderVariableClass = ShaderVariableClass::Vector;
        scv->m_ShaderVariableType = ShaderVariableType::Float;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Add Var 2: _AE_WindowWidth
        ***************************************/
        scv = new ShaderCustomVariable();
        scv->m_Name = AE_CB_WINDOW_WIDTH_VAR_NAME;
        scv->m_StartOffset = offset;
        scv->m_Columns = 1;
        scv->m_Rows = 1;
        scv->m_Elements = 0;
        scv->m_IsArray = false;
        scv->m_ElementSize = sizeof(uint32_t);
        scv->m_Offset = 0;
        scv->m_UserVariable = false;
        scv->m_Size = sizeof(uint32_t);
        scv->m_ShaderVariableClass = ShaderVariableClass::Scalar;
        scv->m_ShaderVariableType = ShaderVariableType::UInt;

        ret = tempCB->AddVariable(scv);

        offset = (scv->m_StartOffset + scv->m_Offset + scv->m_Size);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempCB);

            DeleteMem(scv);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Initialize
        ***************************************/

        ret = tempCB->Initialize();

        if (ret != AEResult::Ok)
        {
            DeleteMem(cb);

            return AEResult::ConstantBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *cb = tempCB;

        return AEResult::Ok;
    }

    AEResult BuildBufferLightBuffer(GraphicDevice* graphicDevice, StructuredBuffer** sb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(sb != nullptr);
        if (sb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;

        /////////////////////////////////////////////////////
        //Create Structured Buffer
        //as in Shader:
        // StructuredBuffer<Light> _AE_LightBuffer : register(t0);
        //

        /****************************************************************************
        *Structured Buffer: _AE_LightBuffer
        ****************************************************************************/
        StructuredBuffer* tempSB = new StructuredBuffer(AE_BF_LIGHT_BUFFER_NAME, AE_BI_LIGHT_BUFFER_CULL_BIND_IDX, false, graphicDevice);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempSB);

            return AEResult::ShaderStructuredBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *sb = tempSB;

        return AEResult::Ok;
    }

    AEResult BuildBufferPerTileLightIndexBuffer(GraphicDevice* graphicDevice, SimpleBuffer** sb)
    {
        AEAssert(graphicDevice != nullptr);
        if (graphicDevice == nullptr)
        {
            return AEResult::GraphicDeviceNull;
        }

        AEAssert(sb != nullptr);
        if (sb == nullptr)
        {
            return AEResult::NullParameter;
        }

        AEResult ret = AEResult::Ok;

        /////////////////////////////////////////////////////
        //Create Structured Buffer
        //as in Shader:
        // RWBuffer<uint> _AE_PerTileLightIndexBuffer
        //

        /****************************************************************************
        *Simple Buffer: _AE_PerTileLightIndexBuffer
        ****************************************************************************/
        SimpleBuffer* tempSB = new SimpleBuffer(AE_BF_PER_TILE_LIGHT_INDEX_BUFFER_NAME, AE_BI_PER_TILE_LIGHT_INDEX_BUFFER_CULL_BIND_IDX, true, ShaderVariableClass::Scalar, ShaderVariableType::UInt, 1, graphicDevice);

        if (ret != AEResult::Ok)
        {
            DeleteMem(tempSB);

            return AEResult::ShaderSimpleBufferInitFailed;
        }

        /**************************************
        *Constant Buffer Finish
        ***************************************/
        *sb = tempSB;

        return AEResult::Ok;
    }

}
