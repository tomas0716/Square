#include "StdAfx.h"
#include "RandomI.h"

IRandomI::IRandomI()
: m_nMin(-INT_MIN), m_nMax(INT_MAX)
{
}

IRandomI::~IRandomI()
{
}

void			IRandomI::SetRange(int Min, int Max)
{
	m_nMin = Min;
	m_nMax = Max;
}

void			IRandomI::SetMin(int Min)
{
	m_nMin = Min;
}

void			IRandomI::SetMax(int Max)
{
	m_nMax = Max;
}

int				IRandomI::GetValue()
{
	return m_nMin <= m_nMax ? GwRandomRange(m_nMin, m_nMax) : GwRandomRange(m_nMin, m_nMin);
}

int				IRandomI::GetMin()
{
	return m_nMin;
}

int				IRandomI::GetMax()
{	
	return m_nMax;
}

BYTE		*	IRandomI::LoadFile(BYTE * pData)
{
	memcpy(&m_nMin, pData, sizeof(int)); 	pData += sizeof(int);
	memcpy(&m_nMax, pData, sizeof(int)); 	pData += sizeof(int);

	return pData;
}

void			IRandomI::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_nMin, sizeof(int));
	kFile.WriteFile(&m_nMax, sizeof(int));
}