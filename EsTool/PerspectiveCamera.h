#pragma once

class CPerspectiveCamera : public TGwGetInstance<CPerspectiveCamera>
{
public:
	CPerspectiveCamera();
	virtual ~CPerspectiveCamera();

	void			OnInitialize(int nWidth, int nHeight);
	void 			Reset();
	void			Update (float fDeltaTime);
	void			Render(float fDeltaTime);
	void 			OnLButtonDown(GwVector2 point);
	void 			OnLButtonUp(GwVector2 point);
	void 			OnRButtonDown(GwVector2 point);
	void 			OnRButtonUp(GwVector2 point);
	void			OnMButtonDown(GwVector2 point);
	void			OnMButtonUp(GwVector2 point);
	void 			OnMouseMove(GwVector2 point);
	void			OnMouseWheel(short sDelta);

	void			OnSizeChange(int nWidth, int nHeight);

	void			SetTargetPos(GwVector vPos);

	IGwCamera	*	GetCamera()						{ return m_pCamera; }

	GwVector		GetCameraEye()					{ return m_vPosition; }
	GwVector		GetCameraTarget()				{ return m_vTarget; }

	GwVector		GetDirection()					{ return m_vDirection; }
	GwVector		GetRightDirection()				{ return m_vRightDirection; }

	void			SetProcessLock(bool IsLock)		{ m_IsProcessLock = IsLock; m_eMouseEventType = eMouseEvent_None; }

private:

	void 			UpdateCamera();
	void			OnKeyProcess();
	void 			OnMouseProcess();
	void 			OnMouseProcess_Release();
	void			Check();

private:

	IGwScnObject	*	m_pSceneObject_Camera;
	IGwCamera		*	m_pCamera;

	GwVector2 			m_vPrevMousePos;
	GwVector2 			m_vMousePos;

	enum	eMovementType
	{
		eMovement_Yaw = 0,
		eMovement_Pitch,

		eMovement_Wheel,

		eMovement_MovingX,
		eMovement_MovingY,
		eMovement_MovingZ,

		eMovement_Max,
	};

	float				m_fMovement[eMovement_Max];

	IGwEventTransformer_Scalar	*	m_pWheelMove;
	float	m_fWheel;
	float	m_fDelta;
	float	m_fMoveTime;

	enum	eMouseEventType
	{
		eMouseEvent_None = 0,
		eMouseEvent_LButtonDown,
		eMouseEvent_RButtonDown,
		eMouseEvent_MButtonDown,
	};

	eMouseEventType		m_eMouseEventType;

	GwVector			m_vPosition;
	GwVector			m_vTarget;

	GwVector			m_vDirection;
	GwVector			m_vRightDirection;

	bool				m_IsProcessLock;
};