#include "StdAfx.h"
#include "UIComponent_MeshCollider.h"

I_IMPLEMENT_DYNCREATE(CUIComponent_MeshCollider, ISingletonObject::GetTypeModule())

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBtnCallback_Mesh_Find

class	CUIComponent_MeshCollider::CBtnCallback_Mesh_Find : public IGwUIButton_Callback
{
public:
	CBtnCallback_Mesh_Find(CUIComponent_MeshCollider * pThis, IFileInfo * pFileInfo)
		: m_pThis(pThis), m_pFileInfo(pFileInfo)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Mesh_Find(m_pFileInfo); 
	}
private:
	CUIComponent_MeshCollider		*	m_pThis;
	IFileInfo						*	m_pFileInfo;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBtnCallback_Mesh_Find

class	CUIComponent_MeshCollider::CBtnCallback_Mesh_Remove : public IGwUIButton_Callback
{
public:
	CBtnCallback_Mesh_Remove(CUIComponent_MeshCollider * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Mesh_Remove(); 
	}
private:
	CUIComponent_MeshCollider		*	m_pThis;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCheckboxCallback_Active

class CUIComponent_MeshCollider::CCheckboxCallback_Active : public IGwUICheckbox_Callback
{
public:
	CCheckboxCallback_Active(CUIComponent_MeshCollider* pThis) :m_pThis(pThis)				{ }

	virtual void	OnCheckChange_TRUE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pThis->OnCheckBox_Active(true);
	}
	virtual void	OnCheckChange_FALSE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pThis->OnCheckBox_Active(false);
	}

private:
	CUIComponent_MeshCollider * m_pThis;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIComponent_MeshCollider

CUIComponent_MeshCollider::CUIComponent_MeshCollider()
: m_pUIDragItemSprite(NULL), m_pStatic_MeshFileName(NULL), m_pCheckBox_CameraRayCast(NULL)
{
}

CUIComponent_MeshCollider::~CUIComponent_MeshCollider(void)
{
	if( m_pUIDragItemSprite != NULL ) 
		delete m_pUIDragItemSprite;

	if( m_pStatic_MeshFileName != NULL ) 
		m_pStatic_MeshFileName->Release();

	if( m_pCheckBox_CameraRayCast != NULL )
		m_pCheckBox_CameraRayCast->Release();
}

void			CUIComponent_MeshCollider::Initialize()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUISprite * pSprite_Title = CUIHelper::CreateUISprite("mesh_collider_title.png");
	m_pBaseControl->AddControl(pSprite_Title); pSprite_Title->Release();
	m_fHeight += pSprite_Title->GetCollisSize()->y + 13;

	IComponent_MeshCollider * pMeshCollider = dynamic_cast<IComponent_MeshCollider*>(m_pComponent);
	if( m_pGameObject != NULL && pMeshCollider != NULL)
	{
		IGwUIStatic * pStatic_Texture = CUIHelper::CreateUIStatic(17, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(1,1,1,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
		pStatic_Texture->SetLocalPos(GwVector2(13, 35));
		pStatic_Texture->SetText(L"¸Þ½Ã");
		m_pBaseControl->AddControl(pStatic_Texture); pStatic_Texture->Release();
		m_fHeight += 25;

		CParameta_MeshFileInfo * pParameta = new CParameta_MeshFileInfo(*pMeshCollider->GetFileInfo());
		m_pUIDragItemSprite = new CUIDragItemSprite(eDragObject_UISprite, 
													eDragObjectSender_UISprite, 
													eDragObjectReceiver_MeshPanel, 
													"EsTool_Resources\\UI\\Inspector\\", 
													"item.png",
													pParameta);
		pParameta->Release();

		m_pUIDragItemSprite->GetDelegate_Drag(eDelegate_Drag_Receive)->Bind<CUIComponent_MeshCollider, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CUIComponent_MeshCollider::OnDragReceive>(this);

		m_pUIDragItemSprite->GetUISprite()->SetLocalPos(GwVector2(12, m_fHeight));
		m_pBaseControl->AddControl(m_pUIDragItemSprite->GetUISprite());

		m_pStatic_MeshFileName = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), true, 0, GwColor(0.8f, 0.8f, 0.8f, 0.8f));
		m_pStatic_MeshFileName->SetLocalPos(GwVector2(16, m_fHeight + 2));

		if( pMeshCollider->GetFileInfo()->m_strFileName == "" )
		{
			m_pStatic_MeshFileName->SetText(L"None");
			m_pBaseControl->AddControl(m_pStatic_MeshFileName);
		}
		else 
		{
			wchar_t wszFileName[128] = { 0, };
			wstring wstrFileName;
			ConvertAnsiStringToWideCch(wszFileName, pMeshCollider->GetFileInfo()->m_strFileName.c_str(), 128);
			IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
			CUIHelper::FittingStringExtent(pUIFont, 150, wszFileName, wstrFileName);
			m_pStatic_MeshFileName->SetText(wstrFileName.c_str());
			m_pBaseControl->AddControl(m_pStatic_MeshFileName);
		}

		IGwUIButton * pButton_Find = CUIHelper::CreateUIButton("find_normal.png", "find_over.png", "find_normal.png", "find_click.png", "find_normal.png");
		pButton_Find->SetLocalPos(GwVector2(165 + 12, m_fHeight+1));
		CBtnCallback_Mesh_Find * pCallback_Mesh_Find = new CBtnCallback_Mesh_Find(this, pMeshCollider->GetFileInfo());
		pButton_Find->SetCallback(pCallback_Mesh_Find); pCallback_Mesh_Find->Release();
		m_pBaseControl->AddControl(pButton_Find); pButton_Find->Release();

		IGwUIButton * pButton_Remove = CUIHelper::CreateUIButton("minus_normal.png", "minus_over.png", "minus_normal.png", "minus_click.png", "minus_normal.png");
		pButton_Remove->SetLocalPos(GwVector2(210 + 12, m_fHeight+1));
		CBtnCallback_Mesh_Remove * pCallback_Mesh_Remove = new CBtnCallback_Mesh_Remove(this);
		pButton_Remove->SetCallback(pCallback_Mesh_Remove); pCallback_Mesh_Remove->Release();
		m_pBaseControl->AddControl(pButton_Remove);	pButton_Remove->Release();

		m_fHeight += 28;

		m_pCheckBox_CameraRayCast = CUIHelper::CreateUICheckBox("checkbox_cameraray_off.png", "checkbox_cameraray_on.png");
		IGwUICheckbox_Callback*	pChkBoxCallback;
		pChkBoxCallback = new CCheckboxCallback_Active(this);
		m_pCheckBox_CameraRayCast->SetCallback(pChkBoxCallback);	pChkBoxCallback->Release();
		m_pCheckBox_CameraRayCast->SetLocalPos(GwVector2(13, m_fHeight+1));
		m_pBaseControl->AddControl(m_pCheckBox_CameraRayCast);

		if( pMeshCollider->IsCameraRayCast() == true )
		{
			m_pCheckBox_CameraRayCast->SetCheck(true);
		}

		m_fHeight += 25;

	}

	Gw::GetBillboardMgr()->EndTextureMerge();
}

void			CUIComponent_MeshCollider::Update(float fDeltaTime)					
{ 
}

void			CUIComponent_MeshCollider::OnButtonClick_Mesh_Find(IFileInfo * pFileInfo)
{
	if( pFileInfo != NULL ) 
	{
		NotificationArg_InspectorComponent_MeshInfo_Find * pArg = new NotificationArg_InspectorComponent_MeshInfo_Find();
		pArg->m_pFileInfo = pFileInfo;

		CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_MeshInfo_Find, pArg);
		pArg->Release();
	}
}

void			CUIComponent_MeshCollider::OnButtonClick_Mesh_Remove()
{
	m_pGameObject->RemoveComponent(m_pComponent);

	CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_Reflesh, NULL);
}

void			CUIComponent_MeshCollider::OnCheckBox_Active(bool IsChecked)
{
	IComponent_MeshCollider * pMeshCollider = dynamic_cast<IComponent_MeshCollider*>(m_pComponent);
	if( pMeshCollider != NULL )
	{
		pMeshCollider->SetCameraRayCast(IsChecked);
	}
}

void			CUIComponent_MeshCollider::OnDragReceive(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender)
{
	switch(eSender)
	{
	case eDragObjectSender_Project_File:
		{
			IComponent_MeshCollider * pMeshCollider = dynamic_cast<IComponent_MeshCollider*>(m_pComponent);

			if( m_pGameObject != NULL && pMeshCollider != NULL && pSrcDragItem != NULL && pDestDragItem != NULL )
			{
				CUIListViewItem * pListViewItem = static_cast<CUIListViewItem*>(pSrcDragItem);
				CUIDragItemSprite * pDragItemSprite = static_cast<CUIDragItemSprite*>(pDestDragItem);

				if( pListViewItem != NULL && pDragItemSprite != NULL )
				{
					switch(pDragItemSprite->GetDragObjectReceiver())
					{
					case eDragObjectReceiver_MeshPanel:
						{
							string strOrigToken = CGwStrTok::GetTokenFormat();
							CGwStrTok::SetTokenFormat(".");
							CGwStrTok::SetString(pListViewItem->GetName().c_str());
							CGwStrTok::SetTokenFormat(strOrigToken.c_str());

							int nTokenNum = CGwStrTok::GetNumToken();
							string strExtent = CGwStrTok::GetToken(nTokenNum-1);

							if( strExtent == "gb" || strExtent == "GB" )
							{
								CParameta_String * pParameta_String = dynamic_cast<CParameta_String*>(pListViewItem->GetParameta());
								CParameta_MeshFileInfo * pMeshFileInfo = dynamic_cast<CParameta_MeshFileInfo*>(pDragItemSprite->GetParameta());

								if( m_pGameObject != NULL && pParameta_String != NULL && pMeshFileInfo != NULL )
								{
									IFileInfo * pFileInfo = dynamic_cast<IFileInfo*>(pMeshFileInfo->GetParameta_MeshFileInfo());

									if( pFileInfo != NULL )
									{
										pFileInfo->m_strPath = pParameta_String->GetParameta_String();
										pFileInfo->m_strPath += "\\";
										pFileInfo->m_strFileName = pListViewItem->GetName();

										IGwStaticObject * pStaticObject = IResourceHelper::CreateStaticObject(pFileInfo->m_strPath.c_str(), pFileInfo->m_strPath.c_str(), pFileInfo->m_strFileName.c_str());

										if( pStaticObject != NULL && pStaticObject->IsIncludingMesh() == TRUE )
										{
											pMeshCollider->SetMeshObject(pStaticObject, *pFileInfo);

											wchar_t wszFileName[128] = { 0, };
											wstring wstrFileName;
											ConvertAnsiStringToWideCch(wszFileName, pFileInfo->m_strFileName.c_str(), 128);
											IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
											CUIHelper::FittingStringExtent(pUIFont, 150, wszFileName, wstrFileName);
											m_pStatic_MeshFileName->SetText(wstrFileName.c_str());
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