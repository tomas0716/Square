#pragma once

struct EventArg_GizmoMove
{
	GwVector			vPos;
	IGwRefObject	*	pObject;
};

class CGizmoObject : public TGwGetInstance<CGizmoObject>
{
private:
	enum eMoveState
	{
		eMoveState_None,
		eMoveState_X,
		eMoveState_Y,
		eMoveState_Z,
		eMoveState_XY,
		eMoveState_XZ,
		eMoveState_YZ,

		eMoveState_Max,
	};

public:
	CGizmoObject();
	~CGizmoObject();

	void				Update(float fDeltaTime);
	void				Render(float fDeltaTime);

	void				Show(GwVector vPos, IGwRefObject * pObject = NULL);
	void				Hide();

	TEventDispatch<EventArg_GizmoMove>	*	GetEvent_Move()	{ return m_pEvent_Move; }

private:

	void				FirstPickingSetting();
	bool				GetPickingPos(eMoveState eState, GwVector vCurrPos, GwVector & vResPos);

	void				OnLButtonDown_Picking(const EventArg_PickingEntity& Arg);
	void				OnLButtonUp_Picking(const EventArg_PickingEntity& Arg);

private:

	IGwScnObject	*	m_pScnObject;
	IGwRefObject	*	m_pObject;
	bool				m_IsShow;

	GwVector			m_vCurrPos;
	GwVector			m_vMoveStatePickPos;
	GwVector			m_vPosInterval;

	eMoveState			m_MoveState;
	eMoveState			m_MoveState_Plane;

	bool				m_IsValid;

	IGwPick			*	m_pPick;
	POINT				m_ptPrevMousePos;

	TEventDispatch<EventArg_GizmoMove>	*	m_pEvent_Move;
};
