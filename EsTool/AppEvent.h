#pragma once

#include "Event.h"
#include "NotificationCenter.h"

// CAppEvent : 프로그램 전역 이벤트 
// 전역적으로 발생하는 이벤트(Update, Render, ResetDevice.. 등등)를
// 수신하기 위해 지나치게 불필요한 코드를 작성해야 하는 경우가 있음.
// 주의 : 호출 순서에 의존적이지 않은 경우에만 사용해야함.

class CAppEvent : public TGwGetInstance<CAppEvent>
{
	friend class CDXUTFramework;

public:
	CAppEvent();
	~CAppEvent();

	// 매프레임마다 호출
	static	TEventDispatch< struct EventArg_ElapsedTime >*	OnUpdate()					{ return ms_pEvent_Update; }
	static 	TEventDispatch< struct EventArg_ElapsedTime >*	OnPostUpdate()				{ return ms_pEvent_PostUpdate; }
	static	TEventDispatch< struct EventArg_ElapsedTime >*	OnRender()					{ return ms_pEvent_Render; }
	static	TEventDispatch< struct EventArg_ElapsedTime >*	OnPostRender()				{ return ms_pEvent_PostRender; }

	static	TEventDispatch< EventArg_Null >*	OnResetDevice()							{ return ms_pEvent_ResetDevice; }
	static	TEventDispatch< EventArg_Null >*	OnLostDevice()							{ return ms_pEvent_LostDevice; }

	// 노티피케이션 센터
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