#pragma once

#include "Disposable.h"

//////////////////////////////////////////////////////////////////////////
//	이벤트 클래스
//////////////////////////////////////////////////////////////////////////
struct	EventArg_Null							
{
	static EventArg_Null Object;
};

// 이벤트 객체
template<typename T = EventArg_Null>
class TEvent : public IGwRefObject
{
public:
	typedef	T			EventArgType;
	typedef	T&			EventArgType_Ref;
	typedef	const T&	EventArgType_ConstRef;

	// 이벤트 인터페이스
	virtual	void	OnEvent(EventArgType_ConstRef Arg) = 0;
};

// 이벤트 처리기 자원 해제 인터페이스 추가
template<typename T = EventArg_Null>
class TEvent_Disposable : public TEvent<T>, public IDisposable
{
};

// 널 이벤트 객체
template<typename T = EventArg_Null>
class TEvent_Null : public TEvent<T>
{
public:
	virtual	void	OnEvent(EventArgType_ConstRef Arg)	{}

	TEvent_Null()	{}
	~TEvent_Null()	{}
};


// 이벤트 대리자
template< typename _Handler, typename _Method, typename T >
class TEvent_Delegate : public TEvent< T >
{
public:
	typedef	_Handler		Handler;
	typedef	_Method			Method;

	virtual	void	OnEvent(EventArgType_ConstRef Arg)
	{
		(m_pHandler->*m_pMethod)(Arg);
	}

	TEvent_Delegate(Handler* pHandler, Method pMethod)
		:m_pHandler(pHandler), m_pMethod(pMethod)	{}	
private:
	Handler*	m_pHandler;
	Method		m_pMethod;
};


template< typename _Handler, typename _Method >
class TEvent_Delegate<_Handler, _Method, EventArg_Null> : public TEvent< EventArg_Null >
{
public:
	typedef	_Handler		Handler;
	typedef	_Method			Method;

	virtual	void	OnEvent(EventArgType_ConstRef Arg)
	{
		(m_pHandler->*m_pMethod)();
	}

	TEvent_Delegate(Handler* pHandler, Method pMethod)
		:m_pHandler(pHandler), m_pMethod(pMethod)	{}	
private:
	Handler*	m_pHandler;
	Method		m_pMethod;
};

template< typename _Handler, typename _Method, typename T >
class TEvent_Disposable_Delegate : public TEvent_Disposable< T >
{
public:
	typedef	_Handler		Handler;
	typedef	_Method			Method;

	virtual	bool	IsDisposed()
	{
		return m_pHandler == NULL;
	}

	virtual	void	Dispose()
	{
		m_pHandler = NULL;
	}

	virtual	void	OnEvent(EventArgType_ConstRef Arg)
	{
		if(IsDisposed())
			return;

		(m_pHandler->*m_pMethod)(Arg);
	}

	TEvent_Disposable_Delegate(Handler* pHandler, Method pMethod)
		:m_pHandler(pHandler), m_pMethod(pMethod)	{}	
private:
	Handler*	m_pHandler;
	Method		m_pMethod;
};

template< typename _Handler, typename _Method >
class TEvent_Disposable_Delegate<_Handler, _Method, EventArg_Null> : public TEvent_Disposable< EventArg_Null >
{
public:
	typedef	_Handler		Handler;
	typedef	_Method			Method;

	virtual	bool	IsDisposed()
	{
		return m_pHandler == NULL;
	}

	virtual	void	Dispose()
	{
		m_pHandler = NULL;
	}

	virtual	void	OnEvent(EventArgType_ConstRef Arg)
	{
		if(IsDisposed())
			return;

		(m_pHandler->*m_pMethod)();
	}

	TEvent_Disposable_Delegate(Handler* pHandler, Method pMethod)
		:m_pHandler(pHandler), m_pMethod(pMethod)	{}	
private:
	Handler*	m_pHandler;
	Method		m_pMethod;
};


// 이벤트 디스패처 : 이벤트를 복수 개의 OUT 이벤트로 전달
template<typename T = EventArg_Null>
class TEventDispatch : public TEvent_Disposable< T >
{
public:
	TEventDispatch();
	~TEventDispatch();

	typedef	TEvent<T>			EventType;

	virtual	void	OnEvent(EventArgType_ConstRef Arg);

	virtual	bool	IsDisposed();
	virtual	void	Dispose();

	void			AddEventOUT(const char* strName, EventType* pEventIN);
	void			ClearEventOUT(const char* strName);
	void			ClearAllEventOUT();

	template<typename _Handler, typename _Method >
	void			AddEventHandler(const char* strName, _Handler* pHandler, _Method pMethod)
	{
		TEvent< T >* pEvent = new TEvent_Delegate<_Handler, _Method, EventArgType>(pHandler, pMethod);
		AddEventOUT(strName, pEvent);
		pEvent->Release();
	}

	// Helper
	template<typename DerivedClass>
	void			AddEvent(const char* strName)
	{
		EventType*	pEvent = new DerivedClass();
		AddEventOUT(strName, pEvent);	pEvent->Release();
	}

	template<typename DerivedClass, typename P1>
	void			AddEvent(const char* strName, const P1& p1)
	{
		EventType*	pEvent = new DerivedClass(p1);
		AddEventOUT(strName, pEvent);	pEvent->Release();
	}

	template<typename DerivedClass, typename P1, typename P2>
	void			AddEvent(const char* strName, const P1& p1, const P2& p2)
	{
		EventType*	pEvent = new DerivedClass(p1, p2);
		AddEventOUT(strName, pEvent);	pEvent->Release();
	}

	template<typename DerivedClass, typename P1, typename P2, typename P3>
	void			AddEvent(const char* strName, const P1& p1, const P2& p2, const P3& p3)
	{
		EventType*	pEvent = new DerivedClass(p1, p2, p3);
		AddEventOUT(strName, pEvent);	pEvent->Release();
	}
	//

	size_t		GetNumEvent()	{ return m_pEventOUT_List.size(); }
private:
	list<pair<string, EventType*>>	m_pEventOUT_List;
	vector< EventType* >			m_pEventList;		// 이벤트 호출시 사용
	bool							m_bDisposed;
};



class TEventHelper
{
public:
	template< typename TArg, typename THandler, typename TMethod >
	static	TEvent_Delegate<THandler, TMethod, TArg>*	CreateDelegate(THandler* pHandler, TMethod pMethod)
	{
		return new TEvent_Delegate<THandler, TMethod, TArg>(pHandler, pMethod);
	}

	template< typename TArg, typename THandler, typename TMethod >
	static	TEvent_Disposable_Delegate<THandler, TMethod, TArg>*	CreateDisposableDelegate(THandler* pHandler, TMethod pMethod)
	{
		return new TEvent_Disposable_Delegate<THandler, TMethod, TArg>(pHandler, pMethod);
	}
};



#include "Event.inl"