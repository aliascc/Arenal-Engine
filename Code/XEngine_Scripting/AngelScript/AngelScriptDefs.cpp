/********************************************************
*
* Author: Carlos Chacón N.
*
* Created: 12/9/2012
*
* Desc:
*
*
* Copyright 2012
*
*********************************************************/

/**********************
*   System Includes   *
***********************/
#include <new>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Color\Color.h"
#include "AngelScriptDefs.h"
#include "Time\XETimeDefs.h"
#include "Logger\LoggerDefs.h"

/********************
*   Function Defs   *
*********************/

/***************************
*   XEAngelScriptHelpers   *
****************************/
namespace XEAngelScriptHelpers
{
	uint32_t GetSizeOfType(asETypeIdFlags typeID)
	{
		switch (typeID)
		{
			case asTYPEID_BOOL:
				return sizeof(bool);

			case asTYPEID_INT8:
				return sizeof(int8_t);

			case asTYPEID_INT16:
				return sizeof(int16_t);

			case asTYPEID_INT32:
				return sizeof(int32_t);

			case asTYPEID_INT64:
				return sizeof(int64_t);

			case asTYPEID_UINT8:
				return sizeof(uint8_t);

			case asTYPEID_UINT16:
				return sizeof(uint16_t);

			case asTYPEID_UINT32:
				return sizeof(uint32_t);

			case asTYPEID_UINT64:
				return sizeof(uint64_t);

			case asTYPEID_FLOAT:
				return sizeof(float);

			case asTYPEID_DOUBLE:
				return sizeof(double);

			default:
				return 0;
		}
	}

	bool TypeIsBasic(asETypeIdFlags typeID)
	{
		switch (typeID)
		{
			case asTYPEID_BOOL:
			case asTYPEID_INT8:
			case asTYPEID_INT16:
			case asTYPEID_INT32:
			case asTYPEID_INT64:
			case asTYPEID_UINT8:
			case asTYPEID_UINT16:
			case asTYPEID_UINT32:
			case asTYPEID_UINT64:
			case asTYPEID_FLOAT:
			case asTYPEID_DOUBLE:
				return true;

			default:
				return false;
		}
	}
}