// find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"

const char* file_find_first_sma(const char* mask, double attr)
{
	wchar_t* wmask = str2wcs(mask);
	if (wmask == nullptr)
	{
		return "";
	}

	if (gstate_find.hFindFile != INVALID_HANDLE_VALUE)
	{
		if (!FindClose(gstate_find.hFindFile))
		{
			// can be safely ignored
			GetLastError(); // to clear it
		}
		
		gstate_find.hFindFile = INVALID_HANDLE_VALUE;
	}

	// FindExInfoBasic doesn't populate ffd.cAlternateFileName
	// ...who's still using short 8.3 filenames?
	gstate_find.hFindFile = FindFirstFileExW(wmask, FindExInfoBasic, &gstate_find.ffd,
		FindExSearchNameMatch, nullptr, 0);
	
	if (gstate_find.hFindFile != INVALID_HANDLE_VALUE)
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		free(wmask);
		return wcs2str_or_empty(gstate_find.ffd.cFileName);
	}
	else
	{
		free(wmask);
		return "";
	}
}

const char* file_find_next_sma()
{
	if (gstate_find.hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = HRESULT_FROM_WIN32(ERROR_NOT_READY);
		return "";
	}
	else if (FindNextFileW(gstate_find.hFindFile, &gstate_find.ffd))
	{
		smafs_status = S_OK;
		return wcs2str_or_empty(gstate_find.ffd.cFileName);
	}
	else
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		return "";
	}
}

double file_find_close_sma()
{
	if (gstate_find.hFindFile == INVALID_HANDLE_VALUE)
	{
		// doing nothing is a success, in my books
		smafs_status = S_OK;
		return true;
	}
	else if (FindClose(gstate_find.hFindFile))
	{
		smafs_status = S_OK;
		gstate_find.hFindFile = INVALID_HANDLE_VALUE;
		return true;
	}
	else
	{
		smafs_status = HRESULT_FROM_WIN32(GetLastError());
		gstate_find.hFindFile = INVALID_HANDLE_VALUE;
		return false;
	}
}
