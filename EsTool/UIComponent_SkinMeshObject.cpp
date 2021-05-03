#include "StdAfx.h"
#include "UIComponent_SkinMeshObject.h"

I_IMPLEMENT_DYNCREATE(CUIComponent_SkinMeshObject, ISingletonObject::GetTypeModule())

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBtnCallback_Texture_Find

class	CUIComponent_SkinMeshObject::CBtnCallback_Texture_Find : public IGwUIButton_Callback
{
public:
	CBtnCallback_Texture_Find(CUIComponent_SkinMeshObject * pThis, ITextureFileInfo * pTextureFileInfo)
		: m_pThis(pThis), m_pTextureFileInfo(pTextureFileInfo)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Texture_Find(m_pTextureFileInfo); 
	}
private:
	CUIComponent_SkinMeshObject		*	m_pThis;
	ITextureFileInfo				*	m_pTextureFileInfo;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBtnCallback_Anim_Find

class	CUIComponent_SkinMeshObject::CBtnCallback_Anim_Find : public IGwUIButton_Callback
{
public:
	CBtnCallback_Anim_Find(CUIComponent_SkinMeshObject * pThis, IFileInfo * pFileInfo)
		: m_pThis(pThis), m_pFileInfo(pFileInfo)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Anim_Find(m_pFileInfo); 
	}
private:
	CUIComponent_SkinMeshObject		*	m_pThis;
	IFileInfo						*	m_pFileInfo;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBtnCallback_Anim_Remove

class	CUIComponent_SkinMeshObject::CBtnCallback_Anim_Remove : public IGwUIButton_Callback
{
public:
	CBtnCallback_Anim_Remove(CUIComponent_SkinMeshObject * pThis, int nIndex)
		: m_pThis(pThis), m_nIndex(nIndex)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Anim_Remove(m_nIndex); 
	}
private:
	CUIComponent_SkinMeshObject		*	m_pThis;
	int									m_nIndex;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIComponent_SkinMeshObject

CUIComponent_SkinMeshObject::CUIComponent_SkinMeshObject(void)
{	
	m_pEmptyFileInfo = new IFileInfo();
}

CUIComponent_SkinMeshObject::~CUIComponent_SkinMeshObject(void)
{
	m_pEmptyFileInfo->Release();

	map<CUIDragItemSprite*,IGwUIButton*>::iterator itr = m_DragItemSpriteTable_TextureInfo.begin();

	for( ; itr != m_DragItemSpriteTable_TextureInfo.end(); ++itr )
	{
		delete itr->first;
		itr->second->Release();
	}
	m_DragItemSpriteTable_TextureInfo.clear();

	itr = m_DragItemSpriteTable_AnimInfo.begin();

	for( ; itr != m_DragItemSpriteTable_AnimInfo.end(); ++itr )
	{
		delete itr->first;
		itr->second->Release();
	}
	m_DragItemSpriteTable_AnimInfo.clear();

	map<ITextureFileInfo*,IGwUIStatic*>::iterator itr_TextureInfo = m_TextureFileInfoUIStaticTable.begin();

	for( ; itr_TextureInfo != m_TextureFileInfoUIStaticTable.end(); ++itr_TextureInfo)
	{
		itr_TextureInfo->second->Release();
	}
	m_TextureFileInfoUIStaticTable.clear();

	map<IFileInfo*,IGwUIStatic*>::iterator itr_AnimInfo = m_AnimFileInfoUIStaticTable.begin();

	for( ; itr_AnimInfo != m_AnimFileInfoUIStaticTable.end(); ++itr_AnimInfo)
	{
		itr_AnimInfo->second->Release();
	}
	m_AnimFileInfoUIStaticTable.clear();

	RemoveAllAnimSlot();
}

void			CUIComponent_SkinMeshObject::Initialize()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUISprite * pSprite_Title = CUIHelper::CreateUISprite("skinmesh_title.png");
	m_pBaseControl->AddControl(pSprite_Title); pSprite_Title->Release();
	m_fHeight += pSprite_Title->GetCollisSize()->y + 13;

	ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

	if( pSkinMesh != NULL )
	{
		IGwUIStatic * pStatic_Texture = CUIHelper::CreateUIStatic(17, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(1,1,1,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
		pStatic_Texture->SetLocalPos(GwVector2(13, 35));
		pStatic_Texture->SetText(L"ÅØ½ºÃÄ");
		m_pBaseControl->AddControl(pStatic_Texture); pStatic_Texture->Release();
		m_fHeight += 30;

		float fHeight = m_fHeight;
		vector<ITextureFileInfo> * pTextureFileInfoList = pSkinMesh->GetTextureFileInfoList();
		vector<ITextureFileInfo>::iterator itr = pTextureFileInfoList->begin();

		for( ; itr != pTextureFileInfoList->end(); ++itr )
		{
			ITextureFileInfo * pTextureFileInfo = &(*itr);

			CParameta_TextureFileInfo * pParameta = new CParameta_TextureFileInfo(*itr);
			CUIDragItemSprite * pUIDragItemSprite = new CUIDragItemSprite(eDragObject_UISprite, 
				eDragObjectSender_UISprite, 
				eDragObjectReceiver_TexturePanel, 
				"EsTool_Resources\\UI\\Inspector\\", 
				"item.png",
				pParameta);
			pParameta->Release();

			pUIDragItemSprite->GetDelegate_Drag(eDelegate_Drag_Receive)->Bind<CUIComponent_SkinMeshObject, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CUIComponent_SkinMeshObject::OnDragReceive>(this);

			pUIDragItemSprite->GetUISprite()->SetLocalPos(GwVector2(12, fHeight));
			m_pBaseControl->AddControl(pUIDragItemSprite->GetUISprite());

			IGwUIStatic * pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), true, 0, GwColor(0.8f, 0.8f, 0.8f, 0.8f));
			pStatic->SetLocalPos(GwVector2(16, fHeight + 2));

			wchar_t wszFileName[128] = { 0, };
			wstring wstrFileName;
			ConvertAnsiStringToWideCch(wszFileName, pTextureFileInfo->m_strFileName.c_str(), 128);
			IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
			CUIHelper::FittingStringExtent(pUIFont, 150, wszFileName, wstrFileName);
			pStatic->SetText(wstrFileName.c_str());
			m_pBaseControl->AddControl(pStatic);
			m_TextureFileInfoUIStaticTable.insert(map<ITextureFileInfo*,IGwUIStatic*>::value_type(pTextureFileInfo, pStatic));

			IGwUIButton * pButton_Find = CUIHelper::CreateUIButton("find_normal.png", "find_over.png", "find_normal.png", "find_click.png", "find_normal.png");
			pButton_Find->SetLocalPos(GwVector2(165 + 12, fHeight+1));
			CBtnCallback_Texture_Find * pCallback_Texture_Find = new CBtnCallback_Texture_Find(this, pTextureFileInfo);
			pButton_Find->SetCallback(pCallback_Texture_Find); pCallback_Texture_Find->Release();
			m_pBaseControl->AddControl(pButton_Find);

			m_DragItemSpriteTable_TextureInfo.insert(map<CUIDragItemSprite*,IGwUIButton*>::value_type(pUIDragItemSprite,pButton_Find));

			fHeight += 22;
			m_fHeight += 22;
		}

		IGwUIStatic * pStatic_Anim = CUIHelper::CreateUIStatic(17, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(1,1,1,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
		pStatic_Anim->SetLocalPos(GwVector2(13, m_fHeight + 8));
		pStatic_Anim->SetText(L"¾Ö´Ï¸ÞÀÌ¼Ç");
		m_pBaseControl->AddControl(pStatic_Anim); pStatic_Anim->Release();
		m_fHeight += 38;
		m_nAnimSlotFirstHeight = m_fHeight;

		ResetAnimSlot();
	}

	Gw::GetBillboardMgr()->EndTextureMerge();
}

void			CUIComponent_SkinMeshObject::Update(float fDeltaTime)					
{ 
}

void			CUIComponent_SkinMeshObject::RemoveAllAnimSlot()
{
	map<CUIDragItemSprite*,IGwUIButton*>::iterator itr_AnimInfo = m_DragItemSpriteTable_AnimInfo.begin();
	for( ; itr_AnimInfo != m_DragItemSpriteTable_AnimInfo.end(); ++itr_AnimInfo )
	{
		m_pBaseControl->RemoveControl(itr_AnimInfo->first->GetUISprite());
		delete itr_AnimInfo->first;
		m_pBaseControl->RemoveControl(itr_AnimInfo->second);
		itr_AnimInfo->second->Release();
	}

	m_DragItemSpriteTable_AnimInfo.clear();

	map<IFileInfo*,IGwUIStatic*>::iterator itr_FileInfo = m_AnimFileInfoUIStaticTable.begin();
	for( ; itr_FileInfo != m_AnimFileInfoUIStaticTable.end(); ++itr_FileInfo )
	{
		m_pBaseControl->RemoveControl(itr_FileInfo->second);
		itr_FileInfo->second->Release();
	}

	m_AnimFileInfoUIStaticTable.clear();

	map<int,IGwUIButton*>::iterator itr_AnimRemove = m_AnimInfo_MinusButtonTable.begin();
	for( ; itr_AnimRemove != m_AnimInfo_MinusButtonTable.end(); ++itr_AnimRemove )
	{
		m_pBaseControl->RemoveControl(itr_AnimRemove->second);
		itr_AnimRemove->second->Release();
	}

	m_AnimInfo_MinusButtonTable.clear();

	m_DragItemSpriteTable_Index.clear();
}

void			CUIComponent_SkinMeshObject::ResetAnimSlot()
{
	float fHeight = m_nAnimSlotFirstHeight;
	m_fHeight = m_nAnimSlotFirstHeight;

	ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

	if( pSkinMesh != NULL )
	{
		int nIndex = 0;
		map<int,IFileInfo>	* pGameFileInfoTable = pSkinMesh->GetAnimFileInfoTable();
		map<int,IFileInfo>::iterator itr = pGameFileInfoTable->begin();
		for( ; itr != pGameFileInfoTable->end(); ++itr, ++nIndex )
		{
			IFileInfo * pFileInfo = &itr->second;

			CUIDragItemSprite * pUIDragItemSprite = new CUIDragItemSprite(eDragObject_UISprite, 
				eDragObjectSender_UISprite, 
				eDragObjectReceiver_AnimPanel, 
				"EsTool_Resources\\UI\\Inspector\\", 
				"item.png",
				pFileInfo);

			pUIDragItemSprite->GetDelegate_Drag(eDelegate_Drag_Receive)->Bind<CUIComponent_SkinMeshObject, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CUIComponent_SkinMeshObject::OnDragReceive>(this);

			pUIDragItemSprite->GetUISprite()->SetLocalPos(GwVector2(12, fHeight));
			m_pBaseControl->AddControl(pUIDragItemSprite->GetUISprite());

			IGwUIStatic * pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), true, 0, GwColor(0.8f, 0.8f, 0.8f, 0.8f));
			pStatic->SetLocalPos(GwVector2(16, fHeight + 2));

			wchar_t wszFileName[128] = { 0, };
			wstring wstrFileName;
			ConvertAnsiStringToWideCch(wszFileName, pFileInfo->m_strFileName.c_str(), 128);
			IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
			CUIHelper::FittingStringExtent(pUIFont, 150, wszFileName, wstrFileName);
			pStatic->SetText(wstrFileName.c_str());
			m_pBaseControl->AddControl(pStatic);

			IGwUIButton * pButton_Find = CUIHelper::CreateUIButton("find_normal.png", "find_over.png", "find_normal.png", "find_click.png", "find_normal.png");
			pButton_Find->SetLocalPos(GwVector2(165 + 12, fHeight+1));
			CBtnCallback_Anim_Find * pCallback_Anim_Find = new CBtnCallback_Anim_Find(this, pFileInfo);
			pButton_Find->SetCallback(pCallback_Anim_Find); pCallback_Anim_Find->Release();
			m_pBaseControl->AddControl(pButton_Find);

			IGwUIButton * pButton_Remove = CUIHelper::CreateUIButton("minus_normal.png", "minus_over.png", "minus_normal.png", "minus_click.png", "minus_normal.png");
			pButton_Remove->SetLocalPos(GwVector2(210 + 12, fHeight+1));
			CBtnCallback_Anim_Remove * pCallback_Anim_Remove = new CBtnCallback_Anim_Remove(this, itr->first);
			pButton_Remove->SetCallback(pCallback_Anim_Remove); pCallback_Anim_Remove->Release();
			m_pBaseControl->AddControl(pButton_Remove);
			m_AnimInfo_MinusButtonTable.insert(map<int,IGwUIButton*>::value_type(nIndex, pButton_Remove));

			m_DragItemSpriteTable_AnimInfo.insert(map<CUIDragItemSprite*,IGwUIButton*>::value_type(pUIDragItemSprite,pButton_Find));
			m_AnimFileInfoUIStaticTable.insert(map<IFileInfo*,IGwUIStatic*>::value_type(pFileInfo, pStatic));

			m_DragItemSpriteTable_Index.insert(map<CUIDragItemSprite*,int>::value_type(pUIDragItemSprite,nIndex));

			fHeight += 22;
			m_fHeight += 22;
		}

		CUIDragItemSprite * pUIDragItemSprite = new CUIDragItemSprite(eDragObject_UISprite, 
			eDragObjectSender_UISprite, 
			eDragObjectReceiver_AnimPanel, 
			"EsTool_Resources\\UI\\Inspector\\", 
			"item.png",
			m_pEmptyFileInfo);

		pUIDragItemSprite->GetDelegate_Drag(eDelegate_Drag_Receive)->Bind<CUIComponent_SkinMeshObject, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CUIComponent_SkinMeshObject::OnDragReceive>(this);

		pUIDragItemSprite->GetUISprite()->SetLocalPos(GwVector2(12, fHeight));
		m_pBaseControl->AddControl(pUIDragItemSprite->GetUISprite());

		IGwUIStatic * pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), true, 0, GwColor(0.8f, 0.8f, 0.8f, 0.8f));
		pStatic->SetLocalPos(GwVector2(16, fHeight + 2));
		pStatic->SetText(L"None");
		m_pBaseControl->AddControl(pStatic);

		IGwUIButton * pButton_Find = CUIHelper::CreateUIButton("find_normal.png", "find_over.png", "find_normal.png", "find_click.png", "find_normal.png");
		pButton_Find->SetLocalPos(GwVector2(165 + 12, fHeight+1));
		CBtnCallback_Anim_Find * pCallback_Anim_Find = new CBtnCallback_Anim_Find(this, NULL);
		pButton_Find->SetCallback(pCallback_Anim_Find); pCallback_Anim_Find->Release();
		m_pBaseControl->AddControl(pButton_Find);

		m_DragItemSpriteTable_AnimInfo.insert(map<CUIDragItemSprite*,IGwUIButton*>::value_type(pUIDragItemSprite,pButton_Find));
		m_AnimFileInfoUIStaticTable.insert(map<IFileInfo*,IGwUIStatic*>::value_type(m_pEmptyFileInfo, pStatic));

		m_fHeight += 30;
	}
}

void			CUIComponent_SkinMeshObject::OnButtonClick_Texture_Find(ITextureFileInfo * pTextureFileInfo)
{
	NotificationArg_InspectorComponent_TextureInfo_Find * pArg = new NotificationArg_InspectorComponent_TextureInfo_Find();
	pArg->m_pTextureFileInfo = pTextureFileInfo;

	CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_TextureInfo_Find, pArg);
	pArg->Release();
}

void			CUIComponent_SkinMeshObject::OnButtonClick_Anim_Find(IFileInfo * pFileInfo)
{
	if( pFileInfo != NULL ) 
	{
		NotificationArg_InspectorComponent_AnimInfo_Find * pArg = new NotificationArg_InspectorComponent_AnimInfo_Find();
		pArg->m_pFileInfo = pFileInfo;

		CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_AnimInfo_Find, pArg);
		pArg->Release();
	}
}

void			CUIComponent_SkinMeshObject::OnButtonClick_Anim_Remove(int nIndex)
{
	ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

	if( pSkinMesh != NULL )
	{
		RemoveAllAnimSlot();

		Gw::GetBillboardMgr()->BeginTextureMerge();
		Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

		pSkinMesh->RemoveAnimSequence(nIndex);
		ResetAnimSlot();

		pSkinMesh->OnAnimPlay();

		Gw::GetBillboardMgr()->EndTextureMerge();
	}
}

void			CUIComponent_SkinMeshObject::OnDragReceive(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender)
{
	switch(eSender)
	{
	case eDragObjectSender_Project_File:
		{
			if( pSrcDragItem != NULL && pDestDragItem != NULL )
			{
				CUIListViewItem * pListViewItem = static_cast<CUIListViewItem*>(pSrcDragItem);
				CUIDragItemSprite * pDragItemSprite = static_cast<CUIDragItemSprite*>(pDestDragItem);

				if( pListViewItem != NULL && pDragItemSprite != NULL )
				{
					switch(pDragItemSprite->GetDragObjectReceiver())
					{
					case eDragObjectReceiver_TexturePanel:
						{
							string strOrigToken = CGwStrTok::GetTokenFormat();
							CGwStrTok::SetTokenFormat(".");
							CGwStrTok::SetString(pListViewItem->GetName().c_str());
							CGwStrTok::SetTokenFormat(strOrigToken.c_str());

							int nTokenNum = CGwStrTok::GetNumToken();
							string strExtent = CGwStrTok::GetToken(nTokenNum-1);

							if( strExtent == "png" || strExtent == "PNG" || strExtent == "dds" || strExtent == "DDS" )
							{
								CParameta_String * pParameta_String = dynamic_cast<CParameta_String*>(pListViewItem->GetParameta());
								CParameta_TextureFileInfo * pParameta_TextureFileInfo = dynamic_cast<CParameta_TextureFileInfo*>(pDragItemSprite->GetParameta());

								if( pParameta_String != NULL && pParameta_TextureFileInfo != NULL )
								{
									ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

									if( pSkinMesh != NULL )
									{
										ITextureFileInfo * pTextureFileInfo = pParameta_TextureFileInfo->GetParameta_TextureFileInfo();

										pTextureFileInfo->m_strPath = pParameta_String->GetParameta_String();
										pTextureFileInfo->m_strFileName = pListViewItem->GetName();
										pSkinMesh->SetTextureFileInfo(pTextureFileInfo);

										map<ITextureFileInfo*,IGwUIStatic*>::iterator itr = m_TextureFileInfoUIStaticTable.begin();

										for( ; itr != m_TextureFileInfoUIStaticTable.end(); ++itr )
										{
											if( itr->first->m_nMeshIndex == pTextureFileInfo->m_nMeshIndex && 
												itr->first->m_nSubMtrlSetIndex == pTextureFileInfo->m_nSubMtrlSetIndex && 
												itr->first->m_nTextureIndex == pTextureFileInfo->m_nTextureIndex )
											{
												wchar_t wszFileName[128] = { 0, };
												wstring wstrFileName;
												ConvertAnsiStringToWideCch(wszFileName, pListViewItem->GetName().c_str(), 128);
												IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
												CUIHelper::FittingStringExtent(pUIFont, 150, wszFileName, wstrFileName);
												itr->second->SetText(wstrFileName.c_str());

												break;
											}
										}
										map<CUIDragItemSprite*,IGwUIButton*>::iterator itr_drag = m_DragItemSpriteTable_TextureInfo.find(pDragItemSprite);
										if( itr_drag != m_DragItemSpriteTable_TextureInfo.end() )
										{
											CBtnCallback_Texture_Find * pCallback_Texture_Find = new CBtnCallback_Texture_Find(this, pTextureFileInfo);
											itr_drag->second->SetCallback(pCallback_Texture_Find); pCallback_Texture_Find->Release();
										}
									}
								}
							}
						}
						break;

					case eDragObjectReceiver_AnimPanel:
						{
							string strOrigToken = CGwStrTok::GetTokenFormat();
							CGwStrTok::SetTokenFormat(".");
							CGwStrTok::SetString(pListViewItem->GetName().c_str());
							CGwStrTok::SetTokenFormat(strOrigToken.c_str());

							int nTokenNum = CGwStrTok::GetNumToken();
							string strExtent = CGwStrTok::GetToken(nTokenNum-1);

							if( strExtent == "ab" || strExtent == "AB" )
							{
								CParameta_String * pParameta_String = dynamic_cast<CParameta_String*>(pListViewItem->GetParameta());
								IFileInfo * pFileInfo = dynamic_cast<IFileInfo*>(pDragItemSprite->GetParameta());

								if( pParameta_String != NULL && pFileInfo != NULL )
								{
									ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

									if( pSkinMesh != NULL )
									{
										pFileInfo->m_strPath = pParameta_String->GetParameta_String();
										pFileInfo->m_strPath += "\\";
										pFileInfo->m_strFileName = pListViewItem->GetName();

										map<CUIDragItemSprite*,int>::iterator itr = m_DragItemSpriteTable_Index.find(pDragItemSprite);

										if( itr != m_DragItemSpriteTable_Index.end() )
										{
											IGwAnimSequence * pAnimSequence = IResourceHelper::Create_AnimSequence(pFileInfo->m_strPath.c_str(), pFileInfo->m_strFileName.c_str());

											if( pAnimSequence != NULL )
											{
												pSkinMesh->ChangeAnimSequence(itr->second, pAnimSequence, *pFileInfo);
												pAnimSequence->Release();

												pSkinMesh->OnAnimPlay();

												RemoveAllAnimSlot();

												Gw::GetBillboardMgr()->BeginTextureMerge();
												Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

												ResetAnimSlot();

												Gw::GetBillboardMgr()->EndTextureMerge();

												CUIDragObject::GetInstance()->Hide();
											}
										}
										else 
										{
											IGwAnimSequence * pAnimSequence = IResourceHelper::Create_AnimSequence(pFileInfo->m_strPath.c_str(), pFileInfo->m_strFileName.c_str());

											if( pAnimSequence != NULL )
											{
												pSkinMesh->AddAnimSequence(pAnimSequence, *pFileInfo);
												pAnimSequence->Release();

												pSkinMesh->OnAnimPlay();

												RemoveAllAnimSlot();

												Gw::GetBillboardMgr()->BeginTextureMerge();
												Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

												ResetAnimSlot();

												Gw::GetBillboardMgr()->EndTextureMerge();

												CUIDragObject::GetInstance()->Hide();

												CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_Height_Change, NULL);
											}
										}

									}
								}
							}
						}
						break;
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