#pragma once

class ClassType
{
public:
	ClassType(const char* strClassName, ClassType* pBaseClass)
		:m_strClassName(strClassName), m_pBaseClass(pBaseClass)	{}

	bool	IsKindOf(ClassType* pClass)
	{
		if (this == pClass)
		{
			return true;
		}

		if (m_pBaseClass != NULL)
		{
			return m_pBaseClass->IsKindOf(pClass);
		}

		return false;
	}

	const char*	GetName()	{ return m_strClassName; }
private:
	const char*	m_strClassName;
	ClassType*	m_pBaseClass;
	
};

#define DeclareRTTIObject()	\
	static	ClassType	_MyClassType;	\
	virtual	ClassType*	GetMyClassType()	const	{ return &_MyClassType; }
#define ImplementRTTIObject(MyClass, BaseClass)	ClassType	MyClass::_MyClassType(#MyClass, &BaseClass::_MyClassType)

// �ǽð�Ÿ��������ü
// �ܼ��� Ŭ������ �̸� �������� üũ. ���ӽ����̽� �� Ŭ������ ���ؼ��� ���� �׽�Ʈ�ȵ�.
// ����!! ���� Ŭ������ ����ο� DeclareRTTIObject() �� 
// ���� ���Ͽ� ImplementRTTIObject(MyClass, BaseClass) �� 
// �� �־��־����.

// 2011.7.13	
// ����ƽ ���� �ʱ�ȭ����(�����ϼ���)�� ���� Ŭ���� �̸��� ����� �ʱ�ȭ �ȵŴ� ���װ� ����.
// Ŭ���� ������ Ÿ�԰�ü�� ����

class IRTTIObject : public IGwRefObject
{
public:
	DeclareRTTIObject();


	template< class T >
	bool IsKindOf() const
	{
		return GetMyClassType()->IsKindOf(&T::_MyClassType);
	}

	// �ش� Ŭ������ �����̸� ��ȯ�� �ϰ�
	// �����ϸ� NULL ��ȯ
	template< class T >
	T*	CastAs() const
	{
		if(IsKindOf<T>())
		{
			return (T*)this;
		}

		return NULL;
	}

	template< class T >
	static T*	CastAs(IRTTIObject* pObj)
	{
		if (pObj != NULL)
		{
			return pObj->CastAs<T>();
		}

		return NULL;
	}	
};