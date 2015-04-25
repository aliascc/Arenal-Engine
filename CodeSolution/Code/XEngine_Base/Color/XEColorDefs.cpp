/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "XEColorDefs.h"
#include "Math\XEMathDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/****************
*   Constants   *
*****************/
namespace XEColors
{
	const Color AliceBlue				=	Color(0.941176f,	0.972549f,	1.0f,		1.0f);
	const Color AntiqueWhite			=	Color(0.980392f,	0.921569f,	0.843137f,	1.0f);
	const Color Aqua					=	Color(0.0f,			1.0f,		1.0f,		1.0f);
	const Color Aquamarine				=	Color(0.498039f,	1.0f,		0.831373f,	1.0f);
	const Color Azure					=	Color(0.941176f,	1.0f,		1.0f,		1.0f);
	const Color Beige					=	Color(0.960784f,	0.960784f,	0.862745f,	1.0f);
	const Color Bisque					=	Color(1.0f,			0.894118f,	0.768627f,	1.0f);
	const Color Black					=	Color(0.0f,			0.0f,		0.0f,		1.0f);
	const Color BlanchedAlmond			=	Color(1.0f,			0.921569f,	0.803922f,	1.0f);
	const Color Blue					=	Color(0.0f,			0.0f,		1.0f,		1.0f);
	const Color BlueViolet				=	Color(0.541176f,	0.168627f,	0.886275f,	1.0f);
	const Color Brown					=	Color(0.647059f,	0.164706f,	0.164706f,	1.0f);
	const Color BurlyWood				=	Color(0.870588f,	0.721569f,	0.529412f,	1.0f);
	const Color CadetBlue				=	Color(0.372549f,	0.619608f,	0.627451f,	1.0f);
	const Color Chartreuse				=	Color(0.498039f,	1.0f,		0.0f,		1.0f);
	const Color Chocolate				=	Color(0.823529f,	0.411765f,	0.117647f,	1.0f);
	const Color Coral					=	Color(1.0f,			0.498039f,	0.313726f,	1.0f);
	const Color CornflowerBlue			=	Color(0.392157f,	0.584314f,	0.929412f,	1.0f);
	const Color Cornsilk				=	Color(1.0f,			0.972549f,	0.862745f,	1.0f);
	const Color Crimson					=	Color(0.862745f,	0.0784314f,	0.235294f,	1.0f);
	const Color Cyan					=	Color(0.0f,			1.0f,		1.0f,		1.0f);
	const Color DarkBlue				=	Color(0.0f,			0.0f,		0.545098f,	1.0f);
	const Color DarkCyan				=	Color(0.0f,			0.545098f,	0.545098f,	1.0f);
	const Color DarkGoldenrod			=	Color(0.721569f,	0.52549f,	0.0431373f,	1.0f);
	const Color DarkGray				=	Color(0.662745f,	0.662745f,	0.662745f,	1.0f);
	const Color DarkGreen				=	Color(0.0f,			0.392157f,	0.0f,		1.0f);
	const Color DarkKhaki				=	Color(0.741176f,	0.717647f,	0.419608f,	1.0f);
	const Color DarkMagenta				=	Color(0.545098f,	0.0f,		0.545098f,	1.0f);
	const Color DarkOliveGreen			=	Color(0.333333f,	0.419608f,	0.184314f,	1.0f);
	const Color DarkOrange				=	Color(1.0f,			0.54902f,	0.0f,		1.0f);
	const Color DarkOrchid				=	Color(0.6f,			0.196078f,	0.8f,		1.0f);
	const Color DarkRed					=	Color(0.545098f,	0.0f,		0.0f,		1.0f);
	const Color DarkSalmon				=	Color(0.913725f,	0.588235f,	0.478431f,	1.0f);
	const Color DarkSeaGreen			=	Color(0.560784f,	0.737255f,	0.545098f,	1.0f);
	const Color DarkSlateBlue			=	Color(0.282353f,	0.239216f,	0.545098f,	1.0f);
	const Color DarkSlateGray			=	Color(0.184314f,	0.309804f,	0.309804f,	1.0f);
	const Color DarkTurquoise			=	Color(0.0f,			0.807843f,	0.819608f,	1.0f);
	const Color DarkViolet				=	Color(0.580392f,	0.0f,		0.827451f,	1.0f);
	const Color DeepPink				=	Color(1.0f,			0.0784314f,	0.576471f,	1.0f);
	const Color DeepSkyBlue				=	Color(0.0f,			0.74902f,	1.0f,		1.0f);
	const Color DimGray					=	Color(0.411765f,	0.411765f,	0.411765f,	1.0f);
	const Color DodgerBlue				=	Color(0.117647f,	0.564706f,	1.0f,		1.0f);
	const Color Firebrick				=	Color(0.698039f,	0.133333f,	0.133333f,	1.0f);
	const Color FloralWhite				=	Color(1.0f,			0.980392f,	0.941176f,	1.0f);
	const Color ForestGreen				=	Color(0.133333f,	0.545098f,	0.133333f,	1.0f);
	const Color Fuchsia					=	Color(1.0f,			0.0f,		1.0f,		1.0f);
	const Color Gainsboro				=	Color(0.862745f,	0.862745f,	0.862745f,	1.0f);
	const Color GhostWhite				=	Color(0.972549f,	0.972549f,	1.0f,		1.0f);
	const Color Gold					=	Color(1.0f,			0.843137f,	0.0f,		1.0f);
	const Color Goldenrod				=	Color(0.854902f,	0.647059f,	0.12549f,	1.0f);
	const Color Gray					=	Color(0.501961f,	0.501961f,	0.501961f,	1.0f);
	const Color Green					=	Color(0.0f,			0.501961f,	0.0f,		1.0f);
	const Color GreenYellow				=	Color(0.678431f,	1.0f,		0.184314f,	1.0f);
	const Color Honeydew				=	Color(0.941176f,	1.0f,		0.941176f,	1.0f);
	const Color HotPink					=	Color(1.0f,			0.411765f,	0.705882f,	1.0f);
	const Color IndianRed				=	Color(0.803922f,	0.360784f,	0.360784f,	1.0f);
	const Color Indigo					=	Color(0.294118f,	0.0f,		0.509804f,	1.0f);
	const Color Ivory					=	Color(1.0f,			1.0f,		0.941176f,	1.0f);
	const Color Khaki					=	Color(0.941176f,	0.901961f,	0.54902f,	1.0f);
	const Color Lavender				=	Color(0.901961f,	0.901961f,	0.980392f,	1.0f);
	const Color LavenderBlush			=	Color(1.0f,			0.941176f,	0.960784f,	1.0f);
	const Color LawnGreen				=	Color(0.486275f,	0.988235f,	0.0f,		1.0f);
	const Color LemonChiffon			=	Color(1.0f,			0.980392f,	0.803922f,	1.0f);
	const Color LightBlue				=	Color(0.678431f,	0.847059f,	0.901961f,	1.0f);
	const Color LightCoral				=	Color(0.941176f,	0.501961f,	0.501961f,	1.0f);
	const Color LightCyan				=	Color(0.878431f,	1.0f,		1.0f,		1.0f);
	const Color LightGoldenrodYellow	=	Color(0.980392f,	0.980392f,	0.823529f,	1.0f);
	const Color LightGreen				=	Color(0.564706f,	0.933333f,	0.564706f,	1.0f);
	const Color LightGray				=	Color(0.827451f,	0.827451f,	0.827451f,	1.0f);
	const Color LightPink				=	Color(1.0f,			0.713726f,	0.756863f,	1.0f);
	const Color LightSalmon				=	Color(1.0f,			0.627451f,	0.478431f,	1.0f);
	const Color LightSeaGreen			=	Color(0.12549f,		0.698039f,	0.666667f,	1.0f);
	const Color LightSkyBlue			=	Color(0.529412f,	0.807843f,	0.980392f,	1.0f);
	const Color LightSlateGray			=	Color(0.466667f,	0.533333f,	0.6f,		1.0f);
	const Color LightSteelBlue			=	Color(0.690196f,	0.768627f,	0.870588f,	1.0f);
	const Color LightYellow				=	Color(1.0f,			1.0f,		0.878431f,	1.0f);
	const Color Lime					=	Color(0.0f,			1.0f,		0.0f,		1.0f);
	const Color LimeGreen				=	Color(0.196078f,	0.803922f,	0.196078f,	1.0f);
	const Color Linen					=	Color(0.980392f,	0.941176f,	0.901961f,	1.0f);
	const Color Magenta					=	Color(1.0f,			0.0f,		1.0f,		1.0f);
	const Color Maroon					=	Color(0.501961f,	0.0f,		0.0f,		1.0f);
	const Color MediumAquamarine		=	Color(0.4f,			0.803922f,	0.666667f,	1.0f);
	const Color MediumBlue				=	Color(0.0f,			0.0f,		0.803922f,	1.0f);
	const Color MediumOrchid			=	Color(0.729412f,	0.333333f,	0.827451f,	1.0f);
	const Color MediumPurple			=	Color(0.576471f,	0.439216f,	0.858824f,	1.0f);
	const Color MediumSeaGreen			=	Color(0.235294f,	0.701961f,	0.443137f,	1.0f);
	const Color MediumSlateBlue			=	Color(0.482353f,	0.407843f,	0.933333f,	1.0f);
	const Color MediumSpringGreen		=	Color(0.0f,			0.980392f,	0.603922f,	1.0f);
	const Color MediumTurquoise			=	Color(0.282353f,	0.819608f,	0.8f,		1.0f);
	const Color MediumVioletRed			=	Color(0.780392f,	0.0823529f, 0.521569f,	1.0f);
	const Color MidnightBlue			=	Color(0.0980392f,	0.0980392f, 0.439216f,	1.0f);
	const Color MintCream				=	Color(0.960784f,	0.01f,		0.980392f,	1.0f);
	const Color MistyRose				=	Color(1.0f,			0.894118f,	0.882353f,	1.0f);
	const Color Moccasin				=	Color(1.0f,			0.894118f,	0.709804f,	1.0f);
	const Color NavajoWhite				=	Color(1.0f,			0.870588f,	0.678431f,	1.0f);
	const Color Navy					=	Color(0.0f,			0.0f,		0.501961f,	1.0f);
	const Color OldLace					=	Color(0.992157f,	0.960784f,	0.901961f,	1.0f);
	const Color Olive					=	Color(0.501961f,	0.501961f,	0.0f,		1.0f);
	const Color OliveDrab				=	Color(0.419608f,	0.556863f,	0.137255f,	1.0f);
	const Color Orange					=	Color(1.0f,			0.647059f,	0.0f,		1.0f);
	const Color OrangeRed				=	Color(1.0f,			0.270588f,	0.0f,		1.0f);
	const Color Orchid					=	Color(0.854902f,	0.439216f,	0.839216f,	1.0f);
	const Color PaleGoldenrod			=	Color(0.933333f,	0.909804f,	0.666667f,	1.0f);
	const Color PaleGreen				=	Color(0.596078f,	0.984314f,	0.596078f,	1.0f);
	const Color PaleTurquoise			=	Color(0.686275f,	0.933333f,	0.933333f,	1.0f);
	const Color PaleVioletRed			=	Color(0.858824f,	0.439216f,	0.576471f,	1.0f);
	const Color PapayaWhip				=	Color(1.0f,			0.937255f,	0.835294f,	1.0f);
	const Color PeachPuff				=	Color(1.0f,			0.854902f,	0.72549f,	1.0f);
	const Color Peru					=	Color(0.803922f,	0.521569f,	0.247059f,	1.0f);
	const Color Pink					=	Color(1.0f,			0.752941f,	0.796078f,	1.0f);
	const Color Plum					=	Color(0.866667f,	0.627451f,	0.866667f,	1.0f);
	const Color PowderBlue				=	Color(0.690196f,	0.878431f,	0.901961f,	1.0f);
	const Color Purple					=	Color(0.501961f,	0.0f,		0.501961f,	1.0f);
	const Color Red						=	Color(1.0f,			0.0f,		0.0f,		1.0f);
	const Color RosyBrown				=	Color(0.737255f,	0.560784f,	0.560784f,	1.0f);
	const Color RoyalBlue				=	Color(0.254902f,	0.411765f,	0.882353f,	1.0f);
	const Color SaddleBrown				=	Color(0.545098f,	0.270588f,	0.0745098f,	1.0f);
	const Color Salmon					=	Color(0.980392f,	0.501961f,	0.447059f,	1.0f);
	const Color SandyBrown				=	Color(0.956863f,	0.643137f,	0.376471f,	1.0f);
	const Color SeaGreen				=	Color(0.180392f,	0.545098f,	0.341176f,	1.0f);
	const Color SeaShell				=	Color(1.0f,			0.960784f,	0.933333f,	1.0f);
	const Color Sienna					=	Color(0.627451f,	0.321569f,	0.176471f,	1.0f);
	const Color Silver					=	Color(0.752941f,	0.752941f,	0.752941f,	1.0f);
	const Color SkyBlue					=	Color(0.529412f,	0.807843f,	0.921569f,	1.0f);
	const Color SlateBlue				=	Color(0.415686f,	0.352941f,	0.803922f,	1.0f);
	const Color SlateGray				=	Color(0.439216f,	0.501961f,	0.564706f,	1.0f);
	const Color Snow					=	Color(1.0f,			0.980392f,	0.980392f,	1.0f);
	const Color SpringGreen				=	Color(0.0f,			1.0f,		0.498039f,	1.0f);
	const Color SteelBlue				=	Color(0.27451f,		0.509804f,	0.705882f,	1.0f);
	const Color Tan						=	Color(0.823529f,	0.705882f,	0.54902f,	1.0f);
	const Color Teal					=	Color(0.0f,			0.501961f,	0.501961f,	1.0f);
	const Color Thistle					=	Color(0.847059f,	0.74902f,	0.847059f,	1.0f);
	const Color Tomato					=	Color(1.0f,			0.388235f,	0.278431f,	1.0f);
	const Color Transparent				=	Color(0.0f,			0.0f,		0.0f,		0.0f);
	const Color Turquoise				=	Color(0.25098f,		0.878431f,	0.815686f,	1.0f);
	const Color Violet					=	Color(0.933333f,	0.509804f,	0.933333f,	1.0f);
	const Color Wheat					=	Color(0.960784f,	0.870588f,	0.701961f,	1.0f);
	const Color White					=	Color(1.0f,			1.0f,		1.0f,		1.0f);
	const Color WhiteSmoke				=	Color(0.960784f,	0.960784f,	0.960784f,	1.0f);
	const Color WorldEngineBackground	=	Color(0.2745f,		0.2745f,	0.2745f,	1.0f);
	const Color Yellow					=	Color(1.0f,			1.0f,		0.0f,		1.0f);
	const Color YellowGreen				=	Color(0.603922f,	0.803922f,	0.196078f,	1.0f);

	/*****************************************************************************************/

	const Color& GetColorByEnum(XEColorType colorType)
	{
		switch (colorType)
		{
			case XEColorType::AliceBlue:
				return AliceBlue;

			case XEColorType::AntiqueWhite:
				return AntiqueWhite;

			case XEColorType::Aqua:
				return Aqua;

			case XEColorType::Aquamarine:
				return Aquamarine;

			case XEColorType::Azure:
				return Azure;

			case XEColorType::Beige:
				return Beige;

			case XEColorType::Bisque:
				return Bisque;

			case XEColorType::Black:
				return Black;

			case XEColorType::BlanchedAlmond:
				return BlanchedAlmond;

			case XEColorType::Blue:
				return Blue;

			case XEColorType::BlueViolet:
				return BlueViolet;

			case XEColorType::Brown:
				return Brown;

			case XEColorType::BurlyWood:
				return BurlyWood;

			case XEColorType::CadetBlue:
				return CadetBlue;

			case XEColorType::Chartreuse:
				return Chartreuse;

			case XEColorType::Chocolate:
				return Chocolate;

			case XEColorType::Coral:
				return Coral;

			case XEColorType::CornflowerBlue:
				return CornflowerBlue;

			case XEColorType::Cornsilk:
				return Cornsilk;

			case XEColorType::Crimson:
				return Crimson;

			case XEColorType::Cyan:
				return Cyan;

			case XEColorType::DarkBlue:
				return DarkBlue;

			case XEColorType::DarkCyan:
				return DarkCyan;

			case XEColorType::DarkGoldenrod:
				return DarkGoldenrod;

			case XEColorType::DarkGray:
				return DarkGray;

			case XEColorType::DarkGreen:
				return DarkGreen;

			case XEColorType::DarkKhaki:
				return DarkKhaki;

			case XEColorType::DarkMagenta:
				return DarkMagenta;

			case XEColorType::DarkOliveGreen:
				return DarkOliveGreen;

			case XEColorType::DarkOrange:
				return DarkOrange;

			case XEColorType::DarkOrchid:
				return DarkOrchid;

			case XEColorType::DarkRed:
				return DarkRed;

			case XEColorType::DarkSalmon:
				return DarkSalmon;

			case XEColorType::DarkSeaGreen:
				return DarkSeaGreen;

			case XEColorType::DarkSlateBlue:
				return DarkSlateBlue;

			case XEColorType::DarkSlateGray:
				return DarkSlateGray;

			case XEColorType::DarkTurquoise:
				return DarkTurquoise;

			case XEColorType::DarkViolet:
				return DarkViolet;

			case XEColorType::DeepPink:
				return DeepPink;

			case XEColorType::DeepSkyBlue:
				return DeepSkyBlue;

			case XEColorType::DimGray:
				return DimGray;

			case XEColorType::DodgerBlue:
				return DodgerBlue;

			case XEColorType::Firebrick:
				return Firebrick;

			case XEColorType::FloralWhite:
				return FloralWhite;

			case XEColorType::ForestGreen:
				return ForestGreen;

			case XEColorType::Fuchsia:
				return Fuchsia;

			case XEColorType::Gainsboro:
				return Gainsboro;

			case XEColorType::GhostWhite:
				return GhostWhite;

			case XEColorType::Gold:
				return Gold;

			case XEColorType::Goldenrod:
				return Goldenrod;

			case XEColorType::Gray:
				return Gray;

			case XEColorType::Green:
				return Green;

			case XEColorType::GreenYellow:
				return GreenYellow;

			case XEColorType::Honeydew:
				return Honeydew;

			case XEColorType::HotPink:
				return HotPink;

			case XEColorType::IndianRed:
				return IndianRed;

			case XEColorType::Indigo:
				return Indigo;

			case XEColorType::Ivory:
				return Ivory;

			case XEColorType::Khaki:
				return Khaki;

			case XEColorType::Lavender:
				return Lavender;

			case XEColorType::LavenderBlush:
				return LavenderBlush;

			case XEColorType::LawnGreen:
				return LawnGreen;

			case XEColorType::LemonChiffon:
				return LemonChiffon;

			case XEColorType::LightBlue:
				return LightBlue;

			case XEColorType::LightCoral:
				return LightCoral;

			case XEColorType::LightCyan:
				return LightCyan;

			case XEColorType::LightGoldenrodYellow:
				return LightGoldenrodYellow;

			case XEColorType::LightGreen:
				return LightGreen;

			case XEColorType::LightGray:
				return LightGray;

			case XEColorType::LightPink:
				return LightPink;

			case XEColorType::LightSalmon:
				return LightSalmon;

			case XEColorType::LightSeaGreen:
				return LightSeaGreen;

			case XEColorType::LightSkyBlue:
				return LightSkyBlue;

			case XEColorType::LightSlateGray:
				return LightSlateGray;

			case XEColorType::LightSteelBlue:
				return LightSteelBlue;

			case XEColorType::LightYellow:
				return LightYellow;

			case XEColorType::Lime:
				return Lime;

			case XEColorType::LimeGreen:
				return LimeGreen;

			case XEColorType::Linen:
				return Linen;

			case XEColorType::Magenta:
				return Magenta;

			case XEColorType::Maroon:
				return Maroon;

			case XEColorType::MediumAquamarine:
				return MediumAquamarine;

			case XEColorType::MediumBlue:
				return MediumBlue;

			case XEColorType::MediumOrchid:
				return MediumOrchid;

			case XEColorType::MediumPurple:
				return MediumPurple;

			case XEColorType::MediumSeaGreen:
				return MediumSeaGreen;

			case XEColorType::MediumSlateBlue:
				return MediumSlateBlue;

			case XEColorType::MediumSpringGreen:
				return MediumSpringGreen;

			case XEColorType::MediumTurquoise:
				return MediumTurquoise;

			case XEColorType::MediumVioletRed:
				return MediumVioletRed;

			case XEColorType::MidnightBlue:
				return MidnightBlue;

			case XEColorType::MintCream:
				return MintCream;

			case XEColorType::MistyRose:
				return MistyRose;

			case XEColorType::Moccasin:
				return Moccasin;

			case XEColorType::NavajoWhite:
				return NavajoWhite;

			case XEColorType::Navy:
				return Navy;

			case XEColorType::OldLace:
				return OldLace;

			case XEColorType::Olive:
				return Olive;

			case XEColorType::OliveDrab:
				return OliveDrab;

			case XEColorType::Orange:
				return Orange;

			case XEColorType::OrangeRed:
				return OrangeRed;

			case XEColorType::Orchid:
				return Orchid;

			case XEColorType::PaleGoldenrod:
				return PaleGoldenrod;

			case XEColorType::PaleGreen:
				return PaleGreen;

			case XEColorType::PaleTurquoise:
				return PaleTurquoise;

			case XEColorType::PaleVioletRed:
				return PaleVioletRed;

			case XEColorType::PapayaWhip:
				return PapayaWhip;

			case XEColorType::PeachPuff:
				return PeachPuff;

			case XEColorType::Peru:
				return Peru;

			case XEColorType::Pink:
				return Pink;

			case XEColorType::Plum:
				return Plum;

			case XEColorType::PowderBlue:
				return PowderBlue;

			case XEColorType::Purple:
				return Purple;

			case XEColorType::Red:
				return Red;

			case XEColorType::RosyBrown:
				return RosyBrown;

			case XEColorType::RoyalBlue:
				return RoyalBlue;

			case XEColorType::SaddleBrown:
				return SaddleBrown;

			case XEColorType::Salmon:
				return Salmon;

			case XEColorType::SandyBrown:
				return SandyBrown;

			case XEColorType::SeaGreen:
				return SeaGreen;

			case XEColorType::SeaShell:
				return SeaShell;

			case XEColorType::Sienna:
				return Sienna;

			case XEColorType::Silver:
				return Silver;

			case XEColorType::SkyBlue:
				return SkyBlue;

			case XEColorType::SlateBlue:
				return SlateBlue;

			case XEColorType::SlateGray:
				return SlateGray;

			case XEColorType::Snow:
				return Snow;

			case XEColorType::SpringGreen:
				return SpringGreen;

			case XEColorType::SteelBlue:
				return SteelBlue;

			case XEColorType::Tan:
				return Tan;

			case XEColorType::Teal:
				return Teal;

			case XEColorType::Thistle:
				return Thistle;

			case XEColorType::Tomato:
				return Tomato;

			case XEColorType::Transparent:
				return Transparent;

			case XEColorType::Turquoise:
				return Turquoise;

			case XEColorType::Violet:
				return Violet;

			case XEColorType::Wheat:
				return Wheat;

			case XEColorType::White:
				return White;

			case XEColorType::WhiteSmoke:
				return WhiteSmoke;

			case XEColorType::WorldEngineBackground:
				return WorldEngineBackground;

			case XEColorType::Yellow:
				return Yellow;

			case XEColorType::YellowGreen:
				return YellowGreen;
			default:
				return Black;
		}
	}

};

/********************
*   Function Defs   *
*********************/