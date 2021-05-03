#include "StdAfx.h"
#include "UITreeView.h"


UINT			CUITreeView::ms_TreeViewIndex = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CButtonCallback_Empty

class CUITreeView::CButtonCallback_Empty : public IGwUIButton_Callback
{
public:
	CButtonCallback_Empty(CUITreeView * pUITreeView) : m_pUITreeView(pUITreeView) { }

	virtual void	OnButton_OK(class IGwUIButton* pControl) 
	{
		m_pUITreeView->OnButtonClick_Empty();
	}

private:
	CUITreeView		*	m_pUITreeView;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CScrollCallback

class CUITreeView::CScrollCallback : public IGwUISlider_Callback
{
public:
	CScrollCallback(CUITreeView* pUITreeView) :m_pUITreeView(pUITreeView)				{ }

	virtual void	OnChangeValue(class IGwUISliderBase* pMyControl,bool bFromInput)
	{
		m_pUITreeView->UpdateScroll();
	}

private:
	CUITreeView * m_pUITreeView;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUITreeView

CUITreeView::CUITreeView(IGwUIDummyCtrl * pDummyCtrl, GwVector2 vPos, GwVector2 vSize)
: m_strUniqueName("CUITreeView"), m_pDummyCtrl(pDummyCtrl), m_vPos(vPos), m_vSize(vSize), m_eDragObjectSender(eDragObjectSender_None), m_eDragObjectReceiver(eDragObjectReceiver_None), 
  m_pScroll_V(NULL), m_dwTreeViewItemIndex(0), m_IsDragSendUse(false), m_IsNameChange(false), m_IsDoubleClickTreeOpen(true), m_vMargin(GwVector2(0,0)), m_pTreeViewItem_Selected(NULL)
{
	IGwBillboardTex * pEmptyTex = Gw::GetBillboardMgr()->GetBillboardTex_Null();
	IGwUIElement_MultiTex		ElemTex;
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pEmptyTex );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_MOUSEOVER, pEmptyTex );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_FOCUS, pEmptyTex );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_PRESSED, pEmptyTex );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_DISABLED, pEmptyTex );

	IGwUIImageInfo_Tex	* pButtonImageInfo = NULL;
	pButtonImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemTex );
	IGwUIButton * pButton_EmptyBack	= Gw::GetUIControlMgr()->CreateUIButton( pButtonImageInfo );	pButtonImageInfo->Release();
	pButton_EmptyBack->SetForceCollisSize(vSize);
	CButtonCallback_Empty * pCallback = new CButtonCallback_Empty(this);
	pButton_EmptyBack->SetCallback(pCallback);	pCallback->Release();
	m_pDummyCtrl->AddControl(pButton_EmptyBack);	pButton_EmptyBack->Release();

	m_pTreeViewItem_Root = new CUITreeViewItem(this);
	m_pTreeViewItem_Root->SetRoot();
	m_pTreeViewItem_Root->SetLocalPos(vPos);
	m_pDummyCtrl->AddControl(m_pTreeViewItem_Root->GetBaseControl());

	m_pDelegate_TreeView = new IDelegate();
	m_pDelegate_DragReceive = new IDelegate();

	m_TreeViewIndex = ms_TreeViewIndex++;

	CAppEvent::OnPostUpdate()->AddEventHandler(m_strUniqueName.GetUniqueName(), this , &CUITreeView::PostUpdate);
}

CUITreeView::~CUITreeView(void)
{
	m_pDummyCtrl->RemoveControl(m_pTreeViewItem_Root->GetBaseControl());
	m_pTreeViewItem_Root->Release();

	if( m_pScroll_V != NULL ) 
	{
		m_pDummyCtrl->RemoveControl(m_pScroll_V);
		m_pScroll_V->Release();
	}

	m_pDelegate_TreeView->Release();
	m_pDelegate_DragReceive->Release();

	if( CAppEvent::OnPostUpdate() != NULL )		CAppEvent::OnPostUpdate()->ClearEventOUT(m_strUniqueName.GetUniqueName());

	m_strUniqueName.DeclareDisposeThisUniqueName();
}

void				CUITreeView::Update(float fDeltaTime)
{
	if( Gw::GetKeyMapper()->GetWheelRotationCount() != 0 && m_pDummyCtrl->GetMyDialog()->IsDialogActive() == true )
	{
		GwVector vDialogPos = *m_pDummyCtrl->GetWorldPos();
		GwVector2 vPos = GwVector2(vDialogPos.x, vDialogPos.y) + m_vPos;
		
		POINT Point = *Gw::GetKeyMapper()->GetCursorPos();

		if( Point.x >= vPos.x && Point.x <= vPos.x + m_vSize.x &&
			Point.y >= vPos.y && Point.y <= vPos.y + m_vSize.y )
		{
			OnMouseWheelMove(Gw::GetKeyMapper()->GetWheelRotationCount());
		}
	}

	int nNumChild = m_pTreeViewItem_Root->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(m_pTreeViewItem_Root->GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->Update(fDeltaTime);
		}
	}

	if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE && CUIDragObject::GetInstance()->IsDraging() == true )
	{
		GwVector vPos;
		vPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;

		GwVector vTreeViewPos = *m_pDummyCtrl->GetWorldPos() + GwVector(m_vPos.x, m_vPos.y, 0);

		if( vTreeViewPos.x <= vPos.x && vTreeViewPos.y <= vPos.y && vTreeViewPos.x + m_vSize.x >= vPos.x && vTreeViewPos.y + m_vSize.y >= vPos.y )
		{
			if( m_pTreeViewItem_Root->OnCheckDrag(GwVector2(vPos.x, vPos.y)) == false )
			{
				CUIDragItemBase * pSrcDragItem = CUIDragObject::GetInstance()->GetDragItemBase();
				(*m_pDelegate_DragReceive)(pSrcDragItem, NULL, pSrcDragItem->GetDragObjectSender());

				IDelegate * pDelegate_Send = pSrcDragItem->GetDelegate_Drag(eDelegate_Drag_Send);
				(*pDelegate_Send)(pSrcDragItem, NULL, m_eDragObjectReceiver);
			}
		}
	}
}

void				CUITreeView::PostUpdate(const EventArg_ElapsedTime& Arg)
{
}

void				CUITreeView::SetAttribute(eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, bool IsNameChanged, GwVector2 vMargin)
{
	m_eDragObjectSender = eDragSender;
	m_eDragObjectReceiver = eDragReceiver;
	m_IsDragSendUse = IsDragSendUse;
	m_IsNameChange = IsNameChanged;
	m_vMargin = vMargin;
}

void				CUITreeView::SetAttribute_TreeViewItem(int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor selColor,
														   const char * pszEmpty, const char * pszPlus, const char * pszMinus, const char * pszSelect, const char * pszEdit)
{
	m_nItemHeightInterval		= nItemHeightInterval;
	m_nFontSize					= nFontSize;
	m_strFontName				= pszFontName;
	m_eWeightType				= eWeightType;
	m_FontColor					= fontColor;
	m_SelFondColor				= selColor;
	m_strEmptyButtonFileName	= pszEmpty;
	m_strPlusFileName			= pszPlus;
	m_strMinusFileName			= pszMinus;
	m_strSelectFileName			= pszSelect;
	m_strEditboxFileName		= pszEdit;
}

void				CUITreeView::SetAttribute_Scroll(GwVector2 vPos, const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext, int nThumbSensitive)
{
	m_pScroll_V = CUIHelper::CreateUIScroll_V(pszTrack, pszThumb, pszPrev, pszNext);
	m_pScroll_V->SetLocalPos(vPos);

	CScrollCallback*	pCallback = new CScrollCallback(this);
	m_pScroll_V->SetCallback(pCallback);	pCallback->Release();

	m_pDummyCtrl->AddControl(m_pScroll_V);

	m_nThumbSensitive = nThumbSensitive;
}

UINT				CUITreeView::GetTreeViewIndex()
{
	return m_TreeViewIndex;
}

CUITreeViewItem	*	CUITreeView::AddItem(CUITreeViewItem * pTreeViewItem, const char * pszItemName, IGwRefObject * pParameta)
{
	if( pTreeViewItem == NULL ) 
		pTreeViewItem = m_pTreeViewItem_Root;

	CUITreeViewItem * pNewTreeViewItem = new CUITreeViewItem(this, ++m_dwTreeViewItemIndex, m_eDragObjectSender, m_eDragObjectReceiver, m_IsDragSendUse, m_IsNameChange, m_IsDoubleClickTreeOpen, m_vSize.x, m_nItemHeightInterval, m_nFontSize, 
															 m_strFontName.c_str(), m_eWeightType, m_FontColor, m_SelFondColor, pszItemName, m_strEmptyButtonFileName.c_str(), m_strPlusFileName.c_str(), 
															 m_strMinusFileName.c_str(), m_strSelectFileName.c_str(), m_strEditboxFileName.c_str(), pParameta);

	pTreeViewItem->AddChild(pNewTreeViewItem);	pNewTreeViewItem->Release();

	if( pTreeViewItem == m_pTreeViewItem_Root ) 
		pNewTreeViewItem->SetVisible(true);

	OnSort();
	UpdateItemPos();
	UpdateScroll();

	return pNewTreeViewItem;
}

void				CUITreeView::Remove(CUITreeViewItem * pTreeViewItem)
{
	CUITreeViewItem * pParent = dynamic_cast<CUITreeViewItem*>(pTreeViewItem->GetParent());

	if( pParent == NULL ) 
		pParent = m_pTreeViewItem_Root;
	
	if( pParent != NULL ) 
	{
		pParent->RemoveChild(pTreeViewItem);

		if( pParent->GetNumChild() == 0 )
		{
			pParent->SetChildOpenFlag(false);
		}
	}

	OnSort();
	UpdateItemPos();
	UpdateScroll();
}

void				CUITreeView::RemoveAll()
{
	int nNumChild = m_pTreeViewItem_Root->GetNumChild();

	for( int i = nNumChild - 1; i >= 0; --i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(m_pTreeViewItem_Root->GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			m_pTreeViewItem_Root->RemoveChild(pTreeViewItem);
		}
	}

	OnSort();
	UpdateItemPos();
	UpdateScroll();
}

CUITreeViewItem *	CUITreeView::FindTreeViewItem_byParameta(IGwRefObject * pParameta)
{
	if( pParameta == NULL ) 
		return NULL;

	return FindTreeViewItem_byParameta(m_pTreeViewItem_Root, pParameta);
}

CUITreeViewItem *	CUITreeView::FindTreeViewItem_byItemName(CUITreeViewItem * pTreeViewItem, string strItemName)
{
	if( pTreeViewItem == NULL )
		pTreeViewItem = m_pTreeViewItem_Root;

	return pTreeViewItem->FindTreeViewItem_byItemName(strItemName);
}

CUITreeViewItem *	CUITreeView::FindTreeViewItem_byParameta(CUITreeViewItem * pTreeViewItem, IGwRefObject * pParameta)
{
	if( pTreeViewItem->GetParameta() == pParameta )
	{
		return pTreeViewItem;
	}

	int nNumChild = pTreeViewItem->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pChildTreeViewItem = dynamic_cast<CUITreeViewItem*>(pTreeViewItem->GetChild_byIndex(i));

		if( pChildTreeViewItem ) 
		{
			CUITreeViewItem * pFindTreeViewItem = FindTreeViewItem_byParameta(pChildTreeViewItem, pParameta);

			if( pFindTreeViewItem != NULL ) 
			{
				return pFindTreeViewItem;
			}
		}
	}

	return NULL;
}

void				CUITreeView::UpdateItemPos()
{
	GwVector2 vPos = m_vMargin;
	int nNumChild = m_pTreeViewItem_Root->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(m_pTreeViewItem_Root->GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->SetLocalPos(vPos);

			float fHeight = pTreeViewItem->GetHeight_IncludeChild();
			vPos.y += fHeight;
		}
	}

	m_nTotalItemHeight = vPos.y;
	m_nTotalItemHeight += 100;
}

void				CUITreeView::UpdateScroll()
{
	int nThumbSensitive = (m_nFontSize + m_nItemHeightInterval * 2) * m_nThumbSensitive;
	int nFirst = CUIHelper::CalcFirstItemIndexScroll_V(m_pScroll_V, m_nTotalItemHeight, nThumbSensitive, m_vSize.y);
	m_pTreeViewItem_Root->SetLocalPos(m_vPos + GwVector2(0,-nFirst));

	Gw::GetUIControlMgr()->UpdateDialog(0);
	GwVector vDialogPos = *m_pDummyCtrl->GetWorldPos();
	m_pTreeViewItem_Root->UpdateVisible(m_vPos + m_vMargin + GwVector2(vDialogPos.x, vDialogPos.y), m_vSize - m_vMargin * 2);
}

void				CUITreeView::SetDoubleClickTreeOpen(bool IsOpen)
{
	m_IsDoubleClickTreeOpen = IsOpen;

	int nNumChild = m_pTreeViewItem_Root->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(m_pTreeViewItem_Root->GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->SetDoubleClickTreeOpen(m_IsDoubleClickTreeOpen);
		}
	}
}

bool				CUITreeView::IsDoubleClickTreeOpen()
{
	return m_IsDoubleClickTreeOpen;
}

void				CUITreeView::SetSelectedItem(CUITreeViewItem * pTreeViewItem)
{
	m_pTreeViewItem_Selected = pTreeViewItem;

	NotificationArg_TreeView Arg;
	Arg.m_pTreeView = this;
	CNotificationCenter::GetInstance()->PostNotification(Notification_TreeViewItem_HideSelImage, &Arg);

	if( pTreeViewItem != NULL )
		pTreeViewItem->SetSelected();
}

CUITreeViewItem *	CUITreeView::GetSelectedItem()
{
	return m_pTreeViewItem_Selected;
}

void				CUITreeView::OnSort()
{
	m_pTreeViewItem_Root->OnSort();
}

void				CUITreeView::OnButtonClick_Empty()
{
	(*m_pDelegate_TreeView)(this);
}

void				CUITreeView::OnMouseWheelMove(int iMoveWheelCount)
{
	RECT rc;
	rc.left = m_pDummyCtrl->GetWorldPos()->x + m_vPos.x;
	rc.right = m_pDummyCtrl->GetWorldPos()->x + m_vPos.x + m_vSize.x;
	rc.top = m_pDummyCtrl->GetWorldPos()->y + m_vPos.y;
	rc.bottom = m_pDummyCtrl->GetWorldPos()->y + m_vPos.y + m_vSize.y;

	GwVector2 vCurPos;
	vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
	vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;

	if( rc.left <= vCurPos.x && rc.right >= vCurPos.x && rc.top <= vCurPos.y && rc.bottom >= vCurPos.y )
	{
		float	fScrollValue = m_pScroll_V->GetCurrValue();

		fScrollValue -= iMoveWheelCount;
		fScrollValue = Clamp(fScrollValue, 0.f, (float)m_pScroll_V->GetRange_Max());

		m_pScroll_V->SetCurrValue(fScrollValue);
	}
}

IDelegate		*	CUITreeView::GetDelegate_TreeView()
{
	return m_pDelegate_TreeView;
}

IDelegate		*	CUITreeView::GetDelegate_DragReceive()
{
	return m_pDelegate_DragReceive;
}

CUITreeViewItem	*	CUITreeView::GetTreeViewItem_Root()
{
	return m_pTreeViewItem_Root;
}
