#pragma once

class	IFile
{
public:
	IFile(void);
	IFile(const char* pszFileName, const bool& bRead);
	virtual	~IFile(void);

	bool	CreateFile(const char* pszFileName, const bool& bRead);
	bool	WriteFile(void *pBuffer, const DWORD& dwSize);
	bool	ReadFile(void *pBuffer, const DWORD& dwSize);
	bool	CloseFile(void);
	bool	CheckFile( const char* pszFileName );
	int		GetSize();

	HANDLE	GetHandle(void)		{ return m_hFile; }
private:
	HANDLE	m_hFile;
};