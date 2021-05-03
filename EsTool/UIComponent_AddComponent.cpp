#include "StdAfx.h"
#include "UIComponent_MeshCollider.h"

I_IMPLEMENT_DYNCREATE(CUIComponent_AddComponent, ISingletonObject::GetTypeModule())

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CBtnCallback_AddComponent

class	CUIComponent_AddComponent::CBtnCallback_AddComponent : public IGwUIButton_Callback
{
public:
	CBtnCallback_AddComponent(CUIComponent_AddComponent * pThis)
	: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_AddComponent(); 
	}
private:
	CUIComponent_AddComponent		*	m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIComponent_MeshCollider

CUIComponent_AddComponent::CUIComponent_AddComponent() : m_IsOpen(false)
{
	m_pComponentListBaseDummy = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
	m_pComponentListBaseDummy->SetLocalPos(GwVector2(23,22));
}

CUIComponent_AddComponent::~CUIComponent_AddComponent(void)
{
	m_pComponentListBaseDummy->Release();
	m_pListView->Release();
}

void			CUIComponent_AddComponent::Initialize()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUIButton * pButton_AddComponent = CUIHelper::CreateUIButton("add_component.png", "add_component.png", "add_component.png", "add_component.png", "add_component.png");
	pButton_AddComponent->SetLocalPos(GwVector2(23, 0));
	CBtnCallback_AddComponent * pCallback_AddComponent = new CBtnCallback_AddComponent(this);
	pButton_AddComponent->SetCallback(pCallback_AddComponent); pCallback_AddComponent->Release();
	m_pBaseControl->AddControl(pButton_AddComponent); pButton_AddComponent->Release();

	IGwUISprite * pSprite = CUIHelper::CreateUISprite("component_list.png");
	m_pComponentListBaseDummy->AddControl(pSprite); pSprite->Release();

	m_fHeight += pSprite->GetCollisSize()->y + 33;

	m_pListView = new CUIListView(m_pComponentListBaseDummy, GwVector2(0,0), GwVector2(230,265));
	m_pListView->SetAttribute(eDragObjectSender_None, eDragObjectReceiver_None, false, false, GwVector2(5,5));
	m_pListView->SetAttribute_ListViewItem(4, 12, "굴림", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(0,0,0,1), "empty.png", "sel.png", "edit.png");
	m_pListView->CreateSlot();
	m_pListView->SetAttribute_Scroll_V(GwVector2(521,33), "track_addcomponent.png", "scroll_bar.png", "scroll.png", "scroll.png", 1);

	int	nNumSlot = m_pListView->GetNumSlot();

	for( int i = 0; i < nNumSlot; ++i )
	{	
		CUIListViewItem * pListViewItem = m_pListView->GetSlot(i);

		if( pListViewItem != NULL )
			pListViewItem->GetDelegate_ListView(eDelegate_ListView_DBClick)->Bind<CUIComponent_AddComponent, CUIListViewItem*, &CUIComponent_AddComponent::OnDoubleClick_AddComponent>(this);
	}

	int nMaxWidth = 0;
	m_pListView->AddItem("메시 컬리더", nMaxWidth);

	Gw::GetBillboardMgr()->EndTextureMerge();
}

void			CUIComponent_AddComponent::Update(float fDeltaTime)					
{ 
}

void			CUIComponent_AddComponent::OnButtonClick_AddComponent()
{
	m_IsOpen = !m_IsOpen;

	if( m_IsOpen == true )
	{
		if( m_pBaseControl->HasChild(m_pComponentListBaseDummy) == false )
			m_pBaseControl->AddControl(m_pComponentListBaseDummy);
	}
	else
	{
		if( m_pBaseControl->HasChild(m_pComponentListBaseDummy) == true )
			m_pBaseControl->RemoveControl(m_pComponentListBaseDummy);
	}
}

void			CUIComponent_AddComponent::OnDoubleClick_AddComponent(CUIListViewItem * pListViewItem)
{
	if( m_pBaseControl->HasChild(m_pComponentListBaseDummy) == true )
		m_pBaseControl->RemoveControl(m_pComponentListBaseDummy);

	if( pListViewItem != NULL && pListViewItem->GetName() == "메시 컬리더" )
	{
		IComponent_MeshCollider * pMeshCollider = new IComponent_MeshCollider(m_pGameObject);
		m_pGameObject->AddComponent(pMeshCollider); pMeshCollider->Release();

		CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_Reflesh, NULL);
	}
}