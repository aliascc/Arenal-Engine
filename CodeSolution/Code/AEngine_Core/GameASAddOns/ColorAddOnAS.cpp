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
#include "precomp_core.h"

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "ColorAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Color\AEColorDefs.h"
#include "Base\BaseFunctions.h"

AETODO("Fix this");
/* Do not include it as we need to use the normal new
//Always include last
#include "Memory\MemLeaks.h"
*/

/********************
*   Function Defs   *
*********************/

/*****************************
*   Color static functions   *
******************************/
static void Color_Constructor(Color* self)
{
    new(self)Color();
}

static void Color_Constructor(const Color& otherColor, Color* self)
{
    new(self)Color(otherColor);
}

static void Color_Constructor(uint32_t otherColor, Color* self)
{
    new(self)Color(otherColor);
}

static void Color_Constructor(uint32_t r, uint32_t g, uint32_t b, uint32_t a, Color* self)
{
    new(self)Color(r, g, b, a);
}

static void Color_Destructor(Color* self)
{
    self->~Color();
}


/************
*   Color   *
*************/
ColorAddOnAS::ColorAddOnAS()
    : AngelScriptAddOn((uint32_t)XEGameASAddOnsID::Color)
{
}

ColorAddOnAS::~ColorAddOnAS()
{
}

AEResult ColorAddOnAS::Register(asIScriptEngine* engine)
{
    AEAssert(engine != nullptr);
    if (engine == nullptr)
    {
        return AEResult::NullParameter;
    }

    if (RegisterColorObject(engine) != AEResult::Ok)
    {
        AETODO("Add Log and return");
        AETODO("Wrong Literal Change");
        std::string errMsg = "";
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_BASIC_CONSTS_ERR_MSG"), __FUNCTION__);
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    if (RegisterColorEnum(engine) != AEResult::Ok)
    {
        std::string errMsg = "";
        AETODO("Add Log and return");
        AETODO("Wrong Literal Change");
        //fmt::format(m_LocalizationManager->GetLiteral("AS_REG_ENUM_ERR_MSG"), __FUNCTION__, "AEResult");
        //m_Logger->AddNewLog(LogLevel::Error, errMsg);

        return AEResult::Fail;
    }

    return AEResult::Ok;
}


AEResult ColorAddOnAS::RegisterColorObject(asIScriptEngine* engine)
{
    int ret = 0;

    /////////////////////
    //Register TimeStamp
    /////////////////////
    ret = engine->RegisterObjectType("Color", sizeof(TimeStamp), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
    if (ret < 0)
    {
        return AEResult::RegObjTypeFail;
    }

    ///////////////////////
    //Properties
    ret = engine->RegisterObjectProperty("Color", "uint8  b", asOFFSET(Color, b));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("Color", "uint8  g", asOFFSET(Color, g));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("Color", "uint8  r", asOFFSET(Color, r));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("Color", "uint8  a", asOFFSET(Color, a));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ret = engine->RegisterObjectProperty("Color", "uint  color", asOFFSET(Color, color));
    if (ret < 0)
    {
        return AEResult::RegObjPropFail;
    }

    ///////////////////////
    //Methods
    ret = engine->RegisterObjectMethod("Color", "Color &opAssign(uint)", asMETHODPR(Color, operator=, (uint32_t), Color&), asCALL_THISCALL);
    if (ret < 0)
    {
        return AEResult::RegObjMethodFail;
    }

    //////////////////////////////////////
    //Constructor & Destructor for AS
    ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void AEAS_Color_Constructor()", asFUNCTIONPR(Color_Constructor, (Color*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void AEAS_Color_Constructor(const Color& in)", asFUNCTIONPR(Color_Constructor, (const Color&, Color*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void AEAS_Color_Constructor(uint)", asFUNCTIONPR(Color_Constructor, (uint32_t, Color*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void AEAS_Color_Constructor(uint, uint, uint, uint)", asFUNCTIONPR(Color_Constructor, (uint32_t, uint32_t, uint32_t, uint32_t, Color*), void), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_DESTRUCT, "void AEAS_Color_Destructor()", asFUNCTION(Color_Destructor), asCALL_CDECL_OBJLAST);
    if (ret < 0)
    {
        return AEResult::RegObjBehaviorFail;
    }

    return AEResult::Ok;
}

AEResult ColorAddOnAS::RegisterColorEnum(asIScriptEngine* engine)
{
    int ret = 0;

    //////////////////////////////////////
    //Color Enum
    ret = engine->RegisterEnum("AEColorType");
    if (ret < 0)
    {
        return AEResult::RegEnumFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "AliceBlue", static_cast<uint32_t>(AEColorType::AliceBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "AntiqueWhite", static_cast<uint32_t>(AEColorType::AntiqueWhite));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Aqua", static_cast<uint32_t>(AEColorType::Aqua));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Aquamarine", static_cast<uint32_t>(AEColorType::Aquamarine));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Azure", static_cast<uint32_t>(AEColorType::Azure));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Beige", static_cast<uint32_t>(AEColorType::Beige));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Bisque", static_cast<uint32_t>(AEColorType::Bisque));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Black", static_cast<uint32_t>(AEColorType::Black));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "BlanchedAlmond", static_cast<uint32_t>(AEColorType::BlanchedAlmond));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Blue", static_cast<uint32_t>(AEColorType::Blue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "BlueViolet", static_cast<uint32_t>(AEColorType::BlueViolet));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Brown", static_cast<uint32_t>(AEColorType::Brown));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "BurlyWood", static_cast<uint32_t>(AEColorType::BurlyWood));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "CadetBlue", static_cast<uint32_t>(AEColorType::CadetBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Chartreuse", static_cast<uint32_t>(AEColorType::Chartreuse));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Chocolate", static_cast<uint32_t>(AEColorType::Chocolate));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Coral", static_cast<uint32_t>(AEColorType::Coral));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "CornflowerBlue", static_cast<uint32_t>(AEColorType::CornflowerBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Cornsilk", static_cast<uint32_t>(AEColorType::Cornsilk));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Crimson", static_cast<uint32_t>(AEColorType::Crimson));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Cyan", static_cast<uint32_t>(AEColorType::Cyan));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkBlue", static_cast<uint32_t>(AEColorType::DarkBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkCyan", static_cast<uint32_t>(AEColorType::DarkCyan));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkGoldenrod", static_cast<uint32_t>(AEColorType::DarkGoldenrod));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkGray", static_cast<uint32_t>(AEColorType::DarkGray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkGreen", static_cast<uint32_t>(AEColorType::DarkGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkKhaki", static_cast<uint32_t>(AEColorType::DarkKhaki));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkMagenta", static_cast<uint32_t>(AEColorType::DarkMagenta));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkOliveGreen", static_cast<uint32_t>(AEColorType::DarkOliveGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkOrange", static_cast<uint32_t>(AEColorType::DarkOrange));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkOrchid", static_cast<uint32_t>(AEColorType::DarkOrchid));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkRed", static_cast<uint32_t>(AEColorType::DarkRed));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkSalmon", static_cast<uint32_t>(AEColorType::DarkSalmon));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkSeaGreen", static_cast<uint32_t>(AEColorType::DarkSeaGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkSlateBlue", static_cast<uint32_t>(AEColorType::DarkSlateBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkSlateGray", static_cast<uint32_t>(AEColorType::DarkSlateGray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkTurquoise", static_cast<uint32_t>(AEColorType::DarkTurquoise));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DarkViolet", static_cast<uint32_t>(AEColorType::DarkViolet));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DeepPink", static_cast<uint32_t>(AEColorType::DeepPink));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DeepSkyBlue", static_cast<uint32_t>(AEColorType::DeepSkyBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DimGray", static_cast<uint32_t>(AEColorType::DimGray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "DodgerBlue", static_cast<uint32_t>(AEColorType::DodgerBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Firebrick", static_cast<uint32_t>(AEColorType::Firebrick));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "FloralWhite", static_cast<uint32_t>(AEColorType::FloralWhite));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "ForestGreen", static_cast<uint32_t>(AEColorType::ForestGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Fuchsia", static_cast<uint32_t>(AEColorType::Fuchsia));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Gainsboro", static_cast<uint32_t>(AEColorType::Gainsboro));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "GhostWhite", static_cast<uint32_t>(AEColorType::GhostWhite));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Gold", static_cast<uint32_t>(AEColorType::Gold));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Goldenrod", static_cast<uint32_t>(AEColorType::Goldenrod));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Gray", static_cast<uint32_t>(AEColorType::Gray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Green", static_cast<uint32_t>(AEColorType::Green));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "GreenYellow", static_cast<uint32_t>(AEColorType::GreenYellow));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Honeydew", static_cast<uint32_t>(AEColorType::Honeydew));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "HotPink", static_cast<uint32_t>(AEColorType::HotPink));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "IndianRed", static_cast<uint32_t>(AEColorType::IndianRed));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Indigo", static_cast<uint32_t>(AEColorType::Indigo));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Ivory", static_cast<uint32_t>(AEColorType::Ivory));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Khaki", static_cast<uint32_t>(AEColorType::Khaki));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Lavender", static_cast<uint32_t>(AEColorType::Lavender));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LavenderBlush", static_cast<uint32_t>(AEColorType::LavenderBlush));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LawnGreen", static_cast<uint32_t>(AEColorType::LawnGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LemonChiffon", static_cast<uint32_t>(AEColorType::LemonChiffon));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightBlue", static_cast<uint32_t>(AEColorType::LightBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightCoral", static_cast<uint32_t>(AEColorType::LightCoral));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightCyan", static_cast<uint32_t>(AEColorType::LightCyan));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightGoldenrodYellow", static_cast<uint32_t>(AEColorType::LightGoldenrodYellow));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightGreen", static_cast<uint32_t>(AEColorType::LightGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightGray", static_cast<uint32_t>(AEColorType::LightGray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightPink", static_cast<uint32_t>(AEColorType::LightPink));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightSalmon", static_cast<uint32_t>(AEColorType::LightSalmon));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightSeaGreen", static_cast<uint32_t>(AEColorType::LightSeaGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightSkyBlue", static_cast<uint32_t>(AEColorType::LightSkyBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightSlateGray", static_cast<uint32_t>(AEColorType::LightSlateGray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightSteelBlue", static_cast<uint32_t>(AEColorType::LightSteelBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LightYellow", static_cast<uint32_t>(AEColorType::LightYellow));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Lime", static_cast<uint32_t>(AEColorType::Lime));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "LimeGreen", static_cast<uint32_t>(AEColorType::LimeGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Linen", static_cast<uint32_t>(AEColorType::Linen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Magenta", static_cast<uint32_t>(AEColorType::Magenta));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Maroon", static_cast<uint32_t>(AEColorType::Maroon));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumAquamarine", static_cast<uint32_t>(AEColorType::MediumAquamarine));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumBlue", static_cast<uint32_t>(AEColorType::MediumBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumOrchid", static_cast<uint32_t>(AEColorType::MediumOrchid));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumPurple", static_cast<uint32_t>(AEColorType::MediumPurple));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumSeaGreen", static_cast<uint32_t>(AEColorType::MediumSeaGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumSlateBlue", static_cast<uint32_t>(AEColorType::MediumSlateBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumSpringGreen", static_cast<uint32_t>(AEColorType::MediumSpringGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumTurquoise", static_cast<uint32_t>(AEColorType::MediumTurquoise));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MediumVioletRed", static_cast<uint32_t>(AEColorType::MediumVioletRed));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MidnightBlue", static_cast<uint32_t>(AEColorType::MidnightBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MintCream", static_cast<uint32_t>(AEColorType::MintCream));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "MistyRose", static_cast<uint32_t>(AEColorType::MistyRose));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Moccasin", static_cast<uint32_t>(AEColorType::Moccasin));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "NavajoWhite", static_cast<uint32_t>(AEColorType::NavajoWhite));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Navy", static_cast<uint32_t>(AEColorType::Navy));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "OldLace", static_cast<uint32_t>(AEColorType::OldLace));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Olive", static_cast<uint32_t>(AEColorType::Olive));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "OliveDrab", static_cast<uint32_t>(AEColorType::OliveDrab));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Orange", static_cast<uint32_t>(AEColorType::Orange));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "OrangeRed", static_cast<uint32_t>(AEColorType::OrangeRed));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Orchid", static_cast<uint32_t>(AEColorType::Orchid));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PaleGoldenrod", static_cast<uint32_t>(AEColorType::PaleGoldenrod));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PaleGreen", static_cast<uint32_t>(AEColorType::PaleGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PaleTurquoise", static_cast<uint32_t>(AEColorType::PaleTurquoise));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PaleVioletRed", static_cast<uint32_t>(AEColorType::PaleVioletRed));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PapayaWhip", static_cast<uint32_t>(AEColorType::PapayaWhip));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PeachPuff", static_cast<uint32_t>(AEColorType::PeachPuff));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Peru", static_cast<uint32_t>(AEColorType::Peru));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Pink", static_cast<uint32_t>(AEColorType::Pink));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Plum", static_cast<uint32_t>(AEColorType::Plum));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "PowderBlue", static_cast<uint32_t>(AEColorType::PowderBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Purple", static_cast<uint32_t>(AEColorType::Purple));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Red", static_cast<uint32_t>(AEColorType::Red));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "RosyBrown", static_cast<uint32_t>(AEColorType::RosyBrown));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "RoyalBlue", static_cast<uint32_t>(AEColorType::RoyalBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SaddleBrown", static_cast<uint32_t>(AEColorType::SaddleBrown));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Salmon", static_cast<uint32_t>(AEColorType::Salmon));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SandyBrown", static_cast<uint32_t>(AEColorType::SandyBrown));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SeaGreen", static_cast<uint32_t>(AEColorType::SeaGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SeaShell", static_cast<uint32_t>(AEColorType::SeaShell));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Sienna", static_cast<uint32_t>(AEColorType::Sienna));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Silver", static_cast<uint32_t>(AEColorType::Silver));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SkyBlue", static_cast<uint32_t>(AEColorType::SkyBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SlateBlue", static_cast<uint32_t>(AEColorType::SlateBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SlateGray", static_cast<uint32_t>(AEColorType::SlateGray));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Snow", static_cast<uint32_t>(AEColorType::Snow));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SpringGreen", static_cast<uint32_t>(AEColorType::SpringGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "SteelBlue", static_cast<uint32_t>(AEColorType::SteelBlue));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Tan", static_cast<uint32_t>(AEColorType::Tan));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Teal", static_cast<uint32_t>(AEColorType::Teal));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Thistle", static_cast<uint32_t>(AEColorType::Thistle));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Tomato", static_cast<uint32_t>(AEColorType::Tomato));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Transparent", static_cast<uint32_t>(AEColorType::Transparent));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Turquoise", static_cast<uint32_t>(AEColorType::Turquoise));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Violet", static_cast<uint32_t>(AEColorType::Violet));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Wheat", static_cast<uint32_t>(AEColorType::Wheat));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "White", static_cast<uint32_t>(AEColorType::White));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "WhiteSmoke", static_cast<uint32_t>(AEColorType::WhiteSmoke));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "Yellow", static_cast<uint32_t>(AEColorType::Yellow));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    ret = engine->RegisterEnumValue("AEColorType", "YellowGreen", static_cast<uint32_t>(AEColorType::YellowGreen));
    if (ret < 0)
    {
        return AEResult::RegEnumValFail;
    }

    //////////////////////////////////////
    //Register Get Color By Enum
    ret = engine->RegisterGlobalFunction("const Color& GetColorByEnum(AEColorType)", asFUNCTION(AEColors::GetColorByEnum), asCALL_CDECL);
    if (ret < 0)
    {
        return AEResult::RegGlobalFuncFail;
    }

    return AEResult::Ok;
}
