/********************************************************
*
* Author: Carlos Chacón N.
*
* Copyright 2012-2015
*
*********************************************************/

#pragma once

#ifndef _BOOST_CUSTOM_FUNCTIONS_H
#define _BOOST_CUSTOM_FUNCTIONS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/
#include "boost/filesystem.hpp" 

/***************************
*   Game Engine Includes   *
****************************/

/************
*   Using   *
*************/

/********************
*   Forward Decls   *
*********************/

/******************
*   Struct Decl   *
*******************/

namespace boost 
{ 
	namespace filesystem
	{
		/// <summary>
		/// Creates a relative file path
		/// <remarks>
		/// This function was created in: http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path/29221546#29221546
		/// </remarks>
		/// <returns>Relative Path</returns>
		path make_relative(path from, path to);
	} 
}

#endif