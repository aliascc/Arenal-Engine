
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
#include "Time\XETimeDefs.h"

#include "PhysicsActor.h"
#include "Utils\Object3D.h"
#include "PhysicColliderBox.h"

//Always include last
#include "Memory\MemLeaks.h"

#include "XML\XEXMLWriter.h"

#include "cppformat\format.h"




#if 0
void main()
{
	std::wstring aa = fmt::format(L"Mi nada {0}, aa {1}, bb {2}, cc {3}, dd {4}", "mio", 1.23f, false, 5.789456, 10);

	std::wcout <<  aa << std::endl;
}


void main()
{
	XEXMLWriter xmlWriter;

	XEResult ret;

	//float fl = 1234567.1234567890f;
	//printf("%.2f\n", fl);

	ret = xmlWriter.CreateXML(L"test.xml", true);
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "Create" << std::endl;
	}

	ret = xmlWriter.StartNode(L"Car1");
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "Start Node" << std::endl;
	}

	ret = xmlWriter.StartNode(L"Car2");
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "Start Node" << std::endl;
	}

	ret = xmlWriter.WriteString(L"Test1", L"Nada");
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "Write Attribute fail" << std::endl;
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "End Node" << std::endl;
	}

	ret = xmlWriter.EndNode();
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "End Node" << std::endl;
	}

	ret = xmlWriter.FinalizeXML();
	if (ret != XEResult::Ok)
	{
		std::cout << "Failed" << "Finalize" << std::endl;
	}
}
#endif


void main()
{
#if defined( _DEBUG )
	MemLeaks::MemoryBegin();
#endif

	XEResult ret = XEResult::Ok;
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
	PhysicsActor* actorParent = new PhysicsActor();
	actorParent->SetObject3D(&object3dParent);
	actorParent->Initialize(physicsManager, PhysicsActorType::Dynamic);
	physicsManager->AddPhysicsActor(actorParent);

	PhysicColliderBox* physicColliderBoxParent = new PhysicColliderBox();
	physicColliderBoxParent->Initialize(physicsManager);
	actorParent->AddPhysicCollider(physicColliderBoxParent);
	/////////////////////////////////////////////////////////

	//////////////////////Main/////////////////////////////////
	PhysicsActor* actor = new PhysicsActor();
	actor->SetObject3D(&object3d);
	actor->Initialize(physicsManager, PhysicsActorType::Dynamic);
	physicsManager->AddPhysicsActor(actor);

	PhysicColliderBox* physicColliderBox = new PhysicColliderBox();
	physicColliderBox->Initialize(physicsManager);
	actor->AddPhysicCollider(physicColliderBox);
	/////////////////////////////////////////////////////////

	//////////////////////Floor/////////////////////////////////
	Object3D object3dFloor;
	PhysicsActor* actorFloor = new PhysicsActor();
	actorFloor->SetObject3D(&object3dFloor);
	actorFloor->Initialize(physicsManager, PhysicsActorType::Static);
	physicsManager->AddPhysicsActor(actorFloor);

	PhysicColliderBox* physicColliderBoxFloor = new PhysicColliderBox();
	physicColliderBoxFloor->Initialize(physicsManager);
	physicColliderBoxFloor->SetSize(glm::vec3(10.0f, 0.1f, 10.0f));
	actorFloor->AddPhysicCollider(physicColliderBoxFloor);
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
		collection->add(*actorIt.second->GetPxRigidActor());
	}

	physx::PxSerialization::complete(*collection, *registry);

	// Binary
	physx::PxSerialization::serializeCollectionToBinary(outStream, *collection, *registry);
	//~Binary

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

	void* mem = new byte[XE_PHYSIC_GET_SIZE_ALIGN(outStream.getSize())];

	memset(mem, 0, XE_PHYSIC_GET_SIZE_ALIGN(outStream.getSize()));

	void* mem128 = XE_PHYSIC_GET_MEM_ALIGN(mem);

	memcpy(mem128, outStream.getData(), outStream.getSize());

	physx::PxCollection* collection2 = physx::PxSerialization::createCollectionFromBinary(mem128, *registry2);

	physicsManager->GetPxScene()->addCollection(*collection2);

	physx::PxSerialObjectId aa;

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

	DeleteMem(physicsManager);

#if defined( _DEBUG )
	MemLeaks::MemoryEnd();
#endif
}

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
