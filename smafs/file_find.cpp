// file_find.cpp : Defines the file_find_*_sma functions.
#include "pch.h"
#include "smafs.h"

HANDLE hFindFile;
WIN32_FIND_DATAW ffd;

const char* file_find_first_sma(const char* mask, double attr)
{
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

	hFindFile = FindFirstFileExW(wmask, FindExInfoBasic, &ffd,
		FindExSearchNameMatch, nullptr, 0);
	
	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		smafs_status = GetLastError();
		free(wmask);
		return wcs2str_notnull(ffd.cFileName);
	}
	else
	{
		free(wmask);
		return "";
	}
}

const char* file_find_next_sma()
{
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = ERROR_NOT_READY;
		return "";
	}
	else if (FindNextFileW(hFindFile, &ffd))
	{
		smafs_status = ERROR_SUCCESS;
		return wcs2str_notnull(ffd.cFileName);
	}
	else
	{
		smafs_status = GetLastError();
		return "";
	}
}

double file_find_close_sma()
{
	if (hFindFile == INVALID_HANDLE_VALUE)
	{
		smafs_status = ERROR_NOT_READY;
		return false;
	}

	if (FindClose(hFindFile))
	{
		smafs_status = ERROR_SUCCESS;
		hFindFile = INVALID_HANDLE_VALUE;
		return true;
	}
	else
	{
		smafs_status = GetLastError();
		hFindFile = INVALID_HANDLE_VALUE;
		return false;
	}
}
