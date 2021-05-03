#pragma once

#include "UItreeViewItem.h"

class CUITreeView : public IGwRefObject
{
public:
	CUITreeView(IGwUIDummyCtrl * pDummyCtrl, GwVector2 vPos, GwVector2 vSize);
	~CUITreeView(void);

	void				Update(float fDeltaTime);

	void				SetAttribute(eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, bool IsNameChanged, GwVector2 vMargin = GwVector2(5,5));

	void				SetAttribute_TreeViewItem(int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor selColor,
												  const char * pszEmpty, const char * pszPlus, const char * pszMinus, const char * pszSelect, const char * pszEdit);

	void				SetAttribute_Scroll(GwVector2 vPos, const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext, int nThumbSensitive);

	UINT				GetTreeViewIndex();
	CUITreeViewItem	*	AddItem(CUITreeViewItem * pTreeViewItem, const char * pszItemName, IGwRefObject * pParameta = NULL);

	void				Remove(CUITreeViewItem * pTreeViewItem);
	void				RemoveAll();

	CUITreeViewItem *	FindTreeViewItem_byParameta(IGwRefObject * pParameta);
	CUITreeViewItem *	FindTreeViewItem_byItemName(CUITreeViewItem * pTreeViewItem, string strItemName);

	void				UpdateItemPos();

	CUITreeViewItem	*	GetTreeViewItem_Root();

	void				UpdateScroll();

	void				SetDoubleClickTreeOpen(bool IsOpen);
	bool				IsDoubleClickTreeOpen();

	void				SetSelectedItem(CUITreeViewItem * pTreeViewItem);
	CUITreeViewItem *	GetSelectedItem();

	void				OnSort();

	void				OnButtonClick_Empty();

	IDelegate		*	GetDelegate_TreeView();
	IDelegate		*	GetDelegate_DragReceive();

private:

	void				PostUpdate(const EventArg_ElapsedTime& Arg);

	CUITreeViewItem *	FindTreeViewItem_byParameta(CUITreeViewItem * pTreeViewItem, IGwRefObject * pParameta);
	void				OnMouseWheelMove(int iMoveWheelCount);

	IGwUIDialog		*	m_pDialog;

	IGwUIDummyCtrl	*	m_pDummyCtrl;

	IGwUIScroll_V	*	m_pScroll_V;

private:

	CUniqueName			m_strUniqueName;

	static UINT			ms_TreeViewIndex;
	UINT				m_TreeViewIndex;

	GwVector2			m_vPos;
	GwVector2			m_vSize;
	GwVector2			m_vMargin;
	int					m_nTotalItemHeight;
	int					m_nThumbSensitive;

	eDragObjectSender	m_eDragObjectSender;
	eDragObjectReceiver	m_eDragObjectReceiver;
	bool				m_IsDragSendUse;
	bool				m_IsNameChange;
	bool				m_IsDoubleClickTreeOpen;

	CUITreeViewItem	*	m_pTreeViewItem_Root;
	CUITreeViewItem *	m_pTreeViewItem_Selected;

	int					m_nItemHeightInterval;
	int					m_nFontSize;
	string				m_strFontName;
	GwFontWeightType	m_eWeightType;
	GwColor				m_FontColor;
	GwColor				m_SelFondColor;
	string				m_strEmptyButtonFileName;
	string				m_strPlusFileName;
	string				m_strMinusFileName;
	string				m_strSelectFileName;
	string				m_strEditboxFileName;

	DWORD				m_dwTreeViewItemIndex;

	IDelegate		*	m_pDelegate_TreeView;
	IDelegate		*	m_pDelegate_DragReceive;

	class CButtonCallback_Empty;
	class CScrollCallback;
};