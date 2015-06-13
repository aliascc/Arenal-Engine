

shared class ScriptObjectTest1 : IScriptObject
{
	GameObject@ gameObject;

	ScriptObjectTest1(GameObject@ gg)
	{
		@gameObject = gg;

		ConsolePrintLine("Constructor 'Script Object Test 1 '");
	}

	~ScriptObjectTest1()
	{
		ConsolePrintLine("Destructor 'Script Object Test 1 '");
	}

	void Initialize() override
	{
		ConsolePrintLine("Initialize 'Script Object Test 1 '");
	}

	void Update(const TimerParams&in timeParams) override
	{
		ConsolePrintLine("Update 'Script Object Test 1 '");

		IScriptObject @objI = m_ScriptCollector.GetScript("ScriptObjectTest1");

		ScriptObjectTest1 @objS = cast<ScriptObjectTest1>(objI);

		objS.PrintMio();
	}

	void PostUpdate(const TimerParams&in timeParams) override
	{
		ConsolePrintLine("PostUpdate 'Script Object Test 1 '");
	}

	void PrintMio()
	{
		ConsolePrintLine("Personal Test1 Method");
	}
};