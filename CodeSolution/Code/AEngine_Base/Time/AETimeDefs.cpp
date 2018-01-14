/*
* Copyright (c) 2018 <Carlos Chac�n>
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
#include <vector>
#include <sstream>

/*************************
*   3rd Party Includes   *
**************************/
#include "boost\lexical_cast.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "AETimeDefs.h"
#include "Base\BaseFunctions.h"

/********************
*   Function Defs   *
*********************/

/******************
*   TimerParams   *
*******************/
TimerParams::TimerParams()
{
}


/****************
*   TimeStamp   *
*****************/
TimeStamp::TimeStamp()
	: m_Day(0)
	, m_Month(0)
	, m_Year(0)
	, m_Hour(0)
	, m_Minute(0)
	, m_Second(0)
{
}

TimeStamp::TimeStamp(const std::wstring& str)
{
	GetFromString(str);
}

AEResult TimeStamp::GetFromString(const std::wstring& str)
{
	if(str.empty())
	{
		return AEResult::EmptyString;
	}

	std::vector<std::wstring> subStrings;

	AE_Base::SplitString(str, subStrings, L"(-|:)", true);

	if(subStrings.size() != 6)
	{
		return AEResult::Fail;
	}

	m_Year		= boost::lexical_cast<uint32_t>(subStrings[0]);
	m_Month		= boost::lexical_cast<uint32_t>(subStrings[1]);
	m_Day		= boost::lexical_cast<uint32_t>(subStrings[2]);
	m_Hour		= boost::lexical_cast<uint32_t>(subStrings[3]);
	m_Minute	= boost::lexical_cast<uint32_t>(subStrings[4]);
	m_Second	= boost::lexical_cast<uint32_t>(subStrings[5]);

	return AEResult::Ok;
}

TimeStamp& TimeStamp::operator=(const std::wstring& str)
{
	GetFromString(str);

	return *this;
}

bool TimeStamp::operator==(const TimeStamp& other)
{
	return (
			this->m_Day == other.m_Day &&
			this->m_Month == other.m_Month &&
			this->m_Year == other.m_Year &&
			this->m_Hour == other.m_Hour &&
			this->m_Minute == other.m_Minute &&
			this->m_Second == other.m_Second
			);
}
	
bool TimeStamp::operator<(const TimeStamp& other)
{
	if(this->m_Year < other.m_Year)
	{
		return true;
	}
	else if(this->m_Month < other.m_Month)
	{
		return true;
	}
	else if(this->m_Day < other.m_Day)
	{
		return true;
	}
	else if(this->m_Hour < other.m_Hour)
	{
		return true;
	}
	else if(this->m_Minute < other.m_Minute)
	{
		return true;
	}
	else if(this->m_Second < other.m_Second)
	{
		return true;
	}
	else
	{
		return false;
	}
}
	
bool TimeStamp::operator>(const TimeStamp& other)
{
	return !(this->operator<(other));
}
	
bool TimeStamp::operator<=(const TimeStamp& other)
{
	if(this->operator==(other))
	{
		return true;
	}
	else
	{
		return this->operator<(other);
	}
}

bool TimeStamp::operator>=(const TimeStamp& other)
{
	if(this->operator==(other))
	{
		return true;
	}
	else
	{
		return this->operator>(other);
	}
}

std::wstring TimeStamp::ToString() const 
{
	std::wstringstream strDate;

	strDate << m_Year << L"-" << m_Month << L"-" << m_Day << L" " << m_Hour << L":" << m_Minute << L":" << m_Second;

	return strDate.str();
}
