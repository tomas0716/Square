#include "StdAfx.h"
#include "UIComponent_SkinMeshObject.h"

I_IMPLEMENT_DYNCREATE(CUIComponent_GameObject_Tag, ISingletonObject::GetTypeModule())

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CItem

class CUIComponent_GameObject_Tag::CItem : public INode
{
public:
	CItem(CUIComponent_GameObject_Tag * pThis, INode * pNode, bool IsChildLeft, int nDepth) 
	: m_pThis(pThis), m_pNode(pNode), m_IsChildOpen(false), m_IsChildLeft(IsChildLeft), m_nDepth(nDepth)
	{
		m_pNode->AddRef();

		Gw::GetBillboardMgr()->BeginTextureMerge();
		Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

		m_pButton = CUIHelper::CreateUIButton("tag_combo_bt_normal.png", "tag_combo_bt_over.png", "tag_combo_bt_normal.png", "tag_combo_bt_click.png", "tag_combo_bt_normal.png");
		CBtnCallback_Button * pCallback_Button = new CBtnCallback_Button(this);
		m_pButton->SetCallback(pCallback_Button); pCallback_Button->Release();

		m_pButton_Child = CUIHelper::CreateUIButton("menu_drop_child_arrow.png", "menu_drop_child_arrow.png", "menu_drop_child_arrow.png", "menu_drop_child_arrow.png", "menu_drop_child_arrow.png");
		CBtnCallback_Button_Child * pCallback_Child = new CBtnCallback_Button_Child(this);
		m_pButton_Child->SetCallback(pCallback_Child); pCallback_Child->Release();

		m_pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(0,0,0,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));

		wchar_t wszFileName[128] = { 0, };
		wstring wstrFileName;
		ConvertAnsiStringToWideCch(wszFileName, m_pNode->GetName().c_str(), 128);
		IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
		CUIHelper::FittingStringExtent(pUIFont, 104, wszFileName, wstrFileName);
		m_pStatic->SetText(wstrFileName.c_str());
		m_pStatic->SetDependencyType(GWUISTATEDEPENDENCY_NOACTION);

		m_pThis->GetBaseControl()->AddControl(m_pButton);

		if( m_pNode->GetNumChild() != 0 )
			m_pThis->GetBaseControl()->AddControl(m_pButton_Child);

		m_pThis->GetBaseControl()->AddControl(m_pStatic);

		Gw::GetBillboardMgr()->EndTextureMerge();

		string strObserverName = "CItem";
		strObserverName += m_pNode->GetName();
		CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_GameObject_Tag_Remove, strObserverName.c_str(), this, &CItem::OnRemove);
	}

	~CItem()
	{
		string strObserverName = "CItem";
		strObserverName += m_pNode->GetName();

		if( CNotificationCenter::GetInstance() != NULL )
		{
			CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_GameObject_Tag_Remove, strObserverName.c_str());
		}

		m_pThis->GetBaseControl()->RemoveControl(m_pButton);
		m_pButton->Release();

		if( m_pNode->GetNumChild() != 0 )
		{
			m_pThis->GetBaseControl()->RemoveControl(m_pButton_Child);
			m_pButton_Child->Release();
		}

		m_pThis->GetBaseControl()->RemoveControl(m_pStatic);
		m_pStatic->Release();

		m_pNode->Release();

		vector<CItem*>::iterator itr = m_ItemList.begin();

		for( ; itr != m_ItemList.end(); ++itr )
		{
			(*itr)->Release();
		}

		m_ItemList.clear();
	}

	void	SetLocalPos(GwVector2 vPos)
	{
		m_pButton->SetLocalPos(vPos);
		m_pButton_Child->SetLocalPos(GwVector2(vPos.x + 4, vPos.y + 3));
		m_pStatic->SetLocalPos(GwVector2(vPos.x + 7 + 12, vPos.y + 4));
	}

	void	SetVisible(bool IsVisible)
	{
		if( IsVisible == true )
		{
			m_pButton->SetVisible();
			m_pButton_Child->SetVisible();
		}
		else 
		{
			m_pButton->SetHidden();
			m_pButton_Child->SetHidden();
		}
	}

	void				OnRemove(NotificationArg * p)
	{
		NotificationArg_InspectorComponent_GameObject_Tag_Remove * pArg = dynamic_cast<NotificationArg_InspectorComponent_GameObject_Tag_Remove*>(p);

		if( pArg != NULL && pArg->m_nDepth == m_nDepth )
		{
			vector<CItem*>::iterator itr = m_ItemList.begin();

			for( ; itr != m_ItemList.end(); ++itr )
			{
				(*itr)->Release();
			}

			m_ItemList.clear();

			m_IsChildOpen = false;
		}
	}

	void				OnButtonClick()
	{
		int nNumChild = m_pNode->GetNumChild();
		if( nNumChild == 0 )
		{
			m_pThis->OnButtonClick_Item(m_pNode);
		}
		else 
		{
			m_IsChildOpen = !m_IsChildOpen;
			if( m_IsChildOpen == true )
			{
				NotificationArg_InspectorComponent_GameObject_Tag_Remove * pArg = new NotificationArg_InspectorComponent_GameObject_Tag_Remove();
				pArg->m_nDepth = m_nDepth;

				CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_GameObject_Tag_Remove, pArg);
				pArg->Release();

				m_IsChildOpen = true;

				GwVector2 vPos = *m_pButton->GetLocalPos();

				if( m_IsChildLeft == true )
					vPos.x -= 135;
				else 
					vPos.x = 0;

				vPos.y += 15;

				int nNumChild = m_pNode->GetNumChild();

				for( int i = 0; i < nNumChild; ++i )
				{
					INode * pChild = m_pNode->GetChild_byIndex(i);

					if( pChild != NULL )
					{
						CItem * pItem = new CItem(m_pThis, pChild, !m_IsChildLeft, m_nDepth + 1);
						pItem->SetLocalPos(vPos);
						m_ItemList.push_back(pItem);

						vPos.y += 21;
					}
				}
			}
			else 
			{
				vector<CItem*>::iterator itr = m_ItemList.begin();

				for( ; itr != m_ItemList.end(); ++itr )
				{
					(*itr)->Release();
				}

				m_ItemList.clear();
			}
		}
	}

	void				OnButtonClick_Child()
	{
		int nNumChild = m_pNode->GetNumChild();
		if( nNumChild == 0 )
		{
			m_pThis->OnButtonClick_Item(m_pNode);
		}
		else 
		{
			m_IsChildOpen = !m_IsChildOpen;
			if( m_IsChildOpen == true )
			{
				NotificationArg_InspectorComponent_GameObject_Tag_Remove * pArg = new NotificationArg_InspectorComponent_GameObject_Tag_Remove();
				pArg->m_nDepth = m_nDepth;

				CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_GameObject_Tag_Remove, pArg);
				pArg->Release();

				m_IsChildOpen = true;

				GwVector2 vPos = *m_pButton->GetLocalPos();

				if( m_IsChildLeft == true )
					vPos.x -= 135;
				else 
					vPos.x = 0;

				vPos.y += 15;

				int nNumChild = m_pNode->GetNumChild();

				for( int i = 0; i < nNumChild; ++i )
				{
					INode * pChild = m_pNode->GetChild_byIndex(i);

					if( pChild != NULL )
					{
						CItem * pItem = new CItem(m_pThis, pChild, !m_IsChildLeft, m_nDepth + 1);
						pItem->SetLocalPos(vPos);
						m_ItemList.push_back(pItem);

						vPos.y += 21;
					}
				}
			}
			else 
			{
				vector<CItem*>::iterator itr = m_ItemList.begin();

				for( ; itr != m_ItemList.end(); ++itr )
				{
					(*itr)->Release();
				}

				m_ItemList.clear();
			}
		}
	}

	IGwUIButton		*	GetButton()
	{
		return m_pButton;
	}

	IGwUIButton		*	GetButton_Child()
	{
		return m_pButton_Child;
	}

private:
	CUIComponent_GameObject_Tag *	m_pThis;
	INode						*	m_pNode;
	IGwUIButton					*	m_pButton;
	IGwUIButton					*	m_pButton_Child;
	IGwUIStatic					*	m_pStatic;

	int								m_nDepth;
	bool							m_IsChildLeft;
	bool							m_IsChildOpen;

	vector<CItem*>					m_ItemList;

	class	CBtnCallback_Button : public IGwUIButton_Callback
	{
	public:
		CBtnCallback_Button(CItem * pThis) : m_pThis(pThis)	{ }

		virtual void	OnButton_OK(class IGwUIButton* pControl) 	{ m_pThis->OnButtonClick(); }

	private:

		CItem		*	m_pThis;
	};

	class	CBtnCallback_Button_Child : public IGwUIButton_Callback
	{
	public:
		CBtnCallback_Button_Child(CItem * pThis) : m_pThis(pThis)	{ }

		virtual void	OnButton_OK(class IGwUIButton* pControl) 	{ m_pThis->OnButtonClick_Child(); }

	private:

		CItem		*	m_pThis;
	};

};

class	CUIComponent_GameObject_Tag::CBtnCallback_Root : public IGwUIButton_Callback
{
public:
	CBtnCallback_Root(CUIComponent_GameObject_Tag * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Root(); 
	}
private:
	CUIComponent_GameObject_Tag		*	m_pThis;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIComponent_SkinMeshObject

CUIComponent_GameObject_Tag::CUIComponent_GameObject_Tag(void) : m_IsOpen(false)
{
}

CUIComponent_GameObject_Tag::~CUIComponent_GameObject_Tag(void)
{
	m_pStatic_Sel->Release();

	vector<CItem*>::iterator itr = m_ItemList.begin();

	for( ; itr != m_ItemList.end(); ++itr )
	{
		(*itr)->Release();
	}

	m_ItemList.clear();
}

void			CUIComponent_GameObject_Tag::Initialize()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUISprite * pSprite;
	IGwUIButton * pButton;

	pSprite = CUIHelper::CreateUISprite("tag_combo_bt_normal.png");
	m_pBaseControl->AddControl(pSprite); pSprite->Release();

	m_pStatic_Sel = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(0,0,0,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
	m_pStatic_Sel->SetLocalPos(GwVector2(4, 4));

	int nTag = m_pGameObject->GetTag();
	INode * pNode = CTableMgr::GetInstance()->GetTable_GameObject_Tag()->FindNode(nTag);
	wchar_t wszFileName[128] = { 0, };
	wstring wstrFileName;
	ConvertAnsiStringToWideCch(wszFileName, pNode->GetName().c_str(), 128);
	IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
	CUIHelper::FittingStringExtent(pUIFont, 104, wszFileName, wstrFileName);
	m_pStatic_Sel->SetText(wstrFileName.c_str());
	m_pStatic_Sel->SetDependencyType(GWUISTATEDEPENDENCY_NOACTION);

	m_pBaseControl->AddControl(m_pStatic_Sel);

	pButton = CUIHelper::CreateUIButton("tag_combo_normal.png", "tag_combo_over.png", "tag_combo_normal.png", "tag_combo_click.png", "tag_combo_normal.png");
	pButton->SetLocalPos(GwVector2(110, 0));
	CBtnCallback_Root * pCallback = new CBtnCallback_Root(this);
	pButton->SetCallback(pCallback); pCallback->Release();
	m_pBaseControl->AddControl(pButton); pButton->Release();

	Gw::GetBillboardMgr()->EndTextureMerge();
}

void			CUIComponent_GameObject_Tag::Update(float fDeltaTime)					
{ 
}

void			CUIComponent_GameObject_Tag::OnButtonClick_Root()
{
	m_IsOpen = !m_IsOpen;
	if( m_IsOpen == true )
	{
		INode * pRoot = CTableMgr::GetInstance()->GetTable_GameObject_Tag()->GetRoot();

		if( pRoot != NULL )
		{
			GwVector2 vPos = GwVector2(0, 21);
			int nNumChild = pRoot->GetNumChild();

			for( int i = 0; i < nNumChild; ++i )
			{
				INode * pChild = pRoot->GetChild_byIndex(i);

				if( pChild != NULL )
				{
					CItem * pItem = new CItem(this, pChild, true, 1);
					pItem->SetLocalPos(vPos);
					m_ItemList.push_back(pItem);

					vPos.y += 21;
				}
			}
		}
	}
	else 
	{
		vector<CItem*>::iterator itr = m_ItemList.begin();

		for( ; itr != m_ItemList.end(); ++itr )
		{
			(*itr)->Release();
		}

		m_ItemList.clear();
	}
}

void			CUIComponent_GameObject_Tag::OnButtonClick_Item(INode * pNode)
{
	wchar_t wszFileName[128] = { 0, };
	wstring wstrFileName;
	ConvertAnsiStringToWideCch(wszFileName, pNode->GetName().c_str(), 128);
	IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
	CUIHelper::FittingStringExtent(pUIFont, 104, wszFileName, wstrFileName);
	m_pStatic_Sel->SetText(wstrFileName.c_str());
	m_pStatic_Sel->SetDependencyType(GWUISTATEDEPENDENCY_NOACTION);

	m_pGameObject->SetTag(pNode->GetID());

	m_IsOpen = false;

	vector<CItem*>::iterator itr = m_ItemList.begin();

	for( ; itr != m_ItemList.end(); ++itr )
	{
		(*itr)->Release();
	}

	m_ItemList.clear();
}