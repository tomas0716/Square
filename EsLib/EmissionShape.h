#pragma once

//////////////////////////////////////////////////////////////////////////
// Base
class IEmissionShape : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(IEmissionShape);
public:
	IEmissionShape();
	virtual ~IEmissionShape();

	virtual BYTE	*	LoadFile(BYTE * pData)		{ return NULL; }
	virtual void		SaveFile(IFile & kFile)		{ }

protected:
};

//////////////////////////////////////////////////////////////////////////
// Cone

class IParticleShape_Cone : public IEmissionShape
{
	I_DECLARE_DYNCREATE(IParticleShape_Cone);
public:
	IParticleShape_Cone();
	virtual ~IParticleShape_Cone();

	virtual void		SetRadius(float fRadius);
	virtual void		SetAngle(float fAngle);
	virtual void		SetHeight(float fHeight);
	virtual void		SetVolume(bool IsVolume);

	virtual float		GetRadius();
	virtual float		GetAngle();
	virtual float		GetHeight();
	virtual bool		IsVolume();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	float			m_fRadius;
	float			m_fAngle;
	float			m_fHeight;
	bool			m_IsVolume;
};

//////////////////////////////////////////////////////////////////////////
// Sphere

class IParticleShape_Sphere : public IEmissionShape
{
	I_DECLARE_DYNCREATE(IParticleShape_Sphere);
public:
	IParticleShape_Sphere();
	virtual ~IParticleShape_Sphere();

	void				SetRadius(float fRadius);
	float				GetRadius();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	float			m_fRadius;
};

//////////////////////////////////////////////////////////////////////////
// Box

class IParticleShape_Box : public IEmissionShape
{
	I_DECLARE_DYNCREATE(IParticleShape_Box);
public:
	IParticleShape_Box();
	virtual ~IParticleShape_Box();
	
	void				SetSize(GwVector vSize);
	GwVector			GetSize();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	GwVector		m_vSize;
};

//////////////////////////////////////////////////////////////////////////
// Mesh

class IParticleShape_Mesh : public IEmissionShape
{
	I_DECLARE_DYNCREATE(IParticleShape_Mesh);
public:
	IParticleShape_Mesh();
	virtual ~IParticleShape_Mesh();

	void				SetMediaPath(string strMediaPath);
	void				SetFileName(string strFileName);
	void				Load();

	string				GetMediaPath();
	string				GetFileName();

	IGwStaticObject	*	GetStaticObject();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	string				m_strMediaPath;
	string				m_strFileName;

	IGwStaticObject*	m_pStaticObject;
};