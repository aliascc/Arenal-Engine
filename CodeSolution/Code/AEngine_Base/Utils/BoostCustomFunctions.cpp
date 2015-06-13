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
#include "BoostCustomFunctions.h"

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
		path make_relative(boost::filesystem::path from, path to)
		{
			// Start at the root path and while they are the same then do nothing then when they first
			// diverge take the remainder of the two path and replace the entire from path with ".."
			// segments.
			path::const_iterator fromIter = from.begin();
			path::const_iterator toIter = to.begin();

			// Loop through both
			while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
			{
				++toIter;
				++fromIter;
			}

			path finalPath;
			while (fromIter != from.end())
			{
				finalPath /= "..";
				++fromIter;
			}

			while (toIter != to.end())
			{
				finalPath /= *toIter;
				++toIter;
			}

			return finalPath;
		}
	} 
}
