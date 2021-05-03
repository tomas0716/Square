#pragma once

class IHashObject : public TGwGetInstance<IHashObject>
{
public:
	IHashObject(void);
	~IHashObject(void);

	ULONG				GetHashCode(const char * pszString);
	const char		*	GetHashString(ULONG uHashValue);

private:

	ULONG				AddHashCode(const char * pszString);
	bool				FindHashCode(const char * pszString, ULONG & HashCode);

	map<string,ULONG>	m_HashTable;
};

