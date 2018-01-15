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
#include <windows.h>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GlobalConfig.h"
#include "GlobalConfigDefs.h"
#include "Base\BaseFunctions.h"

//Always include last
#include "Memory\MemLeaks.h"

namespace AE_GlobalConfig
{
	static AEResult OpenRegistry(HKEY& hKey)
	{
		LONG res = RegOpenKeyExW(HKEY_CURRENT_USER, AE_GC_EDITOR_REGISTRY_SUBKEY, 0, KEY_READ, &hKey);
		if (res != ERROR_SUCCESS)
		{
			return AEResult::ConfigLoadError;
		}

		return AEResult::Ok;
	}

    static void CloseRegistry(HKEY& hKey)
    {
        if (hKey == nullptr)
        {
            return;
        }

        RegCloseKey(hKey);
    }

	static AEResult ReadStringInRegistry(const std::wstring& attributeName, std::wstring& value)
	{
		HKEY hKey = nullptr;
		if (OpenRegistry(hKey) != AEResult::Ok)
		{
			return AEResult::ConfigLoadError;
		}

		DWORD sizeOfBuffer = 0;

		ULONG errorCode = RegQueryValueExW(hKey, attributeName.c_str(), 0, nullptr, nullptr, &sizeOfBuffer);
		if (errorCode != ERROR_SUCCESS)
		{
            CloseRegistry(hKey);
			return AEResult::ConfigLoadError;
		}

		wchar_t* buffer = new wchar_t[sizeOfBuffer];
		errorCode = RegQueryValueExW(hKey, attributeName.c_str(), 0, nullptr, (LPBYTE)buffer, &sizeOfBuffer);
		if (errorCode != ERROR_SUCCESS)
		{
            CloseRegistry(hKey);
			return AEResult::ConfigLoadError;
		}

		value = std::wstring(buffer);

		DeleteMem(buffer);

        CloseRegistry(hKey);

		return AEResult::Ok;
	}

	static AEResult WriteStringToRegistry(const std::wstring& attributeName, const std::wstring& value)
	{
		DWORD sizeOfBuffer = (DWORD)(value.length() + 1) * sizeof(wchar_t);

		LONG status = RegSetKeyValueW(HKEY_LOCAL_MACHINE, AE_GC_EDITOR_REGISTRY_SUBKEY,
			                          attributeName.c_str(), REG_SZ, value.c_str(), sizeOfBuffer);
		if (status != ERROR_SUCCESS)
		{
			return AEResult::ConfigLoadError;
		}

		return AEResult::Ok;
	}

	AEResult GetRecentProjects(std::vector<std::wstring>& recentProjects)
	{
		std::wstring projects = L"";
		if (ReadStringInRegistry(AE_GC_RECENT_PROJECTS, projects) != AEResult::Ok)
		{
			return AEResult::ConfigLoadError;
		}

		AE_Base::SplitString(projects, recentProjects, L";", true);

		return AEResult::Ok;
	}

	AEResult GetRecentProjectDirectory(std::wstring& recentProjectDir)
	{
		recentProjectDir = L"";
		if (ReadStringInRegistry(AE_GC_RECENT_PROJECT_DIR, recentProjectDir) != AEResult::Ok)
		{
			return AEResult::ConfigLoadError;
		}

		return AEResult::Ok;
	}

	AEResult AddRecentProject(const std::wstring& recentProject)
	{
		std::wstring projects = L"";
		if (ReadStringInRegistry(AE_GC_RECENT_PROJECTS, projects) != AEResult::Ok)
		{
			return AEResult::ConfigLoadError;
		}

		projects = recentProject + L";" + projects;

		if (WriteStringToRegistry(AE_GC_RECENT_PROJECTS, projects) != AEResult::Ok)
		{
			return AEResult::ConfigLoadError;
		}

		return AEResult::Ok;
	}

	AEResult SetRecentProjectDirectory(const std::wstring& recentDir)
	{
		if (WriteStringToRegistry(AE_GC_RECENT_PROJECT_DIR, recentDir) != AEResult::Ok)
		{
			return AEResult::ConfigLoadError;
		}

		return AEResult::Ok;
	}
}
