#pragma once

class CGameStage_Hierarchy : public CGameStage_DialogBase
{
	I_DECLARE_DYNCREATE(CGameStage_Hierarchy);
protected:
	CGameStage_Hierarchy(void);
	~CGameStage_Hierarchy(void);

public:
	virtual void		OnCreateDevice();
	virtual void		OnDestroyDevice();

	virtual void		Update(float fDeltaTime);

	void				AddGameObject(IGameObject * pParent, IGameObject * pObject);
	void				AddGameObject_ForEsFile(IGameObject * pParent, IGameObject * pObject);
	void				RemoveGameObject(IGameObject * pObject);
	void				MoveGameObject(IGameObject * pParent, IGameObject * pObject);

	void				SetSelectGameObject (IGameObject * pGameObject);
	void				OnObjectEditState(const EventArg_PopupMenu_Click& Arg);

protected:
	virtual void		OnVisibleChanged();

private:

	void				Recursive_RemoveGameObjectPickingEntity(IGameObject * pObject);
	GwVector			GetCurrPickingPos();

public:

	void				OnLButtonDown_Picking(const EventArg_PickingEntity& Arg);
	void				OnLButtonUp_Picking(const EventArg_PickingEntity& Arg);

	void				OnRButtonDown_Picking(const EventArg_PickingEntity& Arg);
	void				OnRButtonUp_Picking(const EventArg_PickingEntity& Arg);

	void				OnLButtonDown_NonePicking(const EventArg_PickingEntity& Arg);

private:

	void				SetDelegate(CUITreeViewItem * pTreeViewItem);

	void				OnTreeViewItemEmptyClick(CUITreeView * pTreeView);
	void				OnTreeViewItemClick(CUITreeViewItem * pTreeViewItem);
	void				OnTreeViewItemDBClick(CUITreeViewItem * pTreeViewItem);
	void				OnTreeViewItemChangeItemName(CUITreeViewItem * pTreeViewItem, const char * pszItemName);
	void				OnDragSend(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectReceiver eReceiver);
	void				OnDragReceive(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender);

private:

	IGwUIDialog		*	m_pDialog;

	CUITreeView		*	m_pTreeView;

	IGameObject		*	m_pLButtonDownGameObject;
	IGameObject		*	m_pRButtonDownGameObject;
	IGameObject		*	m_pSelGameObject;

	float				m_fLButtonDownProcessTime;
	float				m_fRButtonDownProcessTime;

	CPopupMenu		*	m_pPopupMenu;
	eObjectEditState	m_eObjectEditState;

	IGwPick			*	m_pPick;
	POINT				m_ptPrevMousePos;
	GwVector			m_vObjectEditPickingPos;
};