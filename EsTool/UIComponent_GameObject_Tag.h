#pragma once

class CUIComponent_GameObject_Tag : public CUIComponent
{
	I_DECLARE_DYNCREATE(CUIComponent_GameObject_Tag);
public:
	CUIComponent_GameObject_Tag(void);
	~CUIComponent_GameObject_Tag(void);

	virtual void	Initialize();

	virtual void	Update(float fDeltaTime);

	void			OnButtonClick_Root();
	void			OnButtonClick_Item(INode * pNode);
private:

	bool				m_IsOpen;

	IGwUIStatic		*	m_pStatic_Sel;

	class CItem;
	vector<CItem*>		m_ItemList;

	class CBtnCallback_Root;
};