#pragma once

class ITypeObject
{
public:
	ITypeObject();
	virtual ~ITypeObject();

	void			SetClassHashCode(ULONG uHashCode);
	ULONG			GetClassHashCode();

private:

	ULONG			m_uClassHashCode;
	string			m_strClassName;
};