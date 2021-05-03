#include "stdafx.h"
#include "RandomVec.h"

IRandomVec::IRandomVec()
: m_vMin(GwVector(-FLT_MAX,-FLT_MAX,-FLT_MAX)), m_vMax(GwVector(FLT_MAX,FLT_MAX,FLT_MAX))
{
}

IRandomVec::~IRandomVec()
{
}

void			IRandomVec::SetRange(GwVector Min, GwVector Max)
{
	m_vMin = Min;
	m_vMax = Max;
}

void			IRandomVec::SetMin(GwVector Min)
{
	m_vMin = Min;
}

void			IRandomVec::SetMax(GwVector Max)
{
	m_vMax = Max;
}

GwVector		IRandomVec::GetValue()
{
	GwVector vector;
	vector.x = m_vMin.x <= m_vMax.x ? GwRandomRange(m_vMin.x, m_vMax.x) : GwRandomRange(m_vMax.x, m_vMin.x);
	vector.y = m_vMin.y <= m_vMax.y ? GwRandomRange(m_vMin.y, m_vMax.y) : GwRandomRange(m_vMax.y, m_vMin.y);
	vector.z = m_vMin.z <= m_vMax.z ? GwRandomRange(m_vMin.z, m_vMax.z) : GwRandomRange(m_vMax.z, m_vMin.z);

	return vector;
}

GwVector		IRandomVec::GetMin()
{
	return m_vMin;
}

GwVector		IRandomVec::GetMax()
{
	return m_vMax;
}

BYTE		*	IRandomVec::LoadFile(BYTE * pData)
{
	memcpy(&m_vMin, pData, sizeof(GwVector)); 	pData += sizeof(GwVector);
	memcpy(&m_vMax, pData, sizeof(GwVector)); 	pData += sizeof(GwVector);

	return pData;
}

void			IRandomVec::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_vMin, sizeof(GwVector));
	kFile.WriteFile(&m_vMax, sizeof(GwVector));
}