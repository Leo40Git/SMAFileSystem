#pragma once

#define _WIN32_WINNT  0x0501            // Target Windows XP
#define NTDDI_VERSION 0x05010300        // Windows XP Service Pack 3, specifically
#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <Windows.h>
