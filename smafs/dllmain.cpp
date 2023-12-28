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
	else
	{
		smafs_status = smafs_out_of_memory;
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
	else
	{
		smafs_status = smafs_out_of_memory;
	}

	return dest;
}

uint32_t dtoui32(double in)
{
	if (std::isfinite(in) && in >= 0 && in <= UINT32_MAX)
	{
		smafs_status = smafs_ok;
		return static_cast<uint32_t>(std::trunc(in));
	}

	smafs_status = smafs_invalid_argument;
	return 0;
}

namespace gml
{
	event_perform_async_t event_perform_async;
	ds_map_create_ext_t ds_map_create_ext;
	ds_map_set_real_t ds_map_set_real;
	ds_map_set_string_t ds_map_set_string;

	void init()
	{
		event_perform_async = nullptr;
		ds_map_create_ext = nullptr;
		ds_map_set_real = nullptr;
		ds_map_set_string = nullptr;
	}

	// see https://web.archive.org/web/20160303070839/https://help.yoyogames.com/hc/en-us/articles/216755258-Returning-Values-From-An-Extension-Asynchronously-GMS-v1-3-
	// note that this is determined by the function name in GameMaker: Studio, not its actual name (as used by GetProcAddress)
	// ...that took me 2 hours to figure out.

	dllx double RegisterCallbacks(void* arg1, void* arg2, void* arg3, void* arg4)
	{
		trace("SMAFileSystem: RegisterCallbacks invoked");

		event_perform_async = reinterpret_cast<event_perform_async_t>(arg1);
		ds_map_create_ext = reinterpret_cast<ds_map_create_ext_t>(arg2);
		ds_map_set_real = reinterpret_cast<ds_map_set_real_t>(arg3);
		ds_map_set_string = reinterpret_cast<ds_map_set_string_t>(arg4);

		trace("event_perform_async = 0x%08" PRIXPTR, reinterpret_cast<uintptr_t>(event_perform_async));
		trace("ds_map_create_ext = 0x%08" PRIXPTR, reinterpret_cast<uintptr_t>(ds_map_create_ext));
		trace("ds_map_set_real = 0x%08" PRIXPTR, reinterpret_cast<uintptr_t>(ds_map_set_real));
		trace("ds_map_set_string = 0x%08" PRIXPTR, reinterpret_cast<uintptr_t>(ds_map_set_string));

		return 0;
	}
}

namespace find
{
	void init()
	{
		hFindFile = INVALID_HANDLE_VALUE;
		memset(&ffd, 0, sizeof(ffd));
		dwDesiredAttributes = FILE_ATTRIBUTE_NORMAL;
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
		gml::init();
		find::init();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
