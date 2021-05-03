#include "stdafx.h"
#include "TypeObject.h"


ITypeObject::ITypeObject(void) : m_uClassHashCode(0), m_strClassName("")
{
}

ITypeObject::~ITypeObject(void)
{
}

void			ITypeObject::SetClassHashCode(ULONG uHashCode)
{
	m_uClassHashCode = uHashCode;
}

ULONG			ITypeObject::GetClassHashCode()
{
	return m_uClassHashCode;
}