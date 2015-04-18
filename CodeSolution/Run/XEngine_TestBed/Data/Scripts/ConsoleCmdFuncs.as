//Function Declaration to Pointer
funcdef array<ScriptConsoleLine>@ CALLBACKEXEC(array<wstring>@ subCommands);

//Dictionary to Help
wdictionary Cmd2Help;

void ConsoleInitializeHelpDictionary()
{
	Cmd2Help.deleteAll();

	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_HELP"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_HELP"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_ENGINE"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_ENGINE"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_SHOWGCS"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_SHOWGCS"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_SHOWGCD"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_SHOWGCD"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_SETGCV"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_SETGCV"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_SHOWLOGS"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_SHOWLOGS"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_SAVELOGS"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_SAVELOGS"));
	Cmd2Help.set(m_LocalizationManager.GetLiteral("XE_CONSOLE_CMD_CHANGELANG"), m_LocalizationManager.GetLiteral("XE_CONSOLE_HELP_CHANGELANG"));
}

//Help Commands
array<ScriptConsoleLine>@ ConsoleHelpExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	for (uint i = 0; i < ConsoleCmds.length(); ++i)
	{
		array<wstring> line();
		array<Color> color();

		line.insertLast(ConsoleCmds[i] + ":");
		color.insertLast(GetColorByEnum(XEColorType::Yellow));

		wstring helpStr;
		Cmd2Help.get(ConsoleCmds[i], helpStr);

		line.insertLast(helpStr);
		color.insertLast(GetColorByEnum(XEColorType::White));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleEngineExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	{
		array<wstring> line();
		array<Color> color();

		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_ENGINE_VER") + ":");
		color.insertLast(GetColorByEnum(XEColorType::Yellow));

		line.insertLast(XE_VERSION);
		color.insertLast(GetColorByEnum(XEColorType::Green));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	{
		array<wstring> line();
		array<Color> color();

		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_ENGINE_CODENAME") + ":");
		color.insertLast(GetColorByEnum(XEColorType::Yellow));

		line.insertLast(XE_CODENAME);
		color.insertLast(GetColorByEnum(XEColorType::Green));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleSetGCVExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	if (subCommands.length() < 2)
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_MISS_PARAMS"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	uint id = parseIntW(subCommands[0]);
	if (id == 0)
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_INV_SYNTAX") + ":");
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		lineArr.insertLast(subCommands[0]);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	if (subCommands[1] != "1" && subCommands[1] != "0")
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_INV_SYNTAX") + ":");
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		lineArr.insertLast(subCommands[1]);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

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

	XEResult ret = m_GameComponentCollection.SetGCVisible(id, vis);

	array<wstring> line();
	array<Color> color();

	if (ret == XEResult::Ok)
	{
		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CMD_OK"));
		color.insertLast(GetColorByEnum(XEColorType::Green));
	}
	else
	{
		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CMD_FAIL"));
		color.insertLast(GetColorByEnum(XEColorType::Red));
	}

	ScriptConsoleLine crl;

	@crl.m_SA_Strings = line;
	@crl.m_SA_Colors = color;

	crlArr.insertLast(crl);

	return crlArr;
}

//Show GC Detail
array<ScriptConsoleLine>@ ConsoleShowGCDExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	if (subCommands.length() == 0)
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_MISS_PARAMS"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	uint id = parseIntW(subCommands[0]);
	if (id == 0)
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_INV_SYNTAX") + ":");
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		lineArr.insertLast(subCommands[0]);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	if (m_GameComponentCollection.DoesIDExist(id) == false)
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_INV_ID"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	GCBasicProps gcProp = m_GameComponentCollection.GetGCBasicPropsID(id);

	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();
		lineArr.insertLast("-----------");
		colorArr.insertLast(GetColorByEnum(XEColorType::White));
		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_GC"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Blue));
		lineArr.insertLast("-----------");
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_ID"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Yellow));
		lineArr.insertLast("" + gcProp.m_GCID);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_NAME"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Yellow));
		lineArr.insertLast(gcProp.m_GCName);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CALL_ORDER") + ":");
		colorArr.insertLast(GetColorByEnum(XEColorType::Yellow));
		lineArr.insertLast("" + gcProp.m_GCCallOrder);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_ENABLED") + ":");
		colorArr.insertLast(GetColorByEnum(XEColorType::Yellow));

		wstring strEnable = gcProp.m_Enabled ? m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_YES") : m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_NO");

		lineArr.insertLast(strEnable);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();
		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_DRAW") + ":");
		colorArr.insertLast(GetColorByEnum(XEColorType::Yellow));

		wstring strDrawable = gcProp.m_Drawable ? m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_YES") : m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_NO");

		lineArr.insertLast(strDrawable);
		colorArr.insertLast(GetColorByEnum(XEColorType::White));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);
	}

	{
		if (gcProp.m_Drawable)
		{
			ScriptConsoleLine crl;
			array<wstring> lineArr();
			array<Color> colorArr();
			lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_VISIBLE") + ":");
			colorArr.insertLast(GetColorByEnum(XEColorType::Yellow));

			wstring strVisible = gcProp.m_Visibled ? m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_YES") : m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_NO");

			lineArr.insertLast(strVisible);
			colorArr.insertLast(GetColorByEnum(XEColorType::White));

			@crl.m_SA_Strings = lineArr;
			@crl.m_SA_Colors = colorArr;

			crlArr.insertLast(crl);
		}
	}

	return crlArr;
}

//Show GCs
array<ScriptConsoleLine>@ ConsoleShowGCsExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;
	ScriptConsoleLine crl;

	uint numGCs = m_GameComponentCollection.GetSize();

	array<wstring> lineArr();
	array<Color> colorArr();

	lineArr.insertLast("-----------");
	colorArr.insertLast(GetColorByEnum(XEColorType::White));

	lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_GC_COL"));
	colorArr.insertLast(GetColorByEnum(XEColorType::Blue));

	lineArr.insertLast("-----------");
	colorArr.insertLast(GetColorByEnum(XEColorType::White));

	@crl.m_SA_Strings = lineArr;
	@crl.m_SA_Colors = colorArr;

	crlArr.insertLast(crl);

	for (uint i = 0; i < numGCs; ++i)
	{
		ScriptConsoleLine mCrl;
		GCBasicProps gcProp = m_GameComponentCollection.GetGCBasicProps(i);
		array<wstring> mLineArr();
		array<Color> mColorArr();

		mLineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_ID") + ":");
		mColorArr.insertLast(GetColorByEnum(XEColorType::Yellow));
		mLineArr.insertLast("" + gcProp.m_GCID);
		mColorArr.insertLast(GetColorByEnum(XEColorType::White));

		mLineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_NAME") + ":");
		mColorArr.insertLast(GetColorByEnum(XEColorType::Yellow));
		mLineArr.insertLast(gcProp.m_GCName);
		mColorArr.insertLast(GetColorByEnum(XEColorType::White));

		mLineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CALL_ORDER") + ":");
		mColorArr.insertLast(GetColorByEnum(XEColorType::Yellow));
		mLineArr.insertLast("" + gcProp.m_GCCallOrder);
		mColorArr.insertLast(GetColorByEnum(XEColorType::White));

		@mCrl.m_SA_Strings = mLineArr;
		@mCrl.m_SA_Colors = mColorArr;

		crlArr.insertLast(mCrl);
	}

	ScriptConsoleLine eCrl;
	array<wstring> eLineArr();
	array<Color> eColorArr();

	eLineArr.insertLast("------------------------------------------------------------------");
	eColorArr.insertLast(GetColorByEnum(XEColorType::White));

	@eCrl.m_SA_Strings = eLineArr;
	@eCrl.m_SA_Colors = eColorArr;

	crlArr.insertLast(eCrl);

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleShowLogsExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	array<XELog>@ logsXE = m_Logger.GetLogsScript();

	{
		array<wstring> line();
		array<Color> color();

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		line.insertLast("------------ " + m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_SHOW_LOGS") + " ------------");
		color.insertLast(GetColorByEnum(XEColorType::White));

		crlArr.insertLast(crl);
	}

	for (uint i = 0; i < logsXE.length(); ++i)
	{
		array<wstring> line();
		array<Color> color();

		line.insertLast("" + logsXE[i].m_LogLine + ": ");
		color.insertLast(GetColorByEnum(XEColorType::Yellow));

		line.insertLast(logsXE[i].m_TimeStamp.ToString() + ": ");
		color.insertLast(GetColorByEnum(XEColorType::White));

		line.insertLast(LogLevelStr(logsXE[i].m_Level) + ": ");

		switch (logsXE[i].m_Level)
		{
			case LogLevel::Info:
				color.insertLast(GetColorByEnum(XEColorType::Green));
				break;
			case LogLevel::Warning:
				color.insertLast(GetColorByEnum(XEColorType::Yellow));
				break;
			case LogLevel::Error:
				color.insertLast(GetColorByEnum(XEColorType::Red));
				break;
			case LogLevel::Debug:
				color.insertLast(GetColorByEnum(XEColorType::Blue));
				break;
		}

		line.insertLast(logsXE[i].m_Log);
		color.insertLast(GetColorByEnum(XEColorType::White));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);
	}

	{
		array<wstring> line();
		array<Color> color();

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		line.insertLast("--------------------------------------------------------------");
		color.insertLast(GetColorByEnum(XEColorType::White));

		crlArr.insertLast(crl);
	}

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleSaveLogsExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	//Execute Save logs
	XEResult ret = m_Logger.SaveLogsInFile();

	array<wstring> line();
	array<Color> color();

	if (ret == XEResult::Ok)
	{
		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CMD_OK"));
		color.insertLast(GetColorByEnum(XEColorType::Green));
	}
	else
	{
		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CMD_FAIL"));
		color.insertLast(GetColorByEnum(XEColorType::Red));
	}

	ScriptConsoleLine crl;

	@crl.m_SA_Strings = line;
	@crl.m_SA_Colors = color;

	crlArr.insertLast(crl);

	return crlArr;
}

array<ScriptConsoleLine>@ ConsoleChangeLangExec(array<wstring>@ subCommands)
{
	array<ScriptConsoleLine> crlArr;

	if (subCommands.length() == 0)
	{
		ScriptConsoleLine crl;
		array<wstring> lineArr();
		array<Color> colorArr();

		lineArr.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_MISS_PARAMS"));
		colorArr.insertLast(GetColorByEnum(XEColorType::Red));

		@crl.m_SA_Strings = lineArr;
		@crl.m_SA_Colors = colorArr;

		crlArr.insertLast(crl);

		return crlArr;
	}

	//Execute Save logs
	XEResult ret = m_LocalizationManager.SetDefaultLanguage(subCommands[0]);

	array<wstring> line();
	array<Color> color();

	if (ret == XEResult::Ok)
	{
		InitializeConsoleScript();

		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CMD_OK"));
		color.insertLast(GetColorByEnum(XEColorType::Green));

		line.insertLast(" " + m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CL_HELP"));
		color.insertLast(GetColorByEnum(XEColorType::Yellow));
	}
	else
	{
		line.insertLast(m_LocalizationManager.GetLiteral("XE_CONSOLE_MSG_CMD_FAIL"));
		color.insertLast(GetColorByEnum(XEColorType::Red));
	}

	ScriptConsoleLine crl;

	@crl.m_SA_Strings = line;
	@crl.m_SA_Colors = color;

	crlArr.insertLast(crl);

	return crlArr;
}