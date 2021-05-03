#include "StdAfx.h"
#include "UIDragItemBase.h"

CUIDragItemBase::CUIDragItemBase(eDragObjectType eType, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver) 
: m_eDragObjectType(eType), m_eDragObjectSender(eDragSender), m_eDragObjectReceiver(eDragReceiver)
{
	for( int i = 0; i < eDelegate_Drag_Max; ++i ) 
	{
		m_pDelegate_Drag[i] = new IDelegate();
	}
}

CUIDragItemBase::~CUIDragItemBase(void)
{
	for( int i = 0; i < eDelegate_Drag_Max; ++i ) 
	{
		m_pDelegate_Drag[i]->Release();
	}
}

eDragObjectType		CUIDragItemBase::GetDragObjectType()
{
	return m_eDragObjectType;
}

eDragObjectSender	CUIDragItemBase::GetDragObjectSender()
{
	return m_eDragObjectSender;
}

eDragObjectReceiver	CUIDragItemBase::GetDragObjectReceiver()
{
	return m_eDragObjectReceiver;
}

IDelegate		*	CUIDragItemBase::GetDelegate_Drag(eDelegate_Drag eDelegate)
{
	return m_pDelegate_Drag[eDelegate];
}