

template<class T>
T			*	CGameStageMgr::CreateGameStage(const char * pszClassName, int nGameStageOrder)
{
	IType * pType = ITypeManager::GetInstance()->FindType(pszClassName);
	T * pGameStage = dynamic_cast<T*>(pType->CreateInstance());

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

	return pGameStage;
}

template<class T>
T			*	CGameStageMgr::FindGameStage(const char * pszClassName, int nGameStageOrder)
{
	ULONG uHashCode = IHashObject::GetInstance()->GetHashCode(pszClassName);

	map<ULONG, vector<CGameStage*>>::iterator itr = m_GameStageTable.find(uHashCode);
	if( itr != m_GameStageTable.end() )
	{
		vector<CGameStage*>::iterator itr_find = itr->second.begin();

		for( ; itr_find != itr->second.end(); ++itr_find )
		{
			CGameStage * pGameStage = (*itr_find);
			if( pGameStage->GetOrder() == nGameStageOrder )
			{
				return (T*)pGameStage;
			}
		}
	}

	return NULL;
}

template<class T>
void			CGameStageMgr::RemoveGameStage(const char * pszClassName, int nGameStageOrder)
{
	T * pGameStage = FindGameStage<T>(pszClassName, nGameStageOrder);

	if( pGameStage != NULL )
	{
		m_IsNeedUpdate = true;
		m_RemoveGameStageList.push_back(pGameStage);
	}
}
