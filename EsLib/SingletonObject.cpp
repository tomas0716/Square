#include "stdafx.h"
#include "SingletonObject.h"

bool				ISingletonObject::ms_IsTool = false;

bool				ISingletonObject::ms_Init = false;
ITypeManager	*	ISingletonObject::ms_pTypeManager = NULL;
ITypeModule		*	ISingletonObject::ms_pTypeModul = NULL;

void				ISingletonObject::Initialize()
{
	ITypeManager::GetInstance()->AddModule(ms_pTypeModul);
}

void				ISingletonObject::CreateTypeManager()
{
	ms_pTypeManager = ITypeManager::CreateInstance();
	IHashObject::CreateInstance();
}

void				ISingletonObject::DestroyTypeModul()
{
	ITypeManager::DestroyInstance();
	IHashObject::DestroyInstance();

	delete ms_pTypeModul;
}

void				ISingletonObject::SetTool()
{
	ms_IsTool = true;
}

bool				ISingletonObject::IsTool()
{
	return ms_IsTool;
}

ITypeModule		*	ISingletonObject::GetTypeModule()
{
	if( ms_pTypeModul == NULL )
		ms_pTypeModul = new ITypeModule();

	if( ms_Init == false )
	{
		ms_Init = true;
		CreateTypeManager();
	}

	return ms_pTypeModul;
}