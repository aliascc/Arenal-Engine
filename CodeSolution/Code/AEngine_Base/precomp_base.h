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

#pragma once

/*************************
*   Game Engine Config   *
**************************/
#include "Base\ProjectConfig.h"

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
#include <stack>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <atomic>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>

#include <regex>
#include <random>
#include <limits>
#include <time.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include <Windows.h>
#include <DbgHelp.h>
#include <sys/stat.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

#include "glm\glm.hpp"
#include "glm\gtx\norm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "fmt\format.h"

#include "boost\filesystem.hpp"
#include "boost\lexical_cast.hpp"
#include "boost\algorithm\string.hpp"

#include "tinyxml2.h"

/***************************
*   Game Engine Includes   *
****************************/
