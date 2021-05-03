#pragma once

#include "RTTIObject.h"

class NotificationArg : public IRTTIObject
{
public:
	DeclareRTTIObject();

	NotificationArg():m_bLog(true)	{}

	// 메시지를 보낼 때 로그를 남길지 여부
	void	SetLogFlag(bool bLog)	{ m_bLog = bLog; }
	bool	GetLogFlag()			{ return m_bLog; }

private:
	bool	m_bLog;	
};


template< typename Handler, typename T >
class TEvent_Notification : public TEvent_Disposable<NotificationArg*>
{
public:
	typedef	void (Handler::*PFUNC)(T*);

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
		if (IsDisposed())
		{
			return;
		}

		(m_pHandler->*m_pFunc)(NotificationArg::CastAs<T>(Arg));
	}

	TEvent_Notification(Handler* pHandler, PFUNC pFunc)
	{
		m_pHandler = pHandler;
		m_pFunc = pFunc;
	}

private:
	Handler*	m_pHandler;
	PFUNC		m_pFunc;
};

template< typename Handler>
class TEvent_Notification<Handler, NotificationArg> : public TEvent_Disposable<NotificationArg*>
{
public:
	typedef	void (Handler::*PFUNC)(NotificationArg*);

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
		if (IsDisposed())
		{
			return;
		}

		(m_pHandler->*m_pFunc)(Arg);
	}

	TEvent_Notification(Handler* pHandler, PFUNC pFunc)
	{
		m_pHandler = pHandler;
		m_pFunc = pFunc;
	}

private:
	Handler*	m_pHandler;
	PFUNC		m_pFunc;
};


class CNotificationCenter : public TGwGetInstance<CNotificationCenter>
{
public:
	CNotificationCenter();
	~CNotificationCenter();

	// 이벤트 발송
	// 후에 Post 로 이름 변경
	void	PostNotification(const char* strNotificationName, NotificationArg* p);


	template<typename Handler, typename T>
	TEvent_Disposable<NotificationArg*>*	CreateEvent(Handler* pHandler, void (Handler::*pFunc)(T*))
	{
		return new TEvent_Notification<Handler, T>(pHandler, pFunc);
	}

	template<typename Handler, typename T>
	void	AddObserver(const char* strNotificationName, const char* strObserverName, Handler* pHandler, void (Handler::*pFunc)(T*))
	{
		TEvent<NotificationArg*>* pEvent = new TEvent_Notification<Handler, T>(pHandler, pFunc);
		GetNotification(strNotificationName)->AddEventOUT(strObserverName, pEvent);
		pEvent->Release();
	}

	void	AddObserver(const char* strNotificationName, const char* strObserverName, TEvent<NotificationArg*>* pEvent)
	{
		GetNotification(strNotificationName)->AddEventOUT(strObserverName, pEvent);
	}

	void	RemoveObserver(const char* strNotificationName, const char* strObserverName);

private:
	map< string, TEventDispatch< NotificationArg* >*>	m_eventTable;

	// 이름에 해당하는 이벤트를 반환. 해당 이벤트가 없다면 생성해서 반환
	// 나중에 Private 로 변경 할것
	TEventDispatch< NotificationArg* >*	GetNotification(const char* strName);

	// 소멸자에서는 이 함수를 사용. 해당 이벤트가 없다면 NULL 값 반환
	// 나중에 Private 로 변경 할것
	TEventDispatch< NotificationArg* >*	FindNotification(const char* strName);
};