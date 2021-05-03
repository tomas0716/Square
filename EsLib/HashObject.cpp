#include "stdafx.h"
#include "HashObject.h"


IHashObject::IHashObject(void)
{
}


IHashObject::~IHashObject(void)
{
}

ULONG				IHashObject::GetHashCode(const char * pszString)
{
	ULONG  HashCode = 0;

	if( FindHashCode(pszString,HashCode) )
	{
		return HashCode;
	}

	return AddHashCode(pszString);
}

const char		*	IHashObject::GetHashString(ULONG uHashValue)
{
	map<string,ULONG>::iterator itr = m_HashTable.begin();

	for( ; itr != m_HashTable.end(); ++itr )
	{
		if( itr->second == uHashValue )
		{
			return itr->first.c_str();
		}
	}

	return NULL;
}

ULONG				IHashObject::AddHashCode(const char * pszString)
{
    unsigned long i,len;  
    unsigned long ch;  
    unsigned long result;  
    unsigned long XOR;  

    len     = strlen( pszString );  
    result  = 0;  
    XOR     = 0;  

    for( i=0; i<len; i++ )  
    {  
        ch = (unsigned long)pszString[i];  

        result = ( ( result << 8 ) | ch ) % 16777213UL; // 16777213UL  

        XOR ^= ch;  
    }

	result |= (XOR<<24);

	m_HashTable.insert(map<string,ULONG>::value_type(pszString, result));

    return result;
}

bool				IHashObject::FindHashCode(const char * pszString, ULONG & HashCode)
{
	map<string,ULONG>::iterator itr = m_HashTable.find(pszString);

	if( itr != m_HashTable.end() )
	{
		HashCode = itr->second;
		return true;
	}

	return false;
}