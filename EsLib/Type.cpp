#include "stdafx.h"
#include "Type.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IType
ITypeObject	*	IType::CreateInstance()
{
	if( m_pFncCreateInstance == NULL )
	{
		return NULL;
	}

	ITypeObject * pTypeObject = (*m_pFncCreateInstance)();

	if( pTypeObject )
	{
		pTypeObject->SetClassHashCode(IHashObject::GetInstance()->GetHashCode(m_strClassName.c_str()));
	}
	return pTypeObject;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ITypeRegister
ITypeRegister::ITypeRegister(IType * pType)
{
	if( pType )
	{
		pType->m_TypeModule->AddType(pType);
	}
}