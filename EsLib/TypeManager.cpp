#include "stdafx.h"
#include "TypeManager.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITypeModule

ITypeModule::ITypeModule()
{
}

ITypeModule::~ITypeModule()
{
	m_TypeList.clear();
}

void			ITypeModule::AddType(IType * pType)
{
	m_TypeList.push_back(pType);
}

int				ITypeModule::GetCount()
{
	return (int)m_TypeList.size();
}

IType		*	ITypeModule::GetType_byIndex(int nIndex)
{
	assert(nIndex >= 0 && GetCount() > nIndex);

	return m_TypeList[nIndex];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// ITypeModule

ITypeManager::ITypeManager(void)
{
}

ITypeManager::~ITypeManager(void)
{
	m_TypeList.clear();
}

void			ITypeManager::AddModule(ITypeModule * pModule)
{
	if( pModule )
	{
		int nCount = pModule->GetCount();

		for( int i = 0; i < nCount; ++i )
		{
			IType * pType = pModule->GetType_byIndex(i);

			if( FindType(pType->m_strClassName.c_str()) == NULL )
			{
				m_TypeList.push_back(pType);
			}
		}
	}
}

IType		*	ITypeManager::FindType(const char * pszClassName)
{
	return FindType(IHashObject::GetInstance()->GetHashCode(pszClassName));
}

IType		*	ITypeManager::FindType(ULONG uHashCode)
{
	vector<IType*>::iterator itr = m_TypeList.begin();

	for( ; itr != m_TypeList.end(); ++itr )
	{
		IType * pType = (*itr);

		if( pType )
		{
			ULONG uClassHashCode = IHashObject::GetInstance()->GetHashCode(pType->m_strClassName.c_str());

			if( uClassHashCode == uHashCode )
			{
				return pType;
			}
		}
	}

	return NULL;
}