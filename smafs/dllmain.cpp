// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "smafs.h"

HANDLE hHeap;

void* malloc(size_t size)
{
    void* block = HeapAlloc(hHeap, 0, size);
    if (block == nullptr)
    {
        smafs_status = ERROR_NOT_ENOUGH_MEMORY;
    }
    else
    {
        smafs_status = ERROR_SUCCESS;
    }
    return block;
}

void* realloc(void* block, size_t size)
{
    block = HeapReAlloc(hHeap, 0, block, size);
    if (block == nullptr)
    {
        smafs_status = ERROR_NOT_ENOUGH_MEMORY;
    }
    else
    {
        smafs_status = ERROR_SUCCESS;
    }
    return block;
}

void free(void* block) {
    if (block != nullptr)
    {
        if (!HeapFree(hHeap, 0, block))
        {
            // can be safely ignored
            GetLastError(); // to clear it
        }
    }
}

double smafs_status;

dllx double smafs_get_status()
{
    return smafs_status;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        hHeap = GetProcessHeap();
        hFindFile = INVALID_HANDLE_VALUE;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

