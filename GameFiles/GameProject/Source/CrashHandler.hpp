/**
 * @file CrashHandler.hpp
 * @author Wonhyeong Chae (w.chae@digipen.edu)
 * @brief The goal of the project is to create a crash handler that can be dropped into a game or other project and provide useful information in the event of a crash.
 * @version 0.1
 * @date 2023-09-20
 */
#pragma comment(lib, "dbghelp.lib")

#include <windows.h>
#include <dbghelp.h>
#include <string>
#include <chrono>

 /// @brief This function will write a dump file to the current directory.
 /// @param pException A pointer to the exception that caused the crash.
 /// @return EXCEPTION_CONTINUE_SEARCH
LONG WINAPI WriteDump(EXCEPTION_POINTERS* pException)
{
	if (!IsDebuggerPresent())
	{
		TCHAR szFileName[MAX_PATH];
		TCHAR szModuleName[MAX_PATH];
		TCHAR* szAppName;

		// Retrieve the full path of the current executable
		if (GetModuleFileName(NULL, szModuleName, MAX_PATH))
		{
			// Extract the file name from the full path
			szAppName = wcsrchr(szModuleName, '\\');

			if (szAppName)
			{
				szAppName++;
				wsprintf(szFileName, L"%s.dmp", szAppName);
			}
			else
				wcscpy_s(szFileName, MAX_PATH, L"project.dmp");
		}
		else
			wcscpy_s(szFileName, MAX_PATH, L"project.dmp");

		HANDLE miniDump = CreateFile(szFileName, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		MINIDUMP_EXCEPTION_INFORMATION miniDumpExceptionInfo = {};

		miniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
		miniDumpExceptionInfo.ExceptionPointers = pException;
		miniDumpExceptionInfo.ClientPointers = FALSE;

		MiniDumpWriteDump(GetCurrentProcess(),
			GetCurrentProcessId(),
			miniDump,
			MiniDumpNormal,
			&miniDumpExceptionInfo,
			NULL,
			NULL);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}