#include "StdAfx.h"
#include "UIListView.h"


UINT			CUIListView::ms_ListViewIndex = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CScrollCallback_V

class CUIListView::CScrollCallback_V : public IGwUISlider_Callback
{
public:
	CScrollCallback_V(CUIListView* pUIListView) :m_pUIListView(pUIListView)				{ }

	virtual void	OnChangeValue(class IGwUISliderBase* pMyControl,bool bFromInput)
	{
		m_pUIListView->UpdateScroll_V();
	}

private:
	CUIListView * m_pUIListView;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CScrollCallback_H

class CUIListView::CScrollCallback_H : public IGwUISlider_Callback
{
public:
	CScrollCallback_H(CUIListView* pUIListView) :m_pUIListView(pUIListView)				{ }

	virtual void	OnChangeValue(class IGwUISliderBase* pMyControl,bool bFromInput)
	{
		m_pUIListView->UpdateScroll_H();
	}

private:
	CUIListView * m_pUIListView;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIListView

CUIListView::CUIListView(IGwUIDummyCtrl * pDummyCtrl, GwVector2 vPos, GwVector2 vSize)
: m_pDummyCtrl(pDummyCtrl), m_vPos(vPos), m_vSize(vSize), m_eDragObjectSender(eDragObjectSender_None), 
  m_eDragObjectReceiver(eDragObjectReceiver_None), m_pScroll_V(NULL), m_pScroll_H(NULL), m_pUIFont(NULL), 
  m_dwListViewItemIndex(0), m_IsDragSendUse(false), m_IsNameChange(false), m_vMargin(GwVector2(0,0)), 
  m_nViewItemCount(0), m_nScrollPos(0), m_nMaxWidth(0)
{
	m_pBaseControl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pBaseControl->SetLocalPos(m_vPos);
	m_pDummyCtrl->AddControl(m_pBaseControl);
	m_pDummyCtrl->AddRef();

	m_pDelegate_DragReceive = new IDelegate();

	m_ListViewIndex = ms_ListViewIndex++;
}

CUIListView::~CUIListView(void)
{
	m_pDummyCtrl->RemoveControl(m_pBaseControl);
	m_pBaseControl->Release();

	vector<CUIListViewItem*>::iterator itr = m_ListViewItemList.begin();
	for( ; itr != m_ListViewItemList.end(); ++itr )
	{
		CUIListViewItem * pListViewItem = (*itr);
		if( pListViewItem != NULL ) 
		{
			pListViewItem->Release();
		}
	}
	m_ListViewItemList.clear();

	vector<CUIListViewItemInfo*>::iterator itr_Info = m_ListViewItemInfoList.begin();
	for( ; itr_Info != m_ListViewItemInfoList.end(); ++itr_Info )
	{
		CUIListViewItemInfo * pListViewItemInfo = (*itr_Info);
		if( pListViewItemInfo != NULL ) 
		{
			pListViewItemInfo->Release();
		}
	}
	m_ListViewItemInfoList.clear();

	if( m_pScroll_V != NULL ) 
	{
		m_pDummyCtrl->RemoveControl(m_pScroll_V);
		m_pScroll_V->Release();
	}

	if( m_pScroll_H != NULL ) 
	{
		m_pDummyCtrl->RemoveControl(m_pScroll_H);
		m_pScroll_H->Release();
	}

	if( m_pUIFont != NULL ) 
	{
		m_pUIFont->Release();
	}

	m_pDummyCtrl->Release();
	m_pDelegate_DragReceive->Release();
}

void				CUIListView::Update(float fDeltaTime)
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

	vector<CUIListViewItem*>::iterator itr = m_ListViewItemList.begin();
	for( ; itr != m_ListViewItemList.end(); ++itr )
	{
		CUIListViewItem * pListViewItem = (*itr);
		if( pListViewItem != NULL ) 
		{
			pListViewItem->Update(fDeltaTime);
		}
	}

	if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE && CUIDragObject::GetInstance()->IsDraging() == true )
	{
		GwVector vPos;
		vPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;

		GwVector vListViewPos = *m_pDummyCtrl->GetWorldPos() + GwVector(m_vPos.x, m_vPos.y, 0);

		if( vListViewPos.x <= vPos.x && vListViewPos.y <= vPos.y && vListViewPos.x + m_vSize.x >= vPos.x && vListViewPos.y + m_vSize.y >= vPos.y )
		{
			bool IsFind = false;

			vector<CUIListViewItem*>::iterator itr = m_ListViewItemList.begin();
			for( ; itr != m_ListViewItemList.end(); ++itr )
			{
				CUIListViewItem * pListViewItem = (*itr);
				if( pListViewItem != NULL ) 
				{
					if( pListViewItem->IsDefaultItemInfo() == false && pListViewItem->OnCheckDrag(GwVector2(vPos.x, vPos.y)) == true )
					{
						IsFind = true;
						break;
					}
				}
			}

			if( IsFind == false )
			{
				CUIDragItemBase * pSrcDragItem = CUIDragObject::GetInstance()->GetDragItemBase();
				(*m_pDelegate_DragReceive)(pSrcDragItem, NULL, pSrcDragItem->GetDragObjectSender());

				IDelegate * pDelegate_Send = pSrcDragItem->GetDelegate_Drag(eDelegate_Drag_Send);
				(*pDelegate_Send)(pSrcDragItem, NULL, m_eDragObjectReceiver);
			}
		}
	}
}

void				CUIListView::SetAttribute(eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, bool IsNameChanged, GwVector2 vMargin)
{
	m_eDragObjectSender = eDragSender;
	m_eDragObjectReceiver = eDragReceiver;
	m_IsDragSendUse = IsDragSendUse;
	m_IsNameChange = IsNameChanged;
	m_vMargin = vMargin;
}

void				CUIListView::SetAttribute_ListViewItem(int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor selColor,
														   const char * pszEmpty, const char * pszSelect, const char * pszEdit)
{
	m_nItemHeightInterval		= nItemHeightInterval;
	m_nFontSize					= nFontSize;
	m_strFontName				= pszFontName;
	m_eWeightType				= eWeightType;
	m_FontColor					= fontColor;
	m_SelFondColor				= selColor;
	m_strEmptyButtonFileName	= pszEmpty;
	m_strSelectFileName			= pszSelect;
	m_strEditboxFileName		= pszEdit;

	m_pUIFont = Gw::GetUIMgr()->CreateUIFont(nFontSize,pszFontName,eWeightType, false);
}

void				CUIListView::SetAttribute_Scroll_V(GwVector2 vPos, const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext, int nThumbSensitive)
{
	m_pScroll_V = CUIHelper::CreateUIScroll_V(pszTrack, pszThumb, pszPrev, pszNext);
	m_pScroll_V->SetLocalPos(vPos);

	CScrollCallback_V*	pCallback = new CScrollCallback_V(this);
	m_pScroll_V->SetCallback(pCallback);	pCallback->Release();

	m_pDummyCtrl->AddControl(m_pScroll_V);

	m_nThumbSensitive = nThumbSensitive;
}

void				CUIListView::SetAttribute_Scroll_H(GwVector2 vPos, const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext, int nThumbSensitive)
{
	m_pScroll_H = CUIHelper::CreateUIScroll_H(pszTrack, pszThumb, pszPrev, pszNext);
	m_pScroll_H->SetLocalPos(vPos);

	CScrollCallback_H*	pCallback = new CScrollCallback_H(this);
	m_pScroll_H->SetCallback(pCallback);	pCallback->Release();

	m_pDummyCtrl->AddControl(m_pScroll_H);
}

void				CUIListView::CreateSlot()
{
	float fListViewHeight = m_vSize.y;
	m_nViewItemCount = (int)(fListViewHeight / (m_nFontSize + m_nItemHeightInterval * 2));
	m_vSize.y = m_nViewItemCount * (m_nFontSize + m_nItemHeightInterval * 2);

	for( int i = 0; i < m_nViewItemCount; ++i )
	{
		CUIListViewItem * pListViewItem = new CUIListViewItem(this, m_dwListViewItemIndex++, m_eDragObjectSender, m_eDragObjectReceiver, m_IsDragSendUse, m_IsNameChange,
															  m_vSize.x, m_nItemHeightInterval, m_nFontSize, m_strFontName.c_str(), m_eWeightType, m_FontColor, m_SelFondColor, 
															  m_strEmptyButtonFileName.c_str(), m_strSelectFileName.c_str(), m_strEditboxFileName.c_str());

		m_ListViewItemList.push_back(pListViewItem);

		m_pBaseControl->AddControl(pListViewItem->GetBaseControl());
	}

	GwVector2 vPos = m_vMargin;
	vector<CUIListViewItem*>::iterator itr = m_ListViewItemList.begin();
	for( ; itr != m_ListViewItemList.end(); ++itr )
	{
		CUIListViewItem * pListViewItem = (*itr);
		if( pListViewItem != NULL ) 
		{
			pListViewItem->SetLocalPos(vPos);
			vPos.y += m_nFontSize + m_nItemHeightInterval * 2;
		}
	}
}

int					CUIListView::GetNumSlot()
{
	return (int)m_ListViewItemList.size();
}

CUIListViewItem	*	CUIListView::GetSlot(int nIndex)
{
	if( nIndex < 0 || nIndex >= (int)m_ListViewItemList.size() )
		return NULL;

	return m_ListViewItemList[nIndex];
}

UINT				CUIListView::GetListViewIndex()
{
	return m_ListViewIndex;
}

CUIListViewItemInfo * CUIListView::AddItem(const char * pszItemName, int & nMaxWidth, IGwRefObject * pParameta)
{
	CUIListViewItemInfo * pListViewItemInfo = new CUIListViewItemInfo(pszItemName, pParameta);
	m_ListViewItemInfoList.push_back(pListViewItemInfo);

	m_nTotalItemHeight = m_ListViewItemInfoList.size() * (m_nFontSize + m_nItemHeightInterval * 2) + 30;

	UpdateScroll_V();

	SIZE outSizeExtent;
	HDC	hDC	= CreateCompatibleDC(Gw::GetUIMgr()->GetMainDC());
	SelectObject(hDC, m_pUIFont->GetHFont());
	GetTextExtentPoint32A(hDC , pszItemName, (int)(strlen(pszItemName)), &outSizeExtent);
	DeleteDC(hDC);

	float nWidth = outSizeExtent.cx;
	if( m_nMaxWidth < nWidth )
		m_nMaxWidth = nWidth;

	nMaxWidth = m_nMaxWidth;

	if( m_pScroll_H != NULL ) 
		UpdateScroll_H();

	return pListViewItemInfo;
}

CUIListViewItemInfo * CUIListView::AddItem_Cach(const char * pszItemName, int nWidth, IGwRefObject * pParameta)
{
	CUIListViewItemInfo * pListViewItemInfo = new CUIListViewItemInfo(pszItemName, pParameta);
	m_ListViewItemInfoList.push_back(pListViewItemInfo);

	m_nTotalItemHeight = m_ListViewItemInfoList.size() * (m_nFontSize + m_nItemHeightInterval * 2) + 30;

	UpdateScroll_V();

	if( m_pScroll_H != NULL ) 
	{
		m_nMaxWidth = nWidth;

		UpdateScroll_H();
	}

	return pListViewItemInfo;
}

void				CUIListView::Remove(CUIListViewItemInfo * pListViewItemInfo)
{
}

void				CUIListView::RemoveAll()
{
	m_nMaxWidth = 0;

	vector<CUIListViewItemInfo*>::iterator itr_Info = m_ListViewItemInfoList.begin();
	for( ; itr_Info != m_ListViewItemInfoList.end(); ++itr_Info )
	{
		CUIListViewItemInfo * pListViewItemInfo = (*itr_Info);
		if( pListViewItemInfo != NULL ) 
		{
			pListViewItemInfo->Release();
		}
	}
	m_ListViewItemInfoList.clear();

	m_nTotalItemHeight = 0;

	vector<CUIListViewItem*>::iterator itr_ViewItem = m_ListViewItemList.begin();
	for( ; itr_ViewItem != m_ListViewItemList.end(); ++itr_ViewItem )
	{
		CUIListViewItem * pListViewItem = (*itr_ViewItem);
		if( pListViewItem != NULL ) 
		{
			pListViewItem->SetDefaultItemInfo();
		}
	}

	UpdateScroll_V();
}

CUIListViewItemInfo	*	CUIListView::FindListViewItemInfo_byItemName(string strItemName)
{
	StringToLower(strItemName);
	vector<CUIListViewItemInfo*>::iterator itr = m_ListViewItemInfoList.begin();
	for( ; itr != m_ListViewItemInfoList.end(); ++itr )
	{
		CUIListViewItemInfo * pListViewItemInfo = (*itr);
		if( pListViewItemInfo != NULL ) 
		{
			string strItem = pListViewItemInfo->GetName();
			StringToLower(strItem);

			if( strItemName == strItem )
				return pListViewItemInfo;
		}
	}

	return NULL;
}

CUIListViewItemInfo *	CUIListView::FindListViewItemInfo_byListViewItem(CUIListViewItem * pListViewItem)
{
	return pListViewItem->GetListViewItemInfo();
}

void				CUIListView::SetSelectedItem(CUIListViewItemInfo * pListViewItemInfo)
{
	vector<CUIListViewItemInfo*>::iterator itr = m_ListViewItemInfoList.begin();
	for( ; itr != m_ListViewItemInfoList.end(); ++itr )
	{
		CUIListViewItemInfo * pListViewItemInfo = (*itr);

		if( pListViewItemInfo != NULL )
			pListViewItemInfo->SetListViewItem_State(eListViewItem_State_Normal);
	}

	if( pListViewItemInfo != NULL )
		pListViewItemInfo->SetListViewItem_State(eListViewItem_State_Select);

	UpdateScroll_V();
}

void				CUIListView::OnSelectedItemTopMove()
{
	int nNumListViewItem = (int)m_ListViewItemList.size();
	int nNumListViewItemInfo = (int)m_ListViewItemInfoList.size();
	int nSelectItemIndex = -1;

	vector<CUIListViewItemInfo*>::iterator itr = m_ListViewItemInfoList.begin();
	for( int i = 0 ; itr != m_ListViewItemInfoList.end(); ++itr, ++i )
	{
		CUIListViewItemInfo * pListViewItemInfo = (*itr);

		if( pListViewItemInfo != NULL && pListViewItemInfo->GetListViewItem_State() == eListViewItem_State_Select )
		{
			nSelectItemIndex = i;
			break;
		}
	}

	if( nSelectItemIndex != -1 )
	{
		if( nNumListViewItemInfo - nSelectItemIndex > nNumListViewItem )
		{
			int nSelect = nSelectItemIndex;

			if(nSelect%m_nThumbSensitive != 0)
				nSelect += m_nThumbSensitive - (nNumListViewItemInfo%m_nThumbSensitive);

			nSelect /= m_nThumbSensitive;

			m_pScroll_V->SetCurrValue(nSelect);

			int nSize = (int)m_ListViewItemInfoList.size();

			for( int i = 0; i < m_nViewItemCount; ++i )
			{
				int nIndex = nSelectItemIndex + i;
				if( nIndex < nSize ) 
				{
					if( m_ListViewItemInfoList[nIndex]->GetListViewItem_State() == eListViewItem_State_None )
					{
						m_ListViewItemInfoList[nIndex]->SetListViewItem_State(eListViewItem_State_Normal);
					}
					m_ListViewItemList[i]->SetListViewItemInfo(m_ListViewItemInfoList[nIndex]);
				}
				else 
				{
					m_ListViewItemList[i]->SetDefaultItemInfo();
				}
			}
		}
	}
}

void				CUIListView::UpdateScroll_V()
{
	if( m_pScroll_V != NULL )
	{
		int nFirst = (CUIHelper::CalcFirstItemIndexScroll_V(m_pScroll_V, (int)m_ListViewItemInfoList.size(), m_nThumbSensitive, (int)m_ListViewItemList.size()));
		int nSize = (int)m_ListViewItemInfoList.size();

		for( int i = 0; i < m_nViewItemCount; ++i )
		{
			int nIndex = nFirst + i;
			if( nIndex < nSize ) 
			{
				if( m_ListViewItemInfoList[nIndex]->GetListViewItem_State() == eListViewItem_State_None )
				{
					m_ListViewItemInfoList[nIndex]->SetListViewItem_State(eListViewItem_State_Normal);
				}
				m_ListViewItemList[i]->SetListViewItemInfo(m_ListViewItemInfoList[nIndex]);
			}
			else 
			{
				m_ListViewItemList[i]->SetDefaultItemInfo();
			}
		}
	}
}

void				CUIListView::UpdateScroll_H()
{
	if( m_pScroll_H != NULL )
	{
		int nFirst = CUIHelper::CalcFirstItemIndexScroll_H(m_pScroll_H, m_nMaxWidth + 20, 3, m_vSize.x);
		
		vector<CUIListViewItem*>::iterator itr = m_ListViewItemList.begin();
		for( ; itr != m_ListViewItemList.end(); ++itr )
		{
			CUIListViewItem * pListViewItem = (*itr);
			if( pListViewItem != NULL ) 
			{
				pListViewItem->SetScroll_H_Move(nFirst);
			}
		}
	}
}

void				CUIListView::OnMouseWheelMove(int iMoveWheelCount)
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
		if( m_pScroll_V != NULL ) 
		{
			float	fScrollValue = m_pScroll_V->GetCurrValue();

			fScrollValue -= iMoveWheelCount;
			fScrollValue = Clamp(fScrollValue, 0.f, (float)m_pScroll_V->GetRange_Max());

			m_pScroll_V->SetCurrValue(fScrollValue);
		}
		else
		{
			m_nScrollPos -= iMoveWheelCount;

			if( m_nScrollPos < 0 )
			{
				m_nScrollPos = 0;
			}
			else if( m_nScrollPos > (int)(m_ListViewItemInfoList.size()) - m_nViewItemCount )
			{
				m_nScrollPos = (int)(m_ListViewItemInfoList.size()) - m_nViewItemCount;
			}

			int nSize = (int)m_ListViewItemInfoList.size();

			for( int i = 0; i < m_nViewItemCount; ++i )
			{
				int nIndex = m_nScrollPos + i;
				if( nIndex < nSize ) 
				{
					if( m_ListViewItemInfoList[nIndex]->GetListViewItem_State() == eListViewItem_State_None )
					{
						m_ListViewItemInfoList[nIndex]->SetListViewItem_State(eListViewItem_State_Normal);
					}
					m_ListViewItemList[i]->SetListViewItemInfo(m_ListViewItemInfoList[nIndex]);
				}
				else 
				{
					m_ListViewItemList[i]->SetDefaultItemInfo();
				}
			}
		}
	}
}

IDelegate		*	CUIListView::GetDelegate_DragReceive()
{
	return m_pDelegate_DragReceive;
}
