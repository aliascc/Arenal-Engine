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
#include "AEColorDefs.h"
#include "Math\AEMathDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/****************
*   Constants   *
*****************/
namespace AEColors
{
    const Color AliceBlue               =    Color(0.941176f,    0.972549f,     1.0f,       1.0f);
    const Color AntiqueWhite            =    Color(0.980392f,    0.921569f,     0.843137f,  1.0f);
    const Color Aqua                    =    Color(0.0f,         1.0f,          1.0f,       1.0f);
    const Color Aquamarine              =    Color(0.498039f,    1.0f,          0.831373f,  1.0f);
    const Color Azure                   =    Color(0.941176f,    1.0f,          1.0f,       1.0f);
    const Color Beige                   =    Color(0.960784f,    0.960784f,     0.862745f,  1.0f);
    const Color Bisque                  =    Color(1.0f,         0.894118f,     0.768627f,  1.0f);
    const Color Black                   =    Color(0.0f,         0.0f,          0.0f,       1.0f);
    const Color BlanchedAlmond          =    Color(1.0f,         0.921569f,     0.803922f,  1.0f);
    const Color Blue                    =    Color(0.0f,         0.0f,          1.0f,       1.0f);
    const Color BlueViolet              =    Color(0.541176f,    0.168627f,     0.886275f,  1.0f);
    const Color Brown                   =    Color(0.647059f,    0.164706f,     0.164706f,  1.0f);
    const Color BurlyWood               =    Color(0.870588f,    0.721569f,     0.529412f,  1.0f);
    const Color CadetBlue               =    Color(0.372549f,    0.619608f,     0.627451f,  1.0f);
    const Color Chartreuse              =    Color(0.498039f,    1.0f,          0.0f,       1.0f);
    const Color Chocolate               =    Color(0.823529f,    0.411765f,     0.117647f,  1.0f);
    const Color Coral                   =    Color(1.0f,         0.498039f,     0.313726f,  1.0f);
    const Color CornflowerBlue          =    Color(0.392157f,    0.584314f,     0.929412f,  1.0f);
    const Color Cornsilk                =    Color(1.0f,         0.972549f,     0.862745f,  1.0f);
    const Color Crimson                 =    Color(0.862745f,    0.0784314f,    0.235294f,  1.0f);
    const Color Cyan                    =    Color(0.0f,         1.0f,          1.0f,       1.0f);
    const Color DarkBlue                =    Color(0.0f,         0.0f,          0.545098f,  1.0f);
    const Color DarkCyan                =    Color(0.0f,         0.545098f,     0.545098f,  1.0f);
    const Color DarkGoldenrod           =    Color(0.721569f,    0.52549f,      0.0431373f, 1.0f);
    const Color DarkGray                =    Color(0.662745f,    0.662745f,     0.662745f,  1.0f);
    const Color DarkGreen               =    Color(0.0f,         0.392157f,     0.0f,       1.0f);
    const Color DarkKhaki               =    Color(0.741176f,    0.717647f,     0.419608f,  1.0f);
    const Color DarkMagenta             =    Color(0.545098f,    0.0f,          0.545098f,  1.0f);
    const Color DarkOliveGreen          =    Color(0.333333f,    0.419608f,     0.184314f,  1.0f);
    const Color DarkOrange              =    Color(1.0f,         0.54902f,      0.0f,       1.0f);
    const Color DarkOrchid              =    Color(0.6f,         0.196078f,     0.8f,       1.0f);
    const Color DarkRed                 =    Color(0.545098f,    0.0f,          0.0f,       1.0f);
    const Color DarkSalmon              =    Color(0.913725f,    0.588235f,     0.478431f,  1.0f);
    const Color DarkSeaGreen            =    Color(0.560784f,    0.737255f,     0.545098f,  1.0f);
    const Color DarkSlateBlue           =    Color(0.282353f,    0.239216f,     0.545098f,  1.0f);
    const Color DarkSlateGray           =    Color(0.184314f,    0.309804f,     0.309804f,  1.0f);
    const Color DarkTurquoise           =    Color(0.0f,         0.807843f,     0.819608f,  1.0f);
    const Color DarkViolet              =    Color(0.580392f,    0.0f,          0.827451f,  1.0f);
    const Color DeepPink                =    Color(1.0f,         0.0784314f,    0.576471f,  1.0f);
    const Color DeepSkyBlue             =    Color(0.0f,         0.74902f,      1.0f,       1.0f);
    const Color DimGray                 =    Color(0.411765f,    0.411765f,     0.411765f,  1.0f);
    const Color DodgerBlue              =    Color(0.117647f,    0.564706f,     1.0f,       1.0f);
    const Color Firebrick               =    Color(0.698039f,    0.133333f,     0.133333f,  1.0f);
    const Color FloralWhite             =    Color(1.0f,         0.980392f,     0.941176f,  1.0f);
    const Color ForestGreen             =    Color(0.133333f,    0.545098f,     0.133333f,  1.0f);
    const Color Fuchsia                 =    Color(1.0f,         0.0f,          1.0f,       1.0f);
    const Color Gainsboro               =    Color(0.862745f,    0.862745f,     0.862745f,  1.0f);
    const Color GhostWhite              =    Color(0.972549f,    0.972549f,     1.0f,       1.0f);
    const Color Gold                    =    Color(1.0f,         0.843137f,     0.0f,       1.0f);
    const Color Goldenrod               =    Color(0.854902f,    0.647059f,     0.12549f,   1.0f);
    const Color Gray                    =    Color(0.501961f,    0.501961f,     0.501961f,  1.0f);
    const Color Green                   =    Color(0.0f,         0.501961f,     0.0f,       1.0f);
    const Color GreenYellow             =    Color(0.678431f,    1.0f,          0.184314f,  1.0f);
    const Color Honeydew                =    Color(0.941176f,    1.0f,          0.941176f,  1.0f);
    const Color HotPink                 =    Color(1.0f,         0.411765f,     0.705882f,  1.0f);
    const Color IndianRed               =    Color(0.803922f,    0.360784f,     0.360784f,  1.0f);
    const Color Indigo                  =    Color(0.294118f,    0.0f,          0.509804f,  1.0f);
    const Color Ivory                   =    Color(1.0f,         1.0f,          0.941176f,  1.0f);
    const Color Khaki                   =    Color(0.941176f,    0.901961f,     0.54902f,   1.0f);
    const Color Lavender                =    Color(0.901961f,    0.901961f,     0.980392f,  1.0f);
    const Color LavenderBlush           =    Color(1.0f,         0.941176f,     0.960784f,  1.0f);
    const Color LawnGreen               =    Color(0.486275f,    0.988235f,     0.0f,       1.0f);
    const Color LemonChiffon            =    Color(1.0f,         0.980392f,     0.803922f,  1.0f);
    const Color LightBlue               =    Color(0.678431f,    0.847059f,     0.901961f,  1.0f);
    const Color LightCoral              =    Color(0.941176f,    0.501961f,     0.501961f,  1.0f);
    const Color LightCyan               =    Color(0.878431f,    1.0f,          1.0f,       1.0f);
    const Color LightGoldenrodYellow    =    Color(0.980392f,    0.980392f,     0.823529f,  1.0f);
    const Color LightGreen              =    Color(0.564706f,    0.933333f,     0.564706f,  1.0f);
    const Color LightGray               =    Color(0.827451f,    0.827451f,     0.827451f,  1.0f);
    const Color LightPink               =    Color(1.0f,         0.713726f,     0.756863f,  1.0f);
    const Color LightSalmon             =    Color(1.0f,         0.627451f,     0.478431f,  1.0f);
    const Color LightSeaGreen           =    Color(0.12549f,     0.698039f,     0.666667f,  1.0f);
    const Color LightSkyBlue            =    Color(0.529412f,    0.807843f,     0.980392f,  1.0f);
    const Color LightSlateGray          =    Color(0.466667f,    0.533333f,     0.6f,       1.0f);
    const Color LightSteelBlue          =    Color(0.690196f,    0.768627f,     0.870588f,  1.0f);
    const Color LightYellow             =    Color(1.0f,         1.0f,          0.878431f,  1.0f);
    const Color Lime                    =    Color(0.0f,         1.0f,          0.0f,       1.0f);
    const Color LimeGreen               =    Color(0.196078f,    0.803922f,     0.196078f,  1.0f);
    const Color Linen                   =    Color(0.980392f,    0.941176f,     0.901961f,  1.0f);
    const Color Magenta                 =    Color(1.0f,         0.0f,          1.0f,       1.0f);
    const Color Maroon                  =    Color(0.501961f,    0.0f,          0.0f,       1.0f);
    const Color MediumAquamarine        =    Color(0.4f,         0.803922f,     0.666667f,  1.0f);
    const Color MediumBlue              =    Color(0.0f,         0.0f,          0.803922f,  1.0f);
    const Color MediumOrchid            =    Color(0.729412f,    0.333333f,     0.827451f,  1.0f);
    const Color MediumPurple            =    Color(0.576471f,    0.439216f,     0.858824f,  1.0f);
    const Color MediumSeaGreen          =    Color(0.235294f,    0.701961f,     0.443137f,  1.0f);
    const Color MediumSlateBlue         =    Color(0.482353f,    0.407843f,     0.933333f,  1.0f);
    const Color MediumSpringGreen       =    Color(0.0f,         0.980392f,     0.603922f,  1.0f);
    const Color MediumTurquoise         =    Color(0.282353f,    0.819608f,     0.8f,       1.0f);
    const Color MediumVioletRed         =    Color(0.780392f,    0.0823529f,    0.521569f,  1.0f);
    const Color MidnightBlue            =    Color(0.0980392f,   0.0980392f,    0.439216f,  1.0f);
    const Color MintCream               =    Color(0.960784f,    0.01f,         0.980392f,  1.0f);
    const Color MistyRose               =    Color(1.0f,         0.894118f,     0.882353f,  1.0f);
    const Color Moccasin                =    Color(1.0f,         0.894118f,     0.709804f,  1.0f);
    const Color NavajoWhite             =    Color(1.0f,         0.870588f,     0.678431f,  1.0f);
    const Color Navy                    =    Color(0.0f,         0.0f,          0.501961f,  1.0f);
    const Color OldLace                 =    Color(0.992157f,    0.960784f,     0.901961f,  1.0f);
    const Color Olive                   =    Color(0.501961f,    0.501961f,     0.0f,       1.0f);
    const Color OliveDrab               =    Color(0.419608f,    0.556863f,     0.137255f,  1.0f);
    const Color Orange                  =    Color(1.0f,         0.647059f,     0.0f,       1.0f);
    const Color OrangeRed               =    Color(1.0f,         0.270588f,     0.0f,       1.0f);
    const Color Orchid                  =    Color(0.854902f,    0.439216f,     0.839216f,  1.0f);
    const Color PaleGoldenrod           =    Color(0.933333f,    0.909804f,     0.666667f,  1.0f);
    const Color PaleGreen               =    Color(0.596078f,    0.984314f,     0.596078f,  1.0f);
    const Color PaleTurquoise           =    Color(0.686275f,    0.933333f,     0.933333f,  1.0f);
    const Color PaleVioletRed           =    Color(0.858824f,    0.439216f,     0.576471f,  1.0f);
    const Color PapayaWhip              =    Color(1.0f,         0.937255f,     0.835294f,  1.0f);
    const Color PeachPuff               =    Color(1.0f,         0.854902f,     0.72549f,   1.0f);
    const Color Peru                    =    Color(0.803922f,    0.521569f,     0.247059f,  1.0f);
    const Color Pink                    =    Color(1.0f,         0.752941f,     0.796078f,  1.0f);
    const Color Plum                    =    Color(0.866667f,    0.627451f,     0.866667f,  1.0f);
    const Color PowderBlue              =    Color(0.690196f,    0.878431f,     0.901961f,  1.0f);
    const Color Purple                  =    Color(0.501961f,    0.0f,          0.501961f,  1.0f);
    const Color Red                     =    Color(1.0f,         0.0f,          0.0f,       1.0f);
    const Color RosyBrown               =    Color(0.737255f,    0.560784f,     0.560784f,  1.0f);
    const Color RoyalBlue               =    Color(0.254902f,    0.411765f,     0.882353f,  1.0f);
    const Color SaddleBrown             =    Color(0.545098f,    0.270588f,     0.0745098f, 1.0f);
    const Color Salmon                  =    Color(0.980392f,    0.501961f,     0.447059f,  1.0f);
    const Color SandyBrown              =    Color(0.956863f,    0.643137f,     0.376471f,  1.0f);
    const Color SeaGreen                =    Color(0.180392f,    0.545098f,     0.341176f,  1.0f);
    const Color SeaShell                =    Color(1.0f,         0.960784f,     0.933333f,  1.0f);
    const Color Sienna                  =    Color(0.627451f,    0.321569f,     0.176471f,  1.0f);
    const Color Silver                  =    Color(0.752941f,    0.752941f,     0.752941f,  1.0f);
    const Color SkyBlue                 =    Color(0.529412f,    0.807843f,     0.921569f,  1.0f);
    const Color SlateBlue               =    Color(0.415686f,    0.352941f,     0.803922f,  1.0f);
    const Color SlateGray               =    Color(0.439216f,    0.501961f,     0.564706f,  1.0f);
    const Color Snow                    =    Color(1.0f,         0.980392f,     0.980392f,  1.0f);
    const Color SpringGreen             =    Color(0.0f,         1.0f,          0.498039f,  1.0f);
    const Color SteelBlue               =    Color(0.27451f,     0.509804f,     0.705882f,  1.0f);
    const Color Tan                     =    Color(0.823529f,    0.705882f,     0.54902f,   1.0f);
    const Color Teal                    =    Color(0.0f,         0.501961f,     0.501961f,  1.0f);
    const Color Thistle                 =    Color(0.847059f,    0.74902f,      0.847059f,  1.0f);
    const Color Tomato                  =    Color(1.0f,         0.388235f,     0.278431f,  1.0f);
    const Color Transparent             =    Color(0.0f,         0.0f,          0.0f,       0.0f);
    const Color Turquoise               =    Color(0.25098f,     0.878431f,     0.815686f,  1.0f);
    const Color Violet                  =    Color(0.933333f,    0.509804f,     0.933333f,  1.0f);
    const Color Wheat                   =    Color(0.960784f,    0.870588f,     0.701961f,  1.0f);
    const Color White                   =    Color(1.0f,         1.0f,          1.0f,       1.0f);
    const Color WhiteSmoke              =    Color(0.960784f,    0.960784f,     0.960784f,  1.0f);
    const Color WorldEngineBackground   =    Color(0.2745f,      0.2745f,       0.2745f,    1.0f);
    const Color Yellow                  =    Color(1.0f,         1.0f,          0.0f,       1.0f);
    const Color YellowGreen             =    Color(0.603922f,    0.803922f,     0.196078f,  1.0f);

    /*****************************************************************************************/

    const Color& GetColorByEnum(AEColorType colorType)
    {
        switch (colorType)
        {
            case AEColorType::AliceBlue:
                return AliceBlue;

            case AEColorType::AntiqueWhite:
                return AntiqueWhite;

            case AEColorType::Aqua:
                return Aqua;

            case AEColorType::Aquamarine:
                return Aquamarine;

            case AEColorType::Azure:
                return Azure;

            case AEColorType::Beige:
                return Beige;

            case AEColorType::Bisque:
                return Bisque;

            case AEColorType::Black:
                return Black;

            case AEColorType::BlanchedAlmond:
                return BlanchedAlmond;

            case AEColorType::Blue:
                return Blue;

            case AEColorType::BlueViolet:
                return BlueViolet;

            case AEColorType::Brown:
                return Brown;

            case AEColorType::BurlyWood:
                return BurlyWood;

            case AEColorType::CadetBlue:
                return CadetBlue;

            case AEColorType::Chartreuse:
                return Chartreuse;

            case AEColorType::Chocolate:
                return Chocolate;

            case AEColorType::Coral:
                return Coral;

            case AEColorType::CornflowerBlue:
                return CornflowerBlue;

            case AEColorType::Cornsilk:
                return Cornsilk;

            case AEColorType::Crimson:
                return Crimson;

            case AEColorType::Cyan:
                return Cyan;

            case AEColorType::DarkBlue:
                return DarkBlue;

            case AEColorType::DarkCyan:
                return DarkCyan;

            case AEColorType::DarkGoldenrod:
                return DarkGoldenrod;

            case AEColorType::DarkGray:
                return DarkGray;

            case AEColorType::DarkGreen:
                return DarkGreen;

            case AEColorType::DarkKhaki:
                return DarkKhaki;

            case AEColorType::DarkMagenta:
                return DarkMagenta;

            case AEColorType::DarkOliveGreen:
                return DarkOliveGreen;

            case AEColorType::DarkOrange:
                return DarkOrange;

            case AEColorType::DarkOrchid:
                return DarkOrchid;

            case AEColorType::DarkRed:
                return DarkRed;

            case AEColorType::DarkSalmon:
                return DarkSalmon;

            case AEColorType::DarkSeaGreen:
                return DarkSeaGreen;

            case AEColorType::DarkSlateBlue:
                return DarkSlateBlue;

            case AEColorType::DarkSlateGray:
                return DarkSlateGray;

            case AEColorType::DarkTurquoise:
                return DarkTurquoise;

            case AEColorType::DarkViolet:
                return DarkViolet;

            case AEColorType::DeepPink:
                return DeepPink;

            case AEColorType::DeepSkyBlue:
                return DeepSkyBlue;

            case AEColorType::DimGray:
                return DimGray;

            case AEColorType::DodgerBlue:
                return DodgerBlue;

            case AEColorType::Firebrick:
                return Firebrick;

            case AEColorType::FloralWhite:
                return FloralWhite;

            case AEColorType::ForestGreen:
                return ForestGreen;

            case AEColorType::Fuchsia:
                return Fuchsia;

            case AEColorType::Gainsboro:
                return Gainsboro;

            case AEColorType::GhostWhite:
                return GhostWhite;

            case AEColorType::Gold:
                return Gold;

            case AEColorType::Goldenrod:
                return Goldenrod;

            case AEColorType::Gray:
                return Gray;

            case AEColorType::Green:
                return Green;

            case AEColorType::GreenYellow:
                return GreenYellow;

            case AEColorType::Honeydew:
                return Honeydew;

            case AEColorType::HotPink:
                return HotPink;

            case AEColorType::IndianRed:
                return IndianRed;

            case AEColorType::Indigo:
                return Indigo;

            case AEColorType::Ivory:
                return Ivory;

            case AEColorType::Khaki:
                return Khaki;

            case AEColorType::Lavender:
                return Lavender;

            case AEColorType::LavenderBlush:
                return LavenderBlush;

            case AEColorType::LawnGreen:
                return LawnGreen;

            case AEColorType::LemonChiffon:
                return LemonChiffon;

            case AEColorType::LightBlue:
                return LightBlue;

            case AEColorType::LightCoral:
                return LightCoral;

            case AEColorType::LightCyan:
                return LightCyan;

            case AEColorType::LightGoldenrodYellow:
                return LightGoldenrodYellow;

            case AEColorType::LightGreen:
                return LightGreen;

            case AEColorType::LightGray:
                return LightGray;

            case AEColorType::LightPink:
                return LightPink;

            case AEColorType::LightSalmon:
                return LightSalmon;

            case AEColorType::LightSeaGreen:
                return LightSeaGreen;

            case AEColorType::LightSkyBlue:
                return LightSkyBlue;

            case AEColorType::LightSlateGray:
                return LightSlateGray;

            case AEColorType::LightSteelBlue:
                return LightSteelBlue;

            case AEColorType::LightYellow:
                return LightYellow;

            case AEColorType::Lime:
                return Lime;

            case AEColorType::LimeGreen:
                return LimeGreen;

            case AEColorType::Linen:
                return Linen;

            case AEColorType::Magenta:
                return Magenta;

            case AEColorType::Maroon:
                return Maroon;

            case AEColorType::MediumAquamarine:
                return MediumAquamarine;

            case AEColorType::MediumBlue:
                return MediumBlue;

            case AEColorType::MediumOrchid:
                return MediumOrchid;

            case AEColorType::MediumPurple:
                return MediumPurple;

            case AEColorType::MediumSeaGreen:
                return MediumSeaGreen;

            case AEColorType::MediumSlateBlue:
                return MediumSlateBlue;

            case AEColorType::MediumSpringGreen:
                return MediumSpringGreen;

            case AEColorType::MediumTurquoise:
                return MediumTurquoise;

            case AEColorType::MediumVioletRed:
                return MediumVioletRed;

            case AEColorType::MidnightBlue:
                return MidnightBlue;

            case AEColorType::MintCream:
                return MintCream;

            case AEColorType::MistyRose:
                return MistyRose;

            case AEColorType::Moccasin:
                return Moccasin;

            case AEColorType::NavajoWhite:
                return NavajoWhite;

            case AEColorType::Navy:
                return Navy;

            case AEColorType::OldLace:
                return OldLace;

            case AEColorType::Olive:
                return Olive;

            case AEColorType::OliveDrab:
                return OliveDrab;

            case AEColorType::Orange:
                return Orange;

            case AEColorType::OrangeRed:
                return OrangeRed;

            case AEColorType::Orchid:
                return Orchid;

            case AEColorType::PaleGoldenrod:
                return PaleGoldenrod;

            case AEColorType::PaleGreen:
                return PaleGreen;

            case AEColorType::PaleTurquoise:
                return PaleTurquoise;

            case AEColorType::PaleVioletRed:
                return PaleVioletRed;

            case AEColorType::PapayaWhip:
                return PapayaWhip;

            case AEColorType::PeachPuff:
                return PeachPuff;

            case AEColorType::Peru:
                return Peru;

            case AEColorType::Pink:
                return Pink;

            case AEColorType::Plum:
                return Plum;

            case AEColorType::PowderBlue:
                return PowderBlue;

            case AEColorType::Purple:
                return Purple;

            case AEColorType::Red:
                return Red;

            case AEColorType::RosyBrown:
                return RosyBrown;

            case AEColorType::RoyalBlue:
                return RoyalBlue;

            case AEColorType::SaddleBrown:
                return SaddleBrown;

            case AEColorType::Salmon:
                return Salmon;

            case AEColorType::SandyBrown:
                return SandyBrown;

            case AEColorType::SeaGreen:
                return SeaGreen;

            case AEColorType::SeaShell:
                return SeaShell;

            case AEColorType::Sienna:
                return Sienna;

            case AEColorType::Silver:
                return Silver;

            case AEColorType::SkyBlue:
                return SkyBlue;

            case AEColorType::SlateBlue:
                return SlateBlue;

            case AEColorType::SlateGray:
                return SlateGray;

            case AEColorType::Snow:
                return Snow;

            case AEColorType::SpringGreen:
                return SpringGreen;

            case AEColorType::SteelBlue:
                return SteelBlue;

            case AEColorType::Tan:
                return Tan;

            case AEColorType::Teal:
                return Teal;

            case AEColorType::Thistle:
                return Thistle;

            case AEColorType::Tomato:
                return Tomato;

            case AEColorType::Transparent:
                return Transparent;

            case AEColorType::Turquoise:
                return Turquoise;

            case AEColorType::Violet:
                return Violet;

            case AEColorType::Wheat:
                return Wheat;

            case AEColorType::White:
                return White;

            case AEColorType::WhiteSmoke:
                return WhiteSmoke;

            case AEColorType::WorldEngineBackground:
                return WorldEngineBackground;

            case AEColorType::Yellow:
                return Yellow;

            case AEColorType::YellowGreen:
                return YellowGreen;
            default:
                return Black;
        }
    }

};

/********************
*   Function Defs   *
*********************/
