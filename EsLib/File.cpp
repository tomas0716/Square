#include "StdAfx.h"
#include "Include.h"

IFile::IFile(void)
{
	m_hFile	= NULL;
}

IFile::IFile(const char* pszFileName, const bool& bRead)
{
	m_hFile	= NULL;

	CreateFile(pszFileName, bRead);
}

IFile::~IFile(void)
{
	CloseFile();
}

bool	IFile::CreateFile(const char* pszFileName, const bool& bRead)
{
	DWORD	dwDesiredAccess	= bRead ? GENERIC_READ : GENERIC_WRITE;
	DWORD	dwCreationDisposition	= bRead ? OPEN_EXISTING : CREATE_ALWAYS;

	m_hFile	= ::CreateFileA(pszFileName, dwDesiredAccess, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( INVALID_HANDLE_VALUE == m_hFile )
		return	false;

	return	true;
}

bool	IFile::WriteFile(void *pBuffer, const DWORD& dwSize)
{
	DWORD	dwWrittenSize = 0;

	BOOL	bResult	= ::WriteFile(m_hFile, pBuffer, dwSize, &dwWrittenSize, NULL);
	
	if( bResult == FALSE )
		return	false;

	return	true;
}

bool	IFile::ReadFile(void *pBuffer, const DWORD& dwSize)
{
	DWORD	dwReadSize = 0;

	BOOL	bResult	= ::ReadFile(m_hFile, pBuffer, dwSize, &dwReadSize, NULL);

	if( bResult == FALSE )
		return	false;

	return	false;
}

bool	IFile::CloseFile(void)
{
	::CloseHandle(m_hFile);
	m_hFile	= NULL;

	return	true;
}

bool	IFile::CheckFile( const char* pszFileName )
{
	m_hFile	= ::CreateFileA(pszFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( INVALID_HANDLE_VALUE == m_hFile )
		return false;

	CloseFile();
	return true;
}

int		IFile::GetSize()
{
	if( m_hFile != NULL )
	{
		LARGE_INTEGER fileSize;
		::GetFileSizeEx(m_hFile, &fileSize);

		return fileSize.QuadPart;
	}

	return 0;
}