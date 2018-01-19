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
#include <string>

/*************************
*   3rd Party Includes   *
**************************/

/***************************
*   Game Engine Includes   *
****************************/
#include "Time\AETime.h"
#include "GameApp\GameApp.h"
#include "GameObjectRenderTest.h"
#include "GameObject\GameObject.h"
#include "GameObject\GameObjectManager.h"
#include "Models\Mesh.h"
#include "Models\MeshPart.h"
#include "GameObject\Components\MeshGOC.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "GraphicDevice.h"
#include "Vertex\IndexBuffer.h"
#include "Camera\Camera.h"
#include "Graphic Extensions\Shader Extensions\Properties\ShaderProperties.h"
#include "Shaders\Buffers\ConstantBuffer.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
GameObjectRenderTest::GameObjectRenderTest(GameApp* gameApp, const std::wstring& gameComponentName)
    : DrawableGameComponent(gameApp, gameComponentName)
{
}

GameObjectRenderTest::~GameObjectRenderTest()
{
}

void GameObjectRenderTest::Initialize()
{
    m_GameObjectManager = m_GameApp->GetGameObjectManager();

    m_Camera = m_GameApp->GetGameService<Camera>(L"Camera");

    DrawableGameComponent::Initialize();
}

void GameObjectRenderTest::LoadContent()
{    
    DrawableGameComponent::LoadContent();
}

void GameObjectRenderTest::Update(const TimerParams& timerParams)
{
    DrawableGameComponent::Update(timerParams);
}

void GameObjectRenderTest::Render(const TimerParams& timerParams)
{
    for(auto goIt : *m_GameObjectManager)
    {
        DrawGameObject(goIt.second);
    }

    DrawableGameComponent::Render(timerParams);
}

void GameObjectRenderTest::DrawGameObject(GameObject* gameObject)
{
    AEAssert(gameObject != nullptr);
    if(gameObject == nullptr)
    {
        return;
    }

    if(gameObject->HasMeshGOC() && gameObject->HasMaterialGOCs())
    {
        Mesh* mesh = gameObject->GetMeshGOC()->GetMeshResource();

        if(mesh != nullptr)
        {
            const MeshMaterialsGOCList& matList = gameObject->GetMeshMaterialsGOCList();
            auto matListIt = matList.begin();

            for(uint32_t i = 0; i < mesh->GetNumberMeshParts() && i < matList.size(); i++, matListIt++)
            {
                MeshPart* meshPart = mesh->GetMeshPart(i);

                MeshMaterialGOC* meshMat = *matListIt;

                ShaderProperties* vsProps = meshMat->GetVertexShaderProperties();

                if(vsProps != nullptr)
                {
                    ConstantBuffer* cb = vsProps->GetConstantBuffer(L"_AE_CB_World_View_Proj");

                    cb->SetValueT<glm::mat4>(L"_AE_View", m_Camera->GetViewMatrix());
                    cb->SetValueT<glm::mat4>(L"_AE_PROJection", m_Camera->GetProjectionMatrix());
                    cb->SetValueT<glm::mat4>(L"_AE_World", gameObject->GetWorldTransform());
                }

                meshMat->ApplyShaders(m_GraphicDevice);

                m_GraphicDevice->SetVertexBuffer(meshPart->GetVertexBuffer());
                m_GraphicDevice->SetIndexBuffer(meshPart->GetIndexBuffer());

                m_GraphicDevice->SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

                m_GraphicDevice->DrawIndexed(0, 0, meshPart->GetIndexBuffer()->GetSize());
            }
        }

    }

    for(auto goIt : *gameObject)
    {
        DrawGameObject(goIt.second);
    }
}

void GameObjectRenderTest::OnLostDevice()
{
    DrawableGameComponent::OnLostDevice();
}

void GameObjectRenderTest::OnResetDevice()
{
    DrawableGameComponent::OnResetDevice();
}
