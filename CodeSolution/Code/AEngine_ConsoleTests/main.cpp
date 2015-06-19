

#include "Importers\ImporterAssimp.h"
#include "Writers\WriterAE3D.h"
#include "Importers\ImportersDefs.h"
#include "Content\ModelContent.h"
#include "Content\MeshContent.h"
#include "Content\MeshPartContent.h"
#include "Content\VertexBufferContent.h"

#include <Windows.h>
#include <Xinput.h>
#include <iostream>
#include <stdint.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <stdint.h>

#include "Utils\Singleton.h"
#include <SFML/Audio.hpp>

#include "PhysicsManager.h"
#include "Time\AETimeDefs.h"

#include "PhysicsActor.h"
#include "Utils\Object3D.h"
#include "PhysicColliderBox.h"

//Always include last
#include "Memory\MemLeaks.h"

#include "XML\AEXMLWriter.h"

#include "cppformat\format.h"

#include "Vertex\Types\VertexPositionNormalTexture.h"

#include <fstream>

#include <list>
#include <vector>


#include <atomic>

using namespace std;

void main()
{
	std::atomic<uint32_t> idx = ATOMIC_VAR_INIT(0);

	uint32_t aaa = std::atomic_fetch_add(&idx, 3);

	cout << "aaa: " << aaa << " idx: " << idx << endl;

	aaa = std::atomic_fetch_add(&idx, 3);

	cout << "aaa: " << aaa << " idx: " << idx << endl;

	std::atomic<float> cc;

	float dd = 5;

	float ee = 6;
	float ff = 6;

	//ModelContent* content;

	//ImporterAssimp imp;

	//imp.ImportModel(L"C:/Users/Carlos/Documents/3dsMax/export/Wolf.DAE", &content);

	//std::ofstream outputFile("C:/Users/Carlos/Documents/3dsMax/export/WolfMesh.txt");

	//auto itMesh = content->m_Meshes.begin();
	//auto meshPart = (*itMesh).second->m_MeshParts[0];

	//VertexBufferContent<VertexPositionNormalTexture>* vtxBuffer = (VertexBufferContent<VertexPositionNormalTexture>*)(meshPart->m_VtxBuff);

	////Vtx Size and Idx Size
	//outputFile << meshPart->m_VtxBuff->m_Size << " " << meshPart->m_IdxSize << std::endl;

	////Vtx Pos and Normal
	//for (uint32_t i = 0; i < vtxBuffer->m_Size; i++)
	//{
	//	auto vtx = vtxBuffer->m_Buffer[i];

	//	outputFile << vtx.m_Position.x << " " << vtx.m_Position.y << " " << vtx.m_Position.z << " " <<
	//								vtx.m_Normal.x   << " " << vtx.m_Normal.y   << " " << vtx.m_Normal.z   << std::endl;
	//}

	////Idx
	//for (uint32_t i = 0; i < meshPart->m_IdxSize; i++)
	//{
	//	outputFile << meshPart->m_IdxBuf16[i] << std::endl;
	//}
}

//class IcoSphere
//{
//	public:
//
//		struct Triangle
//		{
//			uint32_t v1;
//			uint32_t v2;
//			uint32_t v3;
//
//			Triangle()
//			{
//			}
//
//			Triangle(uint32_t a, uint32_t b, uint32_t c)
//				: v1(a), v2(b), v3(c)
//			{
//			}
//		};
//
//		vector<VertexPositionNormalTexture> mVtx;
//
//		// return index of point in the middle of p1 and p2
//		uint32_t getMiddlePoint(int p1, int p2)
//		{
//			glm::vec3 middlePos = ((mVtx[p1].m_Position + mVtx[p2].m_Position) / 2.0f);
//
//			VertexPositionNormalTexture newVtx;
//			memset(&newVtx, 0, sizeof(VertexPositionNormalTexture));
//
//			newVtx.m_Position = middlePos;
//
//			//Add vertex makes sure point is on unit sphere
//			uint32_t idx = mVtx.size();
//			mVtx.push_back(newVtx);
//
//			return idx;
//		}
//
//		void Create()
//		{
//			//Get Base Size
//			float t = (1.0f + sqrt(5.0f)) / 2.0f;
//
//			VertexPositionNormalTexture vtx;
//
//			memset(&vtx, 0, sizeof(VertexPositionNormalTexture));
//
//			//////////////////////////
//			//5 faces around point 0
//			//////////////////////////
//
//			vtx.m_Position = glm::vec3(-1, t, 0);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(1, t, 0);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(-1, -t, 0);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(1, -t, 0);
//			mVtx.push_back(vtx);
//
//			vtx.m_Position = glm::vec3(0, -1, t);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(0, 1, t);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(0, -1, -t);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(0, 1, -t);
//			mVtx.push_back(vtx);
//
//			vtx.m_Position = glm::vec3(t, 0, -1);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(t, 0, 1);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(-t, 0, -1);
//			mVtx.push_back(vtx);
//			vtx.m_Position = glm::vec3(-t, 0, 1);
//			mVtx.push_back(vtx);
//
//			//Triangles
//
//			vector<Triangle> idxs;
//
//			// 5 faces around point 0
//			idxs.push_back(Triangle(0, 11, 5));
//			idxs.push_back(Triangle(0, 5, 1));
//			idxs.push_back(Triangle(0, 1, 7));
//			idxs.push_back(Triangle(0, 7, 10));
//			idxs.push_back(Triangle(0, 10, 11));
//
//			// 5 adjacent faces 
//			idxs.push_back(Triangle(1, 5, 9));
//			idxs.push_back(Triangle(5, 11, 4));
//			idxs.push_back(Triangle(11, 10, 2));
//			idxs.push_back(Triangle(10, 7, 6));
//			idxs.push_back(Triangle(7, 1, 8));
//
//			// 5 faces around point 3
//			idxs.push_back(Triangle(3, 9, 4));
//			idxs.push_back(Triangle(3, 4, 2));
//			idxs.push_back(Triangle(3, 2, 6));
//			idxs.push_back(Triangle(3, 6, 8));
//			idxs.push_back(Triangle(3, 8, 9));
//
//			// 5 adjacent faces 
//			idxs.push_back(Triangle(4, 9, 5));
//			idxs.push_back(Triangle(2, 4, 11));
//			idxs.push_back(Triangle(6, 2, 10));
//			idxs.push_back(Triangle(8, 6, 7));
//			idxs.push_back(Triangle(9, 8, 1));
//
//			// refine triangles
//			uint32_t recursionLevel = 3;
//			for (uint32_t i = 0; i < recursionLevel; i++)
//			{
//				vector<Triangle> idxs2;
//
//				uint32_t oldIndexBufSize = (uint32_t)idxs.size();
//
//				for (uint32_t j = 0; j < oldIndexBufSize; j++)
//				{
//					Triangle tri = idxs[i];
//
//					// replace triangle by 4 triangles
//					uint32_t a = getMiddlePoint(tri.v1, tri.v2);
//					uint32_t b = getMiddlePoint(tri.v2, tri.v3);
//					uint32_t c = getMiddlePoint(tri.v3, tri.v1);
//
//					idxs2.push_back(Triangle(tri.v1, a, c));
//					idxs2.push_back(Triangle(tri.v2, b, a));
//					idxs2.push_back(Triangle(tri.v3, c, b));
//					idxs2.push_back(Triangle(a, b, c));
//				}
//
//				idxs = idxs2;
//			}
//
//			for (uint32_t i = 0; i < idxs.size(); i++)
//			{
//				Triangle tri = idxs[i];
//				glm::vec3 e0 = mVtx[tri.v2].m_Position - mVtx[tri.v1].m_Position;
//				glm::vec3 e1 = mVtx[tri.v3].m_Position - mVtx[tri.v1].m_Position;
//
//				glm::vec3 p1 = glm::cross(e0, e1);
//				float p2 = p1.length();
//
//				glm::vec3 nor = p1 / p2;
//			}
//		}
//};



#if 0
void main()
{
	std::wstring aa = fmt::format(L"Mi nada {0}, aa {1}, bb {2}, cc {3}, dd {4}", "mio", 1.23f, false, 5.789456, 10);

	std::wcout <<  aa << std::endl;
}


void main()
{
	AEXMLWriter xmlWriter;

	AEResult ret;

	//float fl = 1234567.1234567890f;
	//printf("%.2f\n", fl);

	ret = xmlWriter.CreateXML(L"test.xml", true);
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "Create" << std::endl;
	}

	ret = xmlWriter.StartNode(L"Car1");
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "Start Node" << std::endl;
	}

	ret = xmlWriter.StartNode(L"Car2");
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "Start Node" << std::endl;
	}

	ret = xmlWriter.WriteString(L"Test1", L"Nada");
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "Write Attribute fail" << std::endl;
	}

	ret = xmlWriter.EndNode();
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "End Node" << std::endl;
	}

	ret = xmlWriter.EndNode();
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "End Node" << std::endl;
	}

	ret = xmlWriter.FinalizeXML();
	if (ret != AEResult::Ok)
	{
		std::cout << "Failed" << "Finalize" << std::endl;
	}
}



void main()
{
#if defined( _DEBUG )
	MemLeaks::MemoryBegin();
#endif

	AEResult ret = AEResult::Ok;
	PhysicsManager* physicsManager = new PhysicsManager();;

	physicsManager->Initialize();

	physicsManager->ConnectToPhysXDebugger();

	Object3D object3dParent;
	object3dParent.SetPositionX(-15.0f);
	object3dParent.SetPositionY(10.0f);

	Object3D object3d;
	object3d.SetPositionX(+15.0f);
	object3d.SetParentObject3D(&object3dParent);

	//////////////////////Parent/////////////////////////////////
	PhysicsActor* actorParent = new PhysicsActor(&object3dParent);
	actorParent->Initialize(physicsManager, PhysicsActorType::Dynamic);
	physicsManager->AddPhysicsActor(actorParent);

	PhysicColliderBox* physicColliderBoxParent = new PhysicColliderBox();
	physicColliderBoxParent->Initialize(physicsManager);
	actorParent->AddPhysicCollider(physicColliderBoxParent);
	/////////////////////////////////////////////////////////

	//////////////////////Main/////////////////////////////////
	PhysicsActor* actor = new PhysicsActor(&object3d);
	actor->Initialize(physicsManager, PhysicsActorType::Dynamic);
	physicsManager->AddPhysicsActor(actor);

	PhysicColliderBox* physicColliderBox = new PhysicColliderBox();
	physicColliderBox->Initialize(physicsManager);
	actor->AddPhysicCollider(physicColliderBox);
	/////////////////////////////////////////////////////////

	//////////////////////Floor/////////////////////////////////
	Object3D object3dFloor;
	PhysicsActor* actorFloor = new PhysicsActor(&object3dFloor);
	actorFloor->Initialize(physicsManager, PhysicsActorType::Static);
	physicsManager->AddPhysicsActor(actorFloor);

	PhysicColliderBox* physicColliderBoxFloor = new PhysicColliderBox();
	physicColliderBoxFloor->Initialize(physicsManager);
	physicColliderBoxFloor->SetSize(glm::vec3(10.0f, 0.1f, 10.0f));
	actorFloor->AddPhysicCollider(physicColliderBoxFloor);

	uint64_t floorID = actorFloor->GetUniqueID();
	/////////////////////////////////////////////////////////

	physicsManager->ConnectToPhysXDebugger();

	//600
	for (size_t i = 0; i < 300; i++)
	{
		system("cls");

		glm::vec3 pos = object3d.GetPosition();
		glm::vec3 rot = object3d.GetRotation();
		glm::vec3 posP = object3dParent.GetPosition();
		std::cout << "Object 3D Pos( x: " << pos.x << ", y: " << pos.y << ", z: " << pos.z << " )" << std::endl;
		std::cout << "Object 3D Rot( x: " << rot.x << ", y: " << rot.y << ", z: " << rot.z << " )" << std::endl;
		std::cout << std::endl;
		std::cout << "Object 3D Parent Pos( x: " << posP.x << ", y: " << posP.y << ", z: " << posP.z << " )" << std::endl;

		float dt = 1.0f / 70.0f;
		uint32_t mil = (uint32_t)(dt * 1000.0f);

		std::chrono::milliseconds dura(mil);
		std::this_thread::sleep_for(dura);

		TimerParams tp;
		tp.m_ElapsedTime = dt;
		physicsManager->Update(tp);

		if (i == 150)
		{
			//physicColliderBox->SetSize(glm::vec3(10, 1, 1));
		}

		if (i == 400)
		{
			actorFloor->ChangePhysicsActorType(PhysicsActorType::Dynamic);
		}

		if (i == 500)
		{
			actorFloor->ChangePhysicsActorType(PhysicsActorType::Static);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	physx::PxSerializationRegistry* registry = physx::PxSerialization::createSerializationRegistry(PxGetPhysics());

	// Create a collection and all objects for serialization
	physx::PxCollection* collection = PxCreateCollection();

	physx::PxSerialization::complete(*collection, *registry);

	// Serialize either to binary or RepX
	//physx::PxDefaultFileOutputStream outStream("serialized.dat");
	physx::PxDefaultMemoryOutputStream  outStream;

	for (auto actorIt : *physicsManager)
	{
		uint64_t* ii = new uint64_t;
		*ii = actorIt.second->GetUniqueID();

		std::cout << actorIt.second->GetUniqueID() << std::endl;
		actorIt.second->GetPxRigidActor()->userData = (void*)ii;
		collection->add(*actorIt.second->GetPxRigidActor(), actorIt.second->GetUniqueID());
	}

	physx::PxSerialization::complete(*collection, *registry);

	// Binary
	physx::PxSerialization::serializeCollectionToBinary(outStream, *collection, *registry);
	//~Binary

	void* mem = new byte[AE_PHYSIC_GET_SIZE_ALIGN(outStream.getSize())];
	memset(mem, 0, AE_PHYSIC_GET_SIZE_ALIGN(outStream.getSize()));
	void* mem128 = AE_PHYSIC_GET_MEM_ALIGN(mem);
	memcpy(mem128, outStream.getData(), outStream.getSize());

	ReleasePhysX(collection);
	ReleasePhysX(registry);

	DeleteMem(physicsManager);

	Sleep(2000);

	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////

	physicsManager = new PhysicsManager();

	physicsManager->Initialize();

	physicsManager->ConnectToPhysXDebugger();

	physx::PxSerializationRegistry* registry2 = physx::PxSerialization::createSerializationRegistry(PxGetPhysics());

	physx::PxCollection* collection2 = physx::PxSerialization::createCollectionFromBinary(mem128, *registry2);

	physicsManager->GetPxScene()->addCollection(*collection2);

	for (uint32_t i = 0; i < collection2->getNbObjects(); i++)
	{
		physx::PxBase& pxBaseFloor = collection2->getObject(i);
		physx::PxType aa = pxBaseFloor.getConcreteType();
		const char* cc = pxBaseFloor.getConcreteTypeName();
		physx::PxRigidActor* rigidFloor = (physx::PxRigidActor*)&pxBaseFloor;
		if (rigidFloor->userData != nullptr)
		{
			uint64_t sndFloorID = *((uint64_t*)rigidFloor->userData);
			std::cout << sndFloorID << std::endl;
		}
	}

	DeleteMemArr(mem);

	//physx::PxSerialObjectId aa;

	for (size_t i = 301; i < 600; i++)
	{
		system("cls");

		float dt = 1.0f / 70.0f;
		uint32_t mil = (uint32_t)(dt * 1000.0f);

		std::chrono::milliseconds dura(mil);
		std::this_thread::sleep_for(dura);

		TimerParams tp;
		tp.m_ElapsedTime = dt;
		physicsManager->Update(tp);
	}

	ReleasePhysX(collection2);
	ReleasePhysX(registry2);

	DeleteMem(physicsManager);

#if defined( _DEBUG )
	MemLeaks::MemoryEnd();
#endif
}
#endif

/*
struct XBoxController
{
	XINPUT_CAPABILITIES m_Capabilities;
	XINPUT_VIBRATION m_Vibration;
	DWORD m_CurrentPacketNumber = 0;
	XINPUT_STATE m_State;
	bool m_Connected = false;

	XBoxController()
	{
		memset(&m_State, 0, sizeof(XINPUT_STATE));
		memset(&m_Capabilities, 0, sizeof(XINPUT_CAPABILITIES));
		memset(&m_Vibration, 0, sizeof(XINPUT_VIBRATION));
	};

	void SetVibrationByPercent(float percent, bool LeftRight)
	{
		if (percent > 1.0f)
		{
			percent = 1.0f;
		}
		else if (percent < 0.01f)
		{
			percent = 0.0f;
		}

		uint16_t amount = (uint16_t)(65535.0f * percent);

		if (LeftRight)
		{
			m_Vibration.wLeftMotorSpeed = amount;
		}
		else
		{
			m_Vibration.wRightMotorSpeed = amount;
		}
	}
};

float GetMov(int16_t LX, int16_t deadZone)
{
	if (LX > deadZone)
	{
		LX -= deadZone;

		int16_t max = INT16_MAX - deadZone;

		return ((float)LX / max);
	}
	else if (LX < -deadZone)
	{
		LX += deadZone;

		int16_t min = INT16_MIN + deadZone;

		return -((float)LX / min);
	}
	else
	{
		return 0.0f;
	}
}

void main()
{

	XBoxController controllers[XUSER_MAX_COUNT];

	DWORD dwResult;
	for (DWORD i = 0; i< XUSER_MAX_COUNT; i++)
	{
		dwResult = XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &controllers[i].m_Capabilities);

		if (dwResult == ERROR_SUCCESS)
		{
			// Simply get the state of the controller from XInput.
			dwResult = XInputGetState(i, &controllers[i].m_State);

			if (dwResult == ERROR_SUCCESS)
			{
				controllers[i].m_Connected = true;
				controllers[i].m_CurrentPacketNumber = controllers[i].m_State.dwPacketNumber;
			}
		}
	}

	bool cont = true;
	uint32_t timeLapsed = 0;
	while (cont)
	{
		Sleep(10);

		//for (uint8_t i = 0; i < XUSER_MAX_COUNT; i++)
		uint8_t i = 0;
		{
			//if (!controllers[i].m_Connected)
			//{
			//	if (timeLapsed >= 10)
			//	{
			//		dwResult = XInputGetCapabilities(i, XINPUT_FLAG_GAMEPAD, &controllers[i].m_Capabilities);

			//		if (dwResult == ERROR_SUCCESS)
			//		{
			//			// Simply get the state of the controller from XInput.
			//			dwResult = XInputGetState(i, &controllers[i].m_State);

			//			if (dwResult == ERROR_SUCCESS)
			//			{
			//				controllers[i].m_Connected = true;
			//				controllers[i].m_CurrentPacketNumber = controllers[i].m_State.dwPacketNumber;
			//			}
			//		}
			//	}

			//	continue;
			//}

			dwResult = XInputGetState(i, &controllers[i].m_State);
			if (dwResult != ERROR_SUCCESS)
			{
				controllers[i].m_Connected = false;
				continue;
			}

			if (controllers[i].m_CurrentPacketNumber == controllers[i].m_State.dwPacketNumber)
			{
				continue;
			}

			system("cls");

			controllers[i].m_CurrentPacketNumber = controllers[i].m_State.dwPacketNumber;

			std::cout << "Right: " << std::endl;
			std::cout << "X: " << GetMov(controllers[i].m_State.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) << std::endl;
			std::cout << "Y: " << GetMov(controllers[i].m_State.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) << std::endl;
			std::cout << std::endl;

			std::cout << "Left: " << std::endl;
			std::cout << "X: " << GetMov(controllers[i].m_State.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) << std::endl;
			std::cout << "Y: " << GetMov(controllers[i].m_State.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) << std::endl;
			std::cout << std::endl;
		}

		if (timeLapsed >= 5000)
		{
			timeLapsed;
		}
		timeLapsed++;
	}
}




if (controllers[i].m_State.Gamepad.wButtons != 0)
{
std::cout << "Button is pressed" << std::endl;
}

if (controllers[i].m_State.Gamepad.wButtons & XINPUT_GAMEPAD_A)
{
std::cout << "Button A is pressed" << std::endl;
}

if (controllers[i].m_State.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
{
std::cout << "Button Back is pressed" << std::endl;

cont = false;
}

if (controllers[i].m_State.Gamepad.wButtons & XINPUT_GAMEPAD_X)
{
std::cout << "Button X is pressed" << std::endl;

controllers[i].SetVibrationByPercent(0.5f, true);

XInputSetState(i, &controllers[i].m_Vibration);
}

if (controllers[i].m_State.Gamepad.wButtons & XINPUT_GAMEPAD_B)
{
std::cout << "Button B is pressed" << std::endl;

controllers[i].SetVibrationByPercent(1.0f, true);
controllers[i].SetVibrationByPercent(1.0f, false);

XInputSetState(i, &controllers[i].m_Vibration);
}

if (controllers[i].m_State.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
{
std::cout << "Button Y is pressed" << std::endl;

controllers[i].SetVibrationByPercent(0.0f, true);
controllers[i].SetVibrationByPercent(0.0f, false);

XInputSetState(i, &controllers[i].m_Vibration);
}

*/
