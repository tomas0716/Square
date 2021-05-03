#include "StdAfx.h"
#include "PopupMenu.h"

class	CPopupMenu::CBtnCallback_Menu : public IGwUIButton_Callback
{
public:
	CBtnCallback_Menu(CPopupMenu * pPopupMenu, eObjectEditState eEditState)
		: m_pPopupMenu(pPopupMenu), m_eEditState(eEditState)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pPopupMenu->OnButtonClick(m_eEditState); 
	}
private:
	CPopupMenu			*	m_pPopupMenu;
	eObjectEditState		m_eEditState;
};

CPopupMenu::CPopupMenu(ULONG nEditStateOption)
{
	int nSize = 0;
	map<int, wstring> TextTable;
	map<int, eObjectEditState>	EditStateTable;
	TextTable[nSize] = L"POSITION";		EditStateTable[nSize++] = eObjectEditState_Position;
	TextTable[nSize] = L"ROTATION";		EditStateTable[nSize++] = eObjectEditState_Rotation;
	TextTable[nSize] = L"UP_DOWN";		EditStateTable[nSize++] = eObjectEditState_UpDown;
	TextTable[nSize] = L"DELETE";		EditStateTable[nSize++] = eObjectEditState_Delete;

	TextTable[nSize] = L"ADD";			EditStateTable[nSize++] = eObjectEditState_PathObject_Add;
	TextTable[nSize] = L"DELETE";		EditStateTable[nSize++] = eObjectEditState_PathObject_Delete;

	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();

	IGwUISprite * pSprite;
	IGwUIStatic * pStatic;
	IGwUIButton * pButton;
	IGwUIButton_Callback * pButtonCallback;

	Gw::GetBillboardMgr()->BeginTextureMerge();

	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\PopupMenu\\");

	int nCount = 0;
	for( int i = 0; i < nSize; ++i )
	{
		if( (nEditStateOption & EditStateTable[i]) != 0 )
		{
			++nCount;
		}
	}

	pSprite = CUIHelper::CreateUISprite("back.png");
	pSprite->SetSize_NormalState(GwVector2(136, nCount * 18 + 6));
	m_pDialog->AddControl(pSprite); pSprite->Release();

	int nHeight = 3;

	for( int i = 0; i < nSize; ++i )
	{
		if( (nEditStateOption & EditStateTable[i]) != 0 )
		{
			pButton = CUIHelper::CreateUIButton("popup_menu_bt_normal.png", "popup_menu_bt_over.png", "popup_menu_bt_normal.png", "popup_menu_bt_click.png", "popup_menu_bt_normal.png");
			pButtonCallback = new CBtnCallback_Menu(this, EditStateTable[i]);
			pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
			pButton->SetLocalPos(GwVector2(3,nHeight));
			m_pDialog->AddControl(pButton); pButton->Release();

			pStatic = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), true, 0, GwColor(0.5f,0.5f,0.5f,0.5f));
			pStatic->SetText(TextTable[i].c_str());
			GwVector2 vSize = *pStatic->GetCollisSize();
			pStatic->SetLocalPos(GwVector2(68 - vSize.x * 0.5f,nHeight + 3));
			pStatic->SetDependencyType(GWUISTATEDEPENDENCY_FROMPARENT);
			m_pDialog->AddControl(pStatic); pStatic->Release();

			nHeight += 18;
		}
	}

	Gw::GetBillboardMgr()->EndTextureMerge();

	m_pEvent_Click = new TEventDispatch< EventArg_PopupMenu_Click >();
}

CPopupMenu::~CPopupMenu(void)
{
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();
	
	m_pEvent_Click->Dispose();
	m_pEvent_Click->Release();
}

void			CPopupMenu::Update(float fDeltaTime)
{
	if( Gw::GetKeyMapper()->WasKeyDown(VK_LBUTTON) == TRUE &&
		IGwUIDialog::GetCurrMouseOverDialog() != m_pDialog )
	{
		HIdePopup();
	}
}

void			CPopupMenu::ShowPopup(GwVector2 vPos)
{
	m_pDialog->SetWorldPos(GwVector(vPos.x, vPos.y, 0));
	Gw::GetUIControlMgr()->AddDialog(m_pDialog);
	Gw::GetUIControlMgr()->UpdateDialog(0);
}

void			CPopupMenu::HIdePopup()
{
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
}

void			CPopupMenu::OnButtonClick(eObjectEditState eEditState)
{
	EventArg_PopupMenu_Click Arg;
	Arg.eEditState = eEditState;
	m_pEvent_Click->OnEvent(Arg);

	HIdePopup();
}