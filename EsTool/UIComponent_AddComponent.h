#pragma once

class CUIComponent_AddComponent : public CUIComponent
{
	I_DECLARE_DYNCREATE(CUIComponent_AddComponent);
public:
	CUIComponent_AddComponent(void);
	~CUIComponent_AddComponent(void);

	virtual void		Initialize();

	virtual void		Update(float fDeltaTime);

	void				OnButtonClick_AddComponent();
	void				OnButtonClick_AddMeshCollider();

private:

	void				OnDoubleClick_AddComponent(CUIListViewItem * pListViewItem);

	IGwUIDummyCtrl	*	m_pComponentListBaseDummy;
	CUIListView		*	m_pListView;
	bool				m_IsOpen;

	class CBtnCallback_AddComponent;
};