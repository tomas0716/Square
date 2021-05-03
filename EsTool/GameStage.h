#pragma once

class CGameStage : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(CGameStage);
public:
	CGameStage();
	virtual ~CGameStage();

	virtual void		OnCreateDevice()			{ }
	virtual void		OnResetDevice()				{ }
	virtual void		OnLostDevice()				{ }
	virtual void		OnDestroyDevice()			{ }

	virtual void		Update(float fDeltaTime)	{ }
	virtual void		Render(float fDeltaTime)	{ }

	void				SetOrder(int nOrder)		{ m_nOrder = nOrder; }
	int					GetOrder()					{ return m_nOrder; }

private:
	int					m_nOrder;
};