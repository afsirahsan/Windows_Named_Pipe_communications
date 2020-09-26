#include "stdafx.h"
#define BUFSIZE 512

class cpipeserver
{
public:
	cpipeserver(wstring pipename);
	void ConnectNamePipe();
private:
	wstring m_strpipename;
};