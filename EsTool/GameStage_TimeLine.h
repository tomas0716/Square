#pragma once

class CGameStage_TimeLine : public CGameStage_DialogBase
{
	I_DECLARE_DYNCREATE(CGameStage_TimeLine);
protected:
	CGameStage_TimeLine(void);
	~CGameStage_TimeLine(void);

public:
	virtual void		OnCreateDevice();
	virtual void		OnDestroyDevice();

	virtual void		Update(float fDeltaTime);

protected:
	virtual void		OnVisibleChanged();

private:

	IGwUIDialog		*	m_pDialog;
};