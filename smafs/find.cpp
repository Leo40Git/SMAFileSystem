// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

HANDLE hFind;
WIN32_FIND_DATA findData;

void smafs_find_init()
{
	hFind = INVALID_HANDLE_VALUE;
	memset(&findData, 0, sizeof(findData));
}

///
dllx const char* file_find_first_sma(const char* mask)
{
	if (hFind != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(hFind))
		{
			// can be safely ignored
			GetLastError(); // to clear it
		}

		hFind = INVALID_HANDLE_VALUE;
	}

	LPTSTR lmask = Utils::MultiByteToTChar(mask);
	if (lmask == nullptr)
	{
		return "";
	}

	// FindExInfoBasic doesn't populate smafs_find_data.cAlternateFileName
	// ...who's still using short 8.3 filenames?
	hFind = FindFirstFileEx(lmask, FindExInfoBasic, &findData,
		FindExSearchNameMatch, nullptr, 0);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		smafs_status = smafs_success;
	}

	free(lmask);

	if (SUCCEEDED(smafs_status))
		return Utils::TCharToMultiByteOrEmpty(findData.cFileName);
	else
		return "";
}

///
dllx const char* file_find_current_sma()
{
	if (hFind == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	smafs_status = smafs_success;
	return Utils::TCharToMultiByteOrEmpty(findData.cFileName);
}

///
dllx double file_find_current_attributes_sma()
{
	if (hFind == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return INVALID_FILE_ATTRIBUTES;
	}

	smafs_status = smafs_success;
	return findData.dwFileAttributes;
}

///
dllg int64_t file_find_current_size_sma()
{
	if (hFind == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return INVALID_FILE_SIZE;
	}

	return (int64_t)
		((uint64_t)findData.nFileSizeLow | ((uint64_t)findData.nFileSizeHigh << 32));
}

///
dllx const char* file_find_next_sma()
{
	if (hFind == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	if (FindNextFile(hFind, &findData))
	{
		smafs_status = smafs_success;
		return Utils::TCharToMultiByteOrEmpty(findData.cFileName);
	}
	else
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		return "";
	}
}

///
dllx double file_find_close_sma()
{
	if (hFind == INVALID_HANDLE_VALUE)
	{
		smafs_status = FindClose(hFind)
			? smafs_success
			: HRESULT_FROM_WIN32(GetLastError());

		// clear the handle even if we didn't close it correctly
		//  (since failed closes can be ignored safely)
		hFind = INVALID_HANDLE_VALUE;
	}
	else
	{
		// doing nothing is a success in my books
		smafs_status = smafs_noop;
	}

	return SUCCEEDED(smafs_status);
}
