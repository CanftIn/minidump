#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <Dbghelp.h>

#pragma auto_inline (off)

#pragma comment( lib, "DbgHelp" )

void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)
{
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	CloseHandle(hDumpFile);
}

LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
{
	CreateDumpFile(_T("Test.dmp"), pException);

	return EXCEPTION_EXECUTE_HANDLER;
}

class CrashTest
{
public:
	void Test()
	{
		Crash();
	}
private:
	int Crash()
	{
		int* i = NULL;
		*i = 1;
		return 1;
	}
};

int main(int argc, _TCHAR* argv[])
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

	CrashTest test;
	test.Test();
	return 0;
}