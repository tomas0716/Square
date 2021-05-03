#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EventArg_PickingEntity

class CPickingEntity;
struct EventArg_PickingEntity
{
	CPickingEntity	*	pPickingEntity;
	IGwBaseMesh		*	pPickingBaseMesh;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPickingEntity

class CPickingEntity : public IGwRefObject
{
public:
	CPickingEntity(IGwScnObject * pScnObject, ITypeObject * pParameta);
	virtual ~CPickingEntity(void);

	void		OnOver(IGwBaseMesh * pBaseMesh);
	void		OnLeave(IGwBaseMesh * pBaseMesh);

	void		OnLButtonDown(IGwBaseMesh * pBaseMesh);
	void		OnLButtonUp(IGwBaseMesh * pBaseMesh);

	void		OnRButtonDown(IGwBaseMesh * pBaseMesh);
	void		OnRButtonUp(IGwBaseMesh * pBaseMesh);

	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_Over()			{ return m_pEvent_Over; }
	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_Leave()		{ return m_pEvent_Leave; }

	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_LButtonDown()	{ return m_pEvent_LButtonDown; }
	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_LButtonUp()	{ return m_pEvent_LButtonUp; }

	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_RButtonDown()	{ return m_pEvent_RButtonDown; }
	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_RButtonUp()	{ return m_pEvent_RButtonUp; }

	IGwScnObject	*	GetScnObject()									{ return m_pScnObject; }
	ITypeObject		*	GetParameta()									{ return m_pParameta; }

private:

	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_Over;
	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_Leave;

	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_LButtonDown;
	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_LButtonUp;

	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_RButtonDown;
	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_RButtonUp;

	IGwScnObject	*	m_pScnObject;
	ITypeObject		*	m_pParameta;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPickingMgr

class CPickingMgr : public TGwGetInstance<CPickingMgr>
{
public:
	CPickingMgr(void);
	virtual ~CPickingMgr(void);

	void				SetCamera(IGwCamera * pCamera);

	CPickingEntity	*	AddPickingEntity(IGwScnObject * pScnObject, ITypeObject * pParameta = NULL);
	void				RemovePickingEntity(IGwScnObject * pScnObject);
	void				ClearAll_PickingEntity();

	TEventDispatch<EventArg_PickingEntity>	*	GetEvent_NonePicking()	{ return m_pEvent_NonePicking; }

private:

	void				PostUpdate(const EventArg_ElapsedTime& Arg);

	IGwPick					*	m_pPick;
	vector<CPickingEntity*>		m_pPickingEntityList;
	CPickingEntity			*	m_pCurrPickingEntity;
	IGwBaseMesh				*	m_pCurrPickingBaseMesh;

	TEventDispatch<EventArg_PickingEntity>	*	m_pEvent_NonePicking;
};