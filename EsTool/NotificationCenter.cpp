#include "StdAfx.h"
#include "NotificationCenter.h"

ImplementRTTIObject(NotificationArg, IRTTIObject);

CNotificationCenter::CNotificationCenter()
{

}

CNotificationCenter::~CNotificationCenter()
{
	for each(pair<string, TEventDispatch< NotificationArg* >*> item in m_eventTable)
	{
		item.second->Dispose();
		item.second->Release();
	}
}

TEventDispatch< NotificationArg* >*	CNotificationCenter::GetNotification(const char* strName)
{
	map< string, TEventDispatch< NotificationArg*>* >::iterator iter = m_eventTable.find(strName);

	if(iter != m_eventTable.end())
		return iter->second;

	TEventDispatch<NotificationArg*>*	pEvent = new TEventDispatch<NotificationArg*>();
	m_eventTable.insert(pair< string, TEventDispatch< NotificationArg*>* >(strName, pEvent));
	return pEvent;
}

TEventDispatch< NotificationArg* >*	CNotificationCenter::FindNotification(const char* strName)
{
	map< string, TEventDispatch< NotificationArg*>* >::iterator iter = m_eventTable.find(strName);

	if(iter != m_eventTable.end())
		return iter->second;

	return NULL;
}

void	CNotificationCenter::PostNotification(const char* strNotificationName, NotificationArg* p)
{
	// 종료시 소멸자에서 NotificationCenter가 제거된 후에 호출하는 경우가 있다.
	if (this != NULL)
	{
		TEventDispatch< NotificationArg* >* pNotification = FindNotification(strNotificationName);

		// pNotifiction 값이 널이면 수신자도 없다.
		if (pNotification != NULL)
		{
			pNotification->OnEvent(p);
		}
	}
}

void	CNotificationCenter::RemoveObserver(const char* strNotificationName, const char* strObserverName)
{
	map< string, TEventDispatch< NotificationArg*>* >::iterator iter = m_eventTable.find(strNotificationName);

	if(iter != m_eventTable.end())
	{
		iter->second->ClearEventOUT(strObserverName);

		// 옵저버가 없으면 제거
		if (iter->second->GetNumEvent() == 0)
		{
			iter->second->Release();
			m_eventTable.erase(iter);
		}
	}
}