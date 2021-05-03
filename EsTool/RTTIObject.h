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

// 실시간타입정보객체
// 단순히 클래스의 이름 조합으로 체크. 네임스페이스 내 클래스에 대해서는 아직 테스트안됨.
// 주의!! 서브 클래스는 선언부에 DeclareRTTIObject() 와 
// 구현 파일에 ImplementRTTIObject(MyClass, BaseClass) 를 
// 꼭 넣어주어야함.

// 2011.7.13	
// 스태틱 변수 초기화순서(컴파일순서)에 따라 클래스 이름이 제대로 초기화 안돼는 버그가 있음.
// 클래스 구분을 타입객체로 변경

class IRTTIObject : public IGwRefObject
{
public:
	DeclareRTTIObject();


	template< class T >
	bool IsKindOf() const
	{
		return GetMyClassType()->IsKindOf(&T::_MyClassType);
	}

	// 해당 클래스의 일종이면 변환을 하고
	// 실패하면 NULL 반환
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