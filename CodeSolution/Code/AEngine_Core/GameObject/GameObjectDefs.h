/** \file 
 * Contains definitions for handling Game Objects
 */

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

#ifndef _GAME_OBJECT_DEFS_H
#define _GAME_OBJECT_DEFS_H

/**********************
*   System Includes   *
***********************/

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "GameAssets\GameAssetDefs.h"

/********************
*   Forward Decls   *
*********************/
class Texture;
class GameAsset;

/**************
*   Defines   *
***************/

#define AE_GAME_OBJ_MANAGER_NODE_NAME                   "GameObjectManager"
#define AE_GAME_OBJ_NODE_NAME                           "GameObject"
#define AE_GAME_OBJ_NAME_PROP                           "Name"
#define AE_GAME_OBJ_POSITION_PROP                       "Position"
#define AE_GAME_OBJ_ROTATION_PROP                       "Rotation"
#define AE_GAME_OBJ_SCALE_PROP                          "Scale"
#define AE_GAME_OBJ_COMPONENTS_NODE_NAME                "Components"
#define AE_GAME_OBJ_CHILDS_NODE_NAME                    "Childs"
#define AE_GAME_OBJ_COMPONENT_ASSETID_PROP              "AssetID"
#define AE_GAME_OBJ_GOC_MESH_NODE_NAME                  "MeshComponent"
#define AE_GAME_OBJ_GOC_MATS_NODE_NAME                  "MaterialComponents"
#define AE_GAME_OBJ_GOC_MAT_NODE_NAME                   "MaterialComponent"
#define AE_GAME_OBJ_GOC_MAT_NAME_PROP                   "MaterialName"
#define AE_GAME_OBJ_GOC_MAT_SHADER_NODE_NAME            "Shader"
#define AE_GAME_OBJ_GOC_MAT_SHADER_TYPE_PROP            "ShaderType"
#define AE_GAME_OBJ_GOC_MAT_SHADER_PROPS_NODE_NAME      "ShaderProperties"
#define AE_GAME_OBJ_GOC_MAT_SHADER_VAR_NODE_NAME        "ShaderVariable"
#define AE_GAME_OBJ_GOC_MAT_SHADER_CB_NODE_NAME         "ConstantBuffer"
#define AE_GAME_OBJ_GOC_MAT_SHADER_CB_NAME_PROP         "ConstantBufferName"
#define AE_GAME_OBJ_GOC_MAT_SHADER_VAR_NAME_PROP        "ShaderVariableName"
#define AE_GAME_OBJ_GOC_MAT_SHADER_VAR_CLASS_PROP       "ShaderVariableClass"
#define AE_GAME_OBJ_GOC_MAT_SHADER_VAR_TYPE_PROP        "ShaderVariableType"
#define AE_GAME_OBJ_GOC_MAT_SHADER_COLUMNS_PROP         "Columns"
#define AE_GAME_OBJ_GOC_MAT_SHADER_ROWS_PROP            "Rows"
#define AE_GAME_OBJ_GOC_MAT_SHADER_VAR_VALUE_PROP       "ShaderVariableValue"
#define AE_GAME_OBJ_GOC_MAT_TEXTURE_NODE_NAME           "ShaderTextureBinding"
#define AE_GAME_OBJ_GOC_MAT_TEXTURE_NAME_PROP           "TextureName"
#define AE_GAME_OBJ_GOC_SCRIPTS_NODE_NAME               "GameObjectScriptComponents"
#define AE_GAME_OBJ_GOC_SCRIPT_NODE_NAME                "GameObjectScriptComponent"
#define AE_GAME_OBJ_GOC_SCRIPT_NAME_PROP                "GameObjectScriptName"
#define AE_GAME_OBJ_GOC_SCRIPT_HAS_INSTANCE_PROP        "HasInstance"
#define AE_GAME_OBJ_GOC_SCRIPT_INSTANCE_NAME_PROP       "InstanceName"
#define AE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NODE_NAME       "GOSProperty"
#define AE_GAME_OBJ_GOC_SCRIPT_PROPERTY_NAME_PROP       "PropertyName"
#define AE_GAME_OBJ_GOC_SCRIPT_PROPERTY_TYPE_PROP       "PropertyType"
#define AE_GAME_OBJ_GOC_SCRIPT_PROPERTY_VALUE_PROP      "PropertyValue"
#define AE_GAME_OBJ_GOC_LIGHT_NODE_NAME                 "LightComponent"
#define AE_GAME_OBJ_GOC_LIGHT_FRUSTUM_CASCADE_PROP      "FrustumCascadeEnabled"
#define AE_GAME_OBJ_GOC_LIGHT_DEBUG_DRAW_PROP           "DebugDrawEnabled"
#define AE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_SIMPLE_PROP  "DrawFrustumSimple"
#define AE_GAME_OBJ_GOC_LIGHT_DRAW_FRUSTUM_ENABLE_PROP  "DrawFrustumEnabled"
#define AE_GAME_OBJ_GOC_LIGHT_LIGHT_TYPE_PROP           "Type"
#define AE_GAME_OBJ_GOC_LIGHT_ENABLED_PROP              "Enabled"
#define AE_GAME_OBJ_GOC_LIGHT_SHADOW_ENABLED_PROP       "ShadowEnabled"
#define AE_GAME_OBJ_GOC_LIGHT_COLOR_PROP                "Color"
#define AE_GAME_OBJ_GOC_LIGHT_FAR_ATTENUATION_PROP      "FarAttenuation"
#define AE_GAME_OBJ_GOC_LIGHT_NEAR_ATTENUATION_PROP     "NearAttenuation"
#define AE_GAME_OBJ_GOC_LIGHT_FALL_OFF_ANGLE_PROP       "FallOffAngle"
#define AE_GAME_OBJ_GOC_LIGHT_ANGLE_PROP                "Angle"
#define AE_GAME_OBJ_GOC_ANIM_NODE_NAME                  "MeshAnimationComponent"
#define AE_GAME_OBJ_GOC_ANIM_BLEND_ANIM_PROP            "BlendAnimation"
#define AE_GAME_OBJ_GOC_ANIM_BLEND_TIME_PROP            "BlendTime"
#define AE_GAME_OBJ_GOC_ANIM_ON_LOOP_ANIM_PROP          "OnLoop"
#define AE_GAME_OBJ_GOC_ANIM_SKELETON_ASSET_ID_PROP     "SkeletonAssetID"
#define AE_GAME_OBJ_GOC_ANIM_ASSET_NODE_NAME            "AnimationAsset"
#define AE_GAME_OBJ_GOC_CAM_NODE_NAME                   "CameraComponent"
#define AE_GAME_OBJ_GOC_CAM_DEBUG_DRAW_ENABLED_PROP     "DebugDrawEnabled"
#define AE_GAME_OBJ_GOC_CAM_IS_DEFAULT_CAM_PROP         "IsDefaultCamera"
#define AE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_ENABLED_PROP   "DrawFrustumEnabled"
#define AE_GAME_OBJ_GOC_CAM_DRAW_FRUSTUM_SIMPLE_PROP    "DrawFrustumSimple"
#define AE_GAME_OBJ_GOC_AUDIO_LISTENER_NODE_NAME        "AudioListenerComponent"
#define AE_GAME_OBJ_GOC_AUDIO_SOURCE_NODE_NAME          "AudioSourceComponent"
#define AE_GAME_OBJ_GOC_AUDIO_SOURCE_NAME_PROP          "Name"
#define AE_GAME_OBJ_GOC_PHYSICS_NODE_NAME               "PhysicsComponent"
#define AE_GAME_OBJ_GOC_PHYSICS_IS_RIGID_BODY_PROP      "IsRigidBody"
#define AE_GAME_OBJ_GOC_PHYSICS_COLLIDER_NODE_NAME      "Collider"
#define AE_GAME_OBJ_GOC_PHYSICS_COLLIDER_TYPE_PROP      "Type"
#define AE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SCALE_PROP     "Scale"
#define AE_GAME_OBJ_GOC_PHYSICS_COLLIDER_SIZE_PROP      "Size"
#define AE_GAME_OBJ_GOC_PHYSICS_COLLIDER_RADIUS_PROP    "Radius"

/**************
*   Typedef   *
***************/
typedef std::function<void (uint64_t)> GameObjectMoveToChildCallback;

/************
*   Enums   *
*************/

enum class GameObjectComponentTypeOption : uint32_t
{
    GameObjectScript = 0,

    Mesh,

    MeshMaterial,

    MeshAnimation,

    Light,

    RigidBody,

    BoxCollider,

    SphereCollider,

    Camera,

    AudioSource,

    AudioListener
};

/// <summary>    
/// Types for Game Object Components
/// </summary>
enum class GameObjectComponentType : uint32_t
{
    /// <summary>
    /// Game Object Component type that handles the Game Object Scripts for the Game Object
    /// </summary>
    GameObjectScript = 0,
    
    /// <summary>
    /// Game Object Component type that handles the Mesh for the Game Object
    /// </summary>
    Mesh,

    /// <summary>
    /// Game Object Component type that handles the Mesh Materials for the Game Object
    /// </summary>
    MeshMaterial,

    /// <summary>
    /// Game Object Component type that handles the Mesh Animations for the Game Object
    /// </summary>
    MeshAnimation,
    
    /// <summary>
    /// Game Object Component type that handles a Light Source that the Game Object emits
    /// </summary>
    Light,

    /// <summary>
    /// Game Object Component type that handles the Physics for the Game Object
    /// </summary>
    Physics,

    /// <summary>
    /// Game Object Component type that handles the a camera that a Game Object owns
    /// </summary>
    Camera,

    /// <summary>
    /// Game Object Component type that handles an audio source that a Game Object emits
    /// </summary>
    AudioSource,

    /// <summary>
    /// Game Object Component type that handles if a Game Object is listening to audio
    /// </summary>
    AudioListener
};

/******************
*   Struct Decl   *
*******************/

template <class T>
struct GameObjectAssetPair sealed : public AEObject
{
    uint64_t m_AssetID = 0;
    uint64_t m_CallerID = 0;
    T* m_ResourceAsset = nullptr;
    OnListenerObjDeletionEvent m_OnListenerObjDeletionEvent = nullptr;
    GameAsset* m_GameAsset = nullptr;

    GameObjectAssetPair()
    {
    }
};

#endif
