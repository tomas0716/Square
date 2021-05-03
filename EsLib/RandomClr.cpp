#include "stdafx.h"
#include "RandomClr.h"

IRandomClr::IRandomClr() 
: m_clrMin(GwColor(0,0,0,0)), m_clrMax(GwColor(255,255,255,255)) 
{
}

IRandomClr::~IRandomClr()
{
}

void			IRandomClr::SetRange(GwColor Min, GwColor Max)
{
	m_clrMin = Min;
	m_clrMax = Max;
}

void			IRandomClr::SetMin(GwColor Min)
{
	m_clrMin = Min;
}

void			IRandomClr::SetMax(GwColor Max)
{
	m_clrMax = Max;
}

GwColor			IRandomClr::GetValue()
{
	GwColor color;
	color.a = m_clrMin.a <= m_clrMax.a ? GwRandomRange(m_clrMin.a, m_clrMax.a) : GwRandomRange(m_clrMax.a, m_clrMin.a);
	color.r = m_clrMin.r <= m_clrMax.r ? GwRandomRange(m_clrMin.r, m_clrMax.r) : GwRandomRange(m_clrMax.r, m_clrMin.r);
	color.g = m_clrMin.g <= m_clrMax.g ? GwRandomRange(m_clrMin.g, m_clrMax.g) : GwRandomRange(m_clrMax.g, m_clrMin.g);
	color.b = m_clrMin.b <= m_clrMax.b ? GwRandomRange(m_clrMin.b, m_clrMax.b) : GwRandomRange(m_clrMax.b, m_clrMin.b);

	return color;
}

GwColor			IRandomClr::GetMin()
{
	return m_clrMin;
}

GwColor			IRandomClr::GetMax()
{
	return m_clrMax;
}

BYTE		*	IRandomClr::LoadFile(BYTE * pData)
{
	memcpy(&m_clrMin, pData, sizeof(GwColor)); 	pData += sizeof(GwColor);
	memcpy(&m_clrMax, pData, sizeof(GwColor)); 	pData += sizeof(GwColor);

	return pData;
}

void			IRandomClr::SaveFile(IFile & kFile)
{
	kFile.WriteFile(&m_clrMin, sizeof(GwColor));
	kFile.WriteFile(&m_clrMax, sizeof(GwColor));
}