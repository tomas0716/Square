#pragma once

class CGameStageMgr : public TGwGetInstance<CGameStageMgr>
{
public:
	CGameStageMgr();
	virtual ~CGameStageMgr();

	void			OnCreateDevice();
	void			OnResetDevice();
	void			OnLostDevice();
	void			OnDestroyDevice();	

	void			Update(float fDeltaTime);
	void			Render(float fDeltaTime);

	void			RemoveAll();

	template<class T>
	T			*	CreateGameStage(const char * pszClassName, int nGameStageOrder = 0);
	void			CreateGameStage(const char * pszClassName, int nGameStageOrder = 0);

	template<class T>
	T			*	FindGameStage(const char * pszClassName, int nGameStageOrder = 0);

	template<class T>
	void			RemoveGameStage(const char * pszClassName, int nGameStageOrder = 0);

private:

	void			UpdateList();
	void			MakeList();

	bool							m_IsNeedUpdate;

	vector<CGameStage*>				m_GameStageList;
	map<ULONG, vector<CGameStage*>>	m_GameStageTable;
	vector<CGameStage*>				m_RemoveGameStageList;
};

#include "GameStageMgr.inl"