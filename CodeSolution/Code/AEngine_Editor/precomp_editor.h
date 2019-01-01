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

#ifndef _PRECOMP_EDITOR_H
#define _PRECOMP_EDITOR_H

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
#include <io.h>
#include <fcntl.h>

#include <Xinput.h>

#include <d3d11_1.h>
#include <D3D11Shader.h>

#if defined(AE_GRAPHIC_DEBUG_DEVICE)
#include <Initguid.h> 
#include <dxgidebug.h>
#include <d3dcommon.h>
#endif

/*************************
*   3rd Party Includes   *
**************************/
#include "angelscript.h"
#include "AngelScript-Add-Ons\scriptarray\scriptarray.h"

#include "glm\glm.hpp"
#include "glm\gtx\norm.hpp"
#include "glm\gtx\transform.hpp"
#include "glm\gtc\quaternion.hpp"

#include "cppformat\format.h"

#include "boost\filesystem.hpp"

#include "libxml\parser.h"
#include "libxml\xmlwriter.h"

#include "SFML\Audio.hpp"

#include "PxPhysicsAPI.h"

#include "DirectXTex\DirectXTex.h"

#include "DirectXTK\SpriteFont.h"
#include "DirectXTK\SpriteBatch.h"

/***************************
*   Game Engine Includes   *
****************************/
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

#include "XML\AEXMLDefs.h"
#include "XML\AEXMLParser.h"
#include "XML\AEXMLWriter.h"

#include "GraphicDevice.h"

#include "Keyboard.h"

#include "AEGameComponentsDefs.h"

#include "Input\InputHandler.h"

#include "Camera\CameraManager.h"
#include "Camera\CameraUpdater.h"

#include "Game Logic Update\GameAudioUpdate.h"
#include "Game Logic Update\GameLightsUpdate.h"
#include "Game Logic Update\GamePhysicsUpdate.h"
#include "Game Logic Update\GameAnimationsUpdate.h"
#include "Game Logic Update\GameObjectScriptCaller.h"
#include "Game Logic Update\GameObjectCameraUpdate.h"

#include "Game Objects Debug\GameObjectsDebugVisualizer.h"

#include "Stats\Console.h"
#include "Stats\DebugStats.h"

#include "Forward+ Rendering\ForwardPlusRendering.h"

#include "ImGui Component\ImGuiComponent.h"

#endif
