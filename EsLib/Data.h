#pragma once

class IFileInfo : public IGwRefObject
{
public:
	IFileInfo() : m_strPath(""), m_strFileName("") { }

	IFileInfo & operator = ( const IFileInfo & kFileInfo)
	{
		this->m_strPath		= kFileInfo.m_strPath;
		this->m_strFileName	= kFileInfo.m_strFileName;

		return *this;
	}

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{
		char szPath[128]		= { 0, };
		char szFileName[64]		= { 0, };

		memcpy(szPath, pData, sizeof(char) * 128);		pData += sizeof(char) * 128;
		memcpy(szFileName, pData, sizeof(char) * 64);	pData += sizeof(char) * 64;

		m_strPath = szPath;
		m_strFileName = szFileName;

		return pData;
	}
	virtual void		SaveFile(IFile & kFile)		
	{
		kFile.WriteFile((void*)m_strPath.c_str(), sizeof(char) * 128);
		kFile.WriteFile((void*)m_strFileName.c_str(), sizeof(char) * 64);
	}

public:

	string		m_strPath;
	string		m_strFileName;
};

class ITextureFileInfo : public IFileInfo
{
public:
	ITextureFileInfo() : m_nSubMtrlSetIndex(0), m_nTextureIndex(0) { }

	ITextureFileInfo & operator = ( const ITextureFileInfo & kTextureFileInfo)
	{
		this->m_strPath				= kTextureFileInfo.m_strPath;
		this->m_strFileName			= kTextureFileInfo.m_strFileName;
		this->m_nMeshIndex			= kTextureFileInfo.m_nMeshIndex;
		this->m_nSubMtrlSetIndex	= kTextureFileInfo.m_nSubMtrlSetIndex;
		this->m_nTextureIndex		= kTextureFileInfo.m_nTextureIndex;

		return *this;
	}

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{
		pData = IFileInfo::LoadFile(pData);

		memcpy(&m_nMeshIndex, pData, sizeof(int));			pData += sizeof(int);
		memcpy(&m_nSubMtrlSetIndex, pData, sizeof(int));	pData += sizeof(int);
		memcpy(&m_nTextureIndex, pData, sizeof(int));		pData += sizeof(int);

		return pData;
	}

	virtual void		SaveFile(IFile & kFile)		
	{
		IFileInfo::SaveFile(kFile);

		kFile.WriteFile(&m_nMeshIndex, sizeof(int));
		kFile.WriteFile(&m_nSubMtrlSetIndex, sizeof(int));
		kFile.WriteFile(&m_nTextureIndex, sizeof(int));
	}

public:

	int			m_nMeshIndex;
	int			m_nSubMtrlSetIndex;
	int			m_nTextureIndex;
};

class IStaticObjectFileInfo : public IFileInfo
{
public:
	IStaticObjectFileInfo() : m_strTexturePath("") { }

	IStaticObjectFileInfo & operator = ( const IStaticObjectFileInfo & kStaticObjectFileInfo)
	{
		this->m_strPath				= kStaticObjectFileInfo.m_strPath;
		this->m_strFileName			= kStaticObjectFileInfo.m_strFileName;
		this->m_strTexturePath		= kStaticObjectFileInfo.m_strTexturePath;

		return *this;
	}

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{
		pData = IFileInfo::LoadFile(pData);

		char szTexturePath[128]		= { 0, };

		memcpy(szTexturePath, pData, sizeof(char) * 128);	pData += sizeof(char) * 128;

		m_strTexturePath = szTexturePath;

		return pData;
	}

	virtual void		SaveFile(IFile & kFile)		
	{
		IFileInfo::SaveFile(kFile);

		kFile.WriteFile((void*)m_strTexturePath.c_str(), sizeof(char) * 128);
	}


private:
	string		m_strTexturePath;
};