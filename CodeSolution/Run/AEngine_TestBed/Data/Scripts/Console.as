#include "ConsoleCmdFuncs.as"

//Console Available Commands
array<wstring> ConsoleCmds;

//Dictionary to Functions;
wdictionary Cmd2Func;

void ConsoleInitializeDictionary()
{
	Cmd2Help.deleteAll();

	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_HELP"), @ConsoleHelpExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_ENGINE"), @ConsoleEngineExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWGCS"), @ConsoleShowGCsExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWGCD"), @ConsoleShowGCDExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SETGCV"), @ConsoleSetGCVExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWLOGS"), @ConsoleShowLogsExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SAVELOGS"), @ConsoleSaveLogsExec);
	Cmd2Func.set(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_CHANGELANG"), @ConsoleChangeLangExec);
}

void ConsoleInitializeStringCommands()
{
	ConsoleCmds.resize(0);

	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_HELP"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_ENGINE"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWGCS"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWGCD"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SETGCV"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SHOWLOGS"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_SAVELOGS"));
	ConsoleCmds.insertLast(m_LocalizationManager.GetLiteral("AE_CONSOLE_CMD_CHANGELANG"));
}

void InitializeConsoleScript()
{
	ConsoleInitializeStringCommands();
	ConsoleInitializeDictionary();
	ConsoleInitializeHelpDictionary();
}

void ConsoleExec(wstring command)
{
	uint i = 0;
	array<wstring>@ subCommands = command.split(" ");

	//Clean empty spaces
	array<uint> emptySpaces;
	for (i = 0; i < subCommands.length(); ++i)
	{
		if (subCommands[i] == "")
		{
			emptySpaces.insertLast(i);
		}
	}

	for (i = 0; i < emptySpaces.length(); ++i)
	{
		subCommands.removeAt(emptySpaces[i]);
	}

	array<ScriptConsoleLine> crl;

	//Send to Appropriate Function to execute
	for (i = 0; i < ConsoleCmds.length(); ++i)
	{
		if (subCommands[0] == ConsoleCmds[i])
		{
			CALLBACKEXEC @handle;
			Cmd2Func.get(ConsoleCmds[i], @handle);

			subCommands.removeAt(0);

			crl = handle(subCommands);

			break;
		}
	}

	if (i == ConsoleCmds.length())
	{
		array<ScriptConsoleLine> crlArr;

		array<wstring> line;
		array<Color> color;

		wstring ukcmmd = m_LocalizationManager.GetLiteral("AE_CONSOLE_MSG_UNK_CMD") + ":";
		line.insertLast(ukcmmd);

		Color colorCode(GetColorByEnum(AEColorType::Red));
		color.insertLast(colorCode);

		line.insertLast(subCommands[0]);
		color.insertLast(GetColorByEnum(AEColorType::White));

		ScriptConsoleLine crl;

		@crl.m_SA_Strings = line;
		@crl.m_SA_Colors = color;

		crlArr.insertLast(crl);

		@m_ScriptConsoleLineArray = crlArr;
	}
	else
	{
		@m_ScriptConsoleLineArray = crl;
	}
}