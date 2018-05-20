//Function Declaration to Pointer
funcdef array<ScriptConsoleLine>@ CALLBACKEXEC(array<string>@ subCommands);

//Dictionary to Help
wdictionary Cmd2Help;

void ConsoleInitializeHelpDictionary()
{
	Cmd2Help.deleteAll();

	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_HELP"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_HELP"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_ENGINE"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_ENGINE"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWGCS"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_SHOWGCS"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWGCD"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_SHOWGCD"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SETGCV"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_SETGCV"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWLOGS"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_SHOWLOGS"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SAVELOGS"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_SAVELOGS"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_CHANGELANG"), m_LocalizationManager.GetLiteral("AE_CONSOLE_HELP_CHANGELANG"));
}

//Help Commands
array<ScriptConsoleLine>@ ConsoleHelpExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	for (uint i = 0; i < ConsoleCmds.length(); ++i)
	{
		array<string> line();
		array<Color> color();

		line.insertLast(ConsoleCmds[i] + ":");
		color.insertLast(GetColorByEnum(AEColorType::Yellow));

		string helpStr;
		Cmd2Help.get(ConsoleCmds[i], helpStr);

		line.insertLast(helpStr);
		color.insertLast(GetColorByEnum(AEColorType::White));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleEngineExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	{
		array<string> line();
		array<Color> color();

		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_ENGINE_VER") + ":");
		color.insertLast(GetColorByEnum(AEColorType::Yellow));

		line.insertLast(AE_VERSION);
		color.insertLast(GetColorByEnum(AEColorType::Green));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	{
		array<string> line();
		array<Color> color();

		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_ENGINE_CODENAME") + ":");
		color.insertLast(GetColorByEnum(AEColorType::Yellow));

		line.insertLast(AE_CODENAME);
		color.insertLast(GetColorByEnum(AEColorType::Green));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleSetGCVExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	if (subCommands.length() < 2)
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_MISS_PARAMS"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	uint id = parseIntW(subCommands[0]);
	if (id == 0)
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_INV_SYNTAX") + ":");
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		lineArr.insertLast(subCommands[0]);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	if (subCommands[1] != "1" && subCommands[1] != "0")
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_INV_SYNTAX") + ":");
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		lineArr.insertLast(subCommands[1]);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	bool vis = true;
	if (subCommands[1] != "1")
	{
		vis = false;
	}

	AEResult ret = m_GameComponentCollection.SetGCVisible(id, vis);

	array<string> line();
	array<Color> color();

	if (ret == AEResult::Ok)
	{
		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CMD_OK"));
		color.insertLast(GetColorByEnum(AEColorType::Green));
	}
	else
	{
		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CMD_FAIL"));
		color.insertLast(GetColorByEnum(AEColorType::Red));
	}

	ScriptConsoleLine crl;

	@crl.m_SA_Strings = line;
	@crl.m_SA_Colors = color;

	crlArr.insertLast(crl);

	return crlArr;
}

//Show GC Detail
array<ScriptConsoleLine>@ ConsoleShowGCDExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	if (subCommands.length() == 0)
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_MISS_PARAMS"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	uint id = parseIntW(subCommands[0]);
	if (id == 0)
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_INV_SYNTAX") + ":");
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		lineArr.insertLast(subCommands[0]);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	if (m_GameComponentCollection.DoesIDExist(id) == false)
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_INV_ID"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	GCBasicProps gcProp = m_GameComponentCollection.GetGCBasicPropsID(id);

	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();
		lineArr.insertLast("-----------");
		colorArr.insertLast(GetColorByEnum(AEColorType::White));
		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_GC"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Blue));
		lineArr.insertLast("-----------");
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_ID"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Yellow));
		lineArr.insertLast("" + gcProp.m_GCID);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_NAME"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Yellow));
		lineArr.insertLast(gcProp.m_GCName);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CALL_ORDER") + ":");
		colorArr.insertLast(GetColorByEnum(AEColorType::Yellow));
		lineArr.insertLast("" + gcProp.m_GCCallOrder);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_ENABLED") + ":");
		colorArr.insertLast(GetColorByEnum(AEColorType::Yellow));

		string strEnable = gcProp.m_Enabled ? m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_YES") : m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_NO");

		lineArr.insertLast(strEnable);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_DRAW") + ":");
		colorArr.insertLast(GetColorByEnum(AEColorType::Yellow));

		string strDrawable = gcProp.m_Drawable ? m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_YES") : m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_NO");

		lineArr.insertLast(strDrawable);
		colorArr.insertLast(GetColorByEnum(AEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		if (gcProp.m_Drawable)
		{
			ScriptConsoleLine crl;
			array<string> lineArr();
			array<Color> colorArr();
			lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_VISIBLE") + ":");
			colorArr.insertLast(GetColorByEnum(AEColorType::Yellow));

			string strVisible = gcProp.m_Visibled ? m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_YES") : m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_NO");

			lineArr.insertLast(strVisible);
			colorArr.insertLast(GetColorByEnum(AEColorType::White));

			@crl.m_SA_Strings = lineArr;
			@crl.m_SA_Colors = colorArr;

			crlArr.insertLast(crl);
		}
	}

	return crlArr;
}

//Show GCs
array<ScriptConsoleLine>@ ConsoleShowGCsExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;
	ScriptConsoleLine crl;

	uint numGCs = m_GameComponentCollection.GetSize();

	array<string> lineArr();
	array<Color> colorArr();

	lineArr.insertLast("-----------");
	colorArr.insertLast(GetColorByEnum(AEColorType::White));

	lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_GC_COL"));
	colorArr.insertLast(GetColorByEnum(AEColorType::Blue));

	lineArr.insertLast("-----------");
	colorArr.insertLast(GetColorByEnum(AEColorType::White));

	@crl.m_SA_Strings = lineArr;
	@crl.m_SA_Colors = colorArr;

	crlArr.insertLast(crl);

	for (uint i = 0; i < numGCs; ++i)
	{
		ScriptConsoleLine mCrl;
		GCBasicProps gcProp = m_GameComponentCollection.GetGCBasicProps(i);
		array<string> mLineArr();
		array<Color> mColorArr();

		mLineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_ID") + ":");
		mColorArr.insertLast(GetColorByEnum(AEColorType::Yellow));
		mLineArr.insertLast("" + gcProp.m_GCID);
		mColorArr.insertLast(GetColorByEnum(AEColorType::White));

		mLineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_NAME") + ":");
		mColorArr.insertLast(GetColorByEnum(AEColorType::Yellow));
		mLineArr.insertLast(gcProp.m_GCName);
		mColorArr.insertLast(GetColorByEnum(AEColorType::White));

		mLineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CALL_ORDER") + ":");
		mColorArr.insertLast(GetColorByEnum(AEColorType::Yellow));
		mLineArr.insertLast("" + gcProp.m_GCCallOrder);
		mColorArr.insertLast(GetColorByEnum(AEColorType::White));

		@mCrl.m_SA_Strings = mLineArr;
		@mCrl.m_SA_Colors = mColorArr;

		crlArr.insertLast(mCrl);
	}

	ScriptConsoleLine eCrl;
	array<string> eLineArr();
	array<Color> eColorArr();

	eLineArr.insertLast("------------------------------------------------------------------");
	eColorArr.insertLast(GetColorByEnum(AEColorType::White));

	@eCrl.m_SA_Strings = eLineArr;
	@eCrl.m_SA_Colors = eColorArr;

	crlArr.insertLast(eCrl);

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleShowLogsExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	array<AELog>@ logsXE = m_Logger.GetLogsScript();

	{
		array<string> line();
		array<Color> color();

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		line.insertLast("------------ " + m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_SHOW_LOGS") + " ------------");
		color.insertLast(GetColorByEnum(AEColorType::White));

		crlArr.insertLast(crl);
	}

	for (uint i = 0; i < logsXE.length(); ++i)
	{
		array<string> line();
		array<Color> color();

		line.insertLast("" + logsXE[i].m_LogLine + ": ");
		color.insertLast(GetColorByEnum(AEColorType::Yellow));

		line.insertLast(logsXE[i].m_TimeStamp.ToString() + ": ");
		color.insertLast(GetColorByEnum(AEColorType::White));

		line.insertLast(LogLevelStr(logsXE[i].m_Level) + ": ");

		switch (logsXE[i].m_Level)
		{
			case LogLevel::Info:
				color.insertLast(GetColorByEnum(AEColorType::Green));
				break;
			case LogLevel::Warning:
				color.insertLast(GetColorByEnum(AEColorType::Yellow));
				break;
			case LogLevel::Error:
				color.insertLast(GetColorByEnum(AEColorType::Red));
				break;
			case LogLevel::Debug:
				color.insertLast(GetColorByEnum(AEColorType::Blue));
				break;
		}

		line.insertLast(logsXE[i].m_Log);
		color.insertLast(GetColorByEnum(AEColorType::White));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	{
		array<string> line();
		array<Color> color();

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		line.insertLast("--------------------------------------------------------------");
		color.insertLast(GetColorByEnum(AEColorType::White));

		crlArr.insertLast(crl);
	}

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleSaveLogsExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	//Execute Save logs
	AEResult ret = m_Logger.SaveLogsInFile();

	array<string> line();
	array<Color> color();

	if (ret == AEResult::Ok)
	{
		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CMD_OK"));
		color.insertLast(GetColorByEnum(AEColorType::Green));
	}
	else
	{
		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CMD_FAIL"));
		color.insertLast(GetColorByEnum(AEColorType::Red));
	}

	ScriptConsoleLine crl;

	@crl.m_SA_Strings = line;
	@crl.m_SA_Colors = color;

	crlArr.insertLast(crl);

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleChangeLangExec(array<string>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	if (subCommands.length() == 0)
	{
		ScriptConsoleLine crl;
		array<string> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_MISS_PARAMS"));
		colorArr.insertLast(GetColorByEnum(AEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	//Execute Save logs
	AEResult ret = m_LocalizationManager.SetDefaultLanguage(subCommands[0]);

	array<string> line();
	array<Color> color();

	if (ret == AEResult::Ok)
	{
		InitializeConsoleScript();

		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CMD_OK"));
		color.insertLast(GetColorByEnum(AEColorType::Green));

		line.insertLast(" " + m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CL_HELP"));
		color.insertLast(GetColorByEnum(AEColorType::Yellow));
	}
	else
	{
		line.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_CMD_FAIL"));
		color.insertLast(GetColorByEnum(AEColorType::Red));
	}

	ScriptConsoleLine crl;

	@crl.m_SA_Strings = line;
	@crl.m_SA_Colors = color;

	crlArr.insertLast(crl);

	return crlArr;
}