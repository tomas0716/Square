#include "stdafx.h"
#include "RandomF.h"

IRandomF::IRandomF()
: m_fMin(-FLT_MAX), m_fMax(FLT_MAX)
{
}

IRandomF::~IRandomF()
{
}

void			IRandomF::SetRange(float Min, float Max)
{
	m_fMin = Min;
	m_fMax = Max;
}

void			IRandomF::SetMin(float Min)
{
	m_fMin = Min;
}

void			IRandomF::SetMax(float Max)
{
	m_fMax = Max;
}

float			IRandomF::GetValue()
{
	return m_fMin <= m_fMax ? GwRandomRange(m_fMin, m_fMax) : GwRandomRange(m_fMin, m_fMin);
}

float			IRandomF::GetMin()
{
	return m_fMin;
}

float			IRandomF::GetMax()
{	
	return m_fMax;
}

BYTE		*	IRandomF::LoadFile(BYTE * pData)
{
	memcpy(&m_fMin, pData, sizeof(float)); 	pData += sizeof(float);
	memcpy(&m_fMax, pData, sizeof(float)); 	pData += sizeof(float);

	return pData;
}

void			IRandomF::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_fMin, sizeof(float));
	kFile.WriteFile(&m_fMax, sizeof(float));
}