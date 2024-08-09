// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

HANDLE smafs_find_handle;
WIN32_FIND_DATAW smafs_find_data;

void smafs_find_init()
{
	smafs_find_handle = INVALID_HANDLE_VALUE;
	memset(&smafs_find_data, 0, sizeof(smafs_find_data));
}

///
dllx const char* file_find_first_sma(const char* mask)
{
	wchar_t* wmask = str2wcs(mask);
	if (wmask == nullptr)
	{
		return "";
	}

	if (smafs_find_handle != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(smafs_find_handle))
		{
			// can be safely ignored
			GetLastError(); // to clear it
		}

		smafs_find_handle = INVALID_HANDLE_VALUE;
	}

	// FindExInfoBasic doesn't populate smafs_find_data.cAlternateFileName
	// ...who's still using short 8.3 filenames?
	smafs_find_handle = FindFirstFileExW(wmask, FindExInfoBasic, &smafs_find_data,
		FindExSearchNameMatch, nullptr, 0);

	HRESULT new_status = smafs_success;
	if (smafs_find_handle == INVALID_HANDLE_VALUE)
	{
		new_status = HRESULT_FROM_WIN32(GetLastError());
	}

	free(wmask);

	smafs_status = new_status;

	return SUCCEEDED(new_status)
		? wcs2str_or_empty(smafs_find_data.cFileName)
		: "";
}

///
dllx const char* file_find_current_sma()
{
	if (smafs_find_handle == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	smafs_status = smafs_success;
	return wcs2str_or_empty(smafs_find_data.cFileName);
}

///
dllx double file_find_current_attributes_sma()
{
	if (smafs_find_handle == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return INVALID_FILE_ATTRIBUTES;
	}

	smafs_status = smafs_success;
	return smafs_find_data.dwFileAttributes;
}

///
dllg int64_t file_find_current_size_sma()
{
	if (smafs_find_handle == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return -1; // INVALID_FILE_SIZE
	}

	// I... have no clue why this is split between two fields, tbh 
	return (int64_t)((uint64_t)smafs_find_data.nFileSizeLow | ((uint64_t)smafs_find_data.nFileSizeHigh << 32));
}

///
dllx const char* file_find_next_sma()
{
	if (smafs_find_handle == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	if (FindNextFileW(smafs_find_handle, &smafs_find_data))
	{
		smafs_status = smafs_success;
		return wcs2str_or_empty(smafs_find_data.cFileName);
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
	if (smafs_find_handle == INVALID_HANDLE_VALUE)
	{
		smafs_status = FindClose(smafs_find_handle)
			? smafs_success
			: HRESULT_FROM_WIN32(GetLastError());

		// clear the handle even if we didn't close it correctly
		//  (since failed closes can be ignored safely)
		smafs_find_handle = INVALID_HANDLE_VALUE;
	}
	else
	{
		// doing nothing is a success in my books
		smafs_status = smafs_noop;
	}

	return SUCCEEDED(smafs_status);
}
