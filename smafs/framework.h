#pragma once

#define _WIN32_WINNT	0x0501	// Target Windows XP
#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <Windows.h>

#include <cstdlib>				// malloc and free, because we transfer ownership of our buffers
#include <cstdint>				// I prefer these typedefs over Windows' screamy ones
#include <cstdio>				// stdout
#include <cstring>				// memcpy
#include <cinttypes>			// printf format helpers
