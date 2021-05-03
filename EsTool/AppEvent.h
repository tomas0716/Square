#pragma once

#include "Event.h"
#include "NotificationCenter.h"

// CAppEvent : ���α׷� ���� �̺�Ʈ 
// ���������� �߻��ϴ� �̺�Ʈ(Update, Render, ResetDevice.. ���)��
// �����ϱ� ���� ����ġ�� ���ʿ��� �ڵ带 �ۼ��ؾ� �ϴ� ��찡 ����.
// ���� : ȣ�� ������ ���������� ���� ��쿡�� ����ؾ���.

class CAppEvent : public TGwGetInstance<CAppEvent>
{
	friend class CDXUTFramework;

public:
	CAppEvent();
	~CAppEvent();

	// �������Ӹ��� ȣ��
	static	TEventDispatch< struct EventArg_ElapsedTime >*	OnUpdate()					{ return ms_pEvent_Update; }
	static 	TEventDispatch< struct EventArg_ElapsedTime >*	OnPostUpdate()				{ return ms_pEvent_PostUpdate; }
	static	TEventDispatch< struct EventArg_ElapsedTime >*	OnRender()					{ return ms_pEvent_Render; }
	static	TEventDispatch< struct EventArg_ElapsedTime >*	OnPostRender()				{ return ms_pEvent_PostRender; }

	static	TEventDispatch< EventArg_Null >*	OnResetDevice()							{ return ms_pEvent_ResetDevice; }
	static	TEventDispatch< EventArg_Null >*	OnLostDevice()							{ return ms_pEvent_LostDevice; }

	// ��Ƽ�����̼� ����
	static	CNotificationCenter*	GetNotificationCenter()		{ return ms_pNotificationCenter; }
private:	

	static	TEventDispatch< EventArg_ElapsedTime >*			ms_pEvent_Update;
	static	TEventDispatch< EventArg_ElapsedTime >*			ms_pEvent_PostUpdate;
	static	TEventDispatch< EventArg_ElapsedTime >*			ms_pEvent_Render;
	static	TEventDispatch< EventArg_ElapsedTime >*			ms_pEvent_PostRender;

	static	TEventDispatch< EventArg_Null >*				ms_pEvent_ResetDevice;
	static	TEventDispatch< EventArg_Null >*				ms_pEvent_LostDevice;

	static	CNotificationCenter*		ms_pNotificationCenter;
};