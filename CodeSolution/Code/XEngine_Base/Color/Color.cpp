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
#include "Color.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
Color::Color()
{
	this->color = 0;
}

Color::Color(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	this->a = glm::clamp<uint8_t>(a, 0, 255);
	this->r = glm::clamp<uint8_t>(r, 0, 255);
	this->g = glm::clamp<uint8_t>(g, 0, 255);
	this->b = glm::clamp<uint8_t>(b, 0, 255);
}

Color::Color(const Color& otherColor)
{
	this->color = otherColor.color;
}

Color::Color(float r, float g, float b, float a)
{
	this->a = glm::clamp<uint8_t>(static_cast<uint8_t>(a * 255.0f), 0, 255);
	this->r = glm::clamp<uint8_t>(static_cast<uint8_t>(r * 255.0f), 0, 255);
	this->g = glm::clamp<uint8_t>(static_cast<uint8_t>(g * 255.0f), 0, 255);
	this->b = glm::clamp<uint8_t>(static_cast<uint8_t>(b * 255.0f), 0, 255);
}

Color::Color(const glm::vec4& rgba)
{
	this->a = glm::clamp<uint8_t>(static_cast<uint8_t>(rgba.a * 255.0f), 0, 255);
	this->r = glm::clamp<uint8_t>(static_cast<uint8_t>(rgba.r * 255.0f), 0, 255);
	this->g = glm::clamp<uint8_t>(static_cast<uint8_t>(rgba.g * 255.0f), 0, 255);
	this->b = glm::clamp<uint8_t>(static_cast<uint8_t>(rgba.b * 255.0f), 0, 255);
}

Color::Color(uint32_t otherColor)
{
	this->color = otherColor;
}

uint32_t Color::GetColorRGBA() const
{
	struct
	{
		union
		{
			struct
			{
				uint8_t r;
				uint8_t g;
				uint8_t b;
				uint8_t a;
			};
			uint32_t color;
		};
	} rgbaFormat;

	rgbaFormat.a = this->a;
	rgbaFormat.r = this->r;
	rgbaFormat.g = this->g;
	rgbaFormat.b = this->b;

	return rgbaFormat.color;
}

glm::vec4 Color::ToVector4f() const
{
	return (glm::vec4)(*this);
}

void Color::FillFloatArray(float* arr) const
{
	XEAssert(arr != nullptr);
	if (arr == nullptr)
	{
		return;
	}

	arr[0] = r / 255.0f;
	arr[1] = g / 255.0f;
	arr[2] = b / 255.0f;
	arr[3] = a / 255.0f;
}

Color::operator glm::vec4() const
{
	glm::vec4 vecColor(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);

	return vecColor;
}

Color::operator glm::ivec4() const
{
	glm::ivec4 vecColor(r, g, b, a);

	return vecColor;
}

Color::operator uint32_t() const
{
	return this->color;
}

Color& Color::operator=(const glm::vec4& vec)
{
	this->a = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.a * 255.0f), 0, 255);
	this->r = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.r * 255.0f), 0, 255);
	this->g = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.g * 255.0f), 0, 255);
	this->b = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.b * 255.0f), 0, 255);

	return *this;
}

Color& Color::operator=(const glm::ivec4& vec)
{
	this->a = glm::clamp<uint8_t>(vec.a, 0, 255);
	this->r = glm::clamp<uint8_t>(vec.r, 0, 255);
	this->g = glm::clamp<uint8_t>(vec.g, 0, 255);
	this->b = glm::clamp<uint8_t>(vec.b, 0, 255);

	return *this;
}

Color& Color::operator=(uint32_t otherColor)
{
	this->color = otherColor;

	return *this;
}

bool Color::operator==(const glm::vec4& vec) const
{
	uint8_t ta = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.a * 255.0f), 0, 255);
	uint8_t tr = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.r * 255.0f), 0, 255);
	uint8_t tg = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.g * 255.0f), 0, 255);
	uint8_t tb = glm::clamp<uint8_t>(static_cast<uint8_t>(vec.b * 255.0f), 0, 255);

	return	(
				a == ta &&
				r == tr &&
				g == tg &&
				b == tb
			);
}

bool Color::operator!=(const glm::vec4& vec) const
{
	return (!(*this == vec));
}

bool Color::operator==(uint32_t otherColor) const
{
	return	(this->color == otherColor);
}

bool Color::operator!=(uint32_t otherColor) const
{
	return (!(*this == otherColor));
}

bool Color::operator == (const Color& otherColor) const
{
	return	(
			this->a == otherColor.a &&
			this->r == otherColor.r &&
			this->g == otherColor.g &&
			this->b == otherColor.b
		);
}

bool Color::operator != (const Color& otherColor) const
{
	return (!(*this == otherColor));
}