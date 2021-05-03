#include "stdafx.h"
#include "PerspectiveCamera.h"

//#define _MOUSE_WHEEL_SMOOTH

CPerspectiveCamera::CPerspectiveCamera() 
: m_vPrevMousePos(GwVector2(0,0)), m_vMousePos(GwVector2(0,0)), m_eMouseEventType(eMouseEvent_None), m_vPosition(GwVector(0,0,0)), m_vTarget(GwVector(0,0,0)),
  m_vDirection(GwVector(0,0,0)), m_vRightDirection(GwVector(0,0,0)), m_IsProcessLock(false)
{
}

CPerspectiveCamera::~CPerspectiveCamera()
{
	m_pCamera->Release();
	m_pSceneObject_Camera->Release();
	m_pWheelMove->Release();
}

void			CPerspectiveCamera::OnInitialize(int nWidth, int nHeight)
{
	for( int i = 0; i < eMovement_Max; ++i )
		m_fMovement[i] = 0;

	m_pCamera = Gw::GetCameraMgr()->CreateCamera();
	m_pCamera->SetScreenWidthHeight(nWidth, nHeight);
	m_pCamera->SetNearClip(100.f);
	m_pCamera->SetFarClip(60000.f);
	m_pCamera->SetProjectionType(GWPROJ_PERSPECTIVE);

	IGwStaticObject* pStaticObj = Gw::GetScnMgr()->CreateStaticObject();
	pStaticObj->AddChild(m_pCamera);	
	m_pSceneObject_Camera = Gw::GetScnMgr()->CreateScnObject();	
	m_pSceneObject_Camera->AddStaticObject(pStaticObj);		pStaticObj->Release();

	m_fWheel = 0;
	m_fDelta = 0;
	m_pWheelMove = Gw::GetEventTransformMgr()->CreateTransformer_Scalar(0);

	Reset();
}

void 			CPerspectiveCamera::Reset()
{
	m_vMousePos.x = m_vPrevMousePos.x = 0;
	m_vMousePos.y = m_vPrevMousePos.y = 0;

	m_fMovement[eMovement_Wheel] 	= 4000.0f;
	m_fMovement[eMovement_Pitch] 	= 30.0f;
	m_fMovement[eMovement_Yaw] 		= 180.0f;

	m_fMovement[eMovement_MovingX]	= 0.0f;
	m_fMovement[eMovement_MovingY]	= 0.0f;
	m_fMovement[eMovement_MovingZ]	= 0.0f;	

	UpdateCamera();
}

void 			CPerspectiveCamera::Update (float fDeltaTime) 
{
	if( m_IsProcessLock == true ) return;

	if( IGwUIDialog::GetCurrMouseOverDialog() == NULL && CUIDragObject::GetInstance()->IsDraging() == false ) 
	{
		OnMouseProcess();

		m_pSceneObject_Camera->Update(fDeltaTime);
	}
	else 
	{
		OnMouseProcess_Release();
	} 

#ifdef _MOUSE_WHEEL_SMOOTH
	m_pWheelMove->Update(fDeltaTime);
	float fCurrTime = m_pWheelMove->GetCurrElapsedTime();
	if( fCurrTime <= m_fMoveTime )
	{
		m_fMovement[eMovement_Wheel] += (m_fDelta * fDeltaTime * fDeltaTime) / m_fMoveTime;
	}
	//m_fDelta = fDelta + m_fWheel;
	//m_fMoveTime = fRenameTime + (float)(sDelta * 0.1f);


	//m_fWheel = m_pWheelMove->GetCurrScalar();
	//m_fMovement[eMovement_Wheel] += m_fWheel;

	UpdateCamera();
#endif
}

void			CPerspectiveCamera::Render(float fDeltaTime)
{
	m_pSceneObject_Camera->Render(fDeltaTime);
}

void 			CPerspectiveCamera::UpdateCamera()
{
	Check();

	float fDeltaX, fDeltaY, fDeltaZ;

	fDeltaX = m_fMovement[eMovement_Wheel] * sinf(D3DXToRadian(m_fMovement[eMovement_Yaw])) * cosf(D3DXToRadian(m_fMovement[eMovement_Pitch]));
	fDeltaY = m_fMovement[eMovement_Wheel] * sinf(D3DXToRadian(m_fMovement[eMovement_Pitch]));
	fDeltaZ = m_fMovement[eMovement_Wheel] * cosf(D3DXToRadian(m_fMovement[eMovement_Yaw])) * cosf(D3DXToRadian(m_fMovement[eMovement_Pitch]));		

	m_vTarget 	= GwVector(m_fMovement[eMovement_MovingX], m_fMovement[eMovement_MovingY], m_fMovement[eMovement_MovingZ]);
	m_vPosition = GwVector(fDeltaX + m_fMovement[eMovement_MovingX], fDeltaY + m_fMovement[eMovement_MovingY], fDeltaZ + m_fMovement[eMovement_MovingZ]);

	m_vDirection = m_vTarget - m_vPosition;
	GwVector vUp = GwVector(0,1,0);
	D3DXVec3Cross(&m_vRightDirection, &m_vDirection, &vUp);

	GwMatA matWorld;

	D3DXMatrixLookAtLH(&matWorld,&m_vPosition,&m_vTarget,&D3DXVECTOR3(0.0f, 1.0f, 0.0f) ); 
	D3DXMatrixTranspose( &matWorld, &matWorld ); 
	matWorld._41 = m_vPosition.x;
	matWorld._42 = m_vPosition.y;
	matWorld._43 = m_vPosition.z;

	m_pCamera->SetWorldMatrix(matWorld);
}

void 			CPerspectiveCamera::OnMouseProcess()
{
	if(Gw::GetKeyMapper()->IsKeyDown(VK_LBUTTON) == TRUE && m_eMouseEventType != eMouseEvent_LButtonDown)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnLButtonDown(vCurPos);
	}
	else if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnLButtonUp(vCurPos);
	}
	else if( Gw::GetKeyMapper()->IsKeyDown(VK_RBUTTON) == TRUE && m_eMouseEventType != eMouseEvent_RButtonDown)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnRButtonDown(vCurPos);
	}
	else if( Gw::GetKeyMapper()->IsKeyUp(VK_RBUTTON) == TRUE)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnRButtonUp(vCurPos);
	}
	else if( Gw::GetKeyMapper()->IsKeyDown(VK_MBUTTON) == TRUE && m_eMouseEventType != eMouseEvent_MButtonDown)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnMButtonDown(vCurPos);
	}
	else if( Gw::GetKeyMapper()->IsKeyUp(VK_MBUTTON) == TRUE)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnMButtonUp(vCurPos);
	}
	else 
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnMouseMove(vCurPos);
	}

	if (Gw::GetKeyMapper()->GetWheelRotationCount() != 0)
	{
		OnMouseWheel(Gw::GetKeyMapper()->GetWheelRotationCount());
	}
}

void 			CPerspectiveCamera::OnMouseProcess_Release()
{
	if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnLButtonUp(vCurPos);
	}
	if( Gw::GetKeyMapper()->IsKeyUp(VK_RBUTTON) == TRUE)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnRButtonUp(vCurPos);
	}
	else if( Gw::GetKeyMapper()->IsKeyUp(VK_MBUTTON) == TRUE)
	{
		GwVector2 vCurPos;
		vCurPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
		vCurPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
		OnMButtonUp(vCurPos);
	}
}

void			CPerspectiveCamera::Check()
{
	if( m_fMovement[eMovement_Pitch] > (180 * 0.4999f) )
		m_fMovement[eMovement_Pitch] = 180 * 0.4999f;
	else if( m_fMovement[eMovement_Pitch] < -(180 * 0.4999f) )
		m_fMovement[eMovement_Pitch] = -(180 * 0.4999f);
}

void 			CPerspectiveCamera::OnLButtonDown(GwVector2 point)
{
	m_vMousePos = point;
	m_eMouseEventType = eMouseEvent_LButtonDown;
}

void 			CPerspectiveCamera::OnLButtonUp(GwVector2 point)
{
	m_eMouseEventType = eMouseEvent_None;
}

void 			CPerspectiveCamera::OnRButtonDown(GwVector2 point)
{
	m_vMousePos = point;
	m_eMouseEventType = eMouseEvent_RButtonDown;
}

void 			CPerspectiveCamera::OnRButtonUp(GwVector2 point)
{
	m_eMouseEventType = eMouseEvent_None;
}


void			CPerspectiveCamera::OnMButtonDown(GwVector2 point)
{
	m_vMousePos = point;
	m_eMouseEventType = eMouseEvent_MButtonDown;
}

void			CPerspectiveCamera::OnMButtonUp(GwVector2 point)
{
	m_eMouseEventType = eMouseEvent_None;
}

void 			CPerspectiveCamera::OnMouseMove(GwVector2 point)
{
	if( m_eMouseEventType == eMouseEvent_LButtonDown )
	{
		m_vPrevMousePos = m_vMousePos;
		m_vMousePos = point;

		GwVector vRightDirection	= m_vRightDirection;
		GwVector vDirection			= m_vDirection;

		D3DXVec3Normalize(&vRightDirection, &vRightDirection);
		D3DXVec3Normalize(&vDirection, &vDirection);

		GwVector vDeltaX = vRightDirection * (m_vMousePos.x - m_vPrevMousePos.x);
		GwVector vDeltaY = vDirection * (m_vMousePos.y - m_vPrevMousePos.y);

		m_fMovement[eMovement_MovingX] += (vDeltaX.x + vDeltaY.x) * 2.4f;
		m_fMovement[eMovement_MovingZ] += (vDeltaX.z + vDeltaY.z) * 2.4f;

		UpdateCamera();
	}

	if( m_eMouseEventType == eMouseEvent_RButtonDown )
	{
		m_vPrevMousePos = m_vMousePos;
		m_vMousePos = point;

		m_fMovement[eMovement_Yaw]	+= (m_vMousePos.x - m_vPrevMousePos.x) * 0.1f;
		m_fMovement[eMovement_Pitch]	-= (m_vPrevMousePos.y - m_vMousePos.y) * 0.1f;

		UpdateCamera();
	}

	if( m_eMouseEventType == eMouseEvent_MButtonDown )
	{
		m_vPrevMousePos = m_vMousePos;
		m_vMousePos = point;

		m_fMovement[eMovement_MovingY]	+= (m_vMousePos.y - m_vPrevMousePos.y) * 0.8f;

		UpdateCamera();
	}
}

void			CPerspectiveCamera::OnMouseWheel(short sDelta)
{
#ifndef _MOUSE_WHEEL_SMOOTH
	float fDelta = (float)sDelta * 30.0f;
#else
	float fDelta = (float)sDelta * 5000.0f;
#endif

	GwVector vDirection = m_vDirection;

	float fLength = D3DXVec3Length(&vDirection);

	/*if( sDelta < 0 && fLength > 10000 )
		return;
	else */if( sDelta > 0 && fLength < 0.1f )
		return;

#ifndef _MOUSE_WHEEL_SMOOTH
	m_fMovement[eMovement_Wheel] += ((float)-fDelta);
#else
	float fRenameTime = 0;
	if( m_fWheel != 0 )
		fRenameTime = m_pWheelMove->GetLastEventTime() - m_pWheelMove->GetCurrElapsedTime();
	fDelta = ((float)-fDelta);

	if( sDelta < 0 ) sDelta *= -1;

	m_fDelta = fDelta + m_fWheel;
	m_fMoveTime = fRenameTime + (float)(sDelta * 0.08f);

	m_pWheelMove->ResetEvent();
	IGwTransformEvent_Scalar * pEvent = NULL;
	pEvent = Gw::GetEventTransformMgr()->CreateEvent_Scalar(0, 0);
	m_pWheelMove->AddEvent(pEvent); pEvent->Release();
	pEvent = Gw::GetEventTransformMgr()->CreateEvent_Scalar(m_fMoveTime, 0);
	m_pWheelMove->AddEvent(pEvent); pEvent->Release();
	m_pWheelMove->Replay();
	m_pWheelMove->SetElapsedTime(0);
#endif

	UpdateCamera();
}	

void			CPerspectiveCamera::OnSizeChange(int nWidth, int nHeight)
{
	m_pCamera->SetScreenWidthHeight(nWidth, nHeight);
}

void			CPerspectiveCamera::SetTargetPos(GwVector vPos)
{
	m_fMovement[eMovement_Wheel] 	= 4000.0f;
	m_fMovement[eMovement_Pitch] 	= 30.0f;
	m_fMovement[eMovement_Yaw] 		= 180.0f;

	m_fMovement[eMovement_MovingX]	= vPos.x;
	m_fMovement[eMovement_MovingY]	= vPos.y;
	m_fMovement[eMovement_MovingZ]	= vPos.z;	

	UpdateCamera();
	m_pSceneObject_Camera->Update(0);
}
