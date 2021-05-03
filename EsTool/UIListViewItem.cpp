#include "StdAfx.h"
#include "UIListViewItem.h"

#include "Event.h"
#include "Util.h"

#define SAFE_ADD_CONTROL(parent, child)		if( parent != NULL ) { if(parent->HasChild(child) == false )	parent->AddControl(child); }
#define SAFE_REMOVE_CONTROL(parent, child)	if( parent != NULL ) { if(parent->HasChild(child) == true )		parent->RemoveControl(child); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CButtonCallback_Sel

class CUIListViewItem::CButtonCallback_Sel : public IGwUIButton_Callback
{
public:
	CButtonCallback_Sel(CUIListViewItem * pListViewItem) : m_pListViewItem(pListViewItem) { }

	virtual void	OnButtonDown(class IGwUIButton* pControl) 
	{
		m_pListViewItem->OnButtonDown_Item();
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 
	{
		m_pListViewItem->OnButtonClick_Item();
	}
	virtual void	OnMouseEnter(class IGwUIButton* pControl) 
	{
		m_pListViewItem->OnMouseEnter_Item();
	}
	virtual void	OnMouseLeave(class IGwUIButton* pControl)
	{
		m_pListViewItem->OnMouseLeave_Item();
	}

private:
	CUIListViewItem		*	m_pListViewItem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIListViewItem
CUIListViewItem::CUIListViewItem(CUIListView * pListView, DWORD dwItemIndex, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, 
								 bool IsNameChange, int nListViewWidth, int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, 
								 GwColor fontColor, GwColor selColor, const char * pszEmpty, const char * pszSelect, const char * pszEdit)
: CUIDragItemBase(eDragObject_ListViewItem, eDragSender, eDragReceiver), m_pListView(pListView), m_IsDragSendUse(IsDragSendUse), m_IsNameChange(IsNameChange), 
  m_nItemHeightInterval(nItemHeightInterval), m_fTime(0), m_fClickTime(0), m_IsDBClickReady(true), m_vClickPos(GwVector2(0,0)), m_IsDowning(false), m_fScroll_H_MoveValue(0.0f), m_fMargin_X(0), m_IsDefaultItemInfo(true)
{
	m_pBaseControl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pBaseControl->SetLocalPos(GwVector2(0,0));

	m_pButton_Sel = CUIHelper::CreateUIButton(pszEmpty);
	m_pSprite_Sel = CUIHelper::CreateUISprite(pszSelect);
	m_pStatic_Name = CUIHelper::CreateUIStatic(nFontSize, pszFontName, eWeightType, fontColor, false);
	m_pSprite_EditboxBack = CUIHelper::CreateUISprite(pszEdit);
	m_pEditbox_ReName = CUIHelper::CreateUIEditBox(nFontSize, pszFontName, eWeightType, fontColor, selColor);
	m_pEditbox_ReName->SetForceCollisSize(*m_pSprite_EditboxBack->GetCollisSize());

	m_pSprite_Sel->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
	m_pStatic_Name->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);

	m_pButton_Sel->SetSize_NormalState(GwVector2(nListViewWidth, nItemHeightInterval * 2 + nFontSize));
	m_pSprite_Sel->SetSize_NormalState(GwVector2(nListViewWidth, nItemHeightInterval * 2 + nFontSize));

	CButtonCallback_Sel * pCallback = new CButtonCallback_Sel(this);
	m_pButton_Sel->SetCallback(pCallback); pCallback->Release();

	for( int i = 0; i < eDelegate_ListView_Max; ++i )
	{
		m_pDelegate_ListView[i] = new IDelegate();
	}

	m_pListViewItemInfo = new CUIListViewItemInfo("", NULL);

	m_strObserverName = "CUIListViewItem";
	char szTemp[32] = { 0, };
	itoa(m_pListView->GetListViewIndex(), szTemp, 10);
	m_strObserverName += "_";
	m_strObserverName += szTemp;
	itoa(dwItemIndex, szTemp, 10);
	m_strObserverName += "_";
	m_strObserverName += szTemp;
	CNotificationCenter::GetInstance()->AddObserver(Notification_ListViewItem_HideSelImage, m_strObserverName.c_str(), this, &CUIListViewItem::OnNotification_HideSelImage);
}

CUIListViewItem::~CUIListViewItem(void)
{
	SAFE_RELEASE(m_pBaseControl);

	SAFE_RELEASE(m_pStatic_Name);
	SAFE_RELEASE(m_pSprite_EditboxBack);
	SAFE_RELEASE(m_pEditbox_ReName);
	SAFE_RELEASE(m_pSprite_Sel);
	SAFE_RELEASE(m_pButton_Sel);

	for( int i = 0; i < eDelegate_ListView_Max; ++i )
	{
		SAFE_RELEASE(m_pDelegate_ListView[i]);
	}

	if( CNotificationCenter::GetInstance() != NULL ) 
	{
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_ListViewItem_HideSelImage, m_strObserverName.c_str());
	}

	m_pListViewItemInfo->Release();
}

void				CUIListViewItem::Update(float fDeltaTime)
{
	m_fTime += fDeltaTime;

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUIListViewItem * pListViewItem = dynamic_cast<CUIListViewItem*>(GetChild_byIndex(i));

		if( pListViewItem != NULL ) 
		{
			pListViewItem->Update(fDeltaTime);
		}
	}

	if( m_IsDragSendUse == true )
	{
		if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE )
		{
			m_IsDowning = false;
		}

		if( m_IsDowning == true )
		{
			GwVector2 vCurrPos;
			vCurrPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
			vCurrPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;

			float fLength = D3DXVec2Length(&(m_vClickPos - vCurrPos));
			if( fLength > 10 )
			{
				m_IsDowning = false;
				// Drag Start

				CUIDragObject::GetInstance()->Show(m_pStatic_Name, this, this);
			}
		}
	}
}

void				CUIListViewItem::SetListViewItemInfo(CUIListViewItemInfo * pListViewItemInfo)
{
	m_IsDefaultItemInfo = false;

	m_pListViewItemInfo->Release();
	m_pListViewItemInfo = pListViewItemInfo;
	m_pListViewItemInfo->AddRef();

	wchar_t wszItemName[STRING_MAX] = { 0, };
	ConvertAnsiStringToWideCch(wszItemName, m_pListViewItemInfo->GetName(), STRING_MAX);
	m_pStatic_Name->SetText(wszItemName);

	SetName(m_pListViewItemInfo->GetName());

	UpdateControlState();
}

CUIListViewItemInfo	*	CUIListViewItem::GetListViewItemInfo()
{
	return m_pListViewItemInfo;
}

void				CUIListViewItem::SetDefaultItemInfo()
{
	m_IsDefaultItemInfo = true;

	m_pListViewItemInfo->Release();
	m_pListViewItemInfo = new CUIListViewItemInfo("", NULL);

	UpdateControlState();
}

bool				CUIListViewItem::IsDefaultItemInfo()
{
	return m_IsDefaultItemInfo;
}

bool				CUIListViewItem::OnCheckDrag(GwVector2 vCursorPos)
{
	GwVector vPos = *m_pBaseControl->GetWorldPos();
	GwVector2 vSize = *m_pButton_Sel->GetSize();

	if( vPos.x <= vCursorPos.x && vPos.y <= vCursorPos.y && vPos.x + vSize.x >= vCursorPos.x && vPos.y + vSize.y >= vCursorPos.y )
	{
		CUIDragItemBase * pSrcDragItem = CUIDragObject::GetInstance()->GetDragItemBase();
		(*m_pDelegate_Drag[eDelegate_Drag_Receive])(pSrcDragItem, (CUIDragItemBase*)(this), pSrcDragItem->GetDragObjectSender());

		IDelegate * pDelegate_Send = pSrcDragItem->GetDelegate_Drag(eDelegate_Drag_Send);
		(*pDelegate_Send)(pSrcDragItem, (CUIDragItemBase*)(this), GetDragObjectReceiver());

		return true;
	}
	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUIListViewItem * pListViewItem = dynamic_cast<CUIListViewItem*>(GetChild_byIndex(i));

		if( pListViewItem != NULL && pListViewItem->OnCheckDrag(vCursorPos) == true )
		{
			return true;
		}
	}

	return false;
}

IGwUIDummyCtrl	*	CUIListViewItem::GetBaseControl()
{
	return m_pBaseControl;
}

void				CUIListViewItem::SetLocalPos(GwVector2 vPos)
{
	m_fMargin_X = vPos.x;
	m_pBaseControl->SetLocalPos(GwVector2(0, vPos.y));
	m_pStatic_Name->SetLocalPos(GwVector2(m_fScroll_H_MoveValue + 3 + m_fMargin_X, m_nItemHeightInterval));
	m_pEditbox_ReName->SetLocalPos(GwVector2(m_fScroll_H_MoveValue + 3 + m_fMargin_X, m_nItemHeightInterval));
}

void				CUIListViewItem::SetScroll_H_Move(float fAdd)
{
	m_fScroll_H_MoveValue = -fAdd;
	m_pStatic_Name->SetLocalPos(GwVector2(m_fScroll_H_MoveValue + 3 + m_fMargin_X, m_nItemHeightInterval));
	m_pEditbox_ReName->SetLocalPos(GwVector2(m_fScroll_H_MoveValue + 3 + m_fMargin_X, m_nItemHeightInterval));
}

void				CUIListViewItem::UpdateControlState()
{
	SAFE_REMOVE_CONTROL(m_pBaseControl, m_pButton_Sel);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pSprite_Sel);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pStatic_Name);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pSprite_EditboxBack);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pEditbox_ReName);

	switch(m_pListViewItemInfo->GetListViewItem_State())
	{
	case eListViewItem_State_Normal:
		{
			SAFE_ADD_CONTROL(m_pBaseControl,m_pButton_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pStatic_Name);
		}
		break;
	case eListViewItem_State_Select:
		{
			SAFE_ADD_CONTROL(m_pBaseControl,m_pButton_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pSprite_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pStatic_Name);
		}
		break;
	case eListViewItem_State_Edit:
		{
			SAFE_ADD_CONTROL(m_pBaseControl,m_pButton_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pSprite_Sel);	
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pStatic_Name);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pSprite_EditboxBack);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pEditbox_ReName);
		}
		break;
	}
}

IGwRefObject	*	CUIListViewItem::GetParameta()
{
	return m_pListViewItemInfo->GetParameta();
}

void				CUIListViewItem::OnNotification_HideSelImage(NotificationArg* p)
{
	NotificationArg_ListView * pArg = dynamic_cast<NotificationArg_ListView*>(p);

	if( pArg != NULL && pArg->m_pListView == m_pListView )
	{
		if( m_pListViewItemInfo->GetListViewItem_State() != eListViewItem_State_None )
		{
			m_pListViewItemInfo->SetListViewItem_State(eListViewItem_State_Normal);
			UpdateControlState();
		}
	}
}

void				CUIListViewItem::OnButtonClick_Item()
{
	if( IsDefaultItemInfo() == false )
	{
		if( m_pListViewItemInfo->GetListViewItem_State() != eListViewItem_State_None )
		{
			NotificationArg_ListView Arg;
			Arg.m_pListView = m_pListView;
			CNotificationCenter::GetInstance()->PostNotification(Notification_ListViewItem_HideSelImage, &Arg);

			m_pListViewItemInfo->SetListViewItem_State(eListViewItem_State_Select); 
			UpdateControlState();
		}

		if( m_fTime - m_fClickTime <= 0.5f )
		{
			m_fClickTime = 0;

			(*m_pDelegate_ListView[eDelegate_ListView_DBClick])(this);
		}
		else
		{
			m_fClickTime = m_fTime;

			(*m_pDelegate_ListView[eDelegate_ListView_Click])(this);
		}
	}
}

void				CUIListViewItem::OnButtonDown_Item()
{
	if( IsDefaultItemInfo() == false )
	{
		m_IsDowning = true;
		m_vClickPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		m_vClickPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
	}
}

void				CUIListViewItem::OnMouseEnter_Item()
{
}

void				CUIListViewItem::OnMouseLeave_Item()
{
	m_IsDowning = false;
}

IDelegate		*	CUIListViewItem::GetDelegate_ListView(eDelegate_ListView eDelegate)	
{
	return m_pDelegate_ListView[eDelegate]; 
}