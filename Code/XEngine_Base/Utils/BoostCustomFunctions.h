/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 4/13/2012
*
* Desc:
*
*
* Copyright 2012
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
		template < >
		path& path::append< typename path::iterator >( typename path::iterator begin, typename path::iterator end, const codecvt_type& cvt)
		{ 
			for( ; begin != end ; ++begin )
				*this /= *begin;
			return *this;
		}

		/// <summary>
		/// Creates a relative file path
		/// <remarks>
		/// This function was created in: https://svn.boost.org/trac/boost/ticket/6249
		/// </remarks>
		/// <returns>Relative Path</returns>
		boost::filesystem::path make_relative( boost::filesystem::path a_From, boost::filesystem::path a_To )
		{
			a_From = boost::filesystem::absolute( a_From ); a_To = boost::filesystem::absolute( a_To );
			boost::filesystem::path ret;
			boost::filesystem::path::const_iterator itrFrom( a_From.begin() ), itrTo( a_To.begin() );
			// Find common base
			for( boost::filesystem::path::const_iterator toEnd( a_To.end() ), fromEnd( a_From.end() ) ; itrFrom != fromEnd && itrTo != toEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo );
			// Navigate backwards in directory to reach previously found base
			for( boost::filesystem::path::const_iterator fromEnd( a_From.end() ); itrFrom != fromEnd; ++itrFrom )
			{
				if( (*itrFrom) != "." )
					ret /= "..";
			}
			// Now navigate down the directory branch
			ret.append( itrTo, a_To.end() );
			return ret;
		}
	} 
}

#endif