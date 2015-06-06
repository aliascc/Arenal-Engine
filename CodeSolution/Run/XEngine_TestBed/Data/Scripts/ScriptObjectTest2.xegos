
#include "ScriptObjectTest1.as"

final class ScriptObjectTest2 : IScriptObject
{
	GameObject@ gameObject;
	int m_Int;
	float m_Float;
	double m_Double;
	bool m_Bool;

	ScriptObjectTest2(GameObject@ gg)
	{
		@gameObject = gg;

		ConsolePrintLine("Constructor 'Script Object Test 2 '");
	}

	~ScriptObjectTest2()
	{
		ConsolePrintLine("Destructor 'Script Object Test 2 '");
	}

	void Initialize() override
	{
		ConsolePrintLine("Initialize 'Script Object Test 2 '");
	}

	void Update(const TimerParams&in timeParams) override
	{
		ConsolePrintLine("Update 'Script Object Test 2 '");

		IScriptObject @objI = m_ScriptCollector.GetScript("ScriptObjectTest1");

		ScriptObjectTest1 @objS = cast<ScriptObjectTest1>(objI);

		objS.PrintMio();

		vec3 rot = gameObject.GetPosition();

		ConsolePrintLine("");

		ConsolePrintLine("Rot x: " + rot.x);
		ConsolePrintLine("Rot y: " + rot.y);
		ConsolePrintLine("Rot z: " + rot.z);

		ConsolePrintLine("");

		gameObject.SetRotation(vec3(7, 8, 9));
	}

	void PostUpdate(const TimerParams&in timeParams) override
	{
		ConsolePrintLine("PostUpdate 'Script Object Test 2 '");

		vec3 nada(5, 6, 7);
		vec3 algo(1.0f, 2.0f, 3.0f);

		ConsolePrintLine("");

		ConsolePrintLine("Nada x: " + nada.x);
		ConsolePrintLine("Nada y: " + nada.y);
		ConsolePrintLine("Nada z: " + nada.z);
		
		ConsolePrintLine("");

		ConsolePrintLine("Algo x: " + algo.x);
		ConsolePrintLine("Algo y: " + algo.y);
		ConsolePrintLine("Algo z: " + algo.z);

		ConsolePrintLine("");

		vec3 suma = nada + algo;

		ConsolePrintLine("Suma x: " + suma.x);
		ConsolePrintLine("Suma y: " + suma.y);
		ConsolePrintLine("Suma z: " + suma.z);

		ConsolePrintLine("");

		algo += suma;

		ConsolePrintLine("Algo x: " + algo.x);
		ConsolePrintLine("Algo y: " + algo.y);
		ConsolePrintLine("Algo z: " + algo.z);

		ConsolePrintLine("");
	}
};