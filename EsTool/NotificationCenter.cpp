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
	// ����� �Ҹ��ڿ��� NotificationCenter�� ���ŵ� �Ŀ� ȣ���ϴ� ��찡 �ִ�.
	if (this != NULL)
	{
		TEventDispatch< NotificationArg* >* pNotification = FindNotification(strNotificationName);

		// pNotifiction ���� ���̸� �����ڵ� ����.
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

		// �������� ������ ����
		if (iter->second->GetNumEvent() == 0)
		{
			iter->second->Release();
			m_eventTable.erase(iter);
		}
	}
}