#include "stdafx.h"
#include "GameStageMgr.h"

CGameStageMgr::CGameStageMgr() : m_IsNeedUpdate(false)
{
}

CGameStageMgr::~CGameStageMgr()
{
	RemoveAll();
}

void			CGameStageMgr::CreateGameStage(const char * pszClassName, int nGameStageOrder)
{
	IType * pType = ITypeManager::GetInstance()->FindType(pszClassName);
	CGameStage * pGameStage = dynamic_cast<CGameStage*>(pType->CreateInstance());

	map<ULONG, vector<CGameStage*>>::iterator itr = m_GameStageTable.find(pGameStage->GetClassHashCode());
	if( itr != m_GameStageTable.end() )
	{
		itr->second.push_back(pGameStage);
	}
	else 
	{
		vector<CGameStage*> gameStageList;
		gameStageList.push_back(pGameStage);
		m_GameStageTable.insert(map<ULONG,vector<CGameStage*>>::value_type(pGameStage->GetClassHashCode(), gameStageList));
	}

	m_GameStageList.push_back(pGameStage);

	pGameStage->OnCreateDevice();
	pGameStage->OnResetDevice();
}

void			CGameStageMgr::OnCreateDevice()
{
	for_each(m_GameStageList.begin(),m_GameStageList.end(),
		mem_fun(&CGameStage::OnResetDevice));
}

void			CGameStageMgr::OnResetDevice()
{
	for_each(m_GameStageList.begin(),m_GameStageList.end(), 
		mem_fun(&CGameStage::OnResetDevice));
}

void			CGameStageMgr::OnLostDevice()
{
	for_each(m_GameStageList.begin(),m_GameStageList.end(), 
		mem_fun(&CGameStage::OnLostDevice));
}

void			CGameStageMgr::OnDestroyDevice()
{
	for_each(m_GameStageList.begin(),m_GameStageList.end(), 
		mem_fun(&CGameStage::OnDestroyDevice));
}

void			CGameStageMgr::Update(float fDeltaTime)
{
	if( m_IsNeedUpdate == true )	UpdateList();
	m_IsNeedUpdate = false;

	for_each(m_GameStageList.begin(),m_GameStageList.end(), 
		bind2nd(mem_fun(&CGameStage::Update),fDeltaTime) );
}

void			CGameStageMgr::Render(float fDeltaTime)
{
	for_each(m_GameStageList.begin(),m_GameStageList.end(), 
		bind2nd(mem_fun(&CGameStage::Render),fDeltaTime) );
}

void			CGameStageMgr::UpdateList()
{
	vector<CGameStage*>::iterator itr_remove = m_RemoveGameStageList.begin();

	for( ; itr_remove != m_RemoveGameStageList.end(); ++itr_remove )
	{
		CGameStage * pRemoveGameStage = (*itr_remove);

		map<ULONG, vector<CGameStage*>>::iterator itr = m_GameStageTable.begin();

		for( ; itr != m_GameStageTable.end(); ++itr )
		{
			vector<CGameStage*>::iterator itr_gs = itr->second.begin();

			for( ; itr_gs != itr->second.end(); )
			{
				CGameStage * pFindGameStage = (*itr_gs);
				if( pRemoveGameStage == pFindGameStage )
				{
					pFindGameStage->OnLostDevice();
					pFindGameStage->OnDestroyDevice();
					pFindGameStage->Release();
					itr_gs = itr->second.erase(itr_gs);
				}
				else 
				{
					++itr_gs;
				}
			}
		}
	}

	m_RemoveGameStageList.clear();

	MakeList();
}

void			CGameStageMgr::MakeList()
{
	m_GameStageList.clear();

	map<ULONG, vector<CGameStage*>>::iterator itr = m_GameStageTable.begin();

	for( ; itr != m_GameStageTable.end(); ++itr )
	{
		vector<CGameStage*>::iterator itr_gs = itr->second.begin();

		for( ; itr_gs != itr->second.end(); ++itr_gs)
		{
			CGameStage * pFindGameStage = (*itr_gs);
			m_GameStageList.push_back(pFindGameStage);
		}
	}
}

void			CGameStageMgr::RemoveAll()
{
	map<ULONG, vector<CGameStage*>>::iterator itr = m_GameStageTable.begin();

	for( ; itr != m_GameStageTable.end(); ++itr )
	{
		vector<CGameStage*>::iterator itr_gs = itr->second.begin();

		for( ; itr_gs != itr->second.end(); ++itr_gs)
		{
			CGameStage * pGameStage = (*itr_gs);
			pGameStage->OnLostDevice();
			pGameStage->OnDestroyDevice();
			pGameStage->Release();
		}
	}

	m_GameStageTable.clear();
	m_GameStageList.clear();
	m_RemoveGameStageList.clear();
}