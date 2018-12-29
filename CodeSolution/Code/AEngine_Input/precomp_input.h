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

#ifndef _PRECOMP_INPUT_H
#define _PRECOMP_INPUT_H

/**********************
*   System Includes   *
***********************/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <stdexcept>

#include <map>
#include <set>
#include <list>
#include <array>
#include <queue>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <mutex>
#include <thread>
#include <functional>

#include <regex>
#include <random>
#include <limits>
#include <time.h>

#include <fstream>
#include <sstream>

#include <Windows.h>

#include <Xinput.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

#include "glm\glm.hpp"
#include "glm\gtx\norm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\quaternion.hpp"
#include "glm\gtx\compatibility.hpp"

#include "cppformat\format.h"

#include "boost\filesystem.hpp"
#include "boost\algorithm\string.hpp"

/***************************
*   Game Engine Includes   *
****************************/
#include "Base\ProjectConfig.h"

#include "Base\Base.h"
#include "Base\Named.h"
#include "Base\AEObject.h"
#include "Base\BaseFunctions.h"
#include "Base\BaseLocations.h"

#include "Math\AEMathDefs.h"

#include "Utils\Singleton.h"
#include "Utils\AERandomDefs.h"
#include "Utils\GlobalConfig.h"
#include "Utils\NullException.h"
#include "Utils\GlobalConfigDefs.h"
#include "Utils\BoostCustomFunctions.h"

#include "Color\Color.h"
#include "Color\AEColorDefs.h"

#include "Logger\Logger.h"
#include "Logger\LoggerDefs.h"

#include "Localization\LocalizationManager.h"
#include "Localization\LocalizationManagerDefs.h"

#include "Time\AETime.h"
#include "Time\AETimer.h"
#include "Time\AETimeDefs.h"

#endif
