/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 9/15/2014
*
* Desc:
*
*
* Copyright 2012-2014
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "ColorAddOnAS.h"
#include "GameASAddOnsDefs.h"
#include "Color\XEColorDefs.h"
#include "Base\BaseFunctions.h"

XETODO("Fix this");
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

XEResult ColorAddOnAS::Register(asIScriptEngine* engine)
{
	XEAssert(engine != nullptr);
	if (engine == nullptr)
	{
		return XEResult::NullParameter;
	}

	if (RegisterColorObject(engine) != XEResult::Ok)
	{
		XETODO("Add Log and return");
		XETODO("Wrong Literal Change");
		std::wstring errMsg = L"";
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_BASIC_CONSTS_ERR_MSG"), __FUNCTIONW__);
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	if (RegisterColorEnum(engine) != XEResult::Ok)
	{
		std::wstring errMsg = L"";
		XETODO("Add Log and return");
		XETODO("Wrong Literal Change");
		//fastformat::fmt(errMsg, m_LocalizationManager->GetLiteral(L"AS_REG_ENUM_ERR_MSG"), __FUNCTIONW__, L"XEResult");
		//m_Logger->AddNewLog(LogLevel::Error, errMsg);

		return XEResult::Fail;
	}

	return XEResult::Ok;
}


XEResult ColorAddOnAS::RegisterColorObject(asIScriptEngine* engine)
{
	int ret = 0;

	/////////////////////
	//Register TimeStamp
	/////////////////////
	ret = engine->RegisterObjectType("Color", sizeof(TimeStamp), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_CDAK);
	if (ret < 0)
	{
		return XEResult::RegObjTypeFail;
	}

	///////////////////////
	//Properties
	ret = engine->RegisterObjectProperty("Color", "uint8  b", asOFFSET(Color, b));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("Color", "uint8  g", asOFFSET(Color, g));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("Color", "uint8  r", asOFFSET(Color, r));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("Color", "uint8  a", asOFFSET(Color, a));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	ret = engine->RegisterObjectProperty("Color", "uint  color", asOFFSET(Color, color));
	if (ret < 0)
	{
		return XEResult::RegObjPropFail;
	}

	///////////////////////
	//Methods
	ret = engine->RegisterObjectMethod("Color", "Color &opAssign(uint)", asMETHODPR(Color, operator=, (uint32_t), Color&), asCALL_THISCALL);
	if (ret < 0)
	{
		return XEResult::RegObjMethodFail;
	}

	//////////////////////////////////////
	//Constructor & Destructor for AS
	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void XEAS_Color_Constructor()", asFUNCTIONPR(Color_Constructor, (Color*), void), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void XEAS_Color_Constructor(const Color& in)", asFUNCTIONPR(Color_Constructor, (const Color&, Color*), void), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void XEAS_Color_Constructor(uint)", asFUNCTIONPR(Color_Constructor, (uint32_t, Color*), void), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_CONSTRUCT, "void XEAS_Color_Constructor(uint, uint, uint, uint)", asFUNCTIONPR(Color_Constructor, (uint32_t, uint32_t, uint32_t, uint32_t, Color*), void), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	ret = engine->RegisterObjectBehaviour("Color", asBEHAVE_DESTRUCT, "void XEAS_Color_Destructor()", asFUNCTION(Color_Destructor), asCALL_CDECL_OBJLAST);
	if (ret < 0)
	{
		return XEResult::RegObjBehaviorFail;
	}

	return XEResult::Ok;
}

XEResult ColorAddOnAS::RegisterColorEnum(asIScriptEngine* engine)
{
	int ret = 0;

	//////////////////////////////////////
	//Color Enum
	ret = engine->RegisterEnum("XEColorType");
	if (ret < 0)
	{
		return XEResult::RegEnumFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "AliceBlue", static_cast<uint32_t>(XEColorType::AliceBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "AntiqueWhite", static_cast<uint32_t>(XEColorType::AntiqueWhite));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Aqua", static_cast<uint32_t>(XEColorType::Aqua));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Aquamarine", static_cast<uint32_t>(XEColorType::Aquamarine));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Azure", static_cast<uint32_t>(XEColorType::Azure));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Beige", static_cast<uint32_t>(XEColorType::Beige));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Bisque", static_cast<uint32_t>(XEColorType::Bisque));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Black", static_cast<uint32_t>(XEColorType::Black));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "BlanchedAlmond", static_cast<uint32_t>(XEColorType::BlanchedAlmond));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Blue", static_cast<uint32_t>(XEColorType::Blue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "BlueViolet", static_cast<uint32_t>(XEColorType::BlueViolet));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Brown", static_cast<uint32_t>(XEColorType::Brown));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "BurlyWood", static_cast<uint32_t>(XEColorType::BurlyWood));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "CadetBlue", static_cast<uint32_t>(XEColorType::CadetBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Chartreuse", static_cast<uint32_t>(XEColorType::Chartreuse));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Chocolate", static_cast<uint32_t>(XEColorType::Chocolate));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Coral", static_cast<uint32_t>(XEColorType::Coral));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "CornflowerBlue", static_cast<uint32_t>(XEColorType::CornflowerBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Cornsilk", static_cast<uint32_t>(XEColorType::Cornsilk));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Crimson", static_cast<uint32_t>(XEColorType::Crimson));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Cyan", static_cast<uint32_t>(XEColorType::Cyan));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkBlue", static_cast<uint32_t>(XEColorType::DarkBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkCyan", static_cast<uint32_t>(XEColorType::DarkCyan));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkGoldenrod", static_cast<uint32_t>(XEColorType::DarkGoldenrod));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkGray", static_cast<uint32_t>(XEColorType::DarkGray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkGreen", static_cast<uint32_t>(XEColorType::DarkGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkKhaki", static_cast<uint32_t>(XEColorType::DarkKhaki));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkMagenta", static_cast<uint32_t>(XEColorType::DarkMagenta));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkOliveGreen", static_cast<uint32_t>(XEColorType::DarkOliveGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkOrange", static_cast<uint32_t>(XEColorType::DarkOrange));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkOrchid", static_cast<uint32_t>(XEColorType::DarkOrchid));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkRed", static_cast<uint32_t>(XEColorType::DarkRed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkSalmon", static_cast<uint32_t>(XEColorType::DarkSalmon));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkSeaGreen", static_cast<uint32_t>(XEColorType::DarkSeaGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkSlateBlue", static_cast<uint32_t>(XEColorType::DarkSlateBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkSlateGray", static_cast<uint32_t>(XEColorType::DarkSlateGray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkTurquoise", static_cast<uint32_t>(XEColorType::DarkTurquoise));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DarkViolet", static_cast<uint32_t>(XEColorType::DarkViolet));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DeepPink", static_cast<uint32_t>(XEColorType::DeepPink));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DeepSkyBlue", static_cast<uint32_t>(XEColorType::DeepSkyBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DimGray", static_cast<uint32_t>(XEColorType::DimGray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "DodgerBlue", static_cast<uint32_t>(XEColorType::DodgerBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Firebrick", static_cast<uint32_t>(XEColorType::Firebrick));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "FloralWhite", static_cast<uint32_t>(XEColorType::FloralWhite));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "ForestGreen", static_cast<uint32_t>(XEColorType::ForestGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Fuchsia", static_cast<uint32_t>(XEColorType::Fuchsia));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Gainsboro", static_cast<uint32_t>(XEColorType::Gainsboro));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "GhostWhite", static_cast<uint32_t>(XEColorType::GhostWhite));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Gold", static_cast<uint32_t>(XEColorType::Gold));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Goldenrod", static_cast<uint32_t>(XEColorType::Goldenrod));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Gray", static_cast<uint32_t>(XEColorType::Gray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Green", static_cast<uint32_t>(XEColorType::Green));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "GreenYellow", static_cast<uint32_t>(XEColorType::GreenYellow));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Honeydew", static_cast<uint32_t>(XEColorType::Honeydew));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "HotPink", static_cast<uint32_t>(XEColorType::HotPink));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "IndianRed", static_cast<uint32_t>(XEColorType::IndianRed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Indigo", static_cast<uint32_t>(XEColorType::Indigo));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Ivory", static_cast<uint32_t>(XEColorType::Ivory));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Khaki", static_cast<uint32_t>(XEColorType::Khaki));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Lavender", static_cast<uint32_t>(XEColorType::Lavender));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LavenderBlush", static_cast<uint32_t>(XEColorType::LavenderBlush));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LawnGreen", static_cast<uint32_t>(XEColorType::LawnGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LemonChiffon", static_cast<uint32_t>(XEColorType::LemonChiffon));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightBlue", static_cast<uint32_t>(XEColorType::LightBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightCoral", static_cast<uint32_t>(XEColorType::LightCoral));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightCyan", static_cast<uint32_t>(XEColorType::LightCyan));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightGoldenrodYellow", static_cast<uint32_t>(XEColorType::LightGoldenrodYellow));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightGreen", static_cast<uint32_t>(XEColorType::LightGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightGray", static_cast<uint32_t>(XEColorType::LightGray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightPink", static_cast<uint32_t>(XEColorType::LightPink));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightSalmon", static_cast<uint32_t>(XEColorType::LightSalmon));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightSeaGreen", static_cast<uint32_t>(XEColorType::LightSeaGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightSkyBlue", static_cast<uint32_t>(XEColorType::LightSkyBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightSlateGray", static_cast<uint32_t>(XEColorType::LightSlateGray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightSteelBlue", static_cast<uint32_t>(XEColorType::LightSteelBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LightYellow", static_cast<uint32_t>(XEColorType::LightYellow));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Lime", static_cast<uint32_t>(XEColorType::Lime));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "LimeGreen", static_cast<uint32_t>(XEColorType::LimeGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Linen", static_cast<uint32_t>(XEColorType::Linen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Magenta", static_cast<uint32_t>(XEColorType::Magenta));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Maroon", static_cast<uint32_t>(XEColorType::Maroon));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumAquamarine", static_cast<uint32_t>(XEColorType::MediumAquamarine));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumBlue", static_cast<uint32_t>(XEColorType::MediumBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumOrchid", static_cast<uint32_t>(XEColorType::MediumOrchid));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumPurple", static_cast<uint32_t>(XEColorType::MediumPurple));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumSeaGreen", static_cast<uint32_t>(XEColorType::MediumSeaGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumSlateBlue", static_cast<uint32_t>(XEColorType::MediumSlateBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumSpringGreen", static_cast<uint32_t>(XEColorType::MediumSpringGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumTurquoise", static_cast<uint32_t>(XEColorType::MediumTurquoise));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MediumVioletRed", static_cast<uint32_t>(XEColorType::MediumVioletRed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MidnightBlue", static_cast<uint32_t>(XEColorType::MidnightBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MintCream", static_cast<uint32_t>(XEColorType::MintCream));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "MistyRose", static_cast<uint32_t>(XEColorType::MistyRose));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Moccasin", static_cast<uint32_t>(XEColorType::Moccasin));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "NavajoWhite", static_cast<uint32_t>(XEColorType::NavajoWhite));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Navy", static_cast<uint32_t>(XEColorType::Navy));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "OldLace", static_cast<uint32_t>(XEColorType::OldLace));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Olive", static_cast<uint32_t>(XEColorType::Olive));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "OliveDrab", static_cast<uint32_t>(XEColorType::OliveDrab));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Orange", static_cast<uint32_t>(XEColorType::Orange));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "OrangeRed", static_cast<uint32_t>(XEColorType::OrangeRed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Orchid", static_cast<uint32_t>(XEColorType::Orchid));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PaleGoldenrod", static_cast<uint32_t>(XEColorType::PaleGoldenrod));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PaleGreen", static_cast<uint32_t>(XEColorType::PaleGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PaleTurquoise", static_cast<uint32_t>(XEColorType::PaleTurquoise));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PaleVioletRed", static_cast<uint32_t>(XEColorType::PaleVioletRed));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PapayaWhip", static_cast<uint32_t>(XEColorType::PapayaWhip));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PeachPuff", static_cast<uint32_t>(XEColorType::PeachPuff));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Peru", static_cast<uint32_t>(XEColorType::Peru));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Pink", static_cast<uint32_t>(XEColorType::Pink));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Plum", static_cast<uint32_t>(XEColorType::Plum));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "PowderBlue", static_cast<uint32_t>(XEColorType::PowderBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Purple", static_cast<uint32_t>(XEColorType::Purple));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Red", static_cast<uint32_t>(XEColorType::Red));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "RosyBrown", static_cast<uint32_t>(XEColorType::RosyBrown));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "RoyalBlue", static_cast<uint32_t>(XEColorType::RoyalBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SaddleBrown", static_cast<uint32_t>(XEColorType::SaddleBrown));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Salmon", static_cast<uint32_t>(XEColorType::Salmon));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SandyBrown", static_cast<uint32_t>(XEColorType::SandyBrown));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SeaGreen", static_cast<uint32_t>(XEColorType::SeaGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SeaShell", static_cast<uint32_t>(XEColorType::SeaShell));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Sienna", static_cast<uint32_t>(XEColorType::Sienna));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Silver", static_cast<uint32_t>(XEColorType::Silver));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SkyBlue", static_cast<uint32_t>(XEColorType::SkyBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SlateBlue", static_cast<uint32_t>(XEColorType::SlateBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SlateGray", static_cast<uint32_t>(XEColorType::SlateGray));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Snow", static_cast<uint32_t>(XEColorType::Snow));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SpringGreen", static_cast<uint32_t>(XEColorType::SpringGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "SteelBlue", static_cast<uint32_t>(XEColorType::SteelBlue));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Tan", static_cast<uint32_t>(XEColorType::Tan));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Teal", static_cast<uint32_t>(XEColorType::Teal));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Thistle", static_cast<uint32_t>(XEColorType::Thistle));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Tomato", static_cast<uint32_t>(XEColorType::Tomato));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Transparent", static_cast<uint32_t>(XEColorType::Transparent));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Turquoise", static_cast<uint32_t>(XEColorType::Turquoise));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Violet", static_cast<uint32_t>(XEColorType::Violet));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Wheat", static_cast<uint32_t>(XEColorType::Wheat));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "White", static_cast<uint32_t>(XEColorType::White));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "WhiteSmoke", static_cast<uint32_t>(XEColorType::WhiteSmoke));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "Yellow", static_cast<uint32_t>(XEColorType::Yellow));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	ret = engine->RegisterEnumValue("XEColorType", "YellowGreen", static_cast<uint32_t>(XEColorType::YellowGreen));
	if (ret < 0)
	{
		return XEResult::RegEnumValFail;
	}

	//////////////////////////////////////
	//Register Get Color By Enum
	ret = engine->RegisterGlobalFunction("const Color& GetColorByEnum(XEColorType)", asFUNCTION(XEColors::GetColorByEnum), asCALL_CDECL);
	if (ret < 0)
	{
		return XEResult::RegGlobalFuncFail;
	}

	return XEResult::Ok;
}