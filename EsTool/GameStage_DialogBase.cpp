#include "StdAfx.h"
#include "GameStage_DialogBase.h"

CGameStage_DialogBase::CGameStage_DialogBase(void)
: m_IsVisible(true)
{
}

CGameStage_DialogBase::~CGameStage_DialogBase(void)
{
}

void		CGameStage_DialogBase::SetVisible(bool IsVisible)	
{
	m_IsVisible = IsVisible; 

	OnVisibleChanged();
}

bool		CGameStage_DialogBase::IsVisible()					
{
	return m_IsVisible; 
}