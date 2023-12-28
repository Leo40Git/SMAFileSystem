// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

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

	// called by GameMaker: Studio (hence why this is here - technically it's an entrypoint)
	// see https://web.archive.org/web/20160303070839/https://help.yoyogames.com/hc/en-us/articles/216755258-Returning-Values-From-An-Extension-Asynchronously-GMS-v1-3-
	// note that this is determined by the function name in GameMaker: Studio, not its actual name (as used by GetProcAddress)
	// ...that took me 2 hours to figure out.

	dllx double RegisterCallbacks(const char* arg1, const char* arg2, const char* arg3, const char* arg4)
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
