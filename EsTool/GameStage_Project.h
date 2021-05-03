#pragma once

class CGameStage_Project : public CGameStage_DialogBase
{
	I_DECLARE_DYNCREATE(CGameStage_Project);
protected:
	CGameStage_Project(void);
	~CGameStage_Project(void);

public:
	virtual void		OnCreateDevice();
	virtual void		OnDestroyDevice();

	virtual void		Update(float fDeltaTime);

protected:
	virtual void		OnVisibleChanged();

private:

	void				RecrsiveAddTreeViewItem_Folder(string strFolder, CUITreeViewItem * pTreeViewItem);
	void				RecrsiveAddTreeViewItem_File(string strFolder);
	vector<string>	*	FindFileList(const char * pszFolderName);

	void				SetDelegate_Folder(CUITreeViewItem * pTreeViewItem);
	void				SetDelegate_File(CUITreeViewItem * pTreeViewItem);

	void				OnTreeViewItemClick_Folder(CUITreeViewItem * pTreeViewItem);
	void				OnDragReceive_Folder(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender);

	void				OnTreeViewItemClick_File(CUITreeViewItem * pTreeViewItem);
	void				OnDragSend_File(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectReceiver eReceiver);
	void				OnDragReceive_File(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender);

	void				OnTextureInfo_Find(NotificationArg * p);
	void				OnAnimInfo_Find(NotificationArg * p);
	void				OnMeshInfo_Find(NotificationArg * p);
private:

	IGwUIDialog		*	m_pDialog;

	CUITreeView		*	m_pTreeView_Folder;
	CUIListView		*	m_pListView_File;

	map<string,vector<string>>	m_FileTable;
	map<string,int>				m_FolderFileWidthTable;
};