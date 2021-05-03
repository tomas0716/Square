#pragma once

#include "UItreeViewItem.h"

class CUIListView : public IGwRefObject
{
public:
	CUIListView(IGwUIDummyCtrl * pDummyCtrl, GwVector2 vPos, GwVector2 vSize);
	~CUIListView(void);

	void				Update(float fDeltaTime);

	void				SetAttribute(eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, bool IsNameChanged, GwVector2 vMargin = GwVector2(5,5));

	void				SetAttribute_ListViewItem(int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor selColor,
												  const char * pszEmpty, const char * pszSelect, const char * pszEdit);

	void				SetAttribute_Scroll_V(GwVector2 vPos, const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext, int nThumbSensitive);
	void				SetAttribute_Scroll_H(GwVector2 vPos, const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext, int nThumbSensitive);
	
	void				CreateSlot();
	int					GetNumSlot();
	CUIListViewItem	*	GetSlot(int nIndex);

	UINT				GetListViewIndex();
	CUIListViewItemInfo * AddItem(const char * pszItemName, int & nWidth, IGwRefObject * pParameta = NULL);
	CUIListViewItemInfo * AddItem_Cach(const char * pszItemName, int nWidth, IGwRefObject * pParameta = NULL);

	void				Remove(CUIListViewItemInfo * pListViewItemInfo);
	void				RemoveAll();

	CUIListViewItemInfo	*	FindListViewItemInfo_byItemName(string strItemName);
	CUIListViewItemInfo *	FindListViewItemInfo_byListViewItem(CUIListViewItem * pListViewItem);

	void				SetSelectedItem(CUIListViewItemInfo * pListViewItemInfo);
	void				OnSelectedItemTopMove();

	void				UpdateScroll_V();
	void				UpdateScroll_H();

	IDelegate		*	GetDelegate_DragReceive();

private:

	void				OnMouseWheelMove(int iMoveWheelCount);

	IGwUIDummyCtrl	*	m_pDummyCtrl;

	IGwUIDummyCtrl	*	m_pBaseControl;
	IGwUIScroll_V	*	m_pScroll_V;
	IGwUIScroll_H	*	m_pScroll_H;
	int					m_nScrollPos;

	IGwUIFont		*	m_pUIFont;
	float				m_nMaxWidth;

private:
	vector<CUIListViewItem*>		m_ListViewItemList;
	vector<CUIListViewItemInfo*>	m_ListViewItemInfoList;
	int								m_nViewItemCount;
private:
	static UINT			ms_ListViewIndex;
	UINT				m_ListViewIndex;

	GwVector2			m_vPos;
	GwVector2			m_vSize;
	GwVector2			m_vMargin;
	int					m_nTotalItemHeight;
	int					m_nThumbSensitive;

	eDragObjectSender	m_eDragObjectSender;
	eDragObjectReceiver	m_eDragObjectReceiver;
	bool				m_IsDragSendUse;
	bool				m_IsNameChange;

	int					m_nItemHeightInterval;
	int					m_nFontSize;
	string				m_strFontName;
	GwFontWeightType	m_eWeightType;
	GwColor				m_FontColor;
	GwColor				m_SelFondColor;
	string				m_strEmptyButtonFileName;
	string				m_strSelectFileName;
	string				m_strEditboxFileName;

	DWORD				m_dwListViewItemIndex;

	IDelegate		*	m_pDelegate_DragReceive;

	class CScrollCallback_V;
	class CScrollCallback_H;
};