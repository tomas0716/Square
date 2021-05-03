#include "StdAfx.h"
#include "UIDragItemBase.h"

CUIDragItemSprite::CUIDragItemSprite(eDragObjectType eType, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, string strFilePath, string strFileName, IGwRefObject * pParameta) 
: CUIDragItemBase(eType, eDragSender, eDragReceiver), m_strUniqueName("CUIDragItemSprite"), m_pParameta(pParameta)
{
	if( m_pParameta != NULL ) m_pParameta->AddRef();

	Gw::GetMtrlMgr()->SetMediaPath(strFilePath.c_str());

	m_pSprite = CUIHelper::CreateUISprite(strFileName.c_str());

	CAppEvent::GetInstance()->OnUpdate()->AddEventHandler(m_strUniqueName.GetUniqueName(), this, &CUIDragItemSprite::OnUpdate);
}

CUIDragItemSprite::~CUIDragItemSprite(void)
{
	if( m_pParameta != NULL ) 
		m_pParameta->Release();

	m_pSprite->Release();

	if( CAppEvent::OnUpdate() != NULL )		
		CAppEvent::OnUpdate()->ClearEventOUT(m_strUniqueName.GetUniqueName());

	m_strUniqueName.DeclareDisposeThisUniqueName();
}

void				CUIDragItemSprite::OnUpdate(const EventArg_ElapsedTime& Arg)
{
	if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE && CUIDragObject::GetInstance()->IsDraging() == true )
	{
		GwVector vCursorPos;
		vCursorPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCursorPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;

		GwVector vPos = *m_pSprite->GetWorldPos();
		GwVector2 vSize = *m_pSprite->GetCollisSize();

		if( vPos.x <= vCursorPos.x && vPos.y <= vCursorPos.y && vPos.x + vSize.x >= vCursorPos.x && vPos.y + vSize.y >= vCursorPos.y )
		{
			CUIDragItemBase * pSrcDragItem = CUIDragObject::GetInstance()->GetDragItemBase();

			IDelegate * pDelegate_Send = pSrcDragItem->GetDelegate_Drag(eDelegate_Drag_Send);
			(*pDelegate_Send)(pSrcDragItem, this, m_eDragObjectReceiver);

			IDelegate * pDelegate_Recv = GetDelegate_Drag(eDelegate_Drag_Receive);
			(*pDelegate_Recv)(pSrcDragItem, this, pSrcDragItem->GetDragObjectSender());
		}
	}
}

IGwUISprite		*	CUIDragItemSprite::GetUISprite()
{
	return m_pSprite;
}

IGwRefObject	*	CUIDragItemSprite::GetParameta()
{
	return m_pParameta;
}