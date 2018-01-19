/** \file 
 * Contains definitions for use of Colors in AEngine 
 */

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

#ifndef _AE_COLOR_DEFS_H
#define _AE_COLOR_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>
#include <Windows.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "glm\glm.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Color.h"
#include "Base\AEObject.h"
#include "Math\AEMathDefs.h"

/************
*   Using   *
*************/

/************
*   Enums   *
*************/
enum class AEColorType : uint32_t
{
    AliceBlue = 0,
    AntiqueWhite,
    Aqua,
    Aquamarine,
    Azure,
    Beige,
    Bisque,
    Black,
    BlanchedAlmond,
    Blue,
    BlueViolet,
    Brown,
    BurlyWood,
    CadetBlue,
    Chartreuse,
    Chocolate,
    Coral,
    CornflowerBlue,
    Cornsilk,
    Crimson,
    Cyan,
    DarkBlue,
    DarkCyan,
    DarkGoldenrod,
    DarkGray,
    DarkGreen,
    DarkKhaki,
    DarkMagenta,
    DarkOliveGreen,
    DarkOrange,
    DarkOrchid,
    DarkRed,
    DarkSalmon,
    DarkSeaGreen,
    DarkSlateBlue,
    DarkSlateGray,
    DarkTurquoise,
    DarkViolet,
    DeepPink,
    DeepSkyBlue,
    DimGray,
    DodgerBlue,
    Firebrick,
    FloralWhite,
    ForestGreen,
    Fuchsia,
    Gainsboro,
    GhostWhite,
    Gold,
    Goldenrod,
    Gray,
    Green,
    GreenYellow,
    Honeydew,
    HotPink,
    IndianRed,
    Indigo,
    Ivory,
    Khaki,
    Lavender,
    LavenderBlush,
    LawnGreen,
    LemonChiffon,
    LightBlue,
    LightCoral,
    LightCyan,
    LightGoldenrodYellow,
    LightGreen,
    LightGray,
    LightPink,
    LightSalmon,
    LightSeaGreen,
    LightSkyBlue,
    LightSlateGray,
    LightSteelBlue,
    LightYellow,
    Lime,
    LimeGreen,
    Linen,
    Magenta,
    Maroon,
    MediumAquamarine,
    MediumBlue,
    MediumOrchid,
    MediumPurple,
    MediumSeaGreen,
    MediumSlateBlue,
    MediumSpringGreen,
    MediumTurquoise,
    MediumVioletRed,
    MidnightBlue,
    MintCream,
    MistyRose,
    Moccasin,
    NavajoWhite,
    Navy,
    OldLace,
    Olive,
    OliveDrab,
    Orange,
    OrangeRed,
    Orchid,
    PaleGoldenrod,
    PaleGreen,
    PaleTurquoise,
    PaleVioletRed,
    PapayaWhip,
    PeachPuff,
    Peru,
    Pink,
    Plum,
    PowderBlue,
    Purple,
    Red,
    RosyBrown,
    RoyalBlue,
    SaddleBrown,
    Salmon,
    SandyBrown,
    SeaGreen,
    SeaShell,
    Sienna,
    Silver,
    SkyBlue,
    SlateBlue,
    SlateGray,
    Snow,
    SpringGreen,
    SteelBlue,
    Tan,
    Teal,
    Thistle,
    Tomato,
    Transparent,
    Turquoise,
    Violet,
    Wheat,
    White,
    WhiteSmoke,
    WorldEngineBackground,
    Yellow,
    YellowGreen,
};

/****************
*   Constants   *
*****************/
namespace AEColors
{
    extern const Color AliceBlue;                   ///< Color that represents Alice Blue
    extern const Color AntiqueWhite;                ///< Color that represents Antique White
    extern const Color Aqua;                        ///< Color that represents Aqua
    extern const Color Aquamarine;                  ///< Color that represents Aquamarine
    extern const Color Azure;                       ///< Color that represents Azure
    extern const Color Beige;                       ///< Color that represents Beige
    extern const Color Bisque;                      ///< Color that represents Bisque
    extern const Color Black;                       ///< Color that represents Black
    extern const Color BlanchedAlmond;              ///< Color that represents Blanched Almond
    extern const Color Blue;                        ///< Color that represents Blue
    extern const Color BlueViolet;                  ///< Color that represents Blue Violet
    extern const Color Brown;                       ///< Color that represents Brown
    extern const Color BurlyWood;                   ///< Color that represents Burly Wood
    extern const Color CadetBlue;                   ///< Color that represents Cadet Blue
    extern const Color Chartreuse;                  ///< Color that represents Chartreuse
    extern const Color Chocolate;                   ///< Color that represents Chocolate
    extern const Color Coral;                       ///< Color that represents Coral
    extern const Color CornflowerBlue;              ///< Color that represents Corn Flower Blue
    extern const Color Cornsilk;                    ///< Color that represents Cornsilk
    extern const Color Crimson;                     ///< Color that represents Crimson
    extern const Color Cyan;                        ///< Color that represents Cyan
    extern const Color DarkBlue;                    ///< Color that represents Dark Blue
    extern const Color DarkCyan;                    ///< Color that represents Dark Cyan
    extern const Color DarkGoldenrod;               ///< Color that represents Dark Goldenrod
    extern const Color DarkGray;                    ///< Color that represents Dark Gray
    extern const Color DarkGreen;                   ///< Color that represents Dark Green
    extern const Color DarkKhaki;                   ///< Color that represents Dark Khaki
    extern const Color DarkMagenta;                 ///< Color that represents Dark Magenta
    extern const Color DarkOliveGreen;              ///< Color that represents Dark Olive Green
    extern const Color DarkOrange;                  ///< Color that represents Dark Orange
    extern const Color DarkOrchid;                  ///< Color that represents Dark Orchid
    extern const Color DarkRed;                     ///< Color that represents Dark Red
    extern const Color DarkSalmon;                  ///< Color that represents Dark Salmon
    extern const Color DarkSeaGreen;                ///< Color that represents Dark Sea Green
    extern const Color DarkSlateBlue;               ///< Color that represents Dark Slate Blue
    extern const Color DarkSlateGray;               ///< Color that represents Dark Slate Gray
    extern const Color DarkTurquoise;               ///< Color that represents Dark Turquoise
    extern const Color DarkViolet;                  ///< Color that represents Dark Violet
    extern const Color DeepPink;                    ///< Color that represents Deep Pink
    extern const Color DeepSkyBlue;                 ///< Color that represents Deep Sky Blue
    extern const Color DimGray;                     ///< Color that represents Dim Gray
    extern const Color DodgerBlue;                  ///< Color that represents Dodger Blue
    extern const Color Firebrick;                   ///< Color that represents Firebrick
    extern const Color FloralWhite;                 ///< Color that represents Floral White
    extern const Color ForestGreen;                 ///< Color that represents Forest Green
    extern const Color Fuchsia;                     ///< Color that represents Fuchsia
    extern const Color Gainsboro;                   ///< Color that represents Gainsboro
    extern const Color GhostWhite;                  ///< Color that represents Ghost White
    extern const Color Gold;                        ///< Color that represents Gold
    extern const Color Goldenrod;                   ///< Color that represents Goldenrod
    extern const Color Gray;                        ///< Color that represents Gray
    extern const Color Green;                       ///< Color that represents Green
    extern const Color GreenYellow;                 ///< Color that represents Green Yellow
    extern const Color Honeydew;                    ///< Color that represents Honeydew
    extern const Color HotPink;                     ///< Color that represents Hot Pink
    extern const Color IndianRed;                   ///< Color that represents Indian Red
    extern const Color Indigo;                      ///< Color that represents Indigo
    extern const Color Ivory;                       ///< Color that represents Ivory
    extern const Color Khaki;                       ///< Color that represents Khaki
    extern const Color Lavender;                    ///< Color that represents Lavender
    extern const Color LavenderBlush;               ///< Color that represents Lavender Blush
    extern const Color LawnGreen;                   ///< Color that represents Lawn Green
    extern const Color LemonChiffon;                ///< Color that represents Lemon Chiffon
    extern const Color LightBlue;                   ///< Color that represents Light Blue
    extern const Color LightCoral;                  ///< Color that represents Light Coral
    extern const Color LightCyan;                   ///< Color that represents Light Cyan
    extern const Color LightGoldenrodYellow;        ///< Color that represents Light Goldenrod Yellow
    extern const Color LightGreen;                  ///< Color that represents Light Green
    extern const Color LightGray;                   ///< Color that represents Light Gray
    extern const Color LightPink;                   ///< Color that represents Light Pink
    extern const Color LightSalmon;                 ///< Color that represents Light Salmon
    extern const Color LightSeaGreen;               ///< Color that represents Light Sea Green
    extern const Color LightSkyBlue;                ///< Color that represents Light Sky Blue
    extern const Color LightSlateGray;              ///< Color that represents Light Slate Gray
    extern const Color LightSteelBlue;              ///< Color that represents Light Steel Blue
    extern const Color LightYellow;                 ///< Color that represents Light Yellow
    extern const Color Lime;                        ///< Color that represents Lime
    extern const Color LimeGreen;                   ///< Color that represents Lime Green
    extern const Color Linen;                       ///< Color that represents Linen
    extern const Color Magenta;                     ///< Color that represents Magenta
    extern const Color Maroon;                      ///< Color that represents Maroon
    extern const Color MediumAquamarine;            ///< Color that represents Medium Aquamarine
    extern const Color MediumBlue;                  ///< Color that represents Medium Blue
    extern const Color MediumOrchid;                ///< Color that represents Medium Orchid
    extern const Color MediumPurple;                ///< Color that represents Medium Purple
    extern const Color MediumSeaGreen;              ///< Color that represents Medium Sea Green
    extern const Color MediumSlateBlue;             ///< Color that represents Medium Slate Blue
    extern const Color MediumSpringGreen;           ///< Color that represents Medium Spring Green
    extern const Color MediumTurquoise;             ///< Color that represents Medium Turquoise
    extern const Color MediumVioletRed;             ///< Color that represents Medium Violet Red
    extern const Color MidnightBlue;                ///< Color that represents Midnight Blue
    extern const Color MintCream;                   ///< Color that represents Mint Cream
    extern const Color MistyRose;                   ///< Color that represents Misty Rose
    extern const Color Moccasin;                    ///< Color that represents Moccasin
    extern const Color NavajoWhite;                 ///< Color that represents Navajo White
    extern const Color Navy;                        ///< Color that represents Navy
    extern const Color OldLace;                     ///< Color that represents Old Lace
    extern const Color Olive;                       ///< Color that represents Olive
    extern const Color OliveDrab;                   ///< Color that represents Olive Drab
    extern const Color Orange;                      ///< Color that represents Orange
    extern const Color OrangeRed;                   ///< Color that represents Orange Red
    extern const Color Orchid;                      ///< Color that represents Orchid
    extern const Color PaleGoldenrod;               ///< Color that represents Pale Goldenrod
    extern const Color PaleGreen;                   ///< Color that represents Pale Green
    extern const Color PaleTurquoise;               ///< Color that represents Pale Turquoise
    extern const Color PaleVioletRed;               ///< Color that represents Pale Violet Red
    extern const Color PapayaWhip;                  ///< Color that represents Papaya Whip
    extern const Color PeachPuff;                   ///< Color that represents Peach Puff
    extern const Color Peru;                        ///< Color that represents Peru
    extern const Color Pink;                        ///< Color that represents Pink
    extern const Color Plum;                        ///< Color that represents Plum
    extern const Color PowderBlue;                  ///< Color that represents Powder Blue
    extern const Color Purple;                      ///< Color that represents Purple
    extern const Color Red;                         ///< Color that represents Red
    extern const Color RosyBrown;                   ///< Color that represents Rosy Brown
    extern const Color RoyalBlue;                   ///< Color that represents Royal Blue
    extern const Color SaddleBrown;                 ///< Color that represents Saddle Brown
    extern const Color Salmon;                      ///< Color that represents Salmon
    extern const Color SandyBrown;                  ///< Color that represents Sandy Brown
    extern const Color SeaGreen;                    ///< Color that represents Sea Green
    extern const Color SeaShell;                    ///< Color that represents Sea Shell
    extern const Color Sienna;                      ///< Color that represents Sienna
    extern const Color Silver;                      ///< Color that represents Silver
    extern const Color SkyBlue;                     ///< Color that represents Sky Blue
    extern const Color SlateBlue;                   ///< Color that represents Slate Blue
    extern const Color SlateGray;                   ///< Color that represents Slate Gray
    extern const Color Snow;                        ///< Color that represents Snow
    extern const Color SpringGreen;                 ///< Color that represents Spring Green
    extern const Color SteelBlue;                   ///< Color that represents Steel Blue
    extern const Color Tan;                         ///< Color that represents Tan
    extern const Color Teal;                        ///< Color that represents Teal
    extern const Color Thistle;                     ///< Color that represents Thistle
    extern const Color Tomato;                      ///< Color that represents Tomato
    extern const Color Transparent;                 ///< Color that represents Transparent
    extern const Color Turquoise;                   ///< Color that represents Turquoise
    extern const Color Violet;                      ///< Color that represents Violet
    extern const Color Wheat;                       ///< Color that represents Wheat
    extern const Color White;                       ///< Color that represents White
    extern const Color WhiteSmoke;                  ///< Color that represents White Smoke
    extern const Color WorldEngineBackground;       ///< Color that represents the World Engine Background
    extern const Color Yellow;                      ///< Color that represents Yellow
    extern const Color YellowGreen;                 ///< Color that represents Yellow Green

    /*****************************************************************************************/

    /// <summary>
    /// Gets a Color by using the AEColorType Enum
    /// </summary>
    /// <param name="colorType">Color Type to get</param>
    /// <returns>Color defined by Enum</returns>
    const Color& GetColorByEnum(AEColorType colorType);
};

/******************
*   Struct Decl   *
*******************/

#endif
