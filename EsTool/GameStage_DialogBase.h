#pragma once

class CGameStage_DialogBase : public CGameStage
{
protected:
	CGameStage_DialogBase(void);
	~CGameStage_DialogBase(void);
	
	virtual void		OnVisibleChanged() = 0;

public:

	virtual void		SetVisible(bool IsVisible);
	virtual bool		IsVisible();

protected:
	bool				m_IsVisible;
};