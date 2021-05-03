#include "StdAfx.h"
#include "UIComponent_PathObject.h"

I_IMPLEMENT_DYNCREATE(CUIComponent_PathObject, ISingletonObject::GetTypeModule())

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCheckboxCallback_Entire

class CUIComponent_PathObject::CCheckboxCallback_Entire : public IGwUICheckbox_Callback
{
public:
	CCheckboxCallback_Entire(CUIComponent_PathObject* pThis) :m_pThis(pThis)				{ }

	virtual void	OnCheckChange_TRUE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pThis->OnCheckBox_Entire(true);
	}
	virtual void	OnCheckChange_FALSE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pThis->OnCheckBox_Entire(false);
	}

private:
	CUIComponent_PathObject * m_pThis;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIEditbox_Callback_Entire

class CUIComponent_PathObject::CUIEditbox_Callback_Entire : public IGwUIEditbox_Callback
{
public:
	CUIEditbox_Callback_Entire( CUIComponent_PathObject * pComponent)
		:m_pComponent(pComponent)
	{
	}

	virtual void	OnChangeText(IGwUIEditbox* pControl)
	{
		m_pComponent->ChangeText_Entire();
	}

private:
	CUIComponent_PathObject	*	m_pComponent;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIEditbox_Callback_Block

class CUIComponent_PathObject::CUIEditbox_Callback_Block : public IGwUIEditbox_Callback
{
public:
	CUIEditbox_Callback_Block( CUIComponent_PathObject * pComponent, sPathDotInfo * pPathDotInfo, CUIComponent_PathObject::eDot_EditBox eEditBox)
		:m_pComponent(pComponent), m_pPathDotInfo(pPathDotInfo), m_eEditBox(eEditBox)
	{
	}

	virtual void	OnChangeText(IGwUIEditbox* pControl)
	{
		m_pComponent->ChangeText_Block(m_pPathDotInfo, m_eEditBox);
	}

private:
	CUIComponent_PathObject	*	m_pComponent;
	sPathDotInfo			*	m_pPathDotInfo;
	CUIComponent_PathObject::eDot_EditBox	m_eEditBox;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Dot_Remove

class	CUIComponent_PathObject::CUIButton_Callback_Dot_Remove : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Dot_Remove(CUIComponent_PathObject * pComponent, sPathDotInfo * pPathDotInfo)
		: m_pComponent(pComponent), m_pPathDotInfo(pPathDotInfo)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pComponent->OnButtonClick_Dot_Remove(m_pPathDotInfo); 
	}
private:
	CUIComponent_PathObject	*	m_pComponent;
	sPathDotInfo			*	m_pPathDotInfo;
	
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Dot_Select

class	CUIComponent_PathObject::CUIButton_Callback_Dot_Select : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Dot_Select(CUIComponent_PathObject * pComponent, sPathDotInfo * pPathDotInfo, int nIndexPlus)
		: m_pComponent(pComponent), m_pPathDotInfo(pPathDotInfo), m_nIndexPlus(nIndexPlus)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pComponent->OnButtonClick_Dot_Select(m_pPathDotInfo, m_nIndexPlus); 
	}
private:
	CUIComponent_PathObject	*	m_pComponent;
	sPathDotInfo			*	m_pPathDotInfo;
	int							m_nIndexPlus;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Controller_Reset_Tangent

class	CUIComponent_PathObject::CUIButton_Callback_Controller_Reset_Tangent : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Controller_Reset_Tangent(CUIComponent_PathObject * pComponent, sPathDotInfo * pPathDotInfo)
		: m_pComponent(pComponent), m_pPathDotInfo(pPathDotInfo)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pComponent->OnButtonClick_Controller_Reset_Tangent(m_pPathDotInfo); 
	}
private:
	CUIComponent_PathObject	*	m_pComponent;
	sPathDotInfo			*	m_pPathDotInfo;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCheckboxCallback_Entire

class CUIComponent_PathObject::CCheckboxCallback_CreationAutoPlay : public IGwUICheckbox_Callback
{
public:
	CCheckboxCallback_CreationAutoPlay(CUIComponent_PathObject* pThis) :m_pThis(pThis)				{ }

	virtual void	OnCheckChange_TRUE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pThis->OnCheckBox_CreationAutoPlay(true);
	}
	virtual void	OnCheckChange_FALSE(IGwUICheckbox* pMyControl,bool bFromInput) 
	{
		m_pThis->OnCheckBox_CreationAutoPlay(false);
	}

private:
	CUIComponent_PathObject * m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_GameObject_Remove

class	CUIComponent_PathObject::CUIButton_Callback_GameObject_Remove : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_GameObject_Remove(CUIComponent_PathObject * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_GameObject_Remove(); 
	}
private:
	CUIComponent_PathObject	* m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Play

class	CUIComponent_PathObject::CUIButton_Callback_Play : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Play(CUIComponent_PathObject * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Play(); 
	}
private:
	CUIComponent_PathObject	* m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Pause

class	CUIComponent_PathObject::CUIButton_Callback_Pause : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Pause(CUIComponent_PathObject * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Pause(); 
	}
private:
	CUIComponent_PathObject	* m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Stop

class	CUIComponent_PathObject::CUIButton_Callback_Stop : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Stop(CUIComponent_PathObject * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Stop(); 
	}
private:
	CUIComponent_PathObject	* m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Align_Tail

class	CUIComponent_PathObject::CUIButton_Callback_Align_Tail : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Align_Tail(CUIComponent_PathObject * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Align_Tail(); 
	}
private:
	CUIComponent_PathObject	* m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIButton_Callback_Reset_Tangent

class	CUIComponent_PathObject::CUIButton_Callback_Reset_Tangent : public IGwUIButton_Callback
{
public:
	CUIButton_Callback_Reset_Tangent(CUIComponent_PathObject * pThis)
		: m_pThis(pThis)
	{
	}
	virtual void	OnButton_OK(class IGwUIButton* pControl) 			
	{
		m_pThis->OnButtonClick_Reset_Tangent(); 
	}
private:
	CUIComponent_PathObject	* m_pThis;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CUIComponent_PathObject

CUIComponent_PathObject::CUIComponent_PathObject(void)
: m_pCheckBox_Entire(NULL), m_pEditbox_EntireTime(NULL), m_pCheckBox_CreationAutoPlay(NULL), m_pUIDragItemSprite_GameObject(NULL), m_pButton_Remove(NULL),
  m_pButton_Play(NULL), m_pButton_Pause(NULL), m_pButton_Stop(NULL)
{
}

CUIComponent_PathObject::~CUIComponent_PathObject(void)
{
	if( CNotificationCenter::GetInstance() != NULL )
	{
		CNotificationCenter::GetInstance()->RemoveObserver(Notification_InspectorComponent_PathObject_InfoChange, "CUIComponent_PathObject");
	}

	if( m_pCheckBox_Entire != NULL )
		m_pCheckBox_Entire->Release();

	if( m_pEditbox_EntireTime != NULL ) 
		m_pEditbox_EntireTime->Release();

	if( m_pCheckBox_CreationAutoPlay != NULL ) 
		m_pCheckBox_CreationAutoPlay->Release();

	if( m_pUIDragItemSprite_GameObject != NULL ) 
		delete m_pUIDragItemSprite_GameObject;

	if( m_pButton_Remove != NULL ) 
		m_pButton_Remove->Release();

	if( m_pStatic_GameObjectName != NULL )
		m_pStatic_GameObjectName->Release();

	if( m_pButton_Play != NULL )
		m_pButton_Play->Release();

	if( m_pButton_Pause != NULL )
		m_pButton_Pause->Release();

	if( m_pButton_Stop != NULL )
		m_pButton_Stop->Release();

	map<sPathDotInfo*,sDotInfo*>::iterator itr = m_DotInfoTable.begin();

	for( ; itr != m_DotInfoTable.end(); ++itr )
	{
		delete itr->second;
	}

	m_DotInfoTable.clear();
}

void			CUIComponent_PathObject::Initialize()
{
	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

	IGwUISprite * pSprite_Title = CUIHelper::CreateUISprite("pathobject_title.png");
	m_pBaseControl->AddControl(pSprite_Title); pSprite_Title->Release();
	m_fHeight += pSprite_Title->GetCollisSize()->y + 13;

	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		char szText[128] = { 0, };
		wchar_t wszText[128] = { 0, };

		IGwUIButton * pButton;
		IGwUIButton_Callback * pButtonCallback;
		IGwUICheckbox_Callback*	pChkBoxCallback;

		m_pButton_Play = CUIHelper::CreateUIButton("play_normal.png", "play_over.png", "play_normal.png", "play_click.png", "play_normal.png");
		m_pButton_Play->SetLocalPos(GwVector2(13, m_fHeight));
		CUIButton_Callback_Play * pCallback_Play = new CUIButton_Callback_Play(this);
		m_pButton_Play->SetCallback(pCallback_Play); pCallback_Play->Release();
		m_pBaseControl->AddControl(m_pButton_Play);

		m_pButton_Pause = CUIHelper::CreateUIButton("pause_normal.png", "pause_over.png", "pause_normal.png", "pause_click.png", "pause_normal.png");
		m_pButton_Pause->SetLocalPos(GwVector2(13, m_fHeight));
		CUIButton_Callback_Pause * pCallback_Pause = new CUIButton_Callback_Pause(this);
		m_pButton_Pause->SetCallback(pCallback_Pause); pCallback_Pause->Release();
		m_pBaseControl->AddControl(m_pButton_Pause);

		m_pButton_Stop = CUIHelper::CreateUIButton("stop_normal.png", "stop_over.png", "stop_normal.png", "stop_click.png", "stop_normal.png");	
		m_pButton_Stop->SetLocalPos(GwVector2(50, m_fHeight));
		CUIButton_Callback_Stop * pCallback_Stop = new CUIButton_Callback_Stop(this);
		m_pButton_Stop->SetCallback(pCallback_Stop); pCallback_Stop->Release();
		m_pBaseControl->AddControl(m_pButton_Stop);

		pButton = CUIHelper::CreateUIButton("Align_normal.png", "Align_over.png", "Align_normal.png", "Align_click.png", "Align_normal.png");	
		pButton->SetLocalPos(GwVector2(140, m_fHeight + 3));
		pButtonCallback = new CUIButton_Callback_Align_Tail(this);
		pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
		m_pBaseControl->AddControl(pButton);

		pButton = CUIHelper::CreateUIButton("Reset_normal.png", "Reset_over.png", "Reset_normal.png", "Reset_click.png", "Reset_normal.png");	
		pButton->SetLocalPos(GwVector2(205, m_fHeight + 3));
		pButtonCallback = new CUIButton_Callback_Reset_Tangent(this);
		pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
		m_pBaseControl->AddControl(pButton);

		switch( pPathObject->GetCurrPlayType() )
		{
		case ePlay_Stop:
		case ePlay_Pause:
		case ePlay_Complete:	m_pButton_Pause->SetHidden();	break;
		case ePlay_Play:		m_pButton_Play->SetHidden();	break;
		}
	
		m_fHeight += 35;

		m_pCheckBox_CreationAutoPlay = CUIHelper::CreateUICheckBox("checkbox_creationautoplay_off.png", "checkbox_creationautoplay_on.png");
		pChkBoxCallback = new CCheckboxCallback_CreationAutoPlay(this);
		m_pCheckBox_CreationAutoPlay->SetCallback(pChkBoxCallback);	pChkBoxCallback->Release();
		m_pCheckBox_CreationAutoPlay->SetLocalPos(GwVector2(13, m_fHeight+1));
		m_pBaseControl->AddControl(m_pCheckBox_CreationAutoPlay);

		if( pPathObject->IsCreationAutoPlay() == true )
		{
			m_pCheckBox_CreationAutoPlay->SetCheck(true);
		}

		m_fHeight += 28;

		m_pUIDragItemSprite_GameObject = new CUIDragItemSprite(eDragObject_UISprite, 
															   eDragObjectSender_UISprite, 
															   eDragObjectReceiver_Project_File, 
															   "EsTool_Resources\\UI\\Inspector\\", 
															   "item.png",
															   NULL);

		m_pUIDragItemSprite_GameObject->GetDelegate_Drag(eDelegate_Drag_Receive)->Bind<CUIComponent_PathObject, CUIDragItemBase*, CUIDragItemBase*, eDragObjectSender, &CUIComponent_PathObject::OnDragReceive_GameObject>(this);

		m_pUIDragItemSprite_GameObject->GetUISprite()->SetLocalPos(GwVector2(13, m_fHeight));
		m_pBaseControl->AddControl(m_pUIDragItemSprite_GameObject->GetUISprite());

		m_pButton_Remove = CUIHelper::CreateUIButton("minus_normal.png", "minus_over.png", "minus_normal.png", "minus_click.png", "minus_normal.png");
		m_pButton_Remove->SetLocalPos(GwVector2(210 + 13, m_fHeight+1));
		pButtonCallback = new CUIButton_Callback_GameObject_Remove(this);
		m_pButton_Remove->SetCallback(pButtonCallback); pButtonCallback->Release();
		m_pBaseControl->AddControl(m_pButton_Remove);

		m_pStatic_GameObjectName = CUIHelper::CreateUIStatic(13, "±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), true, 0, GwColor(0.8f, 0.8f, 0.8f, 0.8f));
		m_pStatic_GameObjectName->SetLocalPos(GwVector2(16, m_fHeight + 2));
		m_pBaseControl->AddControl(m_pStatic_GameObjectName);

		if( pPathObject->IsAttachGameObject() == true )
		{
			wchar_t wszFileName[128] = { 0, };
			wstring wstrFileName;
			ConvertAnsiStringToWideCch(wszFileName, pPathObject->GetAttachGameObject()->GetName().c_str(), 128);
			IGwUIFont * pUIFont = Gw::GetUIMgr()->CreateUIFont(13,"±¼¸²",GWFONTWEIGHT_NORMAL, false);
			CUIHelper::FittingStringExtent(pUIFont, 150, wszFileName, wstrFileName);
			m_pStatic_GameObjectName->SetText(wstrFileName.c_str());
		}
		else
		{
			m_pStatic_GameObjectName->SetText(L"None");
			m_pButton_Remove->SetHidden();
		}

		m_fHeight += 28;

		m_pCheckBox_Entire = CUIHelper::CreateUICheckBox("checkbox_entire_off.png", "checkbox_entire_on.png");
		pChkBoxCallback = new CCheckboxCallback_Entire(this);
		m_pCheckBox_Entire->SetCallback(pChkBoxCallback);	pChkBoxCallback->Release();
		m_pCheckBox_Entire->SetLocalPos(GwVector2(13, m_fHeight+1));
		m_pBaseControl->AddControl(m_pCheckBox_Entire);

		if( pPathObject->GetBlockTime() == false )
		{
			m_pCheckBox_Entire->SetCheck(true);
		}

		m_fHeight += 24;

		IGwUIStatic * pStatic_Number = CUIHelper::CreateUIStatic(12, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(1,1,1,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
		pStatic_Number->SetLocalPos(GwVector2(13, m_fHeight + 2));
		pStatic_Number->SetText(L"ÀüÃ¼ ½Ã°£ : ");
		m_pBaseControl->AddControl(pStatic_Number); pStatic_Number->Release();

		IGwUISprite * pSprite_EditBack = CUIHelper::CreateUISprite("edit.png");
		pSprite_EditBack->SetSize_NormalState(GwVector2(70, 14));
		pSprite_EditBack->SetLocalPos(GwVector2(100, m_fHeight));
		m_pBaseControl->AddControl(pSprite_EditBack); pSprite_EditBack->Release();

		m_pEditbox_EntireTime = CUIHelper::CreateUIEditBox(12,"±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(1,1,1,1), GwColor(0,0,1,1));
		m_pEditbox_EntireTime->SetLimitEditboxSizeX(55);
		m_pEditbox_EntireTime->SetLocalPos(GwVector2(102, m_fHeight + 1));
		sprintf(szText, "%.2f", pPathObject->GetEntireTime());
		ConvertAnsiStringToWideCch(wszText, szText, 128);
		m_pEditbox_EntireTime->SetText(wszText);
		CUIEditbox_Callback_Entire * pCallback = new CUIEditbox_Callback_Entire(this);
		m_pEditbox_EntireTime->SetCallback(pCallback); pCallback->Release();
		m_pBaseControl->AddControl(m_pEditbox_EntireTime);

		Gw::GetBillboardMgr()->EndTextureMerge();

		m_fHeight += 25;

		CreateUI_Dot();

		CNotificationCenter::GetInstance()->AddObserver(Notification_InspectorComponent_PathObject_InfoChange, "CUIComponent_PathObject", this, &CUIComponent_PathObject::OnPathObjectInfoChange);
	}
}

void			CUIComponent_PathObject::CreateUI_Dot()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		map<sPathDotInfo*,sDotInfo*>::iterator itr = m_DotInfoTable.begin();

		for( ; itr != m_DotInfoTable.end(); ++itr )
		{
			m_pBaseControl->RemoveControl(itr->second->pBaseCtrl);
			delete itr->second;
		}

		m_DotInfoTable.clear();

		Gw::GetBillboardMgr()->BeginTextureMerge();
		Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\Inspector\\");

		IGwUIStatic * pStatic;
		IGwUIButton * pButton;
		IGwUISprite * pSprite;
		IGwUIButton_Callback * pButtonCallback;
		IGwUIEditbox_Callback * pEditBoxCallback;

		char szText[128] = { 0, };
		wchar_t wszText[128] = { 0, };

		m_fHeight = 180;

		for( int i = 0, dot = 1; i < pPathObject->GetNumEvent(); ++i )
		{
			sPathDotInfo * pPathDotInfo = pPathObject->GetEvent(i);

			if( pPathDotInfo != NULL && pPathDotInfo->bController == false )
			{
				sDotInfo * pDotInfo = new sDotInfo();
				m_DotInfoTable.insert(map<sPathDotInfo*,sDotInfo*>::value_type(pPathDotInfo, pDotInfo));

				pDotInfo->pBaseCtrl = Gw::GetUIControlMgr()->CreateUIDummyCtrl();
				pDotInfo->pBaseCtrl->SetLocalPos(GwVector2(13, m_fHeight));
				m_pBaseControl->AddControl(pDotInfo->pBaseCtrl);

				pStatic = CUIHelper::CreateUIStatic(12, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(1,1,1,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
				pStatic->SetLocalPos(GwVector2(0, 1));
				wsprintf(wszText, L"%d :", dot);
				pStatic->SetText(wszText);
				pDotInfo->pBaseCtrl->AddControl(pStatic); pStatic->Release();

				pSprite = CUIHelper::CreateUISprite("edit.png");
				pSprite->SetSize_NormalState(GwVector2(50, 14));
				pSprite->SetLocalPos(GwVector2(20, 0));
				pDotInfo->pBaseCtrl->AddControl(pSprite); pSprite->Release();

				pDotInfo->pEditBox[eDot_EditBox_Time] = CUIHelper::CreateUIEditBox(12,"±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(1,1,1,1), GwColor(0,0,1,1));
				pDotInfo->pEditBox[eDot_EditBox_Time]->SetLimitEditboxSizeX(55);
				pDotInfo->pEditBox[eDot_EditBox_Time]->SetLocalPos(GwVector2(22, 1));
				sprintf(szText, "%.2f", pPathDotInfo->fTime);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_Time]->SetText(wszText);
				pEditBoxCallback = new CUIEditbox_Callback_Block(this, pPathDotInfo, eDot_EditBox_Time);
				pDotInfo->pEditBox[eDot_EditBox_Time]->SetCallback(pEditBoxCallback); pEditBoxCallback->Release();
				pDotInfo->pBaseCtrl->AddControl(pDotInfo->pEditBox[eDot_EditBox_Time]);

				pDotInfo->pStatic_Length = CUIHelper::CreateUIStatic(12, "±¼¸²", GWFONTWEIGHT_BOLD, GwColor(1,1,1,1), true, 0, GwColor(0.8f,0.8f,0.8f,0.8f));
				sprintf(szText, "±æÀÌ : %.2f", pPathDotInfo->fLength);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pStatic_Length->SetText(wszText);
				pDotInfo->pStatic_Length->SetLocalPos(GwVector2(80, 1));
				pDotInfo->pBaseCtrl->AddControl(pDotInfo->pStatic_Length);

				pButton = CUIHelper::CreateUIButton("minus_normal.png", "minus_over.png", "minus_normal.png", "minus_click.png", "minus_normal.png");
				pButton->SetLocalPos(GwVector2(200, -1));
				pButtonCallback = new CUIButton_Callback_Dot_Remove(this, pPathDotInfo);
				pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
				pDotInfo->pBaseCtrl->AddControl(pButton);

				m_fHeight += 21;

				pSprite = CUIHelper::CreateUISprite("edit.png");
				pSprite->SetSize_NormalState(GwVector2(60, 14));
				pSprite->SetLocalPos(GwVector2(20, 21));
				pDotInfo->pBaseCtrl->AddControl(pSprite); pSprite->Release();

				pDotInfo->pEditBox[eDot_EditBox_X] = CUIHelper::CreateUIEditBox(12,"±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(1,1,1,1), GwColor(0,0,1,1));
				pDotInfo->pEditBox[eDot_EditBox_X]->SetLimitEditboxSizeX(56);
				pDotInfo->pEditBox[eDot_EditBox_X]->SetLocalPos(GwVector2(22, 21 + 1));
				sprintf(szText, "%.2f", pPathDotInfo->vLocalPos.x);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_X]->SetText(wszText);
				pEditBoxCallback = new CUIEditbox_Callback_Block(this, pPathDotInfo, eDot_EditBox_X);
				pDotInfo->pEditBox[eDot_EditBox_X]->SetCallback(pEditBoxCallback); pEditBoxCallback->Release();
				pDotInfo->pBaseCtrl->AddControl(pDotInfo->pEditBox[eDot_EditBox_X]);

				pSprite = CUIHelper::CreateUISprite("edit.png");
				pSprite->SetSize_NormalState(GwVector2(60, 14));
				pSprite->SetLocalPos(GwVector2(20 + 65, 21));
				pDotInfo->pBaseCtrl->AddControl(pSprite); pSprite->Release();

				pDotInfo->pEditBox[eDot_EditBox_Y] = CUIHelper::CreateUIEditBox(12,"±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(1,1,1,1), GwColor(0,0,1,1));
				pDotInfo->pEditBox[eDot_EditBox_Y]->SetLimitEditboxSizeX(55);
				pDotInfo->pEditBox[eDot_EditBox_Y]->SetLocalPos(GwVector2(22 + 65, 21 + 1));
				sprintf(szText, "%.2f", pPathDotInfo->vLocalPos.y);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_Y]->SetText(wszText);
				pEditBoxCallback = new CUIEditbox_Callback_Block(this, pPathDotInfo, eDot_EditBox_Y);
				pDotInfo->pEditBox[eDot_EditBox_Y]->SetCallback(pEditBoxCallback); pEditBoxCallback->Release();
				pDotInfo->pBaseCtrl->AddControl(pDotInfo->pEditBox[eDot_EditBox_Y]);

				pSprite = CUIHelper::CreateUISprite("edit.png");
				pSprite->SetSize_NormalState(GwVector2(60, 14));
				pSprite->SetLocalPos(GwVector2(20 + 65 + 65, 21));
				pDotInfo->pBaseCtrl->AddControl(pSprite); pSprite->Release();

				pDotInfo->pEditBox[eDot_EditBox_Z] = CUIHelper::CreateUIEditBox(12,"±¼¸²", GWFONTWEIGHT_NORMAL, GwColor(1,1,1,1), GwColor(1,1,1,1), GwColor(0,0,1,1));
				pDotInfo->pEditBox[eDot_EditBox_Z]->SetLimitEditboxSizeX(55);
				pDotInfo->pEditBox[eDot_EditBox_Z]->SetLocalPos(GwVector2(22 + 65 + 65, 21 + 1));
				sprintf(szText, "%.2f", pPathDotInfo->vLocalPos.z);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_Z]->SetText(wszText);
				pEditBoxCallback = new CUIEditbox_Callback_Block(this, pPathDotInfo, eDot_EditBox_Z);
				pDotInfo->pEditBox[eDot_EditBox_Z]->SetCallback(pEditBoxCallback); pEditBoxCallback->Release();
				pDotInfo->pBaseCtrl->AddControl(pDotInfo->pEditBox[eDot_EditBox_Z]);

				m_fHeight += 21;

				pButton = CUIHelper::CreateUIButton("Point_normal.png", "Point_over.png", "Point_normal.png", "Point_click.png", "Point_normal.png");
				pButton->SetLocalPos(GwVector2(20, 42));
				pButtonCallback = new CUIButton_Callback_Dot_Select(this, pPathDotInfo, 0);
				pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
				pDotInfo->pBaseCtrl->AddControl(pButton);

				if( i == pPathObject->GetNumEvent() - 1 )
				{
					pButton = CUIHelper::CreateUIButton("Left_normal.png", "Left_over.png", "Left_normal.png", "Left_click.png", "Left_normal.png");
					pButton->SetLocalPos(GwVector2(22 + 23, 42));
					pButtonCallback = new CUIButton_Callback_Dot_Select(this, pPathDotInfo, -1);
					pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
					pDotInfo->pBaseCtrl->AddControl(pButton);
				}
				else if( i == 0 )
				{
					pButton = CUIHelper::CreateUIButton("Right_normal.png", "Right_over.png", "Right_normal.png", "Right_click.png", "Right_normal.png");
					pButton->SetLocalPos(GwVector2(22 + 23, 42));
					pButtonCallback = new CUIButton_Callback_Dot_Select(this, pPathDotInfo, 1);
					pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
					pDotInfo->pBaseCtrl->AddControl(pButton);
				}
				else 
				{
					pButton = CUIHelper::CreateUIButton("Left_normal.png", "Left_over.png", "Left_normal.png", "Left_click.png", "Left_normal.png");
					pButton->SetLocalPos(GwVector2(22 + 23, 42));
					pButtonCallback = new CUIButton_Callback_Dot_Select(this, pPathDotInfo, -1);
					pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
					pDotInfo->pBaseCtrl->AddControl(pButton);

					pButton = CUIHelper::CreateUIButton("Right_normal.png", "Right_over.png", "Right_normal.png", "Right_click.png", "Right_normal.png");
					pButton->SetLocalPos(GwVector2(22 + 23 + 23, 42));
					pButtonCallback = new CUIButton_Callback_Dot_Select(this, pPathDotInfo, 1);
					pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
					pDotInfo->pBaseCtrl->AddControl(pButton);

					pButton = CUIHelper::CreateUIButton("Reset_normal.png", "Reset_over.png", "Reset_normal.png", "Reset_click.png", "Reset_normal.png");
					pButton->SetLocalPos(GwVector2(22 + 23 + 23 + 40, 42));
					pButtonCallback = new CUIButton_Callback_Controller_Reset_Tangent(this, pPathDotInfo);
					pButton->SetCallback(pButtonCallback); pButtonCallback->Release();
					pDotInfo->pBaseCtrl->AddControl(pButton);
				}

				m_fHeight += 27;

				++dot;
			}
		}

		m_fHeight += 5;

		Gw::GetBillboardMgr()->EndTextureMerge();
	}
}

void			CUIComponent_PathObject::Update(float fDeltaTime)					
{ 
}

void			CUIComponent_PathObject::OnCheckBox_Entire(bool IsEntire)
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		pPathObject->SetBlockTime(!IsEntire);
		pPathObject->ReLoad();
		OnButtonClick_Stop();
	}
}

void			CUIComponent_PathObject::ChangeText_Entire()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		const wchar_t * pText = m_pEditbox_EntireTime->GetText();

		float fValue = _wtof(pText);	

		pPathObject->SetEntireTime(fValue);

		if( pPathObject->GetBlockTime() == false )
		{
			CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

			if( pPathObjectRender != NULL )
			{
				pPathObjectRender->ReLoad();
				OnButtonClick_Stop();
			}
		}
	}
}

void			CUIComponent_PathObject::ChangeText_Block(sPathDotInfo * pPathDotInfo, eDot_EditBox eEditBox)
{
	map<sPathDotInfo*,sDotInfo*>::iterator itr = m_DotInfoTable.find(pPathDotInfo);

	if( itr != m_DotInfoTable.end() )
	{
		GwVector vPrevPos = pPathDotInfo->vLocalPos;

		const wchar_t * pText = itr->second->pEditBox[eEditBox]->GetText();
		float fValue = _wtof(pText);

		switch(eEditBox)
		{
		case eDot_EditBox_Time:		pPathDotInfo->fTime = fValue;			break;
		case eDot_EditBox_X:		pPathDotInfo->vLocalPos.x = fValue;		break;
		case eDot_EditBox_Y:		pPathDotInfo->vLocalPos.y = fValue;		break;
		case eDot_EditBox_Z:		pPathDotInfo->vLocalPos.z = fValue;		break;
		}

		IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

		if( pPathObject != NULL )
		{
			GwVector vDiffer = pPathDotInfo->vLocalPos - vPrevPos;

			sPathDotInfo * pPathDotInfo_Controller = NULL;
			pPathDotInfo_Controller = pPathObject->GetEvent(pPathDotInfo->nIndex - 1);
			if( pPathDotInfo_Controller != NULL )
			{
				pPathDotInfo_Controller->vLocalPos += vDiffer;
			}

			pPathDotInfo_Controller = pPathObject->GetEvent(pPathDotInfo->nIndex + 1);
			if( pPathDotInfo_Controller != NULL )
			{
				pPathDotInfo_Controller->vLocalPos += vDiffer;
			}

			CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

			if( pPathObjectRender != NULL )
			{
				pPathObjectRender->ReLoad();
				OnButtonClick_Stop();
			}
		}
	}
}

void			CUIComponent_PathObject::OnButtonClick_Dot_Remove(sPathDotInfo * pPathDotInfo)
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);
	
	if( pPathObject != NULL )
	{
		int nIndex = pPathDotInfo->nIndex;

		if( pPathObject->GetNumEvent() != 4 )
		{
			if( nIndex == 0 )
			{
				pPathObject->DeleteEvent(0);
				pPathObject->DeleteEvent(0);
				pPathObject->DeleteEvent(0);

				CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

				if( pPathObjectRender != NULL )
				{
					pPathObjectRender->ReLoad();
					pPathObjectRender->RemovePathObjectControllerRender();
				}
			}
			else if( nIndex == pPathObject->GetNumEvent() - 1 )
			{
				pPathObject->DeleteEvent(nIndex-2);
				pPathObject->DeleteEvent(nIndex-2);
				pPathObject->DeleteEvent(nIndex-2);

				CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

				if( pPathObjectRender != NULL )
				{
					pPathObjectRender->ReLoad();
					pPathObjectRender->RemovePathObjectControllerRender();
				}
			}
			else
			{
				pPathObject->DeleteEvent(nIndex-1);
				pPathObject->DeleteEvent(nIndex-1);
				pPathObject->DeleteEvent(nIndex-1);

				CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

				if( pPathObjectRender != NULL )
				{
					pPathObjectRender->ReLoad();
					pPathObjectRender->RemovePathObjectControllerRender();
				}
			}
		}

		CreateUI_Dot();

		CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_Height_Change, NULL);
	}
}

void			CUIComponent_PathObject::OnButtonClick_Dot_Select(sPathDotInfo * pPathDotInfo, int nIndexPlus)
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

		if( pPathObjectRender != NULL )
		{
			pPathObjectRender->SetSelectDot(pPathDotInfo, nIndexPlus);
		}
	}
}

void			CUIComponent_PathObject::OnButtonClick_Controller_Reset_Tangent(sPathDotInfo * pPathDotInfo)
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		int nIndex = pPathDotInfo->nIndex;

		sPathDotInfo * pPathDotInfo_Prev = pPathObject->GetEvent(nIndex - 1);
		sPathDotInfo * pPathDotInfo_Next = pPathObject->GetEvent(nIndex + 1);

		GwVector vInterval = pPathDotInfo->vLocalPos - pPathDotInfo_Prev->vLocalPos;
		pPathDotInfo_Next->vLocalPos = pPathDotInfo->vLocalPos + vInterval;

		CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

		if( pPathObjectRender != NULL )
		{
			pPathObjectRender->ReLoad();
			pPathObjectRender->RemovePathObjectControllerRender();
			OnButtonClick_Stop();
		}

		NotificationArg_InspectorComponent_PathObject_InfoChange * pArg = new NotificationArg_InspectorComponent_PathObject_InfoChange();
		pArg->m_pPathDotInfo = pPathDotInfo;
		OnPathObjectInfoChange(pArg);
		pArg->Release();
	}
}

void			CUIComponent_PathObject::OnCheckBox_CreationAutoPlay(bool IsAutoPlay)
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		pPathObject->SetCreationAutoPlay(IsAutoPlay);
		OnButtonClick_Stop();
	}
}

void			CUIComponent_PathObject::OnButtonClick_GameObject_Remove()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		pPathObject->SetAttachGameObject(NULL);
		m_pStatic_GameObjectName->SetText(L"None");
		m_pButton_Remove->SetHidden();

		OnButtonClick_Stop();
	}	
}

void			CUIComponent_PathObject::OnButtonClick_Play()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		if( pPathObject->GetCurrPlayType() == ePlay_Stop || pPathObject->GetCurrPlayType() == ePlay_Pause )
		{
			pPathObject->OnPlay();
		}
		else if( pPathObject->GetCurrPlayType() == ePlay_Complete )
		{
			pPathObject->OnStop();
			pPathObject->OnPlay();
		}

		m_pButton_Play->SetHidden();
		m_pButton_Pause->SetVisible();
		m_pButton_Stop->SetVisible();
	}
}

void			CUIComponent_PathObject::OnButtonClick_Pause()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		if( pPathObject->GetCurrPlayType() == ePlay_Play )
		{
			pPathObject->OnPause();
		}

		m_pButton_Play->SetVisible();
		m_pButton_Pause->SetHidden();
		m_pButton_Stop->SetVisible();
	}
}

void			CUIComponent_PathObject::OnButtonClick_Stop()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		if( pPathObject->GetCurrPlayType() == ePlay_Play || pPathObject->GetCurrPlayType() == ePlay_Pause  )
		{
			pPathObject->OnStop();
		}

		if( m_pButton_Play != NULL ) m_pButton_Play->SetVisible();
		if( m_pButton_Pause != NULL ) m_pButton_Pause->SetHidden();
		if( m_pButton_Stop != NULL ) m_pButton_Stop->SetVisible();
	}
}

void			CUIComponent_PathObject::OnButtonClick_Align_Tail()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		sPathDotInfo * pPathDotInfo_First = pPathObject->GetEvent(0);
		sPathDotInfo * pPathDotInfo_Tail = pPathObject->GetEvent(pPathObject->GetNumEvent() - 1);
		sPathDotInfo * pPathDotInfo_Tail_Controller = pPathObject->GetEvent(pPathObject->GetNumEvent() - 2);

		GwVector vInterval = pPathDotInfo_Tail_Controller->vLocalPos - pPathDotInfo_Tail->vLocalPos;

		pPathDotInfo_Tail->vLocalPos = pPathDotInfo_First->vLocalPos;
		pPathDotInfo_Tail_Controller->vLocalPos = pPathDotInfo_Tail->vLocalPos + vInterval;

		CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

		if( pPathObjectRender != NULL )
		{
			pPathObjectRender->ReLoad();
			pPathObjectRender->RemovePathObjectControllerRender();
			OnButtonClick_Stop();
		}

		NotificationArg_InspectorComponent_PathObject_InfoChange * pArg = new NotificationArg_InspectorComponent_PathObject_InfoChange();
		pArg->m_pPathDotInfo = pPathDotInfo_Tail;
		OnPathObjectInfoChange(pArg);
		pArg->Release();
	}
}

void			CUIComponent_PathObject::OnButtonClick_Reset_Tangent()
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		sPathDotInfo * pPathDotInfo_First = pPathObject->GetEvent(0);
		sPathDotInfo * pPathDotInfo_First_Controller = pPathObject->GetEvent(1);
		sPathDotInfo * pPathDotInfo_Tail = pPathObject->GetEvent(pPathObject->GetNumEvent() - 1);
		sPathDotInfo * pPathDotInfo_Tail_Controller = pPathObject->GetEvent(pPathObject->GetNumEvent() - 2);

		GwVector vInterval = pPathDotInfo_First->vLocalPos - pPathDotInfo_First_Controller->vLocalPos;

		pPathDotInfo_Tail_Controller->vLocalPos = pPathDotInfo_Tail->vLocalPos + vInterval;

		CPathObjectRender * pPathObjectRender = CPathObjectRenderMgr::GetInstance()->FindPathObjectRender(pPathObject);

		if( pPathObjectRender != NULL )
		{
			pPathObjectRender->ReLoad();
			pPathObjectRender->RemovePathObjectControllerRender();
			OnButtonClick_Stop();
		}

		NotificationArg_InspectorComponent_PathObject_InfoChange * pArg = new NotificationArg_InspectorComponent_PathObject_InfoChange();
		pArg->m_pPathDotInfo = pPathDotInfo_Tail;
		OnPathObjectInfoChange(pArg);
		pArg->Release();
	}
}

void			CUIComponent_PathObject::OnPathObjectInfoChange(NotificationArg * p)
{
	IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

	if( pPathObject != NULL )
	{
		char szText[128] = { 0, };
		wchar_t wszText[128] = { 0, };

		NotificationArg_InspectorComponent_PathObject_InfoChange * pArg = dynamic_cast<NotificationArg_InspectorComponent_PathObject_InfoChange*>(p);

		if( pArg != NULL )
		{
			map<sPathDotInfo*,sDotInfo*>::iterator itr = m_DotInfoTable.find(pArg->m_pPathDotInfo);

			if( itr != m_DotInfoTable.end() )
			{
				sPathDotInfo * pPathDotInfo = itr->first;
				sDotInfo * pDotInfo = itr->second;

				sprintf(szText, "%.2f", pPathDotInfo->vLocalPos.x);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_X]->SetText(wszText);

				sprintf(szText, "%.2f", pPathDotInfo->vLocalPos.y);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_Y]->SetText(wszText);

				sprintf(szText, "%.2f", pPathDotInfo->vLocalPos.z);
				ConvertAnsiStringToWideCch(wszText, szText, 128);
				pDotInfo->pEditBox[eDot_EditBox_Z]->SetText(wszText);
			}

			int nIndex = pArg->m_pPathDotInfo->nIndex;

			for( int i = nIndex; i < pPathObject->GetNumEvent(); ++i )
			{
				sPathDotInfo * pPathDotInfo = pPathObject->GetEvent(i);

				if( pPathDotInfo != NULL && pPathDotInfo->bController == false )
				{
					map<sPathDotInfo*,sDotInfo*>::iterator itr = m_DotInfoTable.find(pPathDotInfo);

					if( itr != m_DotInfoTable.end() )
					{
						sDotInfo * pDotInfo = itr->second;
						sprintf(szText, "±æÀÌ : %.2f", pPathDotInfo->fLength);
						ConvertAnsiStringToWideCch(wszText, szText, 128);
						pDotInfo->pStatic_Length->SetText(wszText);
					}
				}
			}
		}
	}
}

void			CUIComponent_PathObject::OnDragReceive_GameObject(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender)
{
	switch(eSender)
	{
	case eDragObjectSender_Project_File:
		{
			IPathObject * pPathObject = dynamic_cast<IPathObject*>(m_pGameObject);

			if( pPathObject != NULL && pSrcDragItem != NULL && pDestDragItem != NULL )
			{
				CUIListViewItem * pListViewItem = static_cast<CUIListViewItem*>(pSrcDragItem);
				CUIDragItemSprite * pDragItemSprite = static_cast<CUIDragItemSprite*>(pDestDragItem);

				if( pListViewItem != NULL && pDragItemSprite != NULL )
				{
					switch(pDragItemSprite->GetDragObjectReceiver())
					{
					case eDragObjectReceiver_Project_File:
						{
							CParameta_String * pParameta_String = dynamic_cast<CParameta_String*>(pListViewItem->GetParameta());

							if( pParameta_String != NULL )
							{
								string strOrigToken = CGwStrTok::GetTokenFormat();
								CGwStrTok::SetTokenFormat(".");
								CGwStrTok::SetString(pListViewItem->GetName().c_str());
								CGwStrTok::SetTokenFormat(strOrigToken.c_str());

								int nTokenNum = CGwStrTok::GetNumToken();
								string strExtent = CGwStrTok::GetToken(nTokenNum-1);

								if( strExtent == "gb" || strExtent == "GB" )
								{
									IFileInfo kFileInfo;
									kFileInfo.m_strPath = pParameta_String->GetParameta_String();
									kFileInfo.m_strPath += "\\";
									kFileInfo.m_strFileName = pListViewItem->GetName();

									vector<ITextureFileInfo>  Output_MeshTextureInfoList;
									IGwStaticObject	* pStaticObject = IResourceHelper::CreateStaticObject_AutoTexture(kFileInfo.m_strPath, kFileInfo.m_strFileName, Output_MeshTextureInfoList);
									
									//ISkinMesh * pSkinMesh = new ISkinMesh(pListViewItem->GetName().c_str(), pStaticObject, kFileInfo, Output_MeshTextureInfoList);
									//pStaticObject->Release();

									IType * pType = ITypeManager::GetInstance()->FindType("ISkinMesh");
									ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pType->CreateInstance());
									pSkinMesh->SetName(pListViewItem->GetName().c_str());
									pSkinMesh->SetMeshObject(pStaticObject, kFileInfo, Output_MeshTextureInfoList);	pStaticObject->Release();

									pPathObject->SetAttachGameObject(pSkinMesh); pSkinMesh->Release();

									wchar_t wszFileName[128] = { 0, };
									ConvertAnsiStringToWideCch(wszFileName, kFileInfo.m_strFileName.c_str(), 128);
									m_pStatic_GameObjectName->SetText(wszFileName);
									m_pButton_Remove->SetVisible();
								}
								else if( strExtent == "es" || strExtent == "es" )
								{
									IFileInfo kFileInfo;
									kFileInfo.m_strPath = pParameta_String->GetParameta_String();
									kFileInfo.m_strPath += "\\";
									kFileInfo.m_strFileName = pListViewItem->GetName();

									IGameObject * pGameObject = IGameObjectMgr::GetInstance()->LoadFile(kFileInfo.m_strPath.c_str(), kFileInfo.m_strFileName.c_str());
									pPathObject->SetAttachGameObject(pGameObject); pGameObject->Release();

									wchar_t wszFileName[128] = { 0, };
									ConvertAnsiStringToWideCch(wszFileName, kFileInfo.m_strFileName.c_str(), 128);
									m_pStatic_GameObjectName->SetText(wszFileName);
									m_pButton_Remove->SetVisible();
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