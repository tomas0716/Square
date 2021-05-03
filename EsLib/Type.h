#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// IType

class ITypeModule;
class ITypeObject;
//#include "ITypeManager.h"

struct IType
{
public:
	string			m_strClassName;
	ITypeObject	*	(*m_pFncCreateInstance)();
	ITypeModule	*	m_TypeModule;

	ITypeObject	*	CreateInstance();
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ITypeRegister
struct ITypeRegister
{
public:
	ITypeRegister(IType * pType);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Create Declare
#define I_DECLARE_DYNCREATE(class_name)	\
	public: \
	static IType class##class_name; \
	static ITypeObject * CreateTypeObject(); \

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Dynamic Create Implement
#define I_IMPLEMENT_DYNCREATE(class_name, type_module) \
	ITypeObject * class_name::CreateTypeObject() \
	{ \
		return new class_name(); \
	} \
	IType class_name::class##class_name = { #class_name, class_name::CreateTypeObject, type_module }; \
	ITypeRegister Init_##class_name((IType*)(&class_name::class##class_name));