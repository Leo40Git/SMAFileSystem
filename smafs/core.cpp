// core.cpp : Defines shared utility functions and the smafs_* functions.
#include "pch.h"

HRESULT smafs_status;

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
		smafs_status = smafs_success;
		return static_cast<uint32_t>(std::trunc(in));
	}

	smafs_status = smafs_invalid_argument;
	return 0;
}

dllx double smafs_init_raw()
{
	smafs_status = smafs_success;
	return true;
}

dllx double smafs_get_status()
{
	return smafs_status;
}

dllx const char* smafs_get_current_directory()
{
	static wchar_t cwd[4096];
	if (GetCurrentDirectoryW(4096, cwd))
	{
		smafs_status = smafs_success;
		return wcs2str_or_empty(cwd);
	}
	else
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		return "";
	}
}

dllx double smafs_set_current_directory(const char* cwd)
{
	wchar_t* wcwd = str2wcs(cwd);
	if (wcwd == nullptr)
	{
		return false;
	}

	if (SetCurrentDirectoryW(wcwd))
	{
		smafs_status = smafs_success;
	}
	else
	{
		smafs_status = smafs_noop;
	}

	free(wcwd);

	return SUCCEEDED(smafs_status);
}
