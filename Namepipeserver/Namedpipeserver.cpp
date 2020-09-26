// Namedpipeserver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include"serverpipe.h"

cpipeserver::cpipeserver(wstring strpipename)
{
	m_strpipename = strpipename;
}

void cpipeserver::ConnectNamePipe()
{
	
	OVERLAPPED overlap;
	overlap.hEvent = CreateEvent(NULL, 0, 0, L"overlapserver");
	BOOL hconnect = FALSE;
	//while (1)
	{
		HANDLE hnamedpipe = CreateNamedPipe(m_strpipename.c_str(), PIPE_ACCESS_DUPLEX ,
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			0,                        // client time-out 
			NULL);                    // default security attribute 
		if (hnamedpipe != INVALID_HANDLE_VALUE)
		{
			//cout << "server pipe is created\n";
		}
		else
		{
			return;
		}
		hconnect = ConnectNamedPipe(hnamedpipe, NULL);
		auto fnreadandsendmessagetopipe = [](HANDLE& hnamedpip, LPOVERLAPPED overlp)
		{
			DWORD bytesAvail = 0;
			do
			{
			   PeekNamedPipe(hnamedpip, NULL, 0, NULL, &bytesAvail, NULL);
			   if (bytesAvail == 0)
			   {
				  // DisconnectNamedPipe(hnamedpip);
			   }
			}  while (bytesAvail == 0);
			//{
				DWORD nbuffread = 0;
				HANDLE hHeap = GetProcessHeap();
				TCHAR* pchRequest = (TCHAR*)HeapAlloc(hHeap, 0, BUFSIZE * sizeof(TCHAR));
				//char* buffer = (char*)malloc(sizeof(char) * 20); // Increasing the size will also fix it
				BOOL nresultread = ReadFile(hnamedpip, pchRequest, BUFSIZE * sizeof(TCHAR), &nbuffread, NULL);
				if (nbuffread > 0)
				{
					_tprintf(TEXT("Client Request String:\"%s\"\n"), pchRequest);
					//cout << buffer << "\n";
					return TRUE;
				}
			//}
			
			cout << GetLastError();
			return FALSE;
			
		};
		if (hconnect == TRUE)
		{
			//DWORD dw = WaitForSingleObject(overlap.hEvent, INFINITE);
			DWORD ndwbytes = 0;
			//BOOL fsuccess = GetOverlappedResult(hnamedpipe, &overlap, &ndwbytes, 0);
			//switch (dw)
			{
			//case WAIT_OBJECT_0:
				//if (/*fsuccess == TRUE && */ndwbytes > 0)
				{
					int nresult = fnreadandsendmessagetopipe(hnamedpipe, &overlap);
					if (nresult == false)
					{
						cout << "\n issue occurred while read operation is performed\n";
						//break;
					}
				}
				//else
				//{
				//	DisconnectNamedPipe(hnamedpipe);
				//}
				//break;
			//case WAIT_TIMEOUT:
				////CancelIo(hnamedpipe);
				//CloseHandle(hnamedpipe);
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	cpipeserver objpipe(L"\\\\.\\pipe\\mynamedpipe");
	objpipe.ConnectNamePipe();
	cout << "\nThe server pipe operation is done";
	return 0;
}

