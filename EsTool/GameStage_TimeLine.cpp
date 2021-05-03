#include "StdAfx.h"
#include "GameStage_TimeLine.h"

I_IMPLEMENT_DYNCREATE(CGameStage_TimeLine, ISingletonObject::GetTypeModule())

CGameStage_TimeLine::CGameStage_TimeLine(void)
{
}

CGameStage_TimeLine::~CGameStage_TimeLine(void)
{
}

void		CGameStage_TimeLine::OnCreateDevice()
{
	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();
	Gw::GetUIControlMgr()->AddDialog(m_pDialog);
}

void		CGameStage_TimeLine::OnDestroyDevice()
{
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();
}

void		CGameStage_TimeLine::Update(float fDeltaTime)
{

}

void		CGameStage_TimeLine::OnVisibleChanged()
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