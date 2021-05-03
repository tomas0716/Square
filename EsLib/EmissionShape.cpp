#include "stdafx.h"
#include "EmissionShape.h"

//////////////////////////////////////////////////////////////////////////
// Base

I_IMPLEMENT_DYNCREATE(IEmissionShape, ISingletonObject::GetTypeModule())

IEmissionShape::IEmissionShape()
{

}

IEmissionShape::~IEmissionShape()
{

}

//////////////////////////////////////////////////////////////////////////
// Cone

I_IMPLEMENT_DYNCREATE(IParticleShape_Cone, ISingletonObject::GetTypeModule())

IParticleShape_Cone::IParticleShape_Cone() : m_fRadius(1), m_fAngle(30), m_fHeight(5), m_IsVolume(false)
{
}

IParticleShape_Cone::~IParticleShape_Cone()
{
}

void			IParticleShape_Cone::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

void			IParticleShape_Cone::SetAngle(float fAngle)
{
	m_fAngle = fAngle;
}

void			IParticleShape_Cone::SetHeight(float fHeight)
{
	m_fHeight = fHeight;
}

void			IParticleShape_Cone::SetVolume(bool IsVolume)
{
	m_IsVolume =IsVolume;
}

float			IParticleShape_Cone::GetRadius()
{
	return m_fRadius;
}

float			IParticleShape_Cone::GetAngle()
{
	return m_fAngle;
}

float			IParticleShape_Cone::GetHeight()
{
	return m_fHeight;
}

bool			IParticleShape_Cone::IsVolume()
{
	return m_IsVolume;
}

BYTE	*		IParticleShape_Cone::LoadFile(BYTE * pData)
{
	memcpy(&m_fRadius, pData, sizeof(float));	 pData += sizeof(float);
	memcpy(&m_fAngle, pData, sizeof(float));	pData += sizeof(float);
	memcpy(&m_fHeight, pData, sizeof(float));	pData += sizeof(float);
	memcpy(&m_IsVolume, pData, sizeof(bool));	pData += sizeof(bool);

	return pData;
}

void			IParticleShape_Cone::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_fRadius,sizeof(float));
	kFile.WriteFile(&m_fAngle,sizeof(float));
	kFile.WriteFile(&m_fHeight,sizeof(float));
	kFile.WriteFile(&m_IsVolume,sizeof(bool));
}


//////////////////////////////////////////////////////////////////////////
// Sphere

I_IMPLEMENT_DYNCREATE(IParticleShape_Sphere, ISingletonObject::GetTypeModule())

IParticleShape_Sphere::IParticleShape_Sphere() : m_fRadius(30)
{
}

IParticleShape_Sphere::~IParticleShape_Sphere()
{
}

void			IParticleShape_Sphere::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}

float			IParticleShape_Sphere::GetRadius()
{
	return m_fRadius;
}

BYTE	*		IParticleShape_Sphere::LoadFile(BYTE * pData)
{
	memcpy(&m_fRadius, pData, sizeof(float));	 pData += sizeof(float);

	return pData;
}

void			IParticleShape_Sphere::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_fRadius,sizeof(float));
}
	
//////////////////////////////////////////////////////////////////////////
// Box

I_IMPLEMENT_DYNCREATE(IParticleShape_Box, ISingletonObject::GetTypeModule())

IParticleShape_Box::IParticleShape_Box() : m_vSize(GwVector(30,30,30))
{
}

IParticleShape_Box::~IParticleShape_Box()
{
}

void			IParticleShape_Box::SetSize(GwVector vSize)
{
	m_vSize = vSize;
}	

GwVector		IParticleShape_Box::GetSize()
{
	return m_vSize;
}

BYTE	*		IParticleShape_Box::LoadFile(BYTE * pData)
{
	memcpy(&m_vSize, pData, sizeof(GwVector));	 pData += sizeof(GwVector);

	return pData;
}

void			IParticleShape_Box::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_vSize,sizeof(GwVector));
}

//////////////////////////////////////////////////////////////////////////
// Mesh

I_IMPLEMENT_DYNCREATE(IParticleShape_Mesh, ISingletonObject::GetTypeModule())

IParticleShape_Mesh::IParticleShape_Mesh() : m_strMediaPath(""), m_strFileName(""), m_pStaticObject(NULL)
{
	m_pStaticObject = Gw::GetScnMgr()->CreateStaticObject();
}

IParticleShape_Mesh::~IParticleShape_Mesh()
{
	m_pStaticObject->Release();
}

void			IParticleShape_Mesh::SetMediaPath(string strMediaPath)
{
	m_strMediaPath = strMediaPath;
}

void			IParticleShape_Mesh::SetFileName(string strFileName)
{
	m_strFileName = strFileName;
}

void			IParticleShape_Mesh::Load()
{

}

string			IParticleShape_Mesh::GetMediaPath()
{
	return m_strMediaPath;
}

string			IParticleShape_Mesh::GetFileName()
{
	return m_strFileName;
}

IGwStaticObject	*	IParticleShape_Mesh::GetStaticObject()
{
	return m_pStaticObject;
}

BYTE	*		IParticleShape_Mesh::LoadFile(BYTE * pData)
{
	char szData[128] = { 0, };
	memcpy(szData, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;
	m_strMediaPath = szData;
	memcpy(szData, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;
	m_strFileName = szData;

	return pData;
}

void			IParticleShape_Mesh::SaveFile(IFile & kFile)
{
	kFile.WriteFile((void*)m_strMediaPath.c_str(),sizeof(char) * 128);
	kFile.WriteFile((void*)m_strFileName.c_str(),sizeof(char) * 128);
}
