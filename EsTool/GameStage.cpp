#include "stdafx.h"
#include "GameStage.h"

I_IMPLEMENT_DYNCREATE(CGameStage, ISingletonObject::GetTypeModule())

CGameStage::CGameStage() : m_nOrder(0)
{
	
}

CGameStage::~CGameStage()
{

}