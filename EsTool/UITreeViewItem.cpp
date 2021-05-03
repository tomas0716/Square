#include "StdAfx.h"
#include "UITreeViewItem.h"

#include "Event.h"
#include "Util.h"

#define SAFE_ADD_CONTROL(parent, child)		if( parent != NULL && child != NULL ) { if(parent->HasChild(child) == false )	parent->AddControl(child); }
#define SAFE_REMOVE_CONTROL(parent, child)	if( parent != NULL && child != NULL ) { if(parent->HasChild(child) == true )	parent->RemoveControl(child); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CButtonCallback_Sel

class CUITreeViewItem::CButtonCallback_Sel : public IGwUIButton_Callback
{
public:
	CButtonCallback_Sel(CUITreeViewItem * pTreeViewItem) : m_pTreeViewItem(pTreeViewItem) { }

	virtual void	OnButtonDown(class IGwUIButton* pControl) 
	{
		m_pTreeViewItem->OnButtonDown_Item();
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 
	{
		m_pTreeViewItem->OnButtonClick_Item();
	}
	virtual void	OnMouseEnter(class IGwUIButton* pControl) 
	{
		m_pTreeViewItem->OnMouseEnter_Item();
	}
	virtual void	OnMouseLeave(class IGwUIButton* pControl)
	{
		m_pTreeViewItem->OnMouseLeave_Item();
	}

private:
	CUITreeViewItem		*	m_pTreeViewItem;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUITreeViewItem

CUITreeViewItem::CUITreeViewItem(CUITreeView * pTreeView)
: CUIDragItemBase(eDragObject_TreeViewItem, eDragObjectSender_None, eDragObjectReceiver_None), m_pTreeView(pTreeView), m_IsDragSendUse(false), m_IsNameChange(false), m_pStatic_Name(NULL), 
m_pButton_Plus(NULL), m_pButton_Minus(NULL), m_pSprite_EditboxBack(NULL), m_pEditbox_ReName(NULL), m_pSprite_Sel(NULL), m_pButton_Sel(NULL),
m_IsRoot(false), m_eState(eState_Hide), m_IsSelected(false), m_vClickPos(GwVector2(0,0)), m_IsDowning(false), m_IsVisible(false), m_IsEditFirstFrameBlock(false), m_IsDoubleClickTreeOpen(true)
{
	m_pParameta = new IGwRefObject();

	m_pBaseControl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pBaseControl->SetLocalPos(GwVector2(0,0));

	for( int i = 0; i < eDelegate_TreeView_Max; ++i )
	{
		m_pDelegate_TreeView[i] = new IDelegate();
	}
}

CUITreeViewItem::CUITreeViewItem(CUITreeView * pTreeView, DWORD dwItemIndex, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, bool IsNameChange, bool IsDoubleClickTreeOpen, int nTreeViewWidth, 
								 int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor selColor, const char * pszItemName, 
								 const char * pszEmpty, const char * pszPlus, const char * pszMinus, const char * pszSelect, const char * pszEdit, IGwRefObject * pParameta)
: INode(pszItemName), CUIDragItemBase(eDragObject_TreeViewItem, eDragSender, eDragReceiver), m_pTreeView(pTreeView), m_IsDragSendUse(IsDragSendUse), m_IsNameChange(IsNameChange), m_nTreeViewWidth(nTreeViewWidth), 
  m_nItemHeightInterval(nItemHeightInterval), m_fFontSize(nFontSize), m_IsChildOpen(false), m_pParameta(pParameta), m_IsVisible(false),
  m_fTime(0), m_fClickTime(0), m_IsDBClickReady(true), m_IsRoot(false), m_eState(eState_Hide), m_IsSelected(false), m_vClickPos(GwVector2(0,0)), m_IsDowning(false), m_IsEditFirstFrameBlock(false), m_IsDoubleClickTreeOpen(IsDoubleClickTreeOpen)
{
	if( m_pParameta == NULL )	m_pParameta = new IGwRefObject();
	else						m_pParameta->AddRef();

	m_pBaseControl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pBaseControl->SetLocalPos(GwVector2(0,0));

	m_pButton_Sel = CUIHelper::CreateUIButton(pszEmpty);
	m_pSprite_Sel = CUIHelper::CreateUISprite(pszSelect);
	m_pButton_Plus = CUIHelper::CreateUIButton(pszPlus);
	m_pButton_Minus = CUIHelper::CreateUIButton(pszMinus);
	m_pStatic_Name = CUIHelper::CreateUIStatic(nFontSize, pszFontName, eWeightType, fontColor, false);
	m_pSprite_EditboxBack = CUIHelper::CreateUISprite(pszEdit);
	m_pEditbox_ReName = CUIHelper::CreateUIEditBox(nFontSize, pszFontName, eWeightType, fontColor, fontColor, selColor);

	m_pSprite_Sel->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
	m_pStatic_Name->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);

	wchar_t wszItemName[STRING_MAX] = { 0, };
	ConvertAnsiStringToWideCch(wszItemName, pszItemName, STRING_MAX);
	m_pStatic_Name->SetText(wszItemName);
	m_pEditbox_ReName->SetText(wszItemName);

	m_pButton_Sel->SetSize_NormalState(GwVector2(nTreeViewWidth, nItemHeightInterval * 2 + nFontSize));
	m_pSprite_Sel->SetSize_NormalState(GwVector2(nTreeViewWidth, nItemHeightInterval * 2 + nFontSize));
	m_pSprite_EditboxBack->SetSize_NormalState(GwVector2(nTreeViewWidth, nItemHeightInterval * 2 + nFontSize));
	m_pEditbox_ReName->SetLimitEditboxSizeX(nTreeViewWidth - 20);

	IGwUIButton_Callback * pBtnCallback;
	pBtnCallback = CUIHelper::CreateButtonCallback<TBtnCallback_Delegate>(this, &CUITreeViewItem::OnButtonClick_Plus);
	m_pButton_Plus->SetCallback(pBtnCallback); pBtnCallback->Release();

	pBtnCallback = CUIHelper::CreateButtonCallback<TBtnCallback_Delegate>(this, &CUITreeViewItem::OnButtonClick_Minus);
	m_pButton_Minus->SetCallback(pBtnCallback); pBtnCallback->Release();

	CButtonCallback_Sel * pCallback = new CButtonCallback_Sel(this);
	m_pButton_Sel->SetCallback(pCallback); pCallback->Release();

	for( int i = 0; i < eDelegate_TreeView_Max; ++i )
	{
		m_pDelegate_TreeView[i] = new IDelegate();
	}

	SetVisible(false);

	m_strObserverName = "CUITreeViewItem";
	char szTemp[32] = { 0, };
	itoa(m_pTreeView->GetTreeViewIndex(), szTemp, 10);
	m_strObserverName += "_";
	m_strObserverName += szTemp;
	itoa(dwItemIndex, szTemp, 10);
	m_strObserverName += "_";
	m_strObserverName += szTemp;
	CNotificationCenter::GetInstance()->AddObserver(Notification_TreeViewItem_HideSelImage, m_strObserverName.c_str(), this, &CUITreeViewItem::OnNotification_HideSelImage);
}

CUITreeViewItem::~CUITreeViewItem(void)
{
	SAFE_RELEASE(m_pBaseControl);

	SAFE_RELEASE(m_pStatic_Name);
	SAFE_RELEASE(m_pButton_Plus);
	SAFE_RELEASE(m_pButton_Minus);
	SAFE_RELEASE(m_pSprite_EditboxBack);
	SAFE_RELEASE(m_pEditbox_ReName);
	SAFE_RELEASE(m_pSprite_Sel);
	SAFE_RELEASE(m_pButton_Sel);

	m_pParameta->Release();

	for( int i = 0; i < eDelegate_TreeView_Max; ++i )
	{
		SAFE_RELEASE(m_pDelegate_TreeView[i]);
	}

	if( CNotificationCenter::GetInstance() != NULL ) 
	{
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_TreeViewItem_HideSelImage, m_strObserverName.c_str());
	}
}

void				CUITreeViewItem::Update(float fDeltaTime)
{
	m_fTime += fDeltaTime;

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

	if( m_eState == eState_Edit && m_IsEditFirstFrameBlock == false )
	{
		bool IsComplete = false;
		if( Gw::GetKeyMapper()->WasKeyDown(VK_LBUTTON) == TRUE )
		{
			GwVector2 vCurrPos;
			vCurrPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
			vCurrPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;

			GwVector vLeftTop = *m_pSprite_EditboxBack->GetWorldPos();
			GwVector2 vSize = *m_pSprite_EditboxBack->GetCollisSize();
			if( vLeftTop.x > vCurrPos.x || vLeftTop.x + vSize.x < vCurrPos.x && vLeftTop.y > vCurrPos.y || vLeftTop.y + vSize.y < vCurrPos.y )
			{
				IsComplete = true;
			}
		}

		if( Gw::GetKeyMapper()->IsKeyDown(VK_RETURN) )
		{
			IsComplete = true;
		}

		if( IsComplete == true )
		{
			wstring wstrItemName = m_pEditbox_ReName->GetText();

			if( wstrItemName != L"" )
			{
				m_pStatic_Name->SetText(wstrItemName.c_str());

				char szItemName[STRING_MAX] = { 0, };
				ConvertWideStringToAnsiCch(szItemName, wstrItemName.c_str(), STRING_MAX);
				SetName(szItemName);

				// 임시로 이름 바꾸도록 만든다.
				INode * pNode = dynamic_cast<INode*>(m_pParameta);
				if( pNode != NULL )
				{
					pNode->SetName(szItemName);
				}

				m_eState = eState_Select;
				UpdateControlState();

				m_pTreeView->OnSort();
				m_pTreeView->UpdateItemPos();
				m_pTreeView->UpdateScroll();
			}
		}
	}

	if( m_eState == eState_Edit && m_IsEditFirstFrameBlock == true )
	{
		m_IsEditFirstFrameBlock = false;
	}

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->Update(fDeltaTime);
		}
	}

}

bool				CUITreeViewItem::OnCheckDrag(GwVector2 vCursorPos)
{
	if( m_IsVisible == true || m_IsRoot == true )
	{
		if( m_IsRoot == false )
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
		}

		int nNumChild = GetNumChild();

		for( int i = 0; i < nNumChild; ++i )
		{
			CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

			if( pTreeViewItem != NULL && pTreeViewItem->OnCheckDrag(vCursorPos) == true )
			{
				return true;
			}
		}
	}

	return false;
}

void				CUITreeViewItem::AddChild(INode * pNode)
{
	CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(pNode);

	if( pTreeViewItem != NULL ) 
	{
		m_pBaseControl->AddControl(pTreeViewItem->GetBaseControl());
	}

	INode::AddChild(pNode);
	UpdatePlusMinus();

	m_pTreeView->OnSort();
	m_pTreeView->UpdateItemPos();
	m_pTreeView->UpdateScroll();
}

void				CUITreeViewItem::RemoveChild(INode * pNode)
{
	CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(pNode);

	if( pTreeViewItem != NULL ) 
	{
		m_pBaseControl->RemoveControl(pTreeViewItem->GetBaseControl());
	}

	INode::RemoveChild(pNode);
	UpdatePlusMinus();

	m_pTreeView->OnSort();
	m_pTreeView->UpdateItemPos();
	m_pTreeView->UpdateScroll();
}

void				CUITreeViewItem::UpdatePlusMinus()
{
	switch(m_eState)
	{
	case eState_Hide:
		{
			SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pButton_Plus);
			SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pButton_Minus);
		}
		break;

	case eState_Normal:
		{
			int nNumChild = GetNumChild();
			if( nNumChild == 0 )
			{
				SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Plus);
				SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Minus);
			}
			else
			{
				if( m_IsChildOpen == true )
				{
					SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Plus);
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
				else
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Plus);
					SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
			}
		}
		break;

	case eState_Select:
		{
			int nNumChild = GetNumChild();
			if( nNumChild == 0 )
			{
				SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Plus);
				SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Minus);
			}
			else
			{
				if( m_IsChildOpen == true )
				{
					SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Plus);
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
				else
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Plus);
					SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
			}
		}
		break;

	case eState_Edit:
		{
			int nNumChild = GetNumChild();
			if( nNumChild == 0 )
			{
				SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Plus);
				SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Minus);
			}
			else
			{
				if( m_IsChildOpen == true )
				{
					SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Plus);
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
				else
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Plus);
					SAFE_REMOVE_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
			}
		}
		break;
	}
}

void				CUITreeViewItem::SetSelected()
{
	m_IsSelected = true;
	m_eState = eState_Select;
	UpdateControlState();
}

void				CUITreeViewItem::SetChildOpenFlag(bool IsChildOpen)
{
	m_IsChildOpen = IsChildOpen;

	SetVisible(m_IsChildOpen);
}

void				CUITreeViewItem::SetRoot()
{
	m_IsRoot = true;
}

IGwUIDummyCtrl	*	CUITreeViewItem::GetBaseControl()
{
	return m_pBaseControl;
}

void				CUITreeViewItem::SetVisible(bool IsVisible)
{
	m_IsVisible = IsVisible;

	if( m_pBaseControl != NULL ) 
	{
		if( IsVisible == true )
		{
			if( m_IsSelected == true )	m_eState = eState_Select;
			else						m_eState = eState_Normal;

			UpdateControlState();

			int nNumChild = GetNumChild();

			for( int i = 0; i < nNumChild; ++i )
			{
				CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

				if( pTreeViewItem != NULL && m_IsChildOpen == true ) 
				{
					pTreeViewItem->SetVisible(true);
				}
				else 
				{
					pTreeViewItem->SetVisible(false);
				}
			}
		}
		else 
		{
			m_eState = eState_Hide;

			UpdateControlState();

			int nNumChild = GetNumChild();

			for( int i = 0; i < nNumChild; ++i )
			{
				CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

				if( pTreeViewItem != NULL ) 
				{
					pTreeViewItem->SetVisible(false);
				}
			}
		}
	}
}

void				CUITreeViewItem::UpdateVisible(GwVector2 vViewPos, GwVector2 vViewSize)
{
	GwVector2 vPos = *m_pBaseControl->GetWorldPos();

	if( m_pButton_Sel != NULL )
	{
		GwVector2 vSize = *m_pButton_Sel->GetCollisSize();

		if( vPos.y + vSize.y < vViewPos.y || vPos.y > vViewPos.y + vViewSize.y )
			SetVisible_ViewOut(false);
		else
			SetVisible_ViewOut(true);
	}

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->UpdateVisible(vViewPos, vViewSize);
		}
	}
}

void				CUITreeViewItem::SetVisible_ViewOut(bool IsVisible)
{
	if( m_pBaseControl != NULL ) 
	{
		if( m_IsVisible == true && IsVisible == true )
		{
			if( m_IsSelected == true )	m_eState = eState_Select;
			else						m_eState = eState_Normal;

			UpdateControlState();
		}
		else 
		{
			m_eState = eState_Hide;

			UpdateControlState();
		}
	}
}

void				CUITreeViewItem::SetDoubleClickTreeOpen(bool IsOpen)
{
	m_IsDoubleClickTreeOpen = IsOpen;

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->SetDoubleClickTreeOpen(m_IsDoubleClickTreeOpen);
		}
	}
}

CUITreeViewItem *	CUITreeViewItem::FindTreeViewItem_byItemName(string strItemName)
{
	StringToLower(strItemName);

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			string strItem = pTreeViewItem->GetName();
			StringToLower(strItem);

			if( strItemName == strItem )
				return pTreeViewItem;
		}
	}

	return NULL;
}

void				CUITreeViewItem::SetLocalPos(GwVector2 vPos)
{
	if( m_IsRoot == true )
	{
		m_pBaseControl->SetLocalPos(vPos);
	}
	else 
	{
		m_pBaseControl->SetLocalPos(GwVector2(0,vPos.y));
		float fPlusInterval = (m_fFontSize * 0.5f) - (m_pButton_Plus->GetSize()->y * 0.5f);
		m_pButton_Plus->SetLocalPos(GwVector2(vPos.x, fPlusInterval + m_nItemHeightInterval));
		float fMinusInterval = (m_fFontSize * 0.5f) - (m_pButton_Plus->GetSize()->y * 0.5f);
		m_pButton_Minus->SetLocalPos(GwVector2(vPos.x, fMinusInterval + m_nItemHeightInterval));
		m_pStatic_Name->SetLocalPos(GwVector2(vPos.x + m_pButton_Plus->GetSize()->x + 3, m_nItemHeightInterval));
		m_pEditbox_ReName->SetLocalPos(GwVector2(vPos.x + m_pButton_Plus->GetSize()->x + 3, m_nItemHeightInterval));

		vPos.x += 14;
		vPos.y = m_pButton_Sel->GetCollisSize()->y;

		if( m_IsChildOpen == true )
		{
			int nNumChild = GetNumChild();

			for( int i = 0; i < nNumChild; ++i )
			{
				CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));
				pTreeViewItem->SetLocalPos(vPos);
				vPos.y += pTreeViewItem->GetHeight_IncludeChild();
			}
		}
	}
}

void				CUITreeViewItem::SetItemName(const char * pszItemName)
{
	SetName(pszItemName);

	wchar_t wszItemName[STRING_MAX] = { 0, };
	ConvertAnsiStringToWideCch(wszItemName, pszItemName, STRING_MAX);
	m_pStatic_Name->SetText(wszItemName);

	m_pTreeView->OnSort();
	m_pTreeView->UpdateItemPos();
	m_pTreeView->UpdateScroll();
}

const char		*	CUITreeViewItem::GetItemName()
{
	return GetName().c_str();
}

float				CUITreeViewItem::GetHeight()
{
	if( m_pButton_Sel != NULL ) 
	{
		GwVector2 vSize = *m_pButton_Sel->GetCollisSize();
		return vSize.y;
	}

	return 0;
}

float				CUITreeViewItem::GetHeight_IncludeChild()
{
	float fHeight = GetHeight();

	if( m_IsChildOpen == true )
	{
		int nNumChild = GetNumChild();
		for( int i = 0; i < nNumChild; ++i ) 
		{
			CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

			if( pTreeViewItem != NULL ) 
			{
				fHeight += pTreeViewItem->GetHeight_IncludeChild();
			}
		}
	}
	return fHeight;
}

void				CUITreeViewItem::UpdateControlState()
{
	SAFE_REMOVE_CONTROL(m_pBaseControl, m_pButton_Sel);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pSprite_Sel);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pStatic_Name);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pButton_Plus);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pButton_Minus);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pSprite_EditboxBack);
	SAFE_REMOVE_CONTROL(m_pButton_Sel, m_pEditbox_ReName);

	switch(m_eState)
	{
	case eState_Normal:
		{
			SAFE_ADD_CONTROL(m_pBaseControl,m_pButton_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pStatic_Name);

			int nNumChild = GetNumChild();
			if( nNumChild != 0 )
			{
				if( m_IsChildOpen == true )
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
				else
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Plus);
				}
			}
		}
		break;

	case eState_Select:
		{
			SAFE_ADD_CONTROL(m_pBaseControl,m_pButton_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pSprite_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pStatic_Name);

			int nNumChild = GetNumChild();
			if( nNumChild != 0 )
			{
				if( m_IsChildOpen == true )
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
				else
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Plus);
				}
			}

			IGwUIControl::SetFocusControl(m_pButton_Sel);
		}
		break;

	case eState_Edit:
		{
			SAFE_ADD_CONTROL(m_pBaseControl,m_pButton_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pSprite_Sel);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pStatic_Name);

			int nNumChild = GetNumChild();
			if( nNumChild != 0 )
			{
				if( m_IsChildOpen == true )
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Minus);
				}
				else
				{
					SAFE_ADD_CONTROL(m_pButton_Sel,m_pButton_Plus);
				}
			}

			SAFE_ADD_CONTROL(m_pButton_Sel,m_pSprite_EditboxBack);
			SAFE_ADD_CONTROL(m_pButton_Sel,m_pEditbox_ReName);
		}
		break;
	}
}

bool				CompareName(INode * pNode_A, INode * pNode_B)
{
	return pNode_A->GetName() < pNode_B->GetName();
}

void				CUITreeViewItem::OnSort()
{
	sort(m_ChildList.begin(), m_ChildList.end(), CompareName);

	for( int i = 0; i < GetNumChild(); ++i )
	{
		CUITreeViewItem * pItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pItem != NULL )
		{
			pItem->OnSort();
		}
	}
}	

void				CUITreeViewItem::SetParameta(IGwRefObject * pParameta)
{
	m_pParameta->Release();
	m_pParameta = pParameta;

	if( m_pParameta == NULL )	m_pParameta = new IGwRefObject();
	else						m_pParameta->AddRef();
}

IGwRefObject	*	CUITreeViewItem::GetParameta()
{
	return m_pParameta;
}

void				CUITreeViewItem::OnNotification_HideSelImage(NotificationArg* p)
{
	if( m_eState == eState_Edit)
	{
		m_IsEditFirstFrameBlock = false;

		wstring wstrItemName = m_pEditbox_ReName->GetText();

		if( wstrItemName != L"" )
		{
			m_pStatic_Name->SetText(wstrItemName.c_str());

			char szItemName[STRING_MAX] = { 0, };
			ConvertWideStringToAnsiCch(szItemName, wstrItemName.c_str(), STRING_MAX);
			SetName(szItemName);

			m_eState = eState_Select;
			UpdateControlState();
		}
	}

	NotificationArg_TreeView * pArg = dynamic_cast<NotificationArg_TreeView*>(p);

	if( pArg != NULL && pArg->m_pTreeView == m_pTreeView )
	{
		m_IsSelected = false;

		if( m_eState == eState_Hide ) 
			return;

		m_eState = eState_Normal;
		UpdateControlState();
	}
}

void				CUITreeViewItem::OnButtonClick_Plus()
{
	m_IsChildOpen = true;
	UpdateControlState();

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->SetVisible(true);
		}
	}

	m_pTreeView->UpdateItemPos();
	m_pTreeView->UpdateScroll();
}

void				CUITreeViewItem::OnButtonClick_Minus()
{
	m_IsChildOpen = false;
	UpdateControlState();

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

		if( pTreeViewItem != NULL ) 
		{
			pTreeViewItem->SetVisible(false);
		}
	}

	m_pTreeView->UpdateItemPos();
	m_pTreeView->UpdateScroll();
}

void				CUITreeViewItem::OnButtonClick_Item()
{
	if( m_fTime - m_fClickTime <= 0.5f )
	{
		if( m_IsDoubleClickTreeOpen == true )
		{
			m_IsChildOpen = !m_IsChildOpen;

			int nNumChild = GetNumChild();

			for( int i = 0; i < nNumChild; ++i )
			{
				CUITreeViewItem * pTreeViewItem = dynamic_cast<CUITreeViewItem*>(GetChild_byIndex(i));

				if( pTreeViewItem != NULL ) 
				{
					pTreeViewItem->SetVisible(m_IsChildOpen);
				}
			}

			m_pTreeView->UpdateItemPos();
			m_pTreeView->UpdateScroll();
		}

		m_fClickTime = 0;

		(*m_pDelegate_TreeView[eDelegate_TreeView_DBClick])(this);
	}
	else if( m_fTime - m_fClickTime > 0.5f && m_fTime - m_fClickTime <= 1.0f )
	{
		m_fClickTime = 0;

		if( m_IsNameChange == true )
		{
			m_eState = eState_Edit;
			UpdateControlState();
			m_IsEditFirstFrameBlock = true;
		}
	}
	else
	{
		m_fClickTime = m_fTime;

		(*m_pDelegate_TreeView[eDelegate_TreeView_Click])(this);

		//NotificationArg_TreeView Arg;
		//Arg.m_pTreeView = m_pTreeView;
		//CNotificationCenter::GetInstance()->PostNotification(Notification_TreeViewItem_HideSelImage, &Arg);

		m_pTreeView->SetSelectedItem(this);

		m_IsSelected = true;
		m_eState = eState_Select;
		UpdateControlState();
	}
}

void				CUITreeViewItem::OnButtonDown_Item()
{
	m_IsDowning = true;
	m_vClickPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
	m_vClickPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
}

void				CUITreeViewItem::OnMouseEnter_Item()
{
}

void				CUITreeViewItem::OnMouseLeave_Item()
{
	m_IsDowning = false;
}

IDelegate		*	CUITreeViewItem::GetDelegate_TreeView(eDelegate_TreeView eDelegate)	
{
	return m_pDelegate_TreeView[eDelegate]; 
}