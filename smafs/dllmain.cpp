// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

HRESULT smafs_status;

dllx double smafs_get_status()
{
    return smafs_status;
}

wchar_t* str2wcs(const char* src, uint32_t codepage)
{
	int32_t size = MultiByteToWideChar(codepage, 0, src, -1, nullptr, 0);
	if (size == 0)
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		return nullptr;
	}

	wchar_t* dest = calloc<wchar_t>(size);
	if (dest != nullptr)
	{
		if (MultiByteToWideChar(codepage, 0, src, -1, dest, size) != 0)
		{
			smafs_status = 0;
		}
		else
		{
			smafs_status = HRESULT_FROM_WIN32(GetLastError());
			free(dest);
			dest = nullptr;
		}
	}

	return dest;
}

char* wcs2str(const wchar_t* src, uint32_t codepage)
{
	int32_t size = WideCharToMultiByte(codepage, 0, src, -1, nullptr, 0, nullptr, nullptr);
	if (size == 0)
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		return nullptr;
	}

	char* dest = calloc<char>(size);
	if (dest != nullptr)
	{
		if (WideCharToMultiByte(codepage, 0, src, -1, dest, size, nullptr, nullptr) != 0)
		{
			smafs_status = 0;
		}
		else
		{
			smafs_status = HRESULT_FROM_WIN32(GetLastError());
			free(dest);
			dest = nullptr;
		}
	}

	return dest;
}

global_state_find_t gstate_find;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        gstate_find.init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

