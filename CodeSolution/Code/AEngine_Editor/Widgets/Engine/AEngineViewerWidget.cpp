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
#include <Windows.h>
#include <Windowsx.h>

/*************************
*   3rd Party Includes   *
**************************/
#include "qmessagebox.h"

/***************************
*   Game Engine Includes   *
****************************/
#include "Models\Mesh.h"
#include "AudioManager.h"
#include "PhysicsActor.h"
#include "AudioListener.h"
#include "Utils\AEQTDefs.h"
#include "PhysicColliderBox.h"
#include "Lights\LightManager.h"
#include "AEngineViewerWidget.h"
#include "PhysicColliderSphere.h"
#include "GameObject\GameObject.h"
#include "Engine Viewer\EngineViewer.h"
#include "GameAssets\GameAssetManager.h"
#include "GameObject\GameObjectManager.h"
#include "GameObject\Components\MeshGOC.h"
#include "GameObject\Components\LightGOC.h"
#include "GameObject\Components\CameraGOC.h"
#include "GameObject\Components\PhysicsGOC.h"
#include "GameObject\Components\AudioSourceGOC.h"
#include "GameObject\Components\MeshMaterialGOC.h"
#include "GameObject\Components\MeshAnimationGOC.h"
#include "GameObject\Components\AudioListenerGOC.h"
#include "GameObject\Components\GameObjectScriptGOC.h"

//Always include last
#include "Memory\MemLeaks.h"

/********************
*   Function Defs   *
*********************/
AEngineViewerWidget::AEngineViewerWidget(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WidgetAttribute::WA_PaintOnScreen);
    setAttribute(Qt::WidgetAttribute::WA_NoSystemBackground);
    setAttribute(Qt::WidgetAttribute::WA_NativeWindow);
    setAttribute(Qt::WidgetAttribute::WA_DontCreateNativeAncestors);
    setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
    
    //this->setEnabled(false);
}

AEngineViewerWidget::~AEngineViewerWidget()
{
    StopEngineInstanceAndDestroy();
}

QPaintEngine*  AEngineViewerWidget::paintEngine() const
{
    return nullptr; 
}

void AEngineViewerWidget::paintEvent(QPaintEvent * event)
{
    //Nothing
}

void AEngineViewerWidget::StopEngineInstanceAndDestroy()
{
    if (m_EngineViewer == nullptr)
    {
        return;
    }

    m_EngineViewer->ShutDownGameApp();

    if (m_ThreadRunning)
    {
        m_EngineViewerThread.join();

        m_ThreadRunning = false;
    }

    DeleteMem(m_EngineViewer);
}

void AEngineViewerWidget::LinkEditorToEngine()
{
    //Get Window Handler
    HWND viewerWndHdlr = reinterpret_cast<HWND>(this->winId());
    
    //Get Handle Application Instance
    HINSTANCE viewerWndInstance = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(viewerWndHdlr, GWLP_HINSTANCE));

    m_EngineViewer = new EngineViewer(viewerWndInstance);

    m_EngineViewer->SetMainWindow(viewerWndHdlr);
}

AEResult AEngineViewerWidget::StartEngineViewerThread(const std::wstring& configProjFile)
{
    if (m_EngineViewer->IsReady())
    {
        return AEResult::Ok;
    }

    AEResult ret = AEResult::Ok;
    std::wstring errorMsg = L"";

    ret = m_EngineViewer->InitGameApp(AE_CONFIG_ENGINE_DEFAULT_FILE_PATH, configProjFile, errorMsg);
    if(ret != AEResult::Ok)
    {
        QMessageBox::warning(this->parentWidget(), tr("Error"), QString::fromStdWString(errorMsg));

        return ret;
    }

    m_IsReady = true;

    m_ThreadRunning = true;

    m_EngineViewerThread = std::thread(&AEngineViewerWidget::RunEngineViewer, this);

    return ret;
}

void AEngineViewerWidget::RunEngineViewer()
{
    m_EngineViewer->Run();
}

bool AEngineViewerWidget::nativeEvent(const QByteArray & eventType, void * message, long * result)
{
    if(m_IsReady)
    {
        HWND wndHandle = reinterpret_cast<HWND>(this->winId());
        MSG* msg = reinterpret_cast<MSG*>(message);

        if(msg->hwnd == wndHandle && m_EngineViewer != nullptr)
        {
            if(!m_ViewerHasFocus && msg->message == WM_SETCURSOR && HIWORD(msg->lParam) == WM_LBUTTONUP)
            {
                //If user clicked on this window set it to have focus
                SetFocus(wndHandle);

                //HCURSOR hCurs1 = LoadCursor(nullptr, IDC_ARROW); 
                //
                //SetCursor(hCurs1); 

                m_ViewerHasFocus = true;
            }

            if(msg->message == WM_KILLFOCUS)
            {
                m_ViewerHasFocus = false;
            }

            *result = m_EngineViewer->MsgProc(msg->message, msg->wParam, msg->lParam);

            return true;
        }
    }

    return QWidget::nativeEvent(eventType, message, result);;
}

AEResult AEngineViewerWidget::AddRawGameAsset(const std::wstring& fileName, GameContentSubtype gameContentSubtype)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    if(fileName.empty())
    {
        QMessageBox::warning(this->parentWidget(), "Add Raw Game Asset Error", "Filename is empty");

        return AEResult::Fail;
    }

    GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
    appLock.StartLock();

    GameAssetManager* gameAssetManager = m_EngineViewer->GetGameAssetManager();

    AEResult ret = gameAssetManager->CreateNewRawGameAsset(fileName, gameContentSubtype);

    AETODO("Log error and print correct information");
    if(ret != AEResult::Ok)
    {
        QMessageBox::warning(this->parentWidget(), "Add Raw Game Asset Error", "Unable to add Raw Asset");

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult AEngineViewerWidget::ImportRawGameAsset(uint64_t id)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
    appLock.StartLock();
    
    GameAssetManager* gameAssetManager = m_EngineViewer->GetGameAssetManager();

    AEResult ret = gameAssetManager->ImportRawGameAsset(id);
    
    AETODO("Log error and print correct information");
    if(ret != AEResult::Ok)
    {
        QMessageBox::warning(this->parentWidget(), "Raw Game Asset Import Error", "Unable to import Raw Asset");

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult AEngineViewerWidget::ReloadRawGameAssets()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
    appLock.StartLock();
    
    GameAssetManager* gameAssetManager = m_EngineViewer->GetGameAssetManager();

    AEResult ret = gameAssetManager->CheckForLatestRawGameAssetsAndImport();
    
    AETODO("Log error and print correct information");
    if(ret != AEResult::Ok)
    {
        QMessageBox::warning(this->parentWidget(), "Raw Game Asset Import Error", "Unable to import Raw Assets");

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult AEngineViewerWidget::CreateEmptyGameObject()
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    GameAppScopedLock gameLock = m_EngineViewer->GetGameAppScopedLock();
    gameLock.StartLock();

    GameObject* newGameObject = new GameObject(L"Empty Game Object");

    GameObjectManager* gameObjectManager = m_EngineViewer->GetGameObjectManager();

    AEResult ret = gameObjectManager->AddGameObject(newGameObject);
    
    AETODO("Log error and print correct information");
    if(ret != AEResult::Ok)
    {
        DeleteMem(newGameObject);

        QMessageBox::warning(this->parentWidget(), "Add Raw Game Object Error", "Unable to add Game Object");

        return AEResult::Fail;
    }

    return AEResult::Ok;
}

AEResult AEngineViewerWidget::AddGameObjectComponent(uint64_t gameObjectID, GameObjectComponentType componentType, GameObjectComponentTypeOption componentTypeOption)
{
    if(!m_IsReady)
    {
        return AEResult::NotReady;
    }

    GameAppScopedLock appLock = m_EngineViewer->GetGameAppScopedLock();
    appLock.StartLock();

    AEResult ret = AEResult::Ok;

    ////////////////////////////////////
    //Get Game Object from ID
    GameObjectManager* gameObjectManager = m_EngineViewer->GetGameObjectManager();
    GameObject* gameObject = gameObjectManager->GetGameObject(gameObjectID);

    AETODO("Log error and print correct information");
    if(gameObject == nullptr)
    {
        QMessageBox::warning(this->parentWidget(), "Game Object Error", "Unable to find Game Object");

        return AEResult::NotFound;
    }

    ////////////////////////////////////
    //Select Component Type and assign
    switch (componentType)
    {
        case GameObjectComponentType::GameObjectScript:
            {
                AETODO("Add literal from Localization");
                GameObjectScriptGOC* goScript = new GameObjectScriptGOC(gameObject, L"Empty Script", m_EngineViewer->GetAngelScriptManager(), m_EngineViewer->GetGameObjectScriptManager());

                ret = gameObject->AddGameObjectScriptGOC(goScript);
                if (ret != AEResult::Ok)
                {
                    DeleteMem(gameObject);
                }
            }
            break;

        case GameObjectComponentType::Mesh:
            {
                MeshGOC* meshGOC = new MeshGOC(gameObject);

                ret = gameObject->SetMeshGOC(meshGOC);
                if(ret != AEResult::Ok)
                {
                    DeleteMem(meshGOC);
                }
            }
            break;

        case GameObjectComponentType::MeshMaterial:
            {
                AETODO("Add literal from Localization");
                MeshMaterialGOC* meshMaterialGOC = new MeshMaterialGOC(gameObject, L"Empty Mesh Material");

                ret = gameObject->AddMeshMaterialGOC(meshMaterialGOC);
                if(ret != AEResult::Ok)
                {
                    DeleteMem(meshMaterialGOC);
                }
            }
            break;

        case GameObjectComponentType::MeshAnimation:
            {
                MeshAnimationGOC* meshAnimationGOC = new MeshAnimationGOC(gameObject);

                ret = gameObject->SetMeshAnimationGOC(meshAnimationGOC);
                if(ret != AEResult::Ok)
                {
                    DeleteMem(meshAnimationGOC);
                }
            }
            break;

        case GameObjectComponentType::Light:
            {
                LightManager* lightManager = m_EngineViewer->GetLightManager();

                if(lightManager->IsContainerFull())
                {
                    AETODO("Add warning message");

                    ret = AEResult::FullContainer;
                }
                else
                {
                    LightGOC* lightGOC = new LightGOC(gameObject, lightManager);

                    ret = gameObject->SetLightGOC(lightGOC);
                    if(ret != AEResult::Ok)
                    {
                        DeleteMem(lightGOC);
                    }
                }
            }
            break;

        case GameObjectComponentType::Physics:
            {
                bool isNewPhysicsGOC = false;
                PhysicsGOC* physicsGOC = nullptr;
                if (gameObject->HasPhysicsGOC())
                {
                    physicsGOC = gameObject->GetPhysicsGOC();
                }
                else
                {
                    isNewPhysicsGOC = true;
                    physicsGOC = new PhysicsGOC(gameObject, m_EngineViewer->GetPhysicsManager());
                }

                switch (componentTypeOption)
                {
                    case GameObjectComponentTypeOption::RigidBody:
                        if (physicsGOC->IsRigidBody())
                        {
                            return AEResult::ObjExists;
                        }
                        else
                        {
                            ret = physicsGOC->AddRigidBody();
                        }
                        break;

                    case GameObjectComponentTypeOption::BoxCollider:
                        {
                            uint64_t id = 0;

                            ret = physicsGOC->AddCollider(CollisionShape::Box, id);
                            if (ret != AEResult::Ok)
                            {
                                break;
                            }

                            if (gameObject->HasMeshGOC())
                            {
                                PhysicCollider* collider = physicsGOC->GetPhysicsActor()->GetPhysicCollider(id);
                                PhysicColliderBox* boxCollider = dynamic_cast<PhysicColliderBox*>(collider);

                                MeshGOC* meshGOC = gameObject->GetMeshGOC();

                                Mesh* mesh = meshGOC->GetMeshResource();
                                if (mesh != nullptr)
                                {
                                    const BoundingBox& box = mesh->GetBoundingBox();
                                    boxCollider->SetSize(box.GetSize());
                                    boxCollider->SetScale(gameObject->GetScale());
                                }
                            }
                        }
                        break;

                    case GameObjectComponentTypeOption::SphereCollider:
                        {
                            uint64_t id = 0;

                            ret = physicsGOC->AddCollider(CollisionShape::Sphere, id);
                            if (ret != AEResult::Ok)
                            {
                                break;
                            }

                            if (gameObject->HasMeshGOC())
                            {
                                PhysicCollider* collider = physicsGOC->GetPhysicsActor()->GetPhysicCollider(id);
                                PhysicColliderSphere* sphereCollider = dynamic_cast<PhysicColliderSphere*>(collider);

                                MeshGOC* meshGOC = gameObject->GetMeshGOC();

                                Mesh* mesh = meshGOC->GetMeshResource();
                                if (mesh != nullptr)
                                {
                                    const BoundingSphere& sphere = mesh->GetBoundingSphere();
                                    sphereCollider->SetRadius(sphere.m_Radius);
                                    sphereCollider->SetScale(gameObject->GetScale());
                                }
                            }
                        }
                        break;

                    default:
                        AEAssert(false);

                        AETODO("Add better ret code");
                        ret = AEResult::InvalidObjType;
                        break;
                }

                if (isNewPhysicsGOC)
                {
                    ret = gameObject->SetPhysicsGOC(physicsGOC);
                }

                if (ret != AEResult::Ok && isNewPhysicsGOC)
                {
                    DeleteMem(physicsGOC);
                }
            }
            break;

        case GameObjectComponentType::Camera:
            {
                CameraGOC* cameraGOC = new CameraGOC(gameObject, m_EngineViewer->GetCameraManager(), m_EngineViewer->GetGraphicsDevice());

                ret = gameObject->SetCameraGOC(cameraGOC);
                if (ret != AEResult::Ok)
                {
                    DeleteMem(cameraGOC);
                }
            }
            break;

        case GameObjectComponentType::AudioSource:
            {
                AudioSourceGOC* audioSourceGOC = new AudioSourceGOC(gameObject, L"Empty Audio Source");

                ret = gameObject->AddAudioSourceGOC(audioSourceGOC);
                if (ret != AEResult::Ok)
                {
                    DeleteMem(audioSourceGOC);
                }
            }
            break;

        case GameObjectComponentType::AudioListener:
            {
                AudioManager* audioManager = m_EngineViewer->GetAudioManager();
                if (audioManager->IsListenerSet())
                {
                    AETODO("Add warning message");
                    AETODO("Add correct error");

                    ret = AEResult::Fail;
                }
                else
                {
                    AudioListenerGOC* audioListenerGOC = new AudioListenerGOC(gameObject, audioManager);

                    ret = gameObject->SetAudioListenerGOC(audioListenerGOC);
                    if (ret != AEResult::Ok)
                    {
                        DeleteMem(audioListenerGOC);
                    }
                }
            }
            break;

        default:
            AEAssert(false);
            ret = AEResult::InvalidObjType;
            break;
    }

    if(ret != AEResult::Ok)
    {
        AETODO("Log error and print correct information");
        
        QMessageBox::warning(this->parentWidget(), "Game Object Error", "Unable to add Game Object Component");
    }

    return ret;
}
