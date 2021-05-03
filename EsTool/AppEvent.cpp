#include "stdafx.h"
#include "AppEvent.h"
#include "GameEventArgType.h"

TEventDispatch< EventArg_ElapsedTime >*			CAppEvent::ms_pEvent_Update = NULL;
TEventDispatch< EventArg_ElapsedTime >*			CAppEvent::ms_pEvent_PostUpdate = NULL;
TEventDispatch< EventArg_ElapsedTime >*			CAppEvent::ms_pEvent_Render	= NULL;
TEventDispatch< EventArg_ElapsedTime >*			CAppEvent::ms_pEvent_PostRender	= NULL;

TEventDispatch< EventArg_Null >*				CAppEvent::ms_pEvent_ResetDevice = NULL;
TEventDispatch< EventArg_Null >*				CAppEvent::ms_pEvent_LostDevice = NULL;
CNotificationCenter*							CAppEvent::ms_pNotificationCenter = NULL;

CAppEvent::CAppEvent()
{
	ms_pEvent_Update		= new TEventDispatch< EventArg_ElapsedTime >();	
	ms_pEvent_PostUpdate	= new TEventDispatch< EventArg_ElapsedTime >();

	ms_pEvent_Render		= new TEventDispatch< EventArg_ElapsedTime >();
	ms_pEvent_PostRender	= new TEventDispatch< EventArg_ElapsedTime >();

	ms_pEvent_ResetDevice	= new TEventDispatch< EventArg_Null >();
	ms_pEvent_LostDevice	= new TEventDispatch< EventArg_Null >();

	ms_pNotificationCenter = new CNotificationCenter();
}

CAppEvent::~CAppEvent()
{
	ms_pEvent_LostDevice->Dispose();
	ms_pEvent_LostDevice->Release();
	ms_pEvent_LostDevice = NULL;

	ms_pEvent_ResetDevice->Dispose();
	ms_pEvent_ResetDevice->Release();
	ms_pEvent_ResetDevice = NULL;

	ms_pEvent_Render->Dispose();
	ms_pEvent_Render->Release();
	ms_pEvent_Render = NULL;

	ms_pEvent_PostRender->Dispose();
	ms_pEvent_PostRender->Release();
	ms_pEvent_PostRender = NULL;

	ms_pEvent_Update->Dispose();
	ms_pEvent_Update->Release();
	ms_pEvent_Update = NULL;

	ms_pEvent_PostUpdate->Dispose();
	ms_pEvent_PostUpdate->Release();
	ms_pEvent_PostUpdate = NULL;

	SAFE_DELETE(ms_pNotificationCenter);
}