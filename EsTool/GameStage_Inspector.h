#pragma once

class CGameStage_Inspector : public CGameStage_DialogBase
{
	I_DECLARE_DYNCREATE(CGameStage_Inspector);
protected:
	CGameStage_Inspector(void);
	~CGameStage_Inspector(void);

public:
	virtual void		OnCreateDevice();
	virtual void		OnResetDevice();
	virtual void		OnLostDevice();
	virtual void		OnDestroyDevice();

	virtual void		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);

	void				SetGameObject(IGameObject * pGameObject);

	void				UpdateScroll_V();

	void				OnCheckBox_Active(bool IsChecked);

protected:

	void				OnNotification_InspectorComponent_Height_Change(NotificationArg * p);
	void				OnNotification_InspectorComponent_Reflesh(NotificationArg * p);

	void				RemoveComponentAll();
	virtual void		OnVisibleChanged();

private:

	IGwUIDialog				*	m_pDialog;
	IGwUIDummyCtrl			*	m_pComponent_DummyCtrl;

	IGwUIScroll_V			*	m_pScroll_V;
	IGwUICheckbox			*	m_pCheckBox_Active;

	IGameObject				*	m_pGameObject;

	vector<CUIComponent*>		m_ComponentList;

	map<string,string>			m_ComponentClassLinkTable;

	class CScrollCallback_V;
	class CCheckboxCallback_Active;
};