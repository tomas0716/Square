#include "StdAfx.h"
#include "GizmoObject.h"

#include "time.h"

CGizmoObject::CGizmoObject() 
: m_IsShow(false), m_MoveState(eMoveState_None), m_MoveState_Plane(eMoveState_None), m_vCurrPos(GwVector(0,0,0)), m_vMoveStatePickPos(GwVector(0,0,0)),
  m_IsValid(false), m_vPosInterval(GwVector(0,0,0))
{
	clock_t sk = clock();

	m_ptPrevMousePos.x = 0;
	m_ptPrevMousePos.y = 0;

	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();

	IGwStaticObject * pStaticObject = IResourceHelper::CreateStaticObject("EsTool_Resources\\Object\\Mesh\\", "EsTool_Resources\\Object\\Texture\\", "gizmo.gb");
	m_pScnObject->AddStaticObject(pStaticObject);	pStaticObject->Release();

	m_pObject = new IGwRefObject();

	CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(m_pScnObject);
	pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CGizmoObject", this, &CGizmoObject::OnLButtonDown_Picking);
	pPickingEntity->GetEvent_LButtonUp()->AddEventHandler("CGizmoObject", this, &CGizmoObject::OnLButtonUp_Picking);


	m_pEvent_Move	= new TEventDispatch< EventArg_GizmoMove >();

	m_pPick = new IGwPick(CPerspectiveCamera::GetInstance()->GetCamera());
}

CGizmoObject::~CGizmoObject()
{
	if( CPickingMgr::GetInstance() != NULL )
		CPickingMgr::GetInstance()->RemovePickingEntity(m_pScnObject);

	m_pScnObject->Release();
	m_pObject->Release();

	m_pEvent_Move->Dispose();
	m_pEvent_Move->Release();

	m_pPick->Release();
}

void				CGizmoObject::Update(float fDeltaTime)
{
	if( m_IsShow == true && m_IsValid == true )
	{
		GwVector vPickPos;

		if( GetPickingPos(m_MoveState_Plane, m_vMoveStatePickPos, vPickPos) == true )
		{
			switch(m_MoveState)
			{
			case eMoveState_X:	vPickPos.y = m_vMoveStatePickPos.y; vPickPos.z = m_vMoveStatePickPos.z;	break;
			case eMoveState_Y:	vPickPos.x = m_vMoveStatePickPos.x; vPickPos.z = m_vMoveStatePickPos.z;	break;
			case eMoveState_Z:	vPickPos.x = m_vMoveStatePickPos.x; vPickPos.y = m_vMoveStatePickPos.y;	break;
			}

			GwVector vInterval = vPickPos - m_vMoveStatePickPos;

			if( vInterval != GwVector(0,0,0) )
			{
				m_vCurrPos += vInterval;

				EventArg_GizmoMove Arg;
				Arg.vPos = m_vCurrPos;
				Arg.pObject = m_pObject;
				m_pEvent_Move->OnEvent(Arg);
			}

			m_vMoveStatePickPos = vPickPos;
		}
	}

	if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE )
	{
		m_MoveState = eMoveState_None;
		m_IsValid = false;

		CPerspectiveCamera::GetInstance()->SetProcessLock(false);
	}

	GwVector vCameraPos = CPerspectiveCamera::GetInstance()->GetCameraEye();
	GwVector vDistance = vCameraPos - m_vCurrPos;
	float fScale = D3DXVec3Length(&vDistance) / 260;

	GwMatA matWorld;
	D3DXMatrixScaling(&matWorld, fScale, fScale, fScale);
	matWorld._41 = m_vCurrPos.x;
	matWorld._42 = m_vCurrPos.y;
	matWorld._43 = m_vCurrPos.z;

	m_pScnObject->SetWorldMatrix(matWorld);

	m_pScnObject->Update(fDeltaTime);
}

void				CGizmoObject::Render(float fDeltaTime)
{
	if( m_IsShow == true )
	{
		Gw::GetRenderer()->AddScnObject(m_pScnObject);
	}
}

void				CGizmoObject::Show(GwVector vPos, IGwRefObject * pObject)
{
	m_IsShow = true;
	m_vCurrPos = vPos;
	m_pScnObject->SetWorldPos(vPos);
}

void				CGizmoObject::Hide()
{
	m_IsShow = false;
}

void				CGizmoObject::FirstPickingSetting()
{
	m_IsValid = true;

	switch(m_MoveState)
	{
	case eMoveState_X:
		{
			if( GetPickingPos(eMoveState_XY, m_vCurrPos, m_vMoveStatePickPos) == false )
			{
				if( GetPickingPos(eMoveState_XZ, m_vCurrPos, m_vMoveStatePickPos) == false )
				{
					m_IsValid = false;
				}
				else 
				{
					m_MoveState_Plane = eMoveState_XZ;
				}
			}
			else 
			{
				m_MoveState_Plane = eMoveState_XY;
			}
		}
		break;

	case eMoveState_Y:
		{
			if( GetPickingPos(eMoveState_XY, m_vCurrPos, m_vMoveStatePickPos) == false )
			{
				if( GetPickingPos(eMoveState_YZ, m_vCurrPos, m_vMoveStatePickPos) == false )
				{
					m_IsValid = false;
				}
				else 
				{
					m_MoveState_Plane = eMoveState_YZ;
				}
			}
			else 
			{
				m_MoveState_Plane = eMoveState_XY;
			}
		}
		break;

	case eMoveState_Z:
		{
			if( GetPickingPos(eMoveState_XZ, m_vCurrPos, m_vMoveStatePickPos) == false )
			{
				if( GetPickingPos(eMoveState_YZ, m_vCurrPos, m_vMoveStatePickPos) == false )
				{
					m_IsValid = false;
				}
				else 
				{
					m_MoveState_Plane = eMoveState_YZ;
				}
			}
			else 
			{
				m_MoveState_Plane = eMoveState_XZ;
			}
		}
		break;

	case eMoveState_XY:
		{
			m_MoveState_Plane = eMoveState_XY;

			if( GetPickingPos(eMoveState_XY, m_vCurrPos, m_vMoveStatePickPos) == false )
			{
				m_IsValid = false;
			}
		}
		break;

	case eMoveState_XZ:
		{
			m_MoveState_Plane = eMoveState_XZ;

			if( GetPickingPos(eMoveState_XZ, m_vCurrPos, m_vMoveStatePickPos) == false )
			{
				m_IsValid = false;
			}
		}
		break;

	case eMoveState_YZ:
		{
			m_MoveState_Plane = eMoveState_YZ;

			if( GetPickingPos(eMoveState_YZ, m_vCurrPos, m_vMoveStatePickPos) == false )
			{
				m_IsValid = false;
			}
		}
		break;
	}
}

bool				CGizmoObject::GetPickingPos(eMoveState eState, GwVector vCurrPos, GwVector & vResPos)
{
	GwVector vPlane01, vPlane02, vPlane03, vPlane04;

	switch(eState)
	{
	case eMoveState_XY:
		{
			vPlane01 = GwVector(-100000000.0f, 100000000.0f, 0) + vCurrPos;
			vPlane02 = GwVector(100000000.0f, 100000000.0f, 0) + vCurrPos;
			vPlane03 = GwVector(100000000.0f, -100000000.0f, 0) + vCurrPos;
			vPlane04 = GwVector(-100000000.0f, -100000000.0f, 0) + vCurrPos;
		}
		break;
	case eMoveState_XZ:
		{
			vPlane01 = GwVector(-100000000.0f, 0, 100000000.0f) + vCurrPos;
			vPlane02 = GwVector(100000000.0f, 0, 100000000.0f) + vCurrPos;
			vPlane03 = GwVector(100000000.0f, 0, -100000000.0f) + vCurrPos;
			vPlane04 = GwVector(-100000000.0f, 0, -100000000.0f) + vCurrPos;
		}
		break;
	case eMoveState_YZ:
		{
			vPlane01 = GwVector(0, -100000000.0f, 100000000.0f) + vCurrPos;
			vPlane02 = GwVector(0, 100000000.0f, 100000000.0f) + vCurrPos;
			vPlane03 = GwVector(0, 100000000.0f, -100000000.0f) + vCurrPos;
			vPlane04 = GwVector(0, -100000000.0f, -100000000.0f) + vCurrPos;
		}
		break;
	}

	POINT	vCursorPos = Gw::GetKeyMapper()->GetCursorPos(Gw::GethWnd());
	m_pPick->Update(vCursorPos.x, vCursorPos.y);

	GwVector	*vRayOrg, *vRayDir;
	vRayOrg	= const_cast<GwVector*>(m_pPick->GetRayPos());
	vRayDir	= const_cast<GwVector*>(m_pPick->GetRayDir());

	GwVector vHit(0.f, 0.f, 100000000.0f);

	float fTime = GW_FLTMAX;
	bool IsIntersection = false;

	if( RayTriangle(vRayOrg, vRayDir, &vPlane01, &vPlane02, &vPlane03, &fTime) == TRUE )
	{
		if( fTime > 0 )
		{
			IsIntersection = true;
		}
	}
	else if( RayTriangle(vRayOrg, vRayDir, &vPlane01, &vPlane03, &vPlane02, &fTime) == TRUE )
	{
		if( fTime > 0 )
		{
			IsIntersection = true;
		}
	}
	else if( RayTriangle(vRayOrg, vRayDir, &vPlane03, &vPlane04, &vPlane01, &fTime) == TRUE )
	{
		if( fTime > 0 )
		{
			IsIntersection = true;
		}
	}
	else if( RayTriangle(vRayOrg, vRayDir, &vPlane03, &vPlane01, &vPlane04, &fTime) == TRUE )
	{
		if( fTime > 0 )
		{
			IsIntersection = true;
		}
	}

	if( IsIntersection == true )
	{
		vResPos = (*vRayOrg) + ((*vRayDir) * fTime);
	}

	return IsIntersection;
}

void				CGizmoObject::OnLButtonDown_Picking(const EventArg_PickingEntity& Arg)
{
	if( m_IsShow == true && Arg.pPickingEntity != NULL && Arg.pPickingBaseMesh != NULL )
	{
		if( strcmp(Arg.pPickingBaseMesh->GetName(), "gizmo_x") == 0 )
		{
			m_MoveState = eMoveState_X;
			FirstPickingSetting();
		}

		if( strcmp(Arg.pPickingBaseMesh->GetName(), "gizmo_y") == 0 )
		{
			m_MoveState = eMoveState_Y;
			FirstPickingSetting();
		}

		if( strcmp(Arg.pPickingBaseMesh->GetName(), "gizmo_z") == 0 )
		{
			m_MoveState = eMoveState_Z;
			FirstPickingSetting();
		}

		if( strcmp(Arg.pPickingBaseMesh->GetName(), "gizmo_xy") == 0 )
		{
			m_MoveState = eMoveState_XY;
			FirstPickingSetting();
		}

		if( strcmp(Arg.pPickingBaseMesh->GetName(), "gizmo_xz") == 0 )
		{
			m_MoveState = eMoveState_XZ;
			FirstPickingSetting();
		}

		if( strcmp(Arg.pPickingBaseMesh->GetName(), "gizmo_yz") == 0 )
		{
			m_MoveState = eMoveState_YZ;
			FirstPickingSetting();
		}

		m_vPosInterval = m_vMoveStatePickPos - m_vCurrPos;

		if( m_IsValid == true )
		{
			CPerspectiveCamera::GetInstance()->SetProcessLock(true);
		}
	}
}

void				CGizmoObject::OnLButtonUp_Picking(const EventArg_PickingEntity& Arg)
{
	m_MoveState = eMoveState_None;
	m_IsValid = false;

	CPerspectiveCamera::GetInstance()->SetProcessLock(false);
}
