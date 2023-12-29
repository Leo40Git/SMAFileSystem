// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

namespace find
{
	HANDLE hFindFile;
	WIN32_FIND_DATAW ffd;

	void init()
	{
		hFindFile = INVALID_HANDLE_VALUE;
		memset(&ffd, 0, sizeof(ffd));
	}
}

// unfortunately the functions can't actually be *in* the namespace
//  because it breaks dllg (linker issue)
// luckily, the "using namespace" statement exists

///
dllx const char* file_find_first_sma(const char* mask)
{
	using namespace find;

	wchar_t* wmask = str2wcs(mask);
	if (wmask == nullptr)
	{
		return "";
	}

	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(hFindFile))
		{
			// can be safely ignored
			GetLastError(); // to clear it
		}

		hFindFile = INVALID_HANDLE_VALUE;
	}

	// FindExInfoBasic doesn't populate ffd.cAlternateFileName
	// ...who's still using short 8.3 filenames?
	hFindFile = FindFirstFileExW(wmask, FindExInfoBasic, &ffd,
		FindExSearchNameMatch, nullptr, 0);

	HRESULT new_status = smafs_success;
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		new_status = HRESULT_FROM_WIN32(GetLastError());
	}

	free(wmask);

	smafs_status = new_status;

	return SUCCEEDED(new_status)
		? wcs2str_or_empty(ffd.cFileName)
		: "";
}

///
dllx const char* file_find_current_sma()
{
	using namespace find;

	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	smafs_status = smafs_success;
	return wcs2str_or_empty(ffd.cFileName);
}

///
dllx double file_find_current_attributes_sma()
{
	using namespace find;

	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return INVALID_FILE_ATTRIBUTES;
	}

	smafs_status = smafs_success;
	return ffd.dwFileAttributes;
}

///
dllg int64_t file_find_current_size_sma()
{
	using namespace find;

	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return -1; // INVALID_FILE_SIZE
	}

	// I... have no clue why this is split between two fields, tbh 
	return (int64_t)((uint64_t)ffd.nFileSizeLow | ((uint64_t)ffd.nFileSizeHigh << 32));
}

///
dllx const char* file_find_next_sma()
{
	using namespace find;

	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = smafs_invalid_operation;
		return "";
	}

	if (FindNextFileW(hFindFile, &ffd))
	{
		smafs_status = smafs_success;
		return wcs2str_or_empty(ffd.cFileName);
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
	using namespace find;

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
