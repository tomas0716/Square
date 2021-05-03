#pragma once

class CGameStage_Main : public IGwGameStage
{
public:
	CGameStage_Main();
	~CGameStage_Main();

	virtual void		OnCreateDevice();
	virtual void		OnResetDevice();
	virtual void		OnLostDevice();
	virtual void		OnDestroyDevice();

	virtual	BOOL		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);

private:
	CUIHelper	*	m_pUIHelper;
	IGwLight	*	m_pRndLight;
};
