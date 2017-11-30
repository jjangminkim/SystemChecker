// CheckSystem.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "CheckSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 유일한 응용 프로그램 개체입니다.

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// MFC를 초기화합니다. 초기화하지 못한 경우 오류를 인쇄합니다.
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 오류 코드를 필요에 따라 수정합니다.
			_tprintf(_T("Fatal Error: Init MFC Failed.\n"));
			nRetCode = 1;
		}
		else
		{
			// 
            wprintf(L"test");
		}
	}
	else
	{
		// TODO: 오류 코드를 필요에 따라 수정합니다.
		_tprintf(_T("Fatal Error: GetModuleHandle Failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
