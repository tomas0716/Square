#include "StdAfx.h"
#include "GameStage_Project.h"

I_IMPLEMENT_DYNCREATE(CGameStage_Project, ISingletonObject::GetTypeModule())

CGameStage_Project::CGameStage_Project(void)
{
}

CGameStage_Project::~CGameStage_Project(void)
{
}

void		CGameStage_Project::OnCreateDevice()
{
	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();
	Gw::GetUIControlMgr()->AddFixedDialog(m_pDialog);
	m_pDialog->SetWorldPos(GwVector(1060,22,0));

	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Project\\");

	IGwUISprite * pSprite_Back_Middle_Right = CUIHelper::CreateUISprite("back_middle_right.png");
	pSprite_Back_Middle_Right->SetLocalPos(GwVector2(302,33));
	m_pDialog->AddControl(pSprite_Back_Middle_Right); pSprite_Back_Middle_Right->Release();

	IGwUIDummyCtrl * pDummyCtrl_ListView = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pDialog->AddControl(pDummyCtrl_ListView); pDummyCtrl_ListView->Release();
	m_pListView_File = new CUIListView(pDummyCtrl_ListView, GwVector2(302,33), GwVector2(219,964));
	m_pListView_File->SetAttribute(eDragObjectSender_Project_File, eDragObjectReceiver_Project_File, true, false, GwVector2(5,5));
	m_pListView_File->SetAttribute_ListViewItem(4, 12, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(0,0,0,1), "empty.png", "sel.png", "edit.png");
	m_pListView_File->CreateSlot();
	m_pListView_File->SetAttribute_Scroll_V(GwVector2(521,33), "track_file.png", "scroll_bar.png", "scroll.png", "scroll.png", 2);
	m_pListView_File->SetAttribute_Scroll_H(GwVector2(302,992), "track_h.png", "scroll_bar_h.png", "scroll.png", "scroll.png", 2);
	m_pListView_File->GetDelegate_DragReceive()->Bind<CGameStage_Project, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CGameStage_Project::OnDragReceive_File>(this);

	IGwUISprite * pSprite_Back_Middle_Left = CUIHelper::CreateUISprite("back_middle_left.png");
	pSprite_Back_Middle_Left->SetLocalPos(GwVector2(6,33));
	m_pDialog->AddControl(pSprite_Back_Middle_Left); pSprite_Back_Middle_Left->Release();

	IGwUIDummyCtrl * pDummyCtrl_TreeView = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pDialog->AddControl(pDummyCtrl_TreeView); pDummyCtrl_TreeView->Release();
	m_pTreeView_Folder = new CUITreeView(pDummyCtrl_TreeView, GwVector2(6,33), GwVector2(281,978));
	m_pTreeView_Folder->SetAttribute(eDragObjectSender_Project_File, eDragObjectReceiver_Project_Folder, false, false, GwVector2(6,5));
	m_pTreeView_Folder->SetAttribute_TreeViewItem(2, 14, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(0,0,0,1), "empty.png", "plus.png", "minus.png", "sel.png", "edit.png");
	m_pTreeView_Folder->SetAttribute_Scroll(GwVector2(286,33), "track_folder.png", "scroll_bar.png", "scroll.png", "scroll.png", 2);

	IGwUISprite * pSprite_Back = CUIHelper::CreateUISprite("back.png");
	pSprite_Back->SetLocalPos(GwVector2(0,0));
	m_pDialog->AddControl(pSprite_Back); pSprite_Back->Release();
	pSprite_Back->SetDependencyType(GWUISTATEDEPENDENCY_NOACTION);

	CUITreeViewItem	* pTreeViewItem = m_pTreeView_Folder->AddItem(NULL, "MediaPath");
	SetDelegate_Folder(pTreeViewItem);
	RecrsiveAddTreeViewItem_Folder("MediaPath", pTreeViewItem);
	m_pTreeView_Folder->UpdateItemPos();

	Gw::GetBillboardMgr()->EndTextureMerge();

	CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_TextureInfo_Find, "CGameStage_Project", this, &CGameStage_Project::OnTextureInfo_Find);
	CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_AnimInfo_Find, "CGameStage_Project", this, &CGameStage_Project::OnAnimInfo_Find);
	CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_MeshInfo_Find, "CGameStage_Project", this, &CGameStage_Project::OnMeshInfo_Find);
}

void		CGameStage_Project::OnDestroyDevice()
{
	m_pTreeView_Folder->Release();
	m_pListView_File->Release();

	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();

	if( CNotificationCenter::GetInstance() != NULL )
	{
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_TextureInfo_Find, "CGameStage_Project");
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_AnimInfo_Find, "CGameStage_Project");
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_MeshInfo_Find, "CGameStage_Project");
	}
}

void		CGameStage_Project::Update(float fDeltaTime)
{
	m_pTreeView_Folder->Update(fDeltaTime);
	m_pListView_File->Update(fDeltaTime);
}

void		CGameStage_Project::OnVisibleChanged()
{
	if( m_IsVisible == true )
	{
		Gw::GetUIControlMgr()->AddDialog(m_pDialog);
	}
	else
	{
		Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	}
}

void		CGameStage_Project::RecrsiveAddTreeViewItem_Folder(string strFolder, CUITreeViewItem * pTreeViewItem)
{
	HANDLE				search_handle;
	WIN32_FIND_DATAA	file_data;

	TVINSERTSTRUCTA		TI;

	string strTargetDir = strFolder + "\\*.*";

	search_handle = FindFirstFileA( strTargetDir.c_str() , &file_data );

	if( INVALID_HANDLE_VALUE != search_handle )
	{
		do{
			if(  (FILE_ATTRIBUTE_DIRECTORY & file_data.dwFileAttributes ) )			// Æú´õÀÏ °æ¿ì
			{				
				if( strcmp ( file_data.cFileName, "." ) == 0 || 
					strcmp ( file_data.cFileName, ".." ) == 0 || 
					strcmp ( file_data.cFileName, ".svn" ) == 0 )
				{
					continue;
				}
				else if ( file_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
				{
					continue;
				}
				else
				{
					string strNextFolder = strFolder;
					strNextFolder += "\\";
					strNextFolder += file_data.cFileName;

					CParameta_String * pParameta = new CParameta_String(strNextFolder);
					CUITreeViewItem	* pChildTreeViewItem = m_pTreeView_Folder->AddItem(pTreeViewItem, file_data.cFileName, pParameta);
					pParameta->Release();
					SetDelegate_Folder(pChildTreeViewItem);

					RecrsiveAddTreeViewItem_Folder (strNextFolder, pChildTreeViewItem);
				}
			}

		}while( FindNextFileA ( search_handle, &file_data ) );

		FindClose(search_handle);
	}
}

void		CGameStage_Project::RecrsiveAddTreeViewItem_File(string strFolder)
{
	vector<string> * pFileList = FindFileList(strFolder.c_str());

	if( pFileList == NULL ) 
	{
		vector<string> FileList;
		int nMaxWidth = 0;

		HANDLE				search_handle;
		WIN32_FIND_DATAA	file_data;

		TVINSERTSTRUCTA		TI;

		string strTargetDir = strFolder + "\\*.*";

		search_handle = FindFirstFileA( strTargetDir.c_str() , &file_data );

		if( INVALID_HANDLE_VALUE != search_handle )
		{
			do{
				if( !(FILE_ATTRIBUTE_DIRECTORY & file_data.dwFileAttributes ) )
				{
					CParameta_String * pParameta = new CParameta_String(strFolder);
					m_pListView_File->AddItem(file_data.cFileName, nMaxWidth, pParameta);	
					pParameta->Release();
					FileList.push_back(file_data.cFileName);
				}

			}while( FindNextFileA ( search_handle, &file_data ) );

			FindClose(search_handle);
		}
	
		m_FileTable.insert(map<string,vector<string>>::value_type(strFolder, FileList));
		m_FolderFileWidthTable.insert(map<string,int>::value_type(strFolder, nMaxWidth));
	}
	else
	{
		vector<string>::iterator itr = pFileList->begin();

		for( ; itr != pFileList->end(); ++itr )
		{
			CParameta_String * pParameta = new CParameta_String(strFolder);
			m_pListView_File->AddItem_Cach((*itr).c_str(), m_FolderFileWidthTable[strFolder], pParameta);
			pParameta->Release();
		}
	}
}

vector<string>	*	CGameStage_Project::FindFileList(const char * pszFolderName)
{
	map<string,vector<string>>::iterator itr = m_FileTable.find(pszFolderName);

	if( itr != m_FileTable.end() )
	{
		return &itr->second;
	}

	return NULL;
}

void		CGameStage_Project::SetDelegate_Folder(CUITreeViewItem * pTreeViewItem)
{
	IDelegate * pDelegate_TreeView[eDelegate_TreeView_Max] = { NULL, };
	IDelegate * pDelegate_Drag[eDelegate_Drag_Max] = { NULL, };

	for( int i = 0; i < eDelegate_TreeView_Max; ++i )
	{
		pDelegate_TreeView[i] = pTreeViewItem->GetDelegate_TreeView((eDelegate_TreeView)i);
	}

	for( int i = 0; i < eDelegate_Drag_Max; ++i )
	{
		pDelegate_Drag[i] = pTreeViewItem->GetDelegate_Drag((eDelegate_Drag)i);
	}

	pDelegate_TreeView[eDelegate_TreeView_Click]->Bind<CGameStage_Project, CUITreeViewItem*, &CGameStage_Project::OnTreeViewItemClick_Folder>(this);
	pDelegate_Drag[eDelegate_Drag_Receive]->Bind<CGameStage_Project, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CGameStage_Project::OnDragReceive_Folder>(this);
}

void		CGameStage_Project::SetDelegate_File(CUITreeViewItem * pTreeViewItem)
{
	IDelegate * pDelegate_TreeView[eDelegate_TreeView_Max] = { NULL, };
	IDelegate * pDelegate_Drag[eDelegate_Drag_Max] = { NULL, };

	for( int i = 0; i < eDelegate_TreeView_Max; ++i )
	{
		pDelegate_TreeView[i] = pTreeViewItem->GetDelegate_TreeView((eDelegate_TreeView)i);
	}

	for( int i = 0; i < eDelegate_Drag_Max; ++i )
	{
		pDelegate_Drag[i] = pTreeViewItem->GetDelegate_Drag((eDelegate_Drag)i);
	}

	pDelegate_TreeView[eDelegate_TreeView_Click]->Bind<CGameStage_Project, CUITreeViewItem*, &CGameStage_Project::OnTreeViewItemClick_File>(this);
	pDelegate_Drag[eDelegate_Drag_Send]->Bind<CGameStage_Project, CUIDragItemBase*, CUIDragItemBase*, eDragObjectReceiver, &CGameStage_Project::OnDragSend_File>(this);
	pDelegate_Drag[eDelegate_Drag_Receive]->Bind<CGameStage_Project, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CGameStage_Project::OnDragReceive_File>(this);
}

void		CGameStage_Project::OnTreeViewItemClick_Folder(CUITreeViewItem * pTreeViewItem)
{
	m_pListView_File->RemoveAll();

	string strFolder = pTreeViewItem->GetName();
	INode * pParentTreeViewItem = pTreeViewItem;

	while(pParentTreeViewItem)
	{
		pParentTreeViewItem = pParentTreeViewItem->GetParent();

		if( pParentTreeViewItem != NULL )
		{
			string strParentFolder = pParentTreeViewItem->GetName();

			if( strParentFolder != "" )
			{
				strParentFolder += "\\";
				strFolder = strParentFolder + strFolder;
			}
			else 
			{
				break;
			}
		}
	}

	RecrsiveAddTreeViewItem_File(strFolder);
}

void		CGameStage_Project::OnDragReceive_Folder(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender)
{
	switch(eSender)
	{
	case eDragObjectSender_Hierarchy:
		{
			CUITreeViewItem * pTreeViewItem_Folder = static_cast<CUITreeViewItem*>(pDestDragItem);

			if( pTreeViewItem_Folder != NULL )
			{
				CParameta_String * pParameta_String = static_cast<CParameta_String*>(pTreeViewItem_Folder->GetParameta());
				CUITreeViewItem * pTreeViewItem = static_cast<CUITreeViewItem*>(pSrcDragItem);

				if( pParameta_String != NULL && pTreeViewItem != NULL )
				{
					IGameObject * pGameObject = dynamic_cast<IGameObject*>(pTreeViewItem->GetParameta());

					if( pGameObject != NULL )
					{
						string strFileName = pTreeViewItem->GetName();
						strFileName += ".es";

						IGameObjectMgr::GetInstance()->SaveFile(pParameta_String->GetParameta_String().c_str(), strFileName.c_str(), pGameObject);
					}
				}
			}
		}
		break;

	default:
		{
		}
	}
}

void		CGameStage_Project::OnTreeViewItemClick_File(CUITreeViewItem * pTreeViewItem)
{
}

void		CGameStage_Project::OnDragSend_File(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectReceiver eReceiver)
{
}

void		CGameStage_Project::OnDragReceive_File(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender)
{
	switch(eSender)
	{
	case eDragObjectSender_Hierarchy:
		{
			CUITreeViewItem * pTreeViewItem_Folder = m_pTreeView_Folder->GetSelectedItem();

			if( pTreeViewItem_Folder != NULL )
			{
				CParameta_String * pParameta_String = static_cast<CParameta_String*>(pTreeViewItem_Folder->GetParameta());
				CUITreeViewItem * pTreeViewItem = static_cast<CUITreeViewItem*>(pSrcDragItem);

				if( pParameta_String != NULL && pTreeViewItem != NULL )
				{
					IGameObject * pGameObject = dynamic_cast<IGameObject*>(pTreeViewItem->GetParameta());

					if( pGameObject != NULL )
					{
						string strFileName = pTreeViewItem->GetName();
						strFileName += ".es";

						IGameObjectMgr::GetInstance()->SaveFile(pParameta_String->GetParameta_String().c_str(), strFileName.c_str(), pGameObject);
					}
				}
			}
		}
		break;

	default:
		{
		}
	}
}

void		CGameStage_Project::OnTextureInfo_Find(NotificationArg * p)
{
	NotificationArg_InspectorComponent_TextureInfo_Find * pArg = dynamic_cast<NotificationArg_InspectorComponent_TextureInfo_Find*>(p);

	if( pArg != NULL && pArg->m_pTextureFileInfo != NULL && pArg->m_pTextureFileInfo->m_strPath != "" && pArg->m_pTextureFileInfo->m_strFileName != "" )
	{
		CUITreeViewItem * pTreeViewItem = NULL;

		string strFilePath = pArg->m_pTextureFileInfo->m_strPath;
		string strFileName = pArg->m_pTextureFileInfo->m_strFileName;

		string strOrigToken = CGwStrTok::GetTokenFormat();
		CGwStrTok::SetTokenFormat("\\");
		CGwStrTok::SetString(strFilePath.c_str());
		CGwStrTok::SetTokenFormat(strOrigToken.c_str());

		int nTokenNum = CGwStrTok::GetNumToken();
		
		for( int i = 0; i < nTokenNum; ++i )
		{
			string strText = CGwStrTok::GetToken(i);

			if( strText != "" )
			{
				pTreeViewItem = m_pTreeView_Folder->FindTreeViewItem_byItemName(pTreeViewItem, strText);
				pTreeViewItem->SetChildOpenFlag(true);
				m_pTreeView_Folder->SetSelectedItem(pTreeViewItem);
			}
		}

		m_pTreeView_Folder->UpdateItemPos();

		OnTreeViewItemClick_Folder(pTreeViewItem);

		CUIListViewItemInfo * pListViewItemInfo = m_pListView_File->FindListViewItemInfo_byItemName(strFileName);
		m_pListView_File->SetSelectedItem(pListViewItemInfo);
		m_pListView_File->OnSelectedItemTopMove();
	}
}

void		CGameStage_Project::OnAnimInfo_Find(NotificationArg * p)
{
	NotificationArg_InspectorComponent_AnimInfo_Find * pArg = dynamic_cast<NotificationArg_InspectorComponent_AnimInfo_Find*>(p);

	if( pArg != NULL && pArg->m_pFileInfo != NULL && pArg->m_pFileInfo->m_strPath != "" && pArg->m_pFileInfo->m_strFileName != "" )
	{
		CUITreeViewItem * pTreeViewItem = NULL;

		string strFilePath = pArg->m_pFileInfo->m_strPath;
		string strFileName = pArg->m_pFileInfo->m_strFileName;

		string strOrigToken = CGwStrTok::GetTokenFormat();
		CGwStrTok::SetTokenFormat("\\");
		CGwStrTok::SetString(strFilePath.c_str());
		CGwStrTok::SetTokenFormat(strOrigToken.c_str());

		int nTokenNum = CGwStrTok::GetNumToken();
		
		for( int i = 0; i < nTokenNum; ++i )
		{
			string strText = CGwStrTok::GetToken(i);

			if( strText != "" )
			{
				pTreeViewItem = m_pTreeView_Folder->FindTreeViewItem_byItemName(pTreeViewItem, strText);
				pTreeViewItem->SetChildOpenFlag(true);
				m_pTreeView_Folder->SetSelectedItem(pTreeViewItem);
			}
		}

		m_pTreeView_Folder->UpdateItemPos();

		OnTreeViewItemClick_Folder(pTreeViewItem);

		CUIListViewItemInfo * pListViewItemInfo = m_pListView_File->FindListViewItemInfo_byItemName(strFileName);
		m_pListView_File->SetSelectedItem(pListViewItemInfo);
		m_pListView_File->OnSelectedItemTopMove();
	}
}

void		CGameStage_Project::OnMeshInfo_Find(NotificationArg * p)
{
	NotificationArg_InspectorComponent_MeshInfo_Find * pArg = dynamic_cast<NotificationArg_InspectorComponent_MeshInfo_Find*>(p);

	if( pArg != NULL && pArg->m_pFileInfo != NULL && pArg->m_pFileInfo->m_strPath != "" && pArg->m_pFileInfo->m_strFileName != "" )
	{
		CUITreeViewItem * pTreeViewItem = NULL;

		string strFilePath = pArg->m_pFileInfo->m_strPath;
		string strFileName = pArg->m_pFileInfo->m_strFileName;

		string strOrigToken = CGwStrTok::GetTokenFormat();
		CGwStrTok::SetTokenFormat("\\");
		CGwStrTok::SetString(strFilePath.c_str());
		CGwStrTok::SetTokenFormat(strOrigToken.c_str());

		int nTokenNum = CGwStrTok::GetNumToken();
		
		for( int i = 0; i < nTokenNum; ++i )
		{
			string strText = CGwStrTok::GetToken(i);

			if( strText != "" )
			{
				pTreeViewItem = m_pTreeView_Folder->FindTreeViewItem_byItemName(pTreeViewItem, strText);
				pTreeViewItem->SetChildOpenFlag(true);
				m_pTreeView_Folder->SetSelectedItem(pTreeViewItem);
			}
		}

		m_pTreeView_Folder->UpdateItemPos();

		OnTreeViewItemClick_Folder(pTreeViewItem);

		CUIListViewItemInfo * pListViewItemInfo = m_pListView_File->FindListViewItemInfo_byItemName(strFileName);
		m_pListView_File->SetSelectedItem(pListViewItemInfo);
		m_pListView_File->OnSelectedItemTopMove();
	}
}