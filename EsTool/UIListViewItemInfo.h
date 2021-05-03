#pragma once

enum eListViewItem_State
{
	eListViewItem_State_None,
	eListViewItem_State_Normal,
	eListViewItem_State_Select,
	eListViewItem_State_Edit,
};

class CUIListViewItemInfo : public IGwRefObject
{
public:
	CUIListViewItemInfo(const char * pszName, IGwRefObject * pParameta);
	~CUIListViewItemInfo(void);

	void				SetName(const char * pszName);
	const char		*	GetName();

	void				SetParameta(IGwRefObject * pParameta);
	IGwRefObject	*	GetParameta();

	void				SetListViewItem_State(eListViewItem_State eState);
	eListViewItem_State	GetListViewItem_State();

private:

	string				m_strName;
	IGwRefObject	*	m_pParameta;
	eListViewItem_State	m_eState;
};
