#include <Windows.h>
#include <TlHelp32.h>
#include <PsApi.h>
#include <string>
#include <iostream>

class memory
{
public:
	unsigned long long get_base(DWORD procId, const wchar_t* modName);
	DWORD get_pid(const std::wstring& pName);
};