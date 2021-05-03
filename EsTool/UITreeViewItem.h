#pragma once

enum eDelegate_TreeView
{
	eDelegate_TreeView_Click,				// this
	eDelegate_TreeView_DBClick,				// this
	eDelegate_TreeView_ChangeItemName,		// this, itemname

	eDelegate_TreeView_Max,
};

class CUITreeView;

class CUITreeViewItem : public INode, public CUIDragItemBase
{
private:
	CUITreeViewItem(CUITreeView * pTreeView);
	CUITreeViewItem(CUITreeView * pTreeView, DWORD dwItemIndex, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse, bool IsNameChange, bool IsDoubleClickTreeOpen, int nTreeViewWidth, 
					int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor selColor, const char * pszItemName,
					const char * pszEmpty, const char * pszPlus, const char * pszMinus, const char * pszSelect, const char * pszEdit, IGwRefObject * pParameta = NULL);
	~CUITreeViewItem(void);

public:

	virtual void		AddChild(INode * pNode);
	virtual void		RemoveChild(INode * pNode);

	void				SetParameta(IGwRefObject * pParameta);
	IGwRefObject	*	GetParameta();

	void				SetChildOpenFlag(bool IsChildOpen);

	IDelegate		*	GetDelegate_TreeView(eDelegate_TreeView eDelegate);

private:
	void				Update(float fDeltaTime);
	bool				OnCheckDrag(GwVector2 vCursorPos);
	void				UpdatePlusMinus();

	void				SetSelected();

	void				SetRoot();

	IGwUIDummyCtrl	*	GetBaseControl();

	void				SetVisible(bool IsVisible);
	void				UpdateVisible(GwVector2 vViewPos, GwVector2 vViewSize);
	void				SetVisible_ViewOut(bool IsVisible);
	void				SetDoubleClickTreeOpen(bool IsOpen);

	CUITreeViewItem *	FindTreeViewItem_byItemName(string strItemName);

	void				SetLocalPos(GwVector2 vPos);

	void				SetItemName(const char * pszItemName);
	const char		*	GetItemName();

	float				GetHeight();
	float				GetHeight_IncludeChild();

	void				OnSort();

	void				UpdateControlState();

	void				OnNotification_HideSelImage(NotificationArg* p);

	void				OnButtonClick_Plus();
	void				OnButtonClick_Minus();
	void				OnButtonClick_Item();

	void				OnButtonDown_Item();
	void				OnMouseEnter_Item();
	void				OnMouseLeave_Item();

private:

	CUITreeView		*	m_pTreeView;
	IGwUIDummyCtrl	*	m_pBaseControl;
	IGwUIStatic		*	m_pStatic_Name;
	IGwUIButton		*	m_pButton_Plus;
	IGwUIButton		*	m_pButton_Minus;
	IGwUISprite		*	m_pSprite_EditboxBack;
	IGwUIEditbox	*	m_pEditbox_ReName;
	IGwUISprite		*	m_pSprite_Sel;
	IGwUIButton		*	m_pButton_Sel;

private:

	bool				m_IsRoot;

	string				m_strObserverName;

	int					m_nTreeViewWidth;
	bool				m_IsDragSendUse;
	bool				m_IsNameChange;
	int					m_nItemHeightInterval;
	float				m_fFontSize;
	bool				m_IsChildOpen;
	bool				m_IsDoubleClickTreeOpen;

	IGwRefObject	*	m_pParameta;

	bool				m_IsVisible;

	float				m_fTime;
	float				m_fClickTime;
	bool				m_IsDBClickReady;
	bool				m_IsSelected;

	enum eState
	{
		eState_Hide,
		eState_Normal,
		eState_Select,
		eState_Edit,
	};

	eState				m_eState;
	bool				m_IsEditFirstFrameBlock;

	bool				m_IsDowning;
	GwVector2			m_vClickPos;

	IDelegate		*	m_pDelegate_TreeView[eDelegate_TreeView_Max];

	class CButtonCallback_Sel;

	friend class CUITreeView;
};

