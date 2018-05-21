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
#include <assert.h>

/***************************
*   Game Engine Includes   *
****************************/
#include "TestHDR.h"
#include "Utils\AETimeDefs.h"
#include "GameApp\GameApp.h"
#include "GraphicDevice.h"
#include "Models\Model.h"
#include "Vertex\IndexBuffer.h"
#include "Vertex\IVertexBuffer.h"
#include "Models\ModelDefs.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
TestHDR::TestHDR(GameApp* gameApp)
    : DrawableGameComponent(gameApp, "TEST HDR")
    , m_Camera(nullptr)
    , m_BasicColorMaterial(nullptr)
    , m_SphereShape(nullptr)
    , m_World(XMFLOAT4X4IDENTITY)
    , m_WVP(XMFLOAT4X4IDENTITY)
    , m_Room(nullptr)
    , m_BasicDeferredRenderingDraw(nullptr)
    , m_Tea(nullptr)
    , m_BasicTextureMaterial(nullptr)
{
}

TestHDR::~TestHDR()
{
    DeleteMem(m_BasicDeferredRenderingDraw);
    DeleteMem(m_SphereShape);
    DeleteMem(m_BasicColorMaterial);
    DeleteMem(m_Room);
    DeleteMem(m_Tea);
    DeleteMem(m_BasicTextureMaterial);
}

void TestHDR::Initialize()
{
    m_BasicColorMaterial = new BasicColorMaterial();
    m_BasicDeferredRenderingDraw = new BasicDeferredRenderingDraw();
    m_BasicTextureMaterial = new BasicTextureMaterial();

    m_SphereShape = new SphereShape(m_GraphicDevice, 1, XMCOLORBlack);

    m_Camera = static_cast<Camera*>(m_GameApp->GetGameService("Camera"));
    AEAssert(m_Camera);

    m_Room = new Model(m_GraphicDevice, "./Data/Textures/");

    m_Tea = new Model(m_GraphicDevice, "./Data/Textures/");

    DrawableGameComponent::Initialize();
}

void TestHDR::LoadContent()
{
    m_BasicColorMaterial->LoadMaterial(m_GraphicDevice);

    m_BasicDeferredRenderingDraw->LoadMaterial(m_GraphicDevice);

    m_BasicTextureMaterial->LoadMaterial(m_GraphicDevice);

    m_Room->LoadModel("./Data/Models/Test1.FBX", "Room Test");

    //m_Tea->LoadModel("./Data/Models/BoxTrigs.FBX", "Tea pot");
    
    DrawableGameComponent::LoadContent();
}

void TestHDR::Update(const TimerParams& timerParams)
{
    XEMathHelperMatrix::Multiply3(m_World, m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_WVP);

    DrawableGameComponent::Update(timerParams);
}

void TestHDR::Render(const TimerParams& timerParams)
{
    m_BasicColorMaterial->SetWorldViewProjection(m_WVP);

    m_BasicColorMaterial->Begin();
    m_BasicColorMaterial->BeginPass(0);

    m_SphereShape->DrawSphere();
    
    m_BasicColorMaterial->EndPass();
    m_BasicColorMaterial->End();

    AE_Base::BeginUserEventPerf(L"Draw Room");

    DrawModel(m_Room);

    AE_Base::EndUserEventPerf();

    DrawableGameComponent::Render(timerParams);
}

void TestHDR::DrawModel(Model* model)
{
    std::map<std::string, Mesh*> meshCol = model->GetMeshCollection()->GetCollection();
    std::map<std::string, Mesh*>::iterator itMesh = meshCol.begin();
    std::map<std::string, Mesh*>::iterator itEndMesh = meshCol.end();

    //m_BasicDeferredRenderingDraw->SetWorldMatrix(m_World);
    //m_BasicDeferredRenderingDraw->SetWorldViewProjection(m_WVP);

    for(; itMesh != itEndMesh; ++itMesh)
    {
        Mesh* mesh = itMesh->second;

        XEMathHelperMatrix::CreateWorld(mesh->GetPosition(), mesh->GetRotationVector(), mesh->GetScale(), m_World);

        XEMathHelperMatrix::Multiply3(m_World, m_Camera->GetViewMatrix(), m_Camera->GetProjectionMatrix(), m_WVP);

        m_BasicTextureMaterial->SetWorldViewProjection(m_WVP);

        std::vector<MeshPart*> meshPartCol = mesh->GetMeshPartCollection().GetCollection();

        for(uint32_t i = 0; i < meshPartCol.size(); ++i)
        {
            MeshPart* mp = meshPartCol[i];
                        
            IndexBuffer* idxBuff = mp->GetIndexBuffer();
            IVertexBuffer* vtxBuff = mp->GetVertexBuffer();

            m_GraphicDevice->SetIndexBuffer(idxBuff);
            m_GraphicDevice->SetVertexBuffer(vtxBuff);

            //m_BasicDeferredRenderingDraw->SetDiffuseTexture(model->GetTextureMaterial(mp->GetTexMaterialID())->m_DiffuseTexture);

            //m_BasicDeferredRenderingDraw->Begin();
            //m_BasicDeferredRenderingDraw->BeginPass(0);

            m_BasicTextureMaterial->SetDiffuseTexture(model->GetTextureMaterial(mp->GetTexMaterialID())->m_DiffuseTexture);

            m_BasicTextureMaterial->Begin();
            m_BasicTextureMaterial->BeginPass(0);

            m_GraphicDevice->DrawIndexedPrimitive(AE_PT_TRIANGLELIST, 0, 0, vtxBuff->GetSize(), 0, idxBuff->GetSize() / 3);

            //m_BasicDeferredRenderingDraw->EndPass();
            //m_BasicDeferredRenderingDraw->End();

            m_BasicTextureMaterial->EndPass();
            m_BasicTextureMaterial->End();
        }
    }
}

void TestHDR::OnLostDevice()
{    
    m_SphereShape->OnLostDevice();
    m_BasicColorMaterial->OnLostDevice();
    m_BasicDeferredRenderingDraw->OnLostDevice();
    m_Room->OnLostDevice();
    m_BasicTextureMaterial->OnLostDevice();

    DrawableGameComponent::OnLostDevice();
}

void TestHDR::OnResetDevice()
{
    m_SphereShape->OnResetDevice();
    m_BasicColorMaterial->OnResetDevice();
    m_BasicDeferredRenderingDraw->OnResetDevice();
    m_Room->OnResetDevice();
    m_BasicTextureMaterial->OnResetDevice();

    DrawableGameComponent::OnResetDevice();
}