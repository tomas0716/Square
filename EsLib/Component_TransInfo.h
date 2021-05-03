#pragma once

//////////////////////////////////////////////////////////////////////////
// ITransInfo

class ITransInfo : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(ITransInfo);
public:
	//ITransInfo() : m_fTime(0) { }
	ITransInfo();

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{
		memcpy(&m_fTime, pData, sizeof(float));		pData += sizeof(float);

		return pData;
	}
	virtual void		SaveFile(IFile & kFile)	
	{
		kFile.WriteFile(&m_fTime, sizeof(float)); 
	}

public:
	float		m_fTime;
};

//////////////////////////////////////////////////////////////////////////
// ITransScaleInfo

class ITransScaleInfo : public ITransInfo
{
	I_DECLARE_DYNCREATE(ITransScaleInfo);
public:
	//ITransScaleInfo() : m_vTransScale(GwVector(1,1,1))	{ }
	ITransScaleInfo();

	virtual BYTE	*	LoadFile(BYTE * pData)	
	{ 
		pData = ITransInfo::LoadFile(pData);
		memcpy(&m_vTransScale, pData, sizeof(GwVector));	pData += sizeof(GwVector);

		return pData;
	}
	virtual void		SaveFile(IFile & kFile)	
	{ 
		ITransInfo::SaveFile(kFile);
		kFile.WriteFile(&m_vTransScale, sizeof(GwVector));
	}

public:

	GwVector	m_vTransScale;
};

//////////////////////////////////////////////////////////////////////////
// ITransColorInfo

class ITransColorInfo : public ITransInfo
{
	I_DECLARE_DYNCREATE(ITransColorInfo);
public:
	//ITransColorInfo() : m_clrColor(GwColor(0xffffffff))	{ }
	ITransColorInfo();

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{ 
		pData = ITransInfo::LoadFile(pData);
		memcpy(&m_clrColor, pData, sizeof(GwColor));	pData += sizeof(GwColor);

		return pData;
	}
	virtual void		SaveFile(IFile & kFile)		
	{ 
		ITransInfo::SaveFile(kFile);
		kFile.WriteFile(&m_clrColor, sizeof(GwColor));
	}

public:

	GwColor		m_clrColor;
};

//////////////////////////////////////////////////////////////////////////
// ITransAlphaInfo

class ITransAlphaInfo : public ITransInfo
{
	I_DECLARE_DYNCREATE(ITransAlphaInfo);
public:
	//ITransAlphaInfo() : m_fAlpha(1)	{ }
	ITransAlphaInfo();

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{ 
		pData = ITransInfo::LoadFile(pData);
		memcpy(&m_fAlpha, pData, sizeof(float));	pData += sizeof(float);

		return pData;
	}
	virtual void		SaveFile(IFile & kFile)		
	{ 
		ITransInfo::SaveFile(kFile);
		kFile.WriteFile(&m_fAlpha, sizeof(float));
	}

public:

	float		m_fAlpha;
};

//////////////////////////////////////////////////////////////////////////
// ITransRotationInfo

class ITransRotationInfo : public ITransInfo
{
	I_DECLARE_DYNCREATE(ITransRotationInfo);
public:
	//ITransRotationInfo() : m_vRotationAxis(GwVector(0,1,0)), m_fSpeed(10)	{ }
	ITransRotationInfo();

	virtual BYTE	*	LoadFile(BYTE * pData)		
	{
		pData = ITransInfo::LoadFile(pData);
		memcpy(&m_vRotationAxis, pData, sizeof(GwVector));	pData += sizeof(GwVector);
		memcpy(&m_fSpeed, pData, sizeof(float));	pData += sizeof(float);

		return pData;
	}
	virtual void		SaveFile(IFile & kFile)		
	{
		ITransInfo::SaveFile(kFile);
		kFile.WriteFile(&m_vRotationAxis, sizeof(GwVector));
		kFile.WriteFile(&m_fSpeed, sizeof(float));
	}

public:

	GwVector	m_vRotationAxis;
	float		m_fSpeed;
};

//////////////////////////////////////////////////////////////////////////
// ITransSequenceAnimInfo

class ITransSequenceAnimInfo : public ITransInfo
{
	I_DECLARE_DYNCREATE(ITransSequenceAnimInfo);
public:
	//ITransSequenceAnimInfo() : m_strMediaPath(""), m_strFileName("")		{ }
	ITransSequenceAnimInfo();
	virtual ~ITransSequenceAnimInfo()	{ }

	virtual BYTE	*	LoadFile(BYTE * pData)
	{
		pData = ITransInfo::LoadFile(pData);
		char szData[128] = {0, };
		memcpy(szData, pData, sizeof(char) * 128);	pData += sizeof(char) * 128;
		m_strMediaPath = szData;
		memcpy(szData, pData, sizeof(char) * 128);	pData += sizeof(char) * 128;
		m_strFileName = szData;

		return pData;
	}

	virtual void		SaveFile(IFile & kFile)
	{
		ITransInfo::SaveFile(kFile);
		char szData[128] = {0, };
		kFile.WriteFile((void*)(m_strMediaPath.c_str()), sizeof(char) * 128);	
		kFile.WriteFile((void*)(m_strFileName.c_str()), sizeof(char) * 128);	
	}

public:

	string	m_strMediaPath;
	string	m_strFileName;
};