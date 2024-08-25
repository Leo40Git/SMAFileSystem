// core.cpp : Defines shared utility functions and the smafs_* functions.
#include "pch.h"

HRESULT smafs_status;

namespace smafs
{
	LPTSTR A2T(LPCSTR src)
	{
#ifdef UNICODE
		int32_t size = MultiByteToWideChar(CP_UTF8, 0, src, -1, nullptr, 0);
		if (size == 0)
		{
			smafs_status = HRESULT_FROM_WIN32(GetLastError());
			return nullptr;
		}

		LPTSTR dest = (LPTSTR)alloca(size * sizeof(TCHAR));
		if (dest != nullptr)
		{
			if (MultiByteToWideChar(CP_UTF8, 0, src, -1, dest, size) != 0)
			{
				smafs_status = smafs_success;
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
#else
		return (LPTSTR)src;
#endif
	}

	LPSTR T2A(LPCTSTR src)
	{
#ifdef UNICODE
		int32_t size = WideCharToMultiByte(CP_UTF8, 0, src, -1, nullptr, 0, nullptr, nullptr);
		if (size == 0)
		{
			smafs_status = HRESULT_FROM_WIN32(GetLastError());
			return nullptr;
		}

		LPSTR dest = (LPSTR)alloca(size * sizeof(char));
		if (dest != nullptr)
		{
			if (WideCharToMultiByte(CP_UTF8, 0, src, -1, dest, size, nullptr, nullptr) != 0)
			{
				smafs_status = smafs_success;
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
#else
		return (LPSTR)src;
#endif
	}

	int32_t dtoi32(double in)
	{
		if (std::isfinite(in) && in >= INT32_MIN && in <= INT32_MAX)
		{
			smafs_status = smafs_success;
			return static_cast<int32_t>(std::trunc(in));
		}

		smafs_status = smafs_invalid_argument;
		return 0;
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
}

///
dllx double smafs_init_raw()
{
	smafs_status = smafs_success;
	return true;
}

///
dllx double smafs_get_status()
{
	return smafs_status;
}
//#macro smafs_status smafs_get_status()

///
dllx const char* smafs_get_current_directory()
{
	static TCHAR buffer[4096];
	if (GetCurrentDirectory(4096, buffer))
	{
		smafs_status = smafs_success;
		return smafs::T2A_NONNULL(buffer);
	}
	else
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		return "";
	}
}

///
dllx double smafs_set_current_directory(const char* cwd)
{
	LPTSTR lcwd = smafs::A2T(cwd);
	if (lcwd == nullptr)
	{
		return false;
	}

	if (SetCurrentDirectory(lcwd))
	{
		smafs_status = smafs_success;
	}
	else
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
	}

	free(lcwd);
	return SUCCEEDED(smafs_status);
}
