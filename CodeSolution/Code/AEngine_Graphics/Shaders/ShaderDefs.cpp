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
#include "precomp_graphics.h"

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "ShaderDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

/*********************
*   AEShaderHelper   *
**********************/

namespace AEShaderHelper
{

    D3D11_FILTER GetDXFilter(ShaderFilter shaderFilter)
    {
        D3D11_FILTER dxFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

        switch (shaderFilter)
        {
            case ShaderFilter::MinMagMipPoint:
                dxFilter = D3D11_FILTER_MIN_MAG_MIP_POINT;
                break;
            case ShaderFilter::MinMagPoint_MipLinear:
                dxFilter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::MinPoint_MagLinear_MipPoint:
                dxFilter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::MinPoint_MagMipLinear:
                dxFilter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::MinLinear_MagMipPoint:
                dxFilter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
                break;
            case ShaderFilter::MinLinear_MagPoint_MipLinear:
                dxFilter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::MinMagLinear_MipPoint:
                dxFilter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::MinMagMipLinear:
                dxFilter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Anisotropic:
                dxFilter = D3D11_FILTER_ANISOTROPIC;
                break;
            case ShaderFilter::Comp_MinMagMipPoint:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
                break;
            case ShaderFilter::Comp_MinMagPoint_MipLinear:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::Comp_MinPoint_MagLinear_MipPoint:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::Comp_MinPoint_MagMipLinear:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Comp_MinLinear_MagMipPoint:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT;
                break;
            case ShaderFilter::Comp_MinLinear_MagPoint_MipLinear:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::Comp_MinMagLinear_MipPoint:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::Comp_MinMagMipLinear:
                dxFilter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Comp_Anisotropic:
                dxFilter = D3D11_FILTER_COMPARISON_ANISOTROPIC;
                break;
            case ShaderFilter::Minimum_MinMagMipPoint:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
                break;
            case ShaderFilter::Minimum_MinMagPoint_MipLinear:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::Minimum_MinPoint_MagLinear_MipPoint:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::Minimum_MinPoint_MagMipLinear:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Minimum_MinLinear_MagMipPoint:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT;
                break;
            case ShaderFilter::Minimum_MinLinear_MagPoint_MipLinear:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::Minimum_MinMagLinear_MipPoint:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::Minimum_MinMagMipLinear:
                dxFilter = D3D11_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Minimum_Anisotropic:
                dxFilter = D3D11_FILTER_MINIMUM_ANISOTROPIC;
                break;
            case ShaderFilter::Maximum_MinMagMipPoint:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
                break;
            case ShaderFilter::Maximum_MinMagPoint_MipLinear:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::Maximum_MinPoint_MagLinear_MipPoint:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::Maximum_MinPoint_MagMipLinear:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Maximum_MinLinear_MagMipPoint:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT;
                break;
            case ShaderFilter::Maximum_MinLinear_MagPoint_MipLinear:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
                break;
            case ShaderFilter::Maximum_MinMagLinear_MipPoint:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT;
                break;
            case ShaderFilter::Maximum_MinMagMipLinear:
                dxFilter = D3D11_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
                break;
            case ShaderFilter::Maximum_Anisotropic:
                dxFilter = D3D11_FILTER_MAXIMUM_ANISOTROPIC;
                break;
            default:
                {
                    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("DX_TYPE_NOT_FOUND_ERR_MSG"), __FUNCTION__, "DX Filter", static_cast<uint32_t>(shaderFilter), "D3D11_FILTER_MIN_MAG_MIP_LINEAR");
                    AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
                }
                break;
        }

        return dxFilter;
    }

    D3D11_TEXTURE_ADDRESS_MODE GetDXTextureAddressMode(ShaderTextureAddressMode textureAddressMode)
    {
        D3D11_TEXTURE_ADDRESS_MODE dxTAMode = D3D11_TEXTURE_ADDRESS_CLAMP;

        switch (textureAddressMode)
        {
            case ShaderTextureAddressMode::Wrap:
                dxTAMode = D3D11_TEXTURE_ADDRESS_WRAP;
                break;
            case ShaderTextureAddressMode::Mirror:
                dxTAMode = D3D11_TEXTURE_ADDRESS_MIRROR;
                break;
            case ShaderTextureAddressMode::Clamp:
                dxTAMode = D3D11_TEXTURE_ADDRESS_CLAMP;
                break;
            case ShaderTextureAddressMode::Border:
                dxTAMode = D3D11_TEXTURE_ADDRESS_BORDER;
                break;
            case ShaderTextureAddressMode::MirrorOnce:
                dxTAMode = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
                break;
            default:
                {
                    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("DX_TYPE_NOT_FOUND_ERR_MSG"), __FUNCTION__, "DX Texture Address Mode", static_cast<uint32_t>(textureAddressMode), "D3D11_TEXTURE_ADDRESS_CLAMP");
                    AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
                }
                break;
        }

        return dxTAMode;
    }

    D3D11_COMPARISON_FUNC GetDXCompFunct(ShaderCompFunct compFunct)
    {
        D3D11_COMPARISON_FUNC dxCompFunt = D3D11_COMPARISON_NEVER;

        switch (compFunct)
        {
            case ShaderCompFunct::Never:
                dxCompFunt = D3D11_COMPARISON_NEVER;
                break;
            case ShaderCompFunct::Less:
                dxCompFunt = D3D11_COMPARISON_LESS;
                break;
            case ShaderCompFunct::Equal:
                dxCompFunt = D3D11_COMPARISON_EQUAL;
                break;
            case ShaderCompFunct::LessEqual:
                dxCompFunt = D3D11_COMPARISON_LESS_EQUAL;
                break;
            case ShaderCompFunct::Greater:
                dxCompFunt = D3D11_COMPARISON_GREATER;
                break;
            case ShaderCompFunct::NotEqual:
                dxCompFunt = D3D11_COMPARISON_NOT_EQUAL;
                break;
            case ShaderCompFunct::GreaterEqual:
                dxCompFunt = D3D11_COMPARISON_GREATER_EQUAL;
                break;
            case ShaderCompFunct::Always:
                dxCompFunt = D3D11_COMPARISON_ALWAYS;
                break;
            default:
                {
                    std::string msgerr = fmt::format(AELOCMAN.GetLiteral("DX_TYPE_NOT_FOUND_ERR_MSG"), __FUNCTION__, "DX Comparison Function", static_cast<uint32_t>(compFunct), "D3D11_COMPARISON_NEVER");
                    AELOGGER.AddNewLog(LogLevel::Warning, msgerr);
                }
                break;
        }

        return dxCompFunt;
    }

    uint32_t GetShaderVariableTypeSize(ShaderVariableType svType)
    {
        switch (svType)
        {
            case ShaderVariableType::Boolean:
                return sizeof(bool);

            case ShaderVariableType::Int:
                return sizeof(int32_t);

            case ShaderVariableType::UInt:
                return sizeof(uint32_t);

            case ShaderVariableType::UInt8:
                return sizeof(uint8_t);

            case ShaderVariableType::Float:
                return sizeof(float);

            case ShaderVariableType::Double:
                return sizeof(double);

            default:
                return 0;
        }
    }

    bool IsShaderVariableTypeScalar(ShaderVariableType svType)
    {
        switch (svType)
        {
            case ShaderVariableType::Int:
            case ShaderVariableType::UInt:
            case ShaderVariableType::UInt8:
            case ShaderVariableType::Float:
            case ShaderVariableType::Double:
            case ShaderVariableType::Boolean:
                return true;

            default:
                return false;
        }
    }
}
