#include "StdAfx.h"
#include "GameStage_Menu.h"

I_IMPLEMENT_DYNCREATE(CGameStage_Menu, ISingletonObject::GetTypeModule())

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMenuItem

class	CGameStage_Menu::CMenuItem : public INode
{
public:
	CMenuItem(CGameStage_Menu * pGameStage, IGwUIDialog * pDialog, CTable_Menu_Item * pTable_Menu_Item, GwVector2 vPos, bool IsRoots = false) 
	: m_pGameStage(pGameStage), m_pDialog(pDialog), m_pTable_Menu_Item(pTable_Menu_Item), m_vPos(vPos), m_IsRoots(IsRoots), m_IsOpen(false)
	{
		m_pBaseControl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
		m_pBaseControl->SetLocalPos(vPos);
	}

	~CMenuItem()
	{
		m_pBaseControl->Release();

		string strObserverName = "CMenuItem";
		strObserverName += m_pTable_Menu_Item->GetName();

		if( CNotificationCenter::GetInstance() != NULL ) 
		{
			CNotificationCenter::GetInstance()->RemoveObserver(NotificationName_Menu_AllClose, strObserverName.c_str());
		}
	}

	float	Initilaize()
	{
		float fRes = 0;
		if( m_IsRoots == true )
		{
			IGwUIButton * pButton = CUIHelper::CreateUIButton("menu_root_bt_normal.png", "menu_root_bt_over.png", "menu_root_bt_normal.png", "menu_root_bt_click.png", "menu_root_bt_normal.png");
			IGwUIStatic * pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(0,0,0,1), true, 0, GwColor(0,0,0,0));

			WCHAR	wszName[STRING_MAX] = L"";
			ConvertAnsiStringToWideCch(wszName, m_pTable_Menu_Item->GetName().c_str(), STRING_MAX);
			pStatic->SetText(wszName);
			GwVector2 vSize = *pStatic->GetCollisSize();
			vSize.x += 12;
			vSize.y += 6;
			pButton->SetSize_NormalState(vSize);
			pStatic->SetLocalPos(GwVector2(6,3));
			pStatic->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
			pButton->AddControl(pStatic); pStatic->Release();
			m_pBaseControl->AddControl(pButton); pButton->Release();
			m_pDialog->AddControl(m_pBaseControl);

			CBtnCallback_Menu * pCallback = new CBtnCallback_Menu(this);
			pButton->SetCallback(pCallback); pCallback->Release();

			fRes = vSize.x;
		}
		else 
		{
			IGwUIButton * pButton = CUIHelper::CreateUIButton("menu_drop_bt_normal.png", "menu_drop_bt_over.png", "menu_drop_bt_normal.png", "menu_drop_bt_click.png", "menu_drop_bt_normal.png");
			IGwUIStatic * pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(0,0,0,1), true, 0, GwColor(0,0,0,0));

			WCHAR	wszName[STRING_MAX] = L"";
			ConvertAnsiStringToWideCch(wszName, m_pTable_Menu_Item->GetName().c_str(), STRING_MAX);
			pStatic->SetText(wszName);
			pStatic->SetLocalPos(GwVector2(6,3));
			pStatic->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
			pButton->AddControl(pStatic); pStatic->Release();
			m_pBaseControl->AddControl(pButton); pButton->Release();

			if( m_pTable_Menu_Item->GetNumChild() != 0 )
			{
				IGwUISprite * pSprite = CUIHelper::CreateUISprite("menu_drop_child_arrow.png");
				pSprite->SetLocalPos(GwVector2(216, 2));
				pSprite->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
				m_pBaseControl->AddControl(pSprite); pSprite->Release();
			}

			const char *  strShortkeyDesc = m_pTable_Menu_Item->GetShortKeyDesc();
			WCHAR	wszDesc[STRING_MAX] = L"";
			ConvertAnsiStringToWideCch(wszDesc, strShortkeyDesc, STRING_MAX);
			pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(0,0,0,1), true, 0, GwColor(0,0,0,0));
			pStatic->SetText(wszDesc);
			pStatic->SetLocalPos(GwVector2(165,3));
			pStatic->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
			pButton->AddControl(pStatic); pStatic->Release();


			CBtnCallback_Menu * pCallback = new CBtnCallback_Menu(this);
			pButton->SetCallback(pCallback); pCallback->Release();
		}

		GwVector2 vChildPos;
		if( m_IsRoots == true ) vChildPos = GwVector2(m_vPos.x, m_vPos.y + 22);
		else					vChildPos = GwVector2(m_vPos.x + 230, m_vPos.y);

		int nNumChild = m_pTable_Menu_Item->GetNumChild();		

		for( int i = 0; i < nNumChild; ++i )
		{
			CTable_Menu_Item * pTable_Menu_Item = dynamic_cast<CTable_Menu_Item*>(m_pTable_Menu_Item->GetChild_byIndex(i));

			if( pTable_Menu_Item != NULL ) 
			{
				CMenuItem * pMenuItem = new CMenuItem(m_pGameStage, m_pDialog, pTable_Menu_Item, vChildPos);
				pMenuItem->Initilaize();
				AddChild(pMenuItem); pMenuItem->Release();

				vChildPos.y += 18;
			}
		}

		string strObserverName = "CMenuItem";
		strObserverName += m_pTable_Menu_Item->GetName();
		CNotificationCenter::GetInstance()->AddObserver(NotificationName_Menu_AllClose, strObserverName.c_str(), this, &CMenuItem::OnNotification_Detach);

		return fRes;
	}

	void		Attach()
	{
		m_pDialog->AddControl(m_pBaseControl);
	}

	void		Detach()
	{
		m_pDialog->RemoveControl(m_pBaseControl);
		m_IsOpen = false;
	}

	void		OnNotification_Detach(NotificationArg* p)
	{
		if( m_IsRoots == false )
		{
			m_pDialog->RemoveControl(m_pBaseControl);
		}
		m_IsOpen = false;
	}

	void		OnButtonClick()
	{
		int nNumChild = m_pTable_Menu_Item->GetNumChild();
		if( nNumChild == 0 )
		{
			m_pGameStage->OnButtonClick(IHashObject::GetInstance()->GetHashCode(m_pTable_Menu_Item->GetFunction()));
			CNotificationCenter::GetInstance()->PostNotification(NotificationName_Menu_AllClose, NULL);
		}
		else 
		{
			if( m_IsRoots == true && m_IsOpen == false)
			{
				CNotificationCenter::GetInstance()->PostNotification(NotificationName_Menu_AllClose, NULL);
			}

			for( int i = 0; i < nNumChild; ++i )
			{
				CMenuItem * pMenuItem = dynamic_cast<CMenuItem*>(GetChild_byIndex(i));

				if( pMenuItem != NULL ) 
				{
					if( m_IsOpen == true )		pMenuItem->Detach();
					else						pMenuItem->Attach();
				}
			}

			m_IsOpen = !m_IsOpen;
		}
	}

private:

	CGameStage_Menu		*	m_pGameStage;

	IGwUIDialog			*	m_pDialog;
	IGwUIDummyCtrl		*	m_pBaseControl;
	bool					m_IsRoots;
	GwVector2				m_vPos;
	bool					m_IsOpen;

	CTable_Menu_Item	*	m_pTable_Menu_Item;

	class	CBtnCallback_Menu : public IGwUIButton_Callback
	{
	public:
		CBtnCallback_Menu(CMenuItem * pMenuItem)
		: m_pMenuItem(pMenuItem)	
		{
		}
		virtual void	OnButton_OK(class IGwUIButton* pControl) 			
		{
			m_pMenuItem->OnButtonClick(); 
		}
	private:
		CMenuItem	*	m_pMenuItem;
	};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CGameStage_Menu

CGameStage_Menu::CGameStage_Menu(void) : m_bCtrlKeyDown(false)
{
	IHashObject * pHashObject = IHashObject::GetInstance();
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_New"),				&CGameStage_Menu::OnButtonClick_New));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Open"),			&CGameStage_Menu::OnButtonClick_Open));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Save"),			&CGameStage_Menu::OnButtonClick_Save));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Exit"),			&CGameStage_Menu::OnButtonClick_Exit));

	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_EmptyGameObject"),	&CGameStage_Menu::OnButtonClick_EmptyGameObject));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_ParticleObject"),	&CGameStage_Menu::OnButtonClick_ParticleObject));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_PathObject"),		&CGameStage_Menu::OnButtonClick_PathObject));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_CentripetalForce"),&CGameStage_Menu::OnButtonClick_CentripetalForce));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_CentrifugalForce"),&CGameStage_Menu::OnButtonClick_CentrifugalForce));

	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Hierarchy"),		&CGameStage_Menu::OnButtonClick_Hierarchy));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Project"),			&CGameStage_Menu::OnButtonClick_Project));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Inspector"),		&CGameStage_Menu::OnButtonClick_Inspector));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_TimeLine"),		&CGameStage_Menu::OnButtonClick_TimeLine));

	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_FrameRender"),				&CGameStage_Menu::OnButtonClick_FrameRender));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_Collider_FrameRender"),	&CGameStage_Menu::OnButtonClick_Collider_FrameRender));
	m_MenuClassProcTable.insert(map<ULONG,ClassProc>::value_type(pHashObject->GetHashCode("OnButtonClick_PathDotRender"),			&CGameStage_Menu::OnButtonClick_PathDotRender));
}

CGameStage_Menu::~CGameStage_Menu(void)
{
}

void		CGameStage_Menu::OnCreateDevice()
{
	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();
	Gw::GetUIControlMgr()->AddFixedDialog(m_pDialog);

	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Menu\\");
	
	IGwUISprite * pSprite_Back = CUIHelper::CreateUISprite("back.png");
	m_pDialog->AddControl(pSprite_Back); pSprite_Back->Release();

	CreateMenu();
	Gw::GetBillboardMgr()->EndTextureMerge();
}

void		CGameStage_Menu::OnDestroyDevice()
{
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();

	vector<CMenuItem*>::iterator itr = m_MenuItemList.begin();

	for( ; itr != m_MenuItemList.end(); ++itr )
	{
		CMenuItem * pMenuItem = (*itr);
		if( pMenuItem != NULL ) 
			pMenuItem->Release();
	}
	m_MenuItemList.clear();
}

void		CGameStage_Menu::Update(float fDeltaTime)
{
	if(Gw::GetKeyMapper()->IsKeyDown(VK_CONTROL) == TRUE)
	{
		m_bCtrlKeyDown = true;
	}
	else if(Gw::GetKeyMapper()->IsKeyUp(VK_CONTROL) == TRUE)
	{
		m_bCtrlKeyDown = false;
	}

	for( int i = VK_F1; i < VK_F24; ++i )
	{
		if(Gw::GetKeyMapper()->WasKeyDown(i) == TRUE)
		{
			char szTemp[16] = { 0, };
			sprintf(szTemp, "F%d", i - VK_F1 + 1);

			CTable_Menu * pTable_Menu = CTableMgr::GetInstance()->GetTable_Menu();
			INode * pRoot = pTable_Menu->GetRoot();

			RecursiveShortKeyCheck_FSysKey(pRoot, szTemp);
		}
	}

	char sInputChar = ' ';
	char sInputChar02 = ' ';
	for( int i = 65; i <= 90; ++i )
	{
		if(Gw::GetKeyMapper()->WasKeyDown(i) == TRUE || Gw::GetKeyMapper()->WasKeyDown(i+32) == TRUE)
		{
			sInputChar = i;
		}
	}

	if( sInputChar != ' ' )
	{
		CTable_Menu * pTable_Menu = CTableMgr::GetInstance()->GetTable_Menu();
		INode * pRoot = pTable_Menu->GetRoot();

		RecursiveShortKeyCheck(pRoot, sInputChar);
	}

	if(Gw::GetKeyMapper()->WasKeyDown(VK_LBUTTON) == TRUE)
	{
		if(Gw::GetUIControlMgr()->GetFocusDialog() != m_pDialog)
		{
			CNotificationCenter::GetInstance()->PostNotification(NotificationName_Menu_AllClose, NULL);
		}
	}
}

void		CGameStage_Menu::OnVisibleChanged()
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

void		CGameStage_Menu::RecursiveShortKeyCheck(INode * pNode, char sChar)
{
	int nNumChild = pNode->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CTable_Menu_Item * pItem = dynamic_cast<CTable_Menu_Item*>(pNode->GetChild_byIndex(i));

		if( strcmp(pItem->GetSyskey(), "Ctrl") == 0 && m_bCtrlKeyDown == true )
		{
			if( sChar == pItem->GetChar() || sChar+32 == pItem->GetChar() )
			{
				OnButtonClick(IHashObject::GetInstance()->GetHashCode(pItem->GetFunction()));
			}
		}

		RecursiveShortKeyCheck(pItem,sChar);
	}
}

void		CGameStage_Menu::RecursiveShortKeyCheck_FSysKey(INode * pNode, const char * pszFSysKey)
{
	int nNumChild = pNode->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CTable_Menu_Item * pItem = dynamic_cast<CTable_Menu_Item*>(pNode->GetChild_byIndex(i));

		if( strcmp(pItem->GetSyskey(), pszFSysKey) == 0 )
		{
			OnButtonClick(IHashObject::GetInstance()->GetHashCode(pItem->GetFunction()));
		}

		RecursiveShortKeyCheck_FSysKey(pItem,pszFSysKey);
	}
}

void		CGameStage_Menu::Render(float fDeltaTime)
{

}

void		CGameStage_Menu::CreateMenu()
{
	GwVector2 vPos = GwVector2(10, 2);

	CTable_Menu * pTable_Menu = CTableMgr::GetInstance()->GetTable_Menu();
	INode * pRoot = pTable_Menu->GetRoot();

	int nNumChild = pRoot->GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		CTable_Menu_Item * pItem = dynamic_cast<CTable_Menu_Item*>(pRoot->GetChild_byIndex(i));

		if( pItem != NULL ) 
		{
			CMenuItem * pMenuItem = new CMenuItem(this, m_pDialog, pItem, GwVector2(vPos.x, 2), true);
			vPos.x += pMenuItem->Initilaize();
			m_MenuItemList.push_back(pMenuItem);
		}
	}
}

void		CGameStage_Menu::OnButtonClick(ULONG uHashCode)
{
	map<ULONG,ClassProc>::iterator itr = m_MenuClassProcTable.find(uHashCode);
	
	if( itr != m_MenuClassProcTable.end() )
	{
		ClassProc classProc = itr->second;
		(this->*classProc)();
	}
}

void		CGameStage_Menu::OnButtonClick_New()
{
}

void		CGameStage_Menu::OnButtonClick_Open()
{
}

void		CGameStage_Menu::OnButtonClick_Save()
{
	IGameObjectMgr::GetInstance()->SaveHierarchy();
}

void		CGameStage_Menu::OnButtonClick_Exit()
{
}

void		CGameStage_Menu::OnButtonClick_EmptyGameObject()
{
	IType * pType = ITypeManager::GetInstance()->FindType("IGameObject");
	IGameObject * pGameObject = dynamic_cast<IGameObject*>(pType->CreateInstance());
	pGameObject->SetName("GameObject");

	IGameObjectMgr::GetInstance()->AddHierarchyNode(pGameObject);	pGameObject->Release();

	CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");

	if( pGameStage_Hierarchy != NULL ) 
	{
		pGameStage_Hierarchy->AddGameObject(NULL, pGameObject);
	}

	CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

	if( pGameStage_Inspector != NULL ) 
	{
		pGameStage_Inspector->SetGameObject(pGameObject);
	}
}

void		CGameStage_Menu::OnButtonClick_Character()
{
}

void		CGameStage_Menu::OnButtonClick_ParticleObject()
{
	IGwCamera * pCamera = CPerspectiveCamera::GetInstance()->GetCamera();
	IType * pType = ITypeManager::GetInstance()->FindType("IParticleObject");
	IParticleObject * pParticleObject = dynamic_cast<IParticleObject*>(pType->CreateInstance());
	pParticleObject->SetName("ParticleObject");
	pParticleObject->SetCamera(pCamera);
	pParticleObject->OnPlay();

	IGameObjectMgr::GetInstance()->AddHierarchyNode(pParticleObject);	pParticleObject->Release();

	CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");

	if( pGameStage_Hierarchy != NULL ) 
	{
		pGameStage_Hierarchy->AddGameObject(NULL, pParticleObject);
	}

	CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

	if( pGameStage_Inspector != NULL ) 
	{
		pGameStage_Inspector->SetGameObject(pParticleObject);
	}

	IComponent_Trans_Alpha * pComponent_Trans_Alpha = pParticleObject->GetComponent<IComponent_Trans_Alpha>();
	if( pComponent_Trans_Alpha != NULL ) 
	{
		ITransAlphaInfo * pEvent = NULL;

		pEvent = new ITransAlphaInfo();
		pEvent->m_fTime = 0;
		pEvent->m_fAlpha = 0;
		pComponent_Trans_Alpha->AddEvent(pEvent);	pEvent->Release();

		pEvent = new ITransAlphaInfo();
		pEvent->m_fTime = 0.2f;
		pEvent->m_fAlpha = 1;
		pComponent_Trans_Alpha->AddEvent(pEvent);	pEvent->Release();

		pEvent = new ITransAlphaInfo();
		pEvent->m_fTime = 0.5f;
		pEvent->m_fAlpha = 1;
		pComponent_Trans_Alpha->AddEvent(pEvent);	pEvent->Release();

		pEvent = new ITransAlphaInfo();
		pEvent->m_fTime = 0.7f;
		pEvent->m_fAlpha = 0;
		pComponent_Trans_Alpha->AddEvent(pEvent);	pEvent->Release();
	}
}

void		CGameStage_Menu::OnButtonClick_PathObject()
{
	IType * pType = ITypeManager::GetInstance()->FindType("IPathObject");
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(pType->CreateInstance());
	pPathObject->SetName("PathObject");

	pPathObject->AddEvent(0, GwVector(0,300,0), false);
	pPathObject->AddEvent(0, GwVector(100,300,100), true);
	pPathObject->AddEvent(0, GwVector(200,300,-100), true);
	pPathObject->AddEvent(5, GwVector(300,300,0), false);
	pPathObject->ReLoad();

	IGameObjectMgr::GetInstance()->AddHierarchyNode(pPathObject);	pPathObject->Release();

	CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");

	if( pGameStage_Hierarchy != NULL ) 
	{
		pGameStage_Hierarchy->AddGameObject(NULL, pPathObject);
	}

	CGameStage_Inspector * pGameStage_Inspector = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

	if( pGameStage_Inspector != NULL ) 
	{
		pGameStage_Inspector->SetGameObject(pPathObject);
	}

	CPathObjectRenderMgr::GetInstance()->AddPathObject(pPathObject);
}

void		CGameStage_Menu::OnButtonClick_CentripetalForce()
{
}

void		CGameStage_Menu::OnButtonClick_CentrifugalForce()
{
}

void		CGameStage_Menu::OnButtonClick_Hierarchy()
{
	CGameStage_Hierarchy * pGameStage = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");

	if( pGameStage != NULL ) 
	{
		bool IsVisible = pGameStage->IsVisible();
		pGameStage->SetVisible(!IsVisible);
	}
}

void		CGameStage_Menu::OnButtonClick_Project()
{
	CGameStage_Project * pGameStage = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Project>("CGameStage_Project");

	if( pGameStage != NULL ) 
	{
		bool IsVisible = pGameStage->IsVisible();
		pGameStage->SetVisible(!IsVisible);
	}
}

void		CGameStage_Menu::OnButtonClick_Inspector()
{
	CGameStage_Inspector * pGameStage = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Inspector>("CGameStage_Inspector");

	if( pGameStage != NULL ) 
	{
		bool IsVisible = pGameStage->IsVisible();
		pGameStage->SetVisible(!IsVisible);
	}
}

void		CGameStage_Menu::OnButtonClick_TimeLine()
{
	CGameStage_TimeLine * pGameStage = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_TimeLine>("CGameStage_TimeLine");

	if( pGameStage != NULL ) 
	{
		bool IsVisible = pGameStage->IsVisible();
		pGameStage->SetVisible(!IsVisible);
	}
}

void		CGameStage_Menu::OnButtonClick_FrameRender()
{
	CPickingObjectWireFrameRender::GetInstance()->OnRender();
}

void		CGameStage_Menu::OnButtonClick_Collider_FrameRender()
{
	CPickingMeshColliderWireFrameRender::GetInstance()->OnRender();
}

void		CGameStage_Menu::OnButtonClick_PathDotRender()
{
	CPathObjectRenderMgr::GetInstance()->OnPathDotRender();
}