#pragma once

enum eDelegate_ListView
{
	eDelegate_ListView_Click,				// this
	eDelegate_ListView_DBClick,				// this
	eDelegate_ListView_ChangeItemName,		// this, itemname

	eDelegate_ListView_Max,
};

class CUIListView;

class CUIListViewItem : public INode, public CUIDragItemBase
{
private:
	CUIListViewItem(CUIListView * pListView, DWORD dwItemIndex, eDragObjectSender eDragSender, eDragObjectReceiver eDragReceiver, bool IsDragSendUse,
					bool IsNameChange, int nListViewWidth, int nItemHeightInterval, int nFontSize, const char * pszFontName, GwFontWeightType eWeightType,
					GwColor fontColor, GwColor selColor, const char * pszEmpty, const char * pszSelect, const char * pszEdit);
	~CUIListViewItem(void);

public:
	IGwRefObject	*	GetParameta();
	IDelegate		*	GetDelegate_ListView(eDelegate_ListView eDelegate);

private:
	void				Update(float fDeltaTime);

	void				SetListViewItemInfo(CUIListViewItemInfo * pListViewItemInfo);
	CUIListViewItemInfo	*	GetListViewItemInfo();

	void				SetDefaultItemInfo();
	bool				IsDefaultItemInfo();

	bool				OnCheckDrag(GwVector2 vCursorPos);

	IGwUIDummyCtrl	*	GetBaseControl();

	void				SetLocalPos(GwVector2 vPos);
	void				SetScroll_H_Move(float fAdd);

	void				UpdateControlState();

	void				OnNotification_HideSelImage(NotificationArg* p);

	void				OnButtonClick_Item();
	void				OnButtonDown_Item();
	void				OnMouseEnter_Item();
	void				OnMouseLeave_Item();

private:
	CUIListViewItemInfo	*	m_pListViewItemInfo;
private:

	CUIListView			*	m_pListView;
	IGwUIDummyCtrl		*	m_pBaseControl;
	IGwUIStatic			*	m_pStatic_Name;
	IGwUISprite			*	m_pSprite_EditboxBack;
	IGwUIEditbox		*	m_pEditbox_ReName;
	IGwUISprite			*	m_pSprite_Sel;
	IGwUIButton			*	m_pButton_Sel;

private:

	string					m_strObserverName;

	bool					m_IsDragSendUse;
	bool					m_IsNameChange;
	int						m_nItemHeightInterval;

	float					m_fMargin_X;
	float					m_fScroll_H_MoveValue;

	float					m_fTime;
	float					m_fClickTime;
	bool					m_IsDBClickReady;

	bool					m_IsDowning;
	GwVector2				m_vClickPos;

	bool					m_IsDefaultItemInfo;

	IDelegate			*	m_pDelegate_ListView[eDelegate_ListView_Max];

	class CButtonCallback_Sel;

	friend class CUIListView;
};

