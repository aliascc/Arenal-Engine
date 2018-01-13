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
#include "cppformat\format.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Logger\Logger.h"
#include "ErrorCallbackPhysX.h"
#include "Base\BaseFunctions.h"
#include "Localization\LocalizationManager.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/

ErrorCallbackPhysX::ErrorCallbackPhysX()
{
}

ErrorCallbackPhysX::~ErrorCallbackPhysX()
{
}

void ErrorCallbackPhysX::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
	LogLevel logLVL = LogLevel::Info;

	switch (code)
	{
		case physx::PxErrorCode::eNO_ERROR:
			logLVL = LogLevel::Info;
			break;

		case physx::PxErrorCode::eDEBUG_INFO:
			logLVL = LogLevel::Debug;
			break;

		case physx::PxErrorCode::eDEBUG_WARNING:
			logLVL = LogLevel::Debug;
			break;

		case physx::PxErrorCode::eINVALID_PARAMETER:
			logLVL = LogLevel::Error;
			break;

		case physx::PxErrorCode::eINVALID_OPERATION:
			logLVL = LogLevel::Error;
			break;

		case physx::PxErrorCode::eOUT_OF_MEMORY:
			logLVL = LogLevel::Error;
			break;

		case physx::PxErrorCode::eINTERNAL_ERROR:
			logLVL = LogLevel::Error;
			break;

		case physx::PxErrorCode::eABORT:
			logLVL = LogLevel::Error;
			break;

		case physx::PxErrorCode::ePERF_WARNING:
			logLVL = LogLevel::Warning;
			break;

		case physx::PxErrorCode::eMASK_ALL:
			logLVL = LogLevel::Debug;
			break;

		default:
			logLVL = LogLevel::Debug;
			break;
	}

	std::wstring wMessage		= AE_Base::String2WideStr(message);

	std::wstring errMsg = fmt::format(AELOCMAN->GetLiteral(L"PHYSICS_ENGINE_CALLBACK_ERR_MSG"), wMessage);
	AELOGGER->AddNewLog(logLVL, errMsg);
}