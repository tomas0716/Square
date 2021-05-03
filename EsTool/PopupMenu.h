#pragma once

struct EventArg_PopupMenu_Click
{
	eObjectEditState	eEditState;
};

class CPopupMenu : public IGwRefObject
{
public:
	CPopupMenu(ULONG nEditStateOption);
	~CPopupMenu();

	void			Update(float fDeltaTime);

	void			ShowPopup(GwVector2 vPos);
	void			HIdePopup();

	TEventDispatch<EventArg_PopupMenu_Click>	*	GetEvent_Click()			{ return m_pEvent_Click; }

private:

	void			OnButtonClick(eObjectEditState eEditState);

	IGwUIDialog				*	m_pDialog;

	TEventDispatch<EventArg_PopupMenu_Click>	*	m_pEvent_Click;

	class	CBtnCallback_Menu;
};
