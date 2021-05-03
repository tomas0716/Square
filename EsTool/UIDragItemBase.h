#pragma once

enum eDelegate_Drag
{
	eDelegate_Drag_Send,		// Src CUIDragItemBase, Dest CUIDragItemBase, eDragObjectReceiver
	eDelegate_Drag_Receive,		// Src CUIDragItemBase, Dest CUIDragItemBase, eDragObjectSender

	eDelegate_Drag_Max,
};

class CUIDragItemBase
{
public:
	CUIDragItemBase(eDragObjectType eType, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver);
	~CUIDragItemBase(void);

	eDragObjectType		GetDragObjectType();
	eDragObjectSender	GetDragObjectSender();
	eDragObjectReceiver	GetDragObjectReceiver();

	IDelegate		*	GetDelegate_Drag(eDelegate_Drag eDelegate);

protected:
	eDragObjectType		m_eDragObjectType;
	eDragObjectSender	m_eDragObjectSender;
	eDragObjectReceiver	m_eDragObjectReceiver;

	IDelegate		*	m_pDelegate_Drag[eDelegate_Drag_Max];
};
