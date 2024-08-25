// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

HANDLE hFindFile;
WIN32_FIND_DATA w32FindData;

void smafs_find_init()
{
	hFindFile = INVALID_HANDLE_VALUE;
	memset(&w32FindData, 0, sizeof(w32FindData));
}

///
dllx const char* file_find_first_sma(const char* mask)
{
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(hFindFile))
		{
			// can be safely ignored
			GetLastError(); // to clear it
		}

		hFindFile = INVALID_HANDLE_VALUE;
	}

	LPTSTR lmask = smafs::A2T(mask);
	if (lmask == nullptr)
	{
		return "";
	}

	// FindExInfoBasic doesn't populate w32FindData.cAlternateFileName
	// ...who's still using short 8.3 filenames?
	hFindFile = FindFirstFileEx(lmask, FindExInfoBasic, &w32FindData,
		FindExSearchNameMatch, nullptr, 0);
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		smafs_status = smafs_success;
	}

	free(lmask);

	if (SUCCEEDED(smafs_status))
		return smafs::T2A_NONNULL(w32FindData.cFileName);
	else
		return "";
}

///
dllx const char* file_find_current_sma()
{
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	smafs_status = smafs_success;
	return smafs::T2A_NONNULL(w32FindData.cFileName);
}

///
dllx double file_find_current_attributes_sma()
{
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return INVALID_FILE_ATTRIBUTES;
	}

	smafs_status = smafs_success;
	return w32FindData.dwFileAttributes;
}

///
dllg int64_t file_find_current_size_sma()
{
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return INVALID_FILE_SIZE;
	}

	return (int64_t)
		((uint64_t)w32FindData.nFileSizeLow | ((uint64_t)w32FindData.nFileSizeHigh << 32));
}

///
dllx const char* file_find_next_sma()
{
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	if (FindNextFile(hFindFile, &w32FindData))
	{
		smafs_status = smafs_success;
		return smafs::T2A_NONNULL(w32FindData.cFileName);
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
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = FindClose(hFindFile)
			? smafs_success
			: HRESULT_FROM_WIN32(GetLastError());

		// clear the handle even if we didn't close it correctly
		//  (since failed closes can be ignored safely)
		hFindFile = INVALID_HANDLE_VALUE;
	}
	else
	{
		// doing nothing is a success in my books
		smafs_status = smafs_noop;
	}

	return SUCCEEDED(smafs_status);
}
