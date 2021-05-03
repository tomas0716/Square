#pragma once

class CUIComponent : public IGwRefObject, public ITypeObject
{
public:
	CUIComponent(void);
	~CUIComponent(void);
	
	virtual void		SetGameObject(IGameObject * pGameObject);
	virtual void		SetComponent(IComponent * pComponent);
	virtual void		Initialize()								{ }
	virtual int			GetHeight()									{ return m_fHeight; }

	virtual void		OnResetDevice()								{ }
	virtual void		OnLostDevice()								{ }
	virtual void		Update(float fDeltaTime)					{ }
	virtual void		Render(float fDeltaTime)					{ }

	IGwUIDummyCtrl	*	GetBaseControl()							{ return m_pBaseControl; }

	virtual void		SetLocalPos(GwVector2 vPos)					{ m_pBaseControl->SetLocalPos(vPos); }
	virtual GwVector2	GetLocalPos()								{ return *m_pBaseControl->GetLocalPos(); }

protected:

	IGameObject		*	m_pGameObject;
	IComponent		*	m_pComponent;
	IGwUIDummyCtrl	*	m_pBaseControl;
	float				m_fHeight;
};
