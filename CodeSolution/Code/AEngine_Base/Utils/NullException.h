/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _NULL_EXCEPTION_H
#define _NULL_EXCEPTION_H

/**********************
*   System Includes   *
***********************/
#include <stdexcept>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\Base.h"
#include "Base\AEObject.h"

/********************
*   Forward Decls   *
*********************/
class Keyboard;
class XBoxGamepadManager;

/*****************
*   Class Decl   *
******************/

class NullException sealed : public std::exception
{

	private:
		std::string m_Msg;

	public:

		explicit NullException(const char* message) :
			m_Msg(message)
		{
		}

		explicit NullException(const std::string& message) :
			m_Msg(message)
		{
		}

		virtual ~NullException() throw ()
		{
		}

		const char* what() const throw () override
		{
			return m_Msg.c_str();
		}

};

#endif