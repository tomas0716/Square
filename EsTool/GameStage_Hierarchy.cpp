#include "StdAfx.h"
#include "GameStage_Hierarchy.h"

I_IMPLEMENT_DYNCREATE(CGameStage_Hierarchy, ISingletonObject::GetTypeModule())

CGameStage_Hierarchy::CGameStage_Hierarchy(void)
: m_pLButtonDownGameObject(NULL), m_pRButtonDownGameObject(NULL), m_pSelGameObject(NULL), m_eObjectEditState(eObjectEditState_None), m_vObjectEditPickingPos(GwVector(0,0,0)),
  m_fLButtonDownProcessTime(0), m_fRButtonDownProcessTime(0)
{
	m_ptPrevMousePos.x = 0;
	m_ptPrevMousePos.y = 0;
}

CGameStage_Hierarchy::~CGameStage_Hierarchy(void)
{
	m_pPick->Release();
}

void		CGameStage_Hierarchy::OnCreateDevice()
{
	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();
	Gw::GetUIControlMgr()->AddFixedDialog(m_pDialog);
	m_pDialog->SetWorldPos(GwVector(0,22,0));

	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Hierarchy\\");
	
	IGwUISprite * pSprite_Back_Middle = CUIHelper::CreateUISprite("back_middle.png");
	pSprite_Back_Middle->SetLocalPos(GwVector2(4,33));
	m_pDialog->AddControl(pSprite_Back_Middle); pSprite_Back_Middle->Release();

	IGwUIDummyCtrl * pDummyCtrl_TreeView = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pDialog->AddControl(pDummyCtrl_TreeView); pDummyCtrl_TreeView->Release();
	m_pTreeView = new CUITreeView(pDummyCtrl_TreeView, GwVector2(4,33), GwVector2(225,978));
	m_pTreeView->SetAttribute(eDragObjectSender_Hierarchy, eDragObjectReceiver_Hierarchy, true, true, GwVector2(6,5));
	m_pTreeView->SetAttribute_TreeViewItem(2, 14, "굴림", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(0.5f,0.5f,0.5f,1), "empty.png", "plus.png", "minus.png", "sel.png", "edit.png");
	m_pTreeView->SetAttribute_Scroll(GwVector2(229,33), "track.png", "scroll_bar.png", "scroll.png", "scroll.png", 3);
	m_pTreeView->SetDoubleClickTreeOpen(false);

	m_pTreeView->GetDelegate_TreeView()->Bind<CGameStage_Hierarchy, CUITreeView*, &CGameStage_Hierarchy::OnTreeViewItemEmptyClick>(this);
	m_pTreeView->GetDelegate_DragReceive()->Bind<CGameStage_Hierarchy, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CGameStage_Hierarchy::OnDragReceive>(this);

	IGwUISprite * pSprite_Back = CUIHelper::CreateUISprite("back.png");
	pSprite_Back->SetLocalPos(GwVector2(0,0));
	m_pDialog->AddControl(pSprite_Back); pSprite_Back->Release();
	pSprite_Back->SetDependencyType(GWUISTATEDEPENDENCY_NOACTION);

	Gw::GetBillboardMgr()->EndTextureMerge();

	m_pPopupMenu = new CPopupMenu(eObjectEditState_Position|eObjectEditState_Rotation|eObjectEditState_UpDown|eObjectEditState_Delete);
	m_pPopupMenu->GetEvent_Click()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnObjectEditState);

	m_pPick = new IGwPick(CPerspectiveCamera::GetInstance()->GetCamera());

	CPickingMgr::GetInstance()->GetEvent_NonePicking()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnLButtonDown_NonePicking);

	IGameObject * pGameObject_Hierachy = IGameObjectMgr::GetInstance()->GetHierarchyNode();

	int nNumChild = pGameObject_Hierachy->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(pGameObject_Hierachy->GetChild_byIndex(i));

		if( pGameObject != NULL )
			AddGameObject_ForEsFile(NULL, pGameObject);
	}
}

void		CGameStage_Hierarchy::OnDestroyDevice()
{
	m_pTreeView->Release();

	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();

	m_pPopupMenu->Release();

	CPickingMgr::GetInstance()->GetEvent_NonePicking()->ClearEventOUT("CGameStage_Hierarchy");
}

void		CGameStage_Hierarchy::Update(float fDeltaTime)
{
	m_pTreeView->Update(fDeltaTime);
	m_pPopupMenu->Update(fDeltaTime);

	switch(m_eObjectEditState)
	{
	case eObjectEditState_Position:
		{
			GwVector vPickingPos = GetCurrPickingPos();

			if( m_pSelGameObject != NULL )
			{
				IGameObject * pParent = dynamic_cast<IGameObject*>(m_pSelGameObject->GetParent());

				if( pParent != NULL )
				{
					GwVector vParentPos = pParent->GetWorldPos();
					GwMatA matParentWorld = pParent->GetWorldMat();
					GwMatA matParentWorldInverse;
					D3DXMatrixInverse(&matParentWorldInverse, NULL, &matParentWorld);
					vPickingPos -= vParentPos;
					matParentWorldInverse._41 = matParentWorldInverse._42 = matParentWorldInverse._43 = 0;
					D3DXVec3TransformCoord(&vPickingPos, &vPickingPos, &matParentWorldInverse);
				}

				GwVector vPosition = m_pSelGameObject->GetPosition();
				vPickingPos.y = vPosition.y;

				m_pSelGameObject->SetPosition(vPickingPos);

				NotificationArg_GameObject_Position * pArg = new NotificationArg_GameObject_Position();
				pArg->m_vPosition = vPickingPos;

				CNotificationCenter::GetInstance()->PostNotification(Notification_GameObject_Position_Change, pArg);
				pArg->Release();

				CPickingObjectWireFrameRender::GetInstance()->OnUpdate();
			}

			m_vObjectEditPickingPos = vPickingPos;
		}
		break;
	case eObjectEditState_Rotation:
		{
			POINT ptMousePos = *Gw::GetKeyMapper()->GetCursorPos();
			
			float fMove = m_ptPrevMousePos.x - ptMousePos.x;

			if( m_pSelGameObject != NULL )
			{
				GwVector vRot = m_pSelGameObject->GetRotation();
				vRot.y += fMove * 0.5f;

				m_pSelGameObject->SetRotation(vRot);

				NotificationArg_GameObject_Rotation * pArg = new NotificationArg_GameObject_Rotation();
				pArg->m_vRotation = vRot;

				CNotificationCenter::GetInstance()->PostNotification(Notification_GameObject_Rotation_Change, pArg);
				pArg->Release();

				CPickingObjectWireFrameRender::GetInstance()->OnUpdate();
			}

			m_ptPrevMousePos = ptMousePos;
		}
		break;
	case eObjectEditState_UpDown:
		{
			POINT ptMousePos = *Gw::GetKeyMapper()->GetCursorPos();
			
			float fMove = m_ptPrevMousePos.y - ptMousePos.y;

			if( m_pSelGameObject != NULL )
			{
				GwVector vPosition = m_pSelGameObject->GetPosition();
				vPosition.y += fMove;

				m_pSelGameObject->SetPosition(vPosition);

				NotificationArg_GameObject_Position * pArg = new NotificationArg_GameObject_Position();
				pArg->m_vPosition = vPosition;

				CNotificationCenter::GetInstance()->PostNotification(Notification_GameObject_Position_Change, pArg);
				pArg->Release();

				CPickingObjectWireFrameRender::GetInstance()->OnUpdate();
			}

			m_ptPrevMousePos = ptMousePos;
		}
		break;
	case eObjectEditState_Delete:
		{
			if( m_pSelGameObject != NULL )
			{
				RemoveGameObject(m_pSelGameObject);

				CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

				if( pGameStage_Inspector != NULL ) 
				{
					pGameStage_Inspector->SetGameObject(NULL);
				}

				CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(NULL);
				CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(NULL);

				m_pSelGameObject = NULL;
				m_pLButtonDownGameObject = NULL;
				m_pRButtonDownGameObject = NULL;
			}
		}
		break;
	}

	if( Gw::GetKeyMapper()->WasKeyDown(VK_DELETE) == TRUE )
	{
		if( Gw::GetUIControlMgr()->GetFocusDialog() == m_pDialog )
		{
			CUITreeViewItem * pTreeViewItem = m_pTreeView->GetSelectedItem();

			if( pTreeViewItem != NULL )
			{
				IGameObject * pGameObject = dynamic_cast<IGameObject*>(pTreeViewItem->GetParameta());

				if( pGameObject != NULL )
				{
					RemoveGameObject(pGameObject);

					CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

					if( pGameStage_Inspector != NULL ) 
					{
						pGameStage_Inspector->SetGameObject(NULL);
					}

					CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(NULL);
					CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(NULL);

					m_pSelGameObject = NULL;
					m_pLButtonDownGameObject = NULL;
					m_pRButtonDownGameObject = NULL;
				}
			}
		}
	}

	if( Gw::GetKeyMapper()->WasKeyDown(VK_LBUTTON) == TRUE )
	{
		m_eObjectEditState = eObjectEditState_None;
	}
}

void		CGameStage_Hierarchy::AddGameObject(IGameObject * pParent, IGameObject * pObject)
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Hierarchy\\");

	CUITreeViewItem * pFindTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(pParent);
	CUITreeViewItem * pNewTreeViewItem = m_pTreeView->AddItem(pFindTreeViewItem, pObject->GetName().c_str(), pObject);
	SetDelegate(pNewTreeViewItem);
	if( pFindTreeViewItem != NULL ) pFindTreeViewItem->SetChildOpenFlag(true);
	m_pTreeView->UpdateItemPos();

	Gw::GetBillboardMgr()->EndTextureMerge();

	m_pTreeView->SetSelectedItem(pNewTreeViewItem);

	GwMatA matWorld;
	D3DXMatrixIdentity(&matWorld);

	GwMatA matParentWorld;
	D3DXMatrixIdentity(&matParentWorld);
	if( pParent != NULL )
		matParentWorld = pParent->GetWorldMat();

	GwMatA matParentWorldInverse;
	D3DXMatrixInverse(&matParentWorldInverse, NULL, &matParentWorld);

	D3DXMatrixMultiply(&matWorld, &matWorld, &matParentWorldInverse);

	pObject->SetPosition(GwVector(matWorld._41, matWorld._42, matWorld._43));

	matWorld._41 = matWorld._42 = matWorld._43 = 0;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	GwVector vRot = IObject::GetRotation(matWorld);
	pObject->SetRotation(vRot);

	if( pParent != NULL )	pParent->AddChild(pObject);
	else					IGameObjectMgr::GetInstance()->AddHierarchyNode(pObject);

	string strClassName = IHashObject::GetInstance()->GetHashString(pObject->GetClassHashCode());

	if( pObject->IsVisible() == true )
	{
		if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
		{
			ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pObject);

			if( pSkinMesh != NULL )
			{
				IGwScnObject * pScnObject = pSkinMesh->GetScnObject();

				if( pScnObject != NULL ) 
				{
					CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(pScnObject, pObject);
					pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnLButtonDown_Picking);
					pPickingEntity->GetEvent_LButtonUp()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnLButtonUp_Picking);

					pPickingEntity->GetEvent_RButtonDown()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnRButtonDown_Picking);
					pPickingEntity->GetEvent_RButtonUp()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnRButtonUp_Picking);
				}
			}
		}

		CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(pObject);
		CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(pObject);
		m_pSelGameObject = pObject;
	}
}

void		CGameStage_Hierarchy::AddGameObject_ForEsFile(IGameObject * pParent, IGameObject * pObject)
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Hierarchy\\");

	CUITreeViewItem * pFindTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(pParent);
	CUITreeViewItem * pNewTreeViewItem = m_pTreeView->AddItem(pFindTreeViewItem, pObject->GetName().c_str(), pObject);
	SetDelegate(pNewTreeViewItem);
	m_pTreeView->UpdateItemPos();

	Gw::GetBillboardMgr()->EndTextureMerge();

	string strClassName = IHashObject::GetInstance()->GetHashString(pObject->GetClassHashCode());

	if( pObject->IsVisible() == true )
	{
		if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
		{
			ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pObject);

			if( pSkinMesh != NULL )
			{
				IGwScnObject * pScnObject = pSkinMesh->GetScnObject();

				if( pScnObject != NULL ) 
				{
					CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(pScnObject, pObject);
					pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnLButtonDown_Picking);
					pPickingEntity->GetEvent_LButtonUp()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnLButtonUp_Picking);

					pPickingEntity->GetEvent_RButtonDown()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnRButtonDown_Picking);
					pPickingEntity->GetEvent_RButtonUp()->AddEventHandler("CGameStage_Hierarchy", this, &CGameStage_Hierarchy::OnRButtonUp_Picking);
				}
			}
		}

		if( strcmp(strClassName.c_str(), "IPathObject") == 0 )
		{
			IPathObject * pPathObject = dynamic_cast<IPathObject*>(pObject);
			CPathObjectRenderMgr::GetInstance()->AddPathObject(pPathObject);
		}
	}

	int nNumChild = pObject->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IGameObject * pChildGameObject = dynamic_cast<IGameObject*>(pObject->GetChild_byIndex(i));

		if( pChildGameObject )
			AddGameObject_ForEsFile(pObject, pChildGameObject);
	}
}

void		CGameStage_Hierarchy::Recursive_RemoveGameObjectPickingEntity(IGameObject * pObject)
{
	string strClassName = IHashObject::GetInstance()->GetHashString(pObject->GetClassHashCode());
	if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
	{
		ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pObject);

		if( pSkinMesh != NULL )
		{
			IGwScnObject * pScnObject = pSkinMesh->GetScnObject();

			if( pScnObject != NULL ) 
			{
				CPickingMgr::GetInstance()->RemovePickingEntity(pScnObject);
			}
		}
	}

	if( strcmp(strClassName.c_str(), "IPathObject") == 0 )
	{
		IPathObject * pPathObject = dynamic_cast<IPathObject*>(pObject);

		if( pPathObject != NULL )
		{
			CPathObjectRenderMgr::GetInstance()->RemovePathObject(pPathObject);
		}
	}

	int nCount = pObject->GetNumChild();

	for( int i = 0; i < nCount; ++i )
	{
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(pObject->GetChild_byIndex(i));

		if( pGameObject != NULL )
		{
			Recursive_RemoveGameObjectPickingEntity(pGameObject);
		}
	}
}

void		CGameStage_Hierarchy::RemoveGameObject(IGameObject * pObject)
{
	CUITreeViewItem * pTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(pObject);

	if( pTreeViewItem != NULL )
	{
		m_pTreeView->Remove(pTreeViewItem);
		m_pTreeView->UpdateItemPos();
	}

	string strClassName = IHashObject::GetInstance()->GetHashString(pObject->GetClassHashCode());

	if( strClassName == "ISkinMesh" )
	{
		ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pObject);

		if( pSkinMesh != NULL )
		{
			IGwScnObject * pScnObject = pSkinMesh->GetScnObject();

			if( pScnObject != NULL ) 
			{
				CPickingMgr::GetInstance()->RemovePickingEntity(pScnObject);
			}
		}
	}

	if( strClassName == "IPathObject" )
	{
		IPathObject * pPathObject = dynamic_cast<IPathObject*>(pObject);

		if( pPathObject != NULL )
		{
			CPathObjectRenderMgr::GetInstance()->RemovePathObject(pPathObject);
		}
	}

	Recursive_RemoveGameObjectPickingEntity(pObject);

	IGameObject * pParent = dynamic_cast<IGameObject*>(pObject->GetParent());

	if( pParent != NULL )	pParent->RemoveChild(pObject);
	else					IGameObjectMgr::GetInstance()->RemoveHierarchyNode(pObject);
}

void		CGameStage_Hierarchy::MoveGameObject(IGameObject * pParent, IGameObject * pObject)
{
	GwMatA matWorld = pObject->GetWorldMat();
	GwMatA matParentWorld = pParent->GetWorldMat();

	GwMatA matParentWorldInverse;
	D3DXMatrixInverse(&matParentWorldInverse, NULL, &matParentWorld);

	D3DXMatrixMultiply(&matWorld, &matWorld, &matParentWorldInverse);

	pObject->SetPosition(GwVector(matWorld._41, matWorld._42, matWorld._43));

	matWorld._41 = matWorld._42 = matWorld._43 = 0;
	D3DXMatrixInverse(&matWorld, NULL, &matWorld);
	GwVector vRot = IObject::GetRotation(matWorld);
	pObject->SetRotation(vRot);

	pParent->AddChild(pObject);

	CUITreeViewItem * pParentTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(pParent);
	CUITreeViewItem * pTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(pObject);

	if( pParentTreeViewItem == NULL )
		pParentTreeViewItem = m_pTreeView->GetTreeViewItem_Root();

	pParentTreeViewItem->AddChild(pTreeViewItem);

	pParentTreeViewItem->SetChildOpenFlag(true);
	m_pTreeView->UpdateItemPos();

	m_pTreeView->SetSelectedItem(pTreeViewItem);

	m_pSelGameObject = pObject;
}

void		CGameStage_Hierarchy::SetSelectGameObject (IGameObject * pGameObject)
{
	if( pGameObject != NULL )
	{
		m_pSelGameObject = pGameObject;
		CUITreeViewItem * pTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(m_pSelGameObject);

		if( pTreeViewItem != NULL )
		{
			CUITreeViewItem * pParentTreeViewItem = dynamic_cast<CUITreeViewItem*>(pTreeViewItem->GetParent());
			
			while(pParentTreeViewItem != NULL)
			{
				pParentTreeViewItem->SetChildOpenFlag(true);

				pParentTreeViewItem = dynamic_cast<CUITreeViewItem*>(pParentTreeViewItem->GetParent());
			}

			m_pTreeView->SetSelectedItem(pTreeViewItem);
			m_pTreeView->UpdateItemPos();

			CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

			if( pGameStage_Inspector != NULL ) 
			{
				pGameStage_Inspector->SetGameObject(pGameObject);
			}

			CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
			CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
		}
	}
	else 
	{
		m_pSelGameObject = NULL;

		m_pTreeView->SetSelectedItem(NULL);

		CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

		if( pGameStage_Inspector != NULL ) 
		{
			pGameStage_Inspector->SetGameObject(NULL);
		}

		CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(NULL);
		CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(NULL);
	}
}

void		CGameStage_Hierarchy::OnObjectEditState(const EventArg_PopupMenu_Click& Arg)
{
	m_eObjectEditState = Arg.eEditState;
	m_ptPrevMousePos = *Gw::GetKeyMapper()->GetCursorPos();

	m_vObjectEditPickingPos = GetCurrPickingPos();
}

void		CGameStage_Hierarchy::OnVisibleChanged()
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

GwVector	CGameStage_Hierarchy::GetCurrPickingPos()
{
	GwVector vRes = GwVector(0,0,0);

	POINT	vCurPos = Gw::GetKeyMapper()->GetCursorPos(Gw::GethWnd());
	m_pPick->Update(vCurPos.x, vCurPos.y);

	GwVector	*vRayOrg, *vRayDir;
	vRayOrg	= const_cast<GwVector*>(m_pPick->GetRayPos());
	vRayDir	= const_cast<GwVector*>(m_pPick->GetRayDir());

	GwVector vHit(0.f, 0.f, 10000.f);

	GwVector vPlane01 = GwVector(-100000000.0f, 0, 100000000.0f);
	GwVector vPlane02 = GwVector(100000000.0f, 0, 100000000.0f);
	GwVector vPlane03 = GwVector(100000000.0f, 0, -100000000.0f);
	GwVector vPlane04 = GwVector(-100000000.0f, 0, -100000000.0f);

	float fTime = GW_FLTMAX;
	bool IsIntersection = false;

	if( RayTriangle(vRayOrg, vRayDir, &vPlane01, &vPlane02, &vPlane03, &fTime) == TRUE )
	{
		if( fTime > 0 )
		{
			IsIntersection = true;
		}
	}
	else if( RayTriangle(vRayOrg, vRayDir, &vPlane03, &vPlane04, &vPlane01, &fTime) == TRUE )
	{
		if( fTime > 0 )
		{
			IsIntersection = true;
		}
	}

	if( IsIntersection == true )
	{
		vRes = (*vRayOrg) + ((*vRayDir) * fTime);
	}

	return vRes;
}

void		CGameStage_Hierarchy::OnLButtonDown_Picking(const EventArg_PickingEntity& Arg)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(Arg.pPickingEntity->GetParameta());

	if( pGameObject != NULL )
	{
		m_pLButtonDownGameObject = pGameObject;
		m_fLButtonDownProcessTime = CFrameWorks::GetProcessTime();
	}
}

void		CGameStage_Hierarchy::OnLButtonUp_Picking(const EventArg_PickingEntity& Arg)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(Arg.pPickingEntity->GetParameta());

	float fProcessTime = CFrameWorks::GetProcessTime();

	if( pGameObject != NULL && pGameObject == m_pLButtonDownGameObject && pGameObject != m_pSelGameObject && fProcessTime - m_fLButtonDownProcessTime < 0.15f )
	{
		m_pSelGameObject = pGameObject;
		CUITreeViewItem * pTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(m_pSelGameObject);

		if( pTreeViewItem != NULL )
		{
			CUITreeViewItem * pParentTreeViewItem = dynamic_cast<CUITreeViewItem*>(pTreeViewItem->GetParent());
			
			while(pParentTreeViewItem != NULL)
			{
				pParentTreeViewItem->SetChildOpenFlag(true);

				pParentTreeViewItem = dynamic_cast<CUITreeViewItem*>(pParentTreeViewItem->GetParent());
			}

			m_pTreeView->SetSelectedItem(pTreeViewItem);
			m_pTreeView->UpdateItemPos();

			CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

			if( pGameStage_Inspector != NULL ) 
			{
				pGameStage_Inspector->SetGameObject(pGameObject);
			}

			CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
			CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
		}
	}
}

void		CGameStage_Hierarchy::OnRButtonDown_Picking(const EventArg_PickingEntity& Arg)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(Arg.pPickingEntity->GetParameta());

	if( pGameObject != NULL )
	{
		m_pRButtonDownGameObject = pGameObject;
		m_fRButtonDownProcessTime = CFrameWorks::GetProcessTime();
	}
}

void		CGameStage_Hierarchy::OnRButtonUp_Picking(const EventArg_PickingEntity& Arg)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(Arg.pPickingEntity->GetParameta());

	float fProcessTime = CFrameWorks::GetProcessTime();

	if( pGameObject != NULL && pGameObject == m_pRButtonDownGameObject && pGameObject == m_pSelGameObject && fProcessTime - m_fRButtonDownProcessTime < 0.15f )
	{
		CUITreeViewItem * pTreeViewItem = m_pTreeView->FindTreeViewItem_byParameta(pGameObject);

		POINT Point = *Gw::GetKeyMapper()->GetCursorPos();
		m_pPopupMenu->ShowPopup(GwVector2(Point.x, Point.y));
	}
}

void		CGameStage_Hierarchy::OnLButtonDown_NonePicking(const EventArg_PickingEntity& Arg)
{
	m_pLButtonDownGameObject = NULL;
	m_pRButtonDownGameObject = NULL;
	m_pSelGameObject = NULL;

	m_pTreeView->SetSelectedItem(NULL);

	CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

	if( pGameStage_Inspector != NULL ) 
	{
		pGameStage_Inspector->SetGameObject(NULL);
	}

	CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(NULL);
	CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(NULL);
}

void		CGameStage_Hierarchy::SetDelegate(CUITreeViewItem * pTreeViewItem)
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

	pDelegate_TreeView[eDelegate_TreeView_Click]->Bind<CGameStage_Hierarchy, CUITreeViewItem*, &CGameStage_Hierarchy::OnTreeViewItemClick>(this);
	pDelegate_TreeView[eDelegate_TreeView_DBClick]->Bind<CGameStage_Hierarchy, CUITreeViewItem*, &CGameStage_Hierarchy::OnTreeViewItemDBClick>(this);
	pDelegate_TreeView[eDelegate_TreeView_ChangeItemName]->Bind<CGameStage_Hierarchy, CUITreeViewItem*, const char *, &CGameStage_Hierarchy::OnTreeViewItemChangeItemName>(this);

	pDelegate_Drag[eDelegate_Drag_Send]->Bind<CGameStage_Hierarchy, CUIDragItemBase*, CUIDragItemBase*, eDragObjectReceiver, &CGameStage_Hierarchy::OnDragSend>(this);
	pDelegate_Drag[eDelegate_Drag_Receive]->Bind<CGameStage_Hierarchy, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CGameStage_Hierarchy::OnDragReceive>(this);
}

void		CGameStage_Hierarchy::OnTreeViewItemEmptyClick(CUITreeView * pTreeView)
{
	m_pTreeView->SetSelectedItem(NULL);

	m_pSelGameObject = NULL;

	CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

	if( pGameStage_Inspector != NULL ) 
	{
		pGameStage_Inspector->SetGameObject(NULL);
	}

	CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(NULL);
	CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(NULL);
}

void		CGameStage_Hierarchy::OnTreeViewItemClick(CUITreeViewItem * pTreeViewItem)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(pTreeViewItem->GetParameta());

	if( pGameObject != NULL )
	{
		m_pSelGameObject = pGameObject;

		CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

		if( pGameStage_Inspector != NULL ) 
		{
			pGameStage_Inspector->SetGameObject(pGameObject);
		}

		CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
		CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
	}
}

void		CGameStage_Hierarchy::OnTreeViewItemDBClick(CUITreeViewItem * pTreeViewItem)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(pTreeViewItem->GetParameta());

	if( pGameObject != NULL )
	{
		CPerspectiveCamera::GetInstance()->SetTargetPos(pGameObject->GetWorldPos());

		m_pSelGameObject = pGameObject;

		CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

		if( pGameStage_Inspector != NULL ) 
		{
			pGameStage_Inspector->SetGameObject(pGameObject);
		}

		CPickingObjectWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
		CPickingMeshColliderWireFrameRender::GetInstance()->SetSelGameObject(pGameObject);
	}
}

void		CGameStage_Hierarchy::OnTreeViewItemChangeItemName(CUITreeViewItem * pTreeViewItem, const char * pszItemName)
{
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(pTreeViewItem->GetParameta());
	if( pGameObject != NULL )
	{
		pGameObject->SetName(pszItemName);
	}
}

void		CGameStage_Hierarchy::OnDragSend(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectReceiver eReceiver)
{
}

void		CGameStage_Hierarchy::OnDragReceive(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender)
{
	switch(eSender)
	{
	case eDragObjectSender_Project_File:
		{
			switch(pSrcDragItem->GetDragObjectType())
			{
			case eDragObject_ListViewItem:
				{
					CUIListViewItem * pSrcListViewItem = static_cast<CUIListViewItem*>(pSrcDragItem);
					CUITreeViewItem * pDestTreeViewItem = static_cast<CUITreeViewItem*>(pDestDragItem);

					if( pSrcListViewItem != NULL )
					{
						CParameta_String * pParameta = dynamic_cast<CParameta_String*>(pSrcListViewItem->GetParameta());
						string strFolder = pParameta->GetParameta_String();
						strFolder += "\\";
						string strFileName = pSrcListViewItem->GetName();

						string strOrigToken = CGwStrTok::GetTokenFormat();
						CGwStrTok::SetTokenFormat(".");
						CGwStrTok::SetString(strFileName.c_str());
						CGwStrTok::SetTokenFormat(strOrigToken.c_str());

						int nTokenNum = CGwStrTok::GetNumToken();
						string strExtent = CGwStrTok::GetToken(nTokenNum-1);

						if( strExtent == "gb" || strExtent == "Gb" || strExtent == "gB" || strExtent == "GB" )
						{
							string strItemName = "";
							for( int i = 0; i < nTokenNum - 1; ++i )
							{
								strItemName += CGwStrTok::GetToken(i);
								if( i < nTokenNum - 2 )
								{
									strItemName += ".";
								}
							}

							vector<ITextureFileInfo> MeshTextureInfoList;
							IGwStaticObject * pStaticObject = IResourceHelper::CreateStaticObject_AutoTexture(strFolder.c_str(), strFileName.c_str(), MeshTextureInfoList);

							if( pStaticObject != NULL && pStaticObject->IsIncludingMesh() == TRUE ) // Mesh 만 있다면 애니를 적용할수 있다 판단한다. ( 텍스쳐 애니가 있는 파일은 판별할수 없기 때문 )
							{
								IFileInfo kFileInfo;
								kFileInfo.m_strPath = strFolder;
								kFileInfo.m_strFileName = strFileName;

								IType * pType = ITypeManager::GetInstance()->FindType("ISkinMesh");
								ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pType->CreateInstance());
								pSkinMesh->SetName(strItemName.c_str());
								pSkinMesh->SetMeshObject(pStaticObject, kFileInfo, MeshTextureInfoList);	pStaticObject->Release();
							
								//ISkinMesh * pSkinMesh = new ISkinMesh(strItemName.c_str(), pStaticObject, kFileInfo, MeshTextureInfoList);	
								//pStaticObject->Release();

								kFileInfo = IResourceHelper::FindBoneFileInfo(strFolder.c_str(), strFileName.c_str());

								if( kFileInfo.m_strFileName != "" && kFileInfo.m_strPath  != "" ) 
								{
									pStaticObject = IResourceHelper::CreateStaticObject_AutoTexture(kFileInfo.m_strPath.c_str(), kFileInfo.m_strFileName.c_str(), MeshTextureInfoList);

									if( pStaticObject != NULL )
									{
										pSkinMesh->SetBoneObject(pStaticObject, kFileInfo);
										pStaticObject->Release();
									}
								}

								if( pDestTreeViewItem == NULL )
								{
									AddGameObject(NULL, pSkinMesh);
								}
								else
								{
									IGameObject * pParentObject = dynamic_cast<IGameObject*>(pDestTreeViewItem->GetParameta());
									if( pParentObject != NULL )
									{
										AddGameObject(pParentObject, pSkinMesh);
									}
									else
									{
										AddGameObject(NULL, pSkinMesh);
									}
								}

								CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

								if( pGameStage_Inspector != NULL ) 
								{
									pGameStage_Inspector->SetGameObject(pSkinMesh);
								}

								pSkinMesh->Release();
							}
							else 
							{
								pStaticObject->Release();
							}
						}
						else if( strExtent == "es" )
						{
							IGameObject * pGameObject = IGameObjectMgr::GetInstance()->LoadFile(strFolder.c_str(), strFileName.c_str());
							IGameObject * pParentObject = NULL;
							if( pDestTreeViewItem != NULL )
								pParentObject = dynamic_cast<IGameObject*>(pDestTreeViewItem->GetParameta());

							if( pParentObject != NULL )	pParentObject->AddChild(pGameObject);
							else						IGameObjectMgr::GetInstance()->AddHierarchyNode(pGameObject);

							AddGameObject_ForEsFile(pParentObject, pGameObject);

							CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

							if( pGameStage_Inspector != NULL ) 
							{
								pGameStage_Inspector->SetGameObject(pGameObject);
							}

							pGameObject->Release();
						}
					}
				}
				break;
			}
		}
		break;

	case eDragObjectSender_Hierarchy:
		{
			CUITreeViewItem * pSrcTreeViewItem = static_cast<CUITreeViewItem*>(pSrcDragItem);
			CUITreeViewItem * pDestTreeViewItem = static_cast<CUITreeViewItem*>(pDestDragItem);

			if( pSrcTreeViewItem != NULL )
			{
				IGameObject * pGameObject = dynamic_cast<IGameObject*>(pSrcTreeViewItem->GetParameta());
				IGameObject * pParentObject = NULL;
				
				if( pDestTreeViewItem != NULL )
				{
					pParentObject = dynamic_cast<IGameObject*>(pDestTreeViewItem->GetParameta());

					if( pParentObject == NULL )
						pParentObject = IGameObjectMgr::GetInstance()->GetHierarchyNode();

					if( pParentObject == pGameObject->GetParent() )
						return;
				}
				else 
				{
					if( IGameObjectMgr::GetInstance()->GetHierarchyNode() == pGameObject->GetParent() )
						return;

					pParentObject = IGameObjectMgr::GetInstance()->GetHierarchyNode();
				}

				if( pGameObject == pParentObject || pGameObject->IsChildAllNode(pParentObject) == true )
					return;
				
				MoveGameObject(pParentObject, pGameObject);

				CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

				if( pGameStage_Inspector != NULL ) 
				{
					pGameStage_Inspector->SetGameObject(pGameObject);
				}
			}
		}
		break;

	default:
		{
		}
	}
}