#pragma once

class CParameta_String : public IGwRefObject
{
public:
	CParameta_String(string strParameta) : m_strParameta(strParameta) { }

	string			GetParameta_String()		{ return m_strParameta; }

private:
	string			m_strParameta;
};

class CParameta_TextureFileInfo : public IGwRefObject
{
public:
	CParameta_TextureFileInfo(ITextureFileInfo & Info) : m_TextureFileInfo(Info) { }

	ITextureFileInfo * GetParameta_TextureFileInfo()		{ return &m_TextureFileInfo; }

private:
	ITextureFileInfo			m_TextureFileInfo;
};

class CParameta_MeshFileInfo : public IGwRefObject
{
public:
	CParameta_MeshFileInfo(IFileInfo & Info) : m_MeshFileInfo(Info) { }

	IFileInfo * GetParameta_MeshFileInfo()		{ return &m_MeshFileInfo; }

private:
	IFileInfo			m_MeshFileInfo;
};