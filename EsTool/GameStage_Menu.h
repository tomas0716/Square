#pragma once

class CGameStage_Menu : public CGameStage_DialogBase
{
	I_DECLARE_DYNCREATE(CGameStage_Menu);
protected:
	CGameStage_Menu(void);
	~CGameStage_Menu(void);

public:
	void				OnCreateDevice();
	void				OnDestroyDevice();

	void				Update(float fDeltaTime);
	void				Render(float fDeltaTime);

	void				OnButtonClick(ULONG uHashCode);

	void				OnButtonClick_New();
	void				OnButtonClick_Open();
	void				OnButtonClick_Save();
	void				OnButtonClick_Exit();

	void				OnButtonClick_EmptyGameObject();
	void				OnButtonClick_Character();
	void				OnButtonClick_ParticleObject();
	void				OnButtonClick_DynamicObject();
	void				OnButtonClick_StaticObject();
	void				OnButtonClick_PathObject();
	void				OnButtonClick_CentripetalForce();
	void				OnButtonClick_CentrifugalForce();

	void				OnButtonClick_Hierarchy();
	void				OnButtonClick_Project();
	void				OnButtonClick_Inspector();
	void				OnButtonClick_TimeLine();

	void				OnButtonClick_FrameRender();
	void				OnButtonClick_Collider_FrameRender();
	void				OnButtonClick_PathDotRender();

protected:
	virtual void		OnVisibleChanged();

private:

	void				CreateMenu();
	void				RecursiveShortKeyCheck(INode * pNode, char sChar);
	void				RecursiveShortKeyCheck_FSysKey(INode * pNode, const char * pszFSysKey);

	IGwUIDialog		*	m_pDialog;
	IGwUIButton		*	m_pButton;

	class CMenuItem;
	vector<CMenuItem*>	m_MenuItemList;

	typedef void (CGameStage_Menu::*ClassProc)();
	map<ULONG,ClassProc>		m_MenuClassProcTable;

	bool				m_bCtrlKeyDown;
};