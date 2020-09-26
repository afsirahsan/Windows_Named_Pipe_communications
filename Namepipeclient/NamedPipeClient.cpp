// NamedPipeClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BUFSIZE 512

int _tmain(int argc, _TCHAR* argv[])
{
	OVERLAPPED hclientoverlap;
	hclientoverlap.hEvent = CreateEvent(NULL, TRUE, TRUE, L"overlapserver");
	//while (1)
	{
		HANDLE hnamedpipecl = CreateFile(L"\\\\.\\pipe\\mynamedpipe",   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			FILE_ATTRIBUTE_NORMAL, // default attributes 
			0);          // no template file 
		if (hnamedpipecl != INVALID_HANDLE_VALUE)
		{
			//DWORD dwMode = PIPE_ACCESS_DUPLEX;
			//BOOL fSuccess = SetNamedPipeHandleState(
			//	hnamedpipecl,    // pipe handle 
			//	&dwMode,  // new pipe mode 
			//	NULL,     // don't set maximum bytes 
			//	NULL);    // don't set maximum time 
			//if (!fSuccess)
			//{
			//	_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
			//	return (0);
			//}
			hclientoverlap.hEvent = OpenEvent(NULL, TRUE, L"overlapserver");
			//BOOL dw = WaitNamedPipe(L"\\\\.\\pipe\\mynamedpipe", INFINITE);
			//if (dw)
			{
				char chbuff[1000];
				DWORD ndwbytes = 0;
				BOOL bwritten = FALSE;
				HANDLE hHeap = GetProcessHeap();
				TCHAR* pchReply = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));
				wstring wstr = L"data from client hello server";
				DWORD pchBytes = (lstrlen(pchReply) + 1) * sizeof(TCHAR);
				//wcscpy_s(pchReply, wstr.c_str());
				while (bwritten != TRUE)
				{
					bwritten = WriteFile(hnamedpipecl, wstr.c_str(), sizeof(wchar_t)*lstrlen(wstr.c_str()), &pchBytes, NULL);
				}
				
				if (bwritten)
				{
					SetEvent(hclientoverlap.hEvent);
					cout << "\n message sent to client\n";
					CloseHandle(hnamedpipecl);

				}
				else
				{
					cout << "lasterror=" << GetLastError() << "\n";
				}
			}
		}
		else
		{
			cout << "createfile fails lasterror=" << GetLastError() << "\n";
		}
	}
	
	return 0;
}

