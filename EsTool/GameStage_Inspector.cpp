#include "StdAfx.h"
#include "GameStage_Inspector.h"

I_IMPLEMENT_DYNCREATE(CGameStage_Inspector, ISingletonObject::GetTypeModule())

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CScrollCallback_V

class CGameStage_Inspector::CScrollCallback_V : public IGwUISlider_Callback
{
public:
	CScrollCallback_V(CGameStage_Inspector* pInspector) :m_pInspector(pInspector)				{ }

	virtual void	OnChangeValue(class IGwUISliderBase* pMyControl,bool bFromInput)
	{
		m_pInspector->UpdateScroll_V();
	}

private:
	CGameStage_Inspector * m_pInspector;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCheckboxCallback_Active

class CGameStage_Inspector::CCheckboxCallback_Active : public IGwUICheckbox_Callback
{
public:
	CCheckboxCallback_Active(CGameStage_Inspector* pInspector) :m_pInspector(pInspector)				{ }

	virtual void	OnCheckChange_TRUE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pInspector->OnCheckBox_Active(true);
	}
	virtual void	OnCheckChange_FALSE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pInspector->OnCheckBox_Active(false);
	}

private:
	CGameStage_Inspector * m_pInspector;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameStage_Inspector

CGameStage_Inspector::CGameStage_Inspector(void) : m_pGameObject(NULL)
{
	m_ComponentClassLinkTable["IComponent_MeshCollider"] = "CUIComponent_MeshCollider";

	CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_Height_Change, "CGameStage_Inspector", this, &CGameStage_Inspector::OnNotification_InspectorComponent_Height_Change);
	CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_Reflesh, "CGameStage_Inspector", this, &CGameStage_Inspector::OnNotification_InspectorComponent_Reflesh);
}

CGameStage_Inspector::~CGameStage_Inspector(void)
{
	if( CNotificationCenter::GetInstance() != NULL )
	{
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_Height_Change, "CGameStage_Inspector");
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_Reflesh, "CGameStage_Inspector");
	}
}

void		CGameStage_Inspector::OnCreateDevice()
{
	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();
	Gw::GetUIControlMgr()->AddFixedDialog(m_pDialog);
	m_pDialog->SetWorldPos(GwVector(1600,22,0));

	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUISprite * pSprite_Back_Middle = CUIHelper::CreateUISprite("back_middle.png");
	pSprite_Back_Middle->SetLocalPos(GwVector2(4,33));
	m_pDialog->AddControl(pSprite_Back_Middle); pSprite_Back_Middle->Release();

	m_pComponent_DummyCtrl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pComponent_DummyCtrl->SetLocalPos(GwVector2(3,33));
	m_pDialog->AddControl(m_pComponent_DummyCtrl);

	m_pCheckBox_Active = CUIHelper::CreateUICheckBox("checkbox_active_off.png", "checkbox_active_on.png");
	IGwUICheckbox_Callback*	pChkBoxCallback;
	pChkBoxCallback = new CCheckboxCallback_Active(this);
	m_pCheckBox_Active->SetCallback(pChkBoxCallback);	pChkBoxCallback->Release();
	m_pCheckBox_Active->SetLocalPos(GwVector2(5, 4));
	m_pComponent_DummyCtrl->AddControl(m_pCheckBox_Active);
	m_pCheckBox_Active->SetHidden();

	m_pScroll_V = CUIHelper::CreateUIScroll_V("track.png", "scroll_bar.png", "scroll.png", "scroll.png");
	m_pScroll_V->SetLocalPos(GwVector2(280,33));

	CScrollCallback_V*	pCallback = new CScrollCallback_V(this);
	m_pScroll_V->SetCallback(pCallback);	pCallback->Release();

	m_pDialog->AddControl(m_pScroll_V);

	IGwUISprite * pSprite_Back = CUIHelper::CreateUISprite("back.png");
	pSprite_Back->SetLocalPos(GwVector2(0,0));
	pSprite_Back->SetDependencyType(GWUISTATEDEPENDENCY_NOACTION);
	m_pDialog->AddControl(pSprite_Back); pSprite_Back->Release();

	Gw::GetBillboardMgr()->EndTextureMerge();
}

void		CGameStage_Inspector::OnResetDevice()
{

}

void		CGameStage_Inspector::OnLostDevice()
{

}

void		CGameStage_Inspector::OnDestroyDevice()
{
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();
	m_pScroll_V->Release();

	if( m_pGameObject != NULL )
		m_pGameObject->Release();

	RemoveComponentAll();
	m_pComponent_DummyCtrl->Release();
	m_pCheckBox_Active->Release();
}

void		CGameStage_Inspector::Update(float fDeltaTime)
{
	vector<CUIComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		CUIComponent * pComponent = (*itr);

		if( pComponent != NULL )
		{
			pComponent->Update(fDeltaTime);
		}
	}
}

void		CGameStage_Inspector::Render(float fDeltaTime)
{
	vector<CUIComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		CUIComponent * pComponent = (*itr);

		if( pComponent != NULL )
		{
			pComponent->Render(fDeltaTime);
		}
	}
}

void		CGameStage_Inspector::SetGameObject(IGameObject * pGameObject)
{
	//return;

	RemoveComponentAll();

	m_pCheckBox_Active->SetHidden();

	if( m_pGameObject != NULL ) m_pGameObject->Release();
	m_pGameObject = pGameObject;

	if( m_pGameObject != NULL ) 
	{
		m_pGameObject->AddRef();

		m_pCheckBox_Active->SetVisible();
		m_pCheckBox_Active->SetCheck(m_pGameObject->IsVisible());

		CUIComponent_GameObject_Tag * pComponent_GameObject_Tag = new CUIComponent_GameObject_Tag();
		pComponent_GameObject_Tag->SetGameObject(m_pGameObject);
		pComponent_GameObject_Tag->SetLocalPos(GwVector2(140, 4));
		m_ComponentList.push_back(pComponent_GameObject_Tag);

		GwVector2 vLocalPos = GwVector2(5,30);

		CUIComponent_Transform * pComponent_Transform = new CUIComponent_Transform();
		pComponent_Transform->SetGameObject(m_pGameObject);
		pComponent_Transform->SetLocalPos(vLocalPos);
		vLocalPos.y += pComponent_Transform->GetHeight();

		m_ComponentList.push_back(pComponent_Transform);

		string strClassName = IHashObject::GetInstance()->GetHashString(pGameObject->GetClassHashCode());
		if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
		{
			CUIComponent_SkinMeshObject * pComponent_SkingMeshObject = new CUIComponent_SkinMeshObject();
			pComponent_SkingMeshObject->SetGameObject(m_pGameObject);
			pComponent_SkingMeshObject->SetLocalPos(vLocalPos);
			vLocalPos.y += pComponent_SkingMeshObject->GetHeight();

			m_ComponentList.push_back(pComponent_SkingMeshObject);
		}
		else if( strcmp(strClassName.c_str(), "IPathObject") == 0 )
		{
			CUIComponent_PathObject * pComponent_PathObject = new CUIComponent_PathObject();
			pComponent_PathObject->SetGameObject(m_pGameObject);
			pComponent_PathObject->SetLocalPos(vLocalPos);
			vLocalPos.y += pComponent_PathObject->GetHeight();

			m_ComponentList.push_back(pComponent_PathObject);
		}

		int nNumComponent = m_pGameObject->GetNumComponent();

		for( int i = 0; i < nNumComponent; ++i )
		{
			IComponent * pComponent = m_pGameObject->GetComponent_byIndex(i);

			if( pComponent != NULL )
			{
				string strClassName = m_ComponentClassLinkTable[IHashObject::GetInstance()->GetHashString(pComponent->GetClassHashCode())];
				IType * pType = ITypeManager::GetInstance()->FindType(strClassName.c_str());

				if( pType != NULL )
				{
					CUIComponent * pUIComponent = dynamic_cast<CUIComponent*>(pType->CreateInstance());
					pUIComponent->SetComponent(pComponent);
					pUIComponent->SetGameObject(m_pGameObject);
					pUIComponent->SetLocalPos(vLocalPos);
					vLocalPos.y += pUIComponent->GetHeight();

					m_ComponentList.push_back(pUIComponent);
				}
			}
		}

		CUIComponent_AddComponent * pComponent_AddComponent = new CUIComponent_AddComponent();
		pComponent_AddComponent->SetGameObject(m_pGameObject);
		pComponent_AddComponent->SetLocalPos(vLocalPos);
		vLocalPos.y += pComponent_AddComponent->GetHeight();

		m_ComponentList.push_back(pComponent_AddComponent);

		// ÄÞÆ÷³ÍÆ®´Â ²¨²Ù·Î Add ½ÃÅ²´Ù.
		vector<CUIComponent*>::reverse_iterator itr = m_ComponentList.rbegin();
		for( ; itr != m_ComponentList.rend(); ++itr )
		{
			m_pComponent_DummyCtrl->AddControl((*itr)->GetBaseControl());
		}
	}
}

void		CGameStage_Inspector::UpdateScroll_V()
{
	float fTotalHeight = 30;
	vector<CUIComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		CUIComponent * pComponent = (*itr);

		if( pComponent != NULL )
		{
			fTotalHeight += pComponent->GetHeight();
		}
	}

	int nFirst = CUIHelper::CalcFirstItemIndexScroll_V(m_pScroll_V, fTotalHeight, 5, 978);

	m_pComponent_DummyCtrl->SetLocalPos(GwVector2(3,33 - nFirst));
}

void		CGameStage_Inspector::OnCheckBox_Active(bool IsChecked)
{
	if( m_pGameObject != NULL )
	{
		m_pGameObject->SetVisible(IsChecked);
	}

	if( IsChecked == true )
	{
		string strClassName = IHashObject::GetInstance()->GetHashString(m_pGameObject->GetClassHashCode());

		if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
		{
			ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

			if( pSkinMesh != NULL )
			{
				IGwScnObject * pScnObject = pSkinMesh->GetScnObject();
				CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");

				if( pScnObject != NULL && pGameStage_Hierarchy != NULL ) 
				{
					CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(pScnObject, m_pGameObject);
					pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CGameStage_Hierarchy", pGameStage_Hierarchy, &CGameStage_Hierarchy::OnLButtonDown_Picking);
					pPickingEntity->GetEvent_LButtonUp()->AddEventHandler("CGameStage_Hierarchy", pGameStage_Hierarchy, &CGameStage_Hierarchy::OnLButtonUp_Picking);

					pPickingEntity->GetEvent_RButtonDown()->AddEventHandler("CGameStage_Hierarchy", pGameStage_Hierarchy, &CGameStage_Hierarchy::OnRButtonDown_Picking);
					pPickingEntity->GetEvent_RButtonUp()->AddEventHandler("CGameStage_Hierarchy", pGameStage_Hierarchy, &CGameStage_Hierarchy::OnRButtonUp_Picking);
				}
			}
		}
		else if( strcmp(strClassName.c_str(), "IPathObject") == 0 )
		{
			IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

			if( pPathObject != NULL )
			{
				CPathObjectRenderMgr::GetInstance()->AddPathObject(pPathObject);
			}
		}
	}
	else 
	{
		string strClassName = IHashObject::GetInstance()->GetHashString(m_pGameObject->GetClassHashCode());

		if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
		{
			ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pGameObject);

			if( pSkinMesh != NULL )
			{
				IGwScnObject * pScnObject = pSkinMesh->GetScnObject();

				if( pScnObject != NULL ) 
				{
					CPickingMgr::GetInstance()->RemovePickingEntity(pScnObject);
				}
			}
		}
		else if( strcmp(strClassName.c_str(), "IPathObject") == 0 )
		{
			IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

			if( pPathObject != NULL )
			{
				CPathObjectRenderMgr::GetInstance()->RemovePathObject(pPathObject);
				CGizmoObject::GetInstance()->Hide();
			}
		}
	}
}

void		CGameStage_Inspector::OnNotification_InspectorComponent_Height_Change(NotificationArg * p)
{
	GwVector2 vLocalPos = GwVector2(5,30);

	vector<CUIComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		CUIComponent * pComponent = (*itr);

		if( pComponent != NULL )
		{
			pComponent->SetLocalPos(vLocalPos);
			vLocalPos.y += pComponent->GetHeight();
		}
	}
}

void		CGameStage_Inspector::OnNotification_InspectorComponent_Reflesh(NotificationArg * p)
{
	SetGameObject(m_pGameObject);
}

void		CGameStage_Inspector::RemoveComponentAll()
{
	vector<CUIComponent*>::iterator itr = m_ComponentList.begin();

	for( ; itr != m_ComponentList.end(); ++itr )
	{
		CUIComponent * pComponent = (*itr);

		if( pComponent != NULL ) 
		{
			m_pComponent_DummyCtrl->RemoveControl(pComponent->GetBaseControl());

			pComponent->Release();
		}
	}

	m_ComponentList.clear();
}

void		CGameStage_Inspector::OnVisibleChanged()
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