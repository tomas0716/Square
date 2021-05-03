#include "StdAfx.h"
#include "PickingMgr.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPickingEntity

CPickingEntity::CPickingEntity(IGwScnObject * pScnObject, ITypeObject * pParameta)
: m_pScnObject(pScnObject), m_pParameta(pParameta)
{
	m_pScnObject->AddRef();

	m_pEvent_Over	= new TEventDispatch< EventArg_PickingEntity >();
	m_pEvent_Leave	= new TEventDispatch< EventArg_PickingEntity >();

	m_pEvent_LButtonDown	= new TEventDispatch< EventArg_PickingEntity >();
	m_pEvent_LButtonUp		= new TEventDispatch< EventArg_PickingEntity >();

	m_pEvent_RButtonDown	= new TEventDispatch< EventArg_PickingEntity >();
	m_pEvent_RButtonUp		= new TEventDispatch< EventArg_PickingEntity >();
}

CPickingEntity::~CPickingEntity(void)
{
	m_pScnObject->Release();

	m_pEvent_Over->Dispose();
	m_pEvent_Over->Release();

	m_pEvent_Leave->Dispose();
	m_pEvent_Leave->Release();

	m_pEvent_LButtonDown->Dispose();
	m_pEvent_LButtonDown->Release();

	m_pEvent_LButtonUp->Dispose();
	m_pEvent_LButtonUp->Release();

	m_pEvent_RButtonDown->Dispose();
	m_pEvent_RButtonDown->Release();

	m_pEvent_RButtonUp->Dispose();
	m_pEvent_RButtonUp->Release();
}

void		CPickingEntity::OnOver(IGwBaseMesh * pBaseMesh)
{
	EventArg_PickingEntity arg;
	arg.pPickingEntity = this;
	arg.pPickingBaseMesh = pBaseMesh;
	m_pEvent_Over->OnEvent(arg);
}

void		CPickingEntity::OnLeave(IGwBaseMesh * pBaseMesh)
{
	EventArg_PickingEntity arg;
	arg.pPickingEntity = this;
	arg.pPickingBaseMesh = pBaseMesh;
	m_pEvent_Leave->OnEvent(arg);
}

void		CPickingEntity::OnLButtonDown(IGwBaseMesh * pBaseMesh)
{
	EventArg_PickingEntity arg;
	arg.pPickingEntity = this;
	arg.pPickingBaseMesh = pBaseMesh;
	m_pEvent_LButtonDown->OnEvent(arg);
}

void		CPickingEntity::OnLButtonUp(IGwBaseMesh * pBaseMesh)
{
	EventArg_PickingEntity arg;
	arg.pPickingEntity = this;
	arg.pPickingBaseMesh = pBaseMesh;
	m_pEvent_LButtonUp->OnEvent(arg);
}

void		CPickingEntity::OnRButtonDown(IGwBaseMesh * pBaseMesh)
{
	EventArg_PickingEntity arg;
	arg.pPickingEntity = this;
	arg.pPickingBaseMesh = pBaseMesh;
	m_pEvent_RButtonDown->OnEvent(arg);
}

void		CPickingEntity::OnRButtonUp(IGwBaseMesh * pBaseMesh)
{
	EventArg_PickingEntity arg;
	arg.pPickingEntity = this;
	arg.pPickingBaseMesh = pBaseMesh;
	m_pEvent_RButtonUp->OnEvent(arg);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CPickingMgr

CPickingMgr::CPickingMgr(void) : m_pCurrPickingEntity(NULL), m_pCurrPickingBaseMesh(NULL)
{
	m_pPick = new IGwPick(NULL);

	m_pEvent_NonePicking = new TEventDispatch< EventArg_PickingEntity >();
}

CPickingMgr::~CPickingMgr(void)
{
	m_pPick->Release();

	m_pEvent_NonePicking->Dispose();
	m_pEvent_NonePicking->Release();

	ClearAll_PickingEntity();

	if( CAppEvent::OnPostUpdate() != NULL )
		CAppEvent::OnPostUpdate()->ClearEventOUT("CPickingMgr");
}

void				CPickingMgr::SetCamera(IGwCamera * pCamera)
{
	m_pPick->Release();
	m_pPick = new IGwPick(pCamera);

	CAppEvent::OnPostUpdate()->ClearEventOUT("CPickingMgr");
	CAppEvent::OnPostUpdate()->AddEventHandler("CPickingMgr", this , &CPickingMgr::PostUpdate);
}

void				CPickingMgr::PostUpdate(const EventArg_ElapsedTime& Arg)
{
	if( IGwUIControl::GetCurrPressedControl() == IGwUIControl::GetControl_Null() && 
		IGwUIControl::GetCurrMouseOverControl() == IGwUIControl::GetControl_Null() )
	{
		POINT	vCurPos = Gw::GetKeyMapper()->GetCursorPos(Gw::GethWnd());
		m_pPick->Update(vCurPos.x, vCurPos.y);

		GwVector	*vRayOrg, *vRayDir;
		vRayOrg	= const_cast<GwVector*>(m_pPick->GetRayPos());
		vRayDir	= const_cast<GwVector*>(m_pPick->GetRayDir());

		GwAABB		aabb;
		GwVector	vTemp;
		GwVector	vHit(0.f, 0.f, 10000.f);

		vector<CPickingEntity*>		kPickingEntityList;
		vector<CPickingEntity*>::iterator itr = m_pPickingEntityList.begin();

		CPickingEntity * pPickingEntity = NULL;
		IGwBaseMesh * pPickingBaseMesh = NULL;
		float fDistanceTime = FLT_MAX;

		for( ; itr != m_pPickingEntityList.end(); ++itr )
		{
			vector<IGwRndNode*> MeshList;
			(*itr)->GetScnObject()->FindAllTreeNodes(MeshList, GWRNDNODE_MESH);

			vector<IGwRndNode*>::iterator itr_RndNode = MeshList.begin();

			for( ; itr_RndNode != MeshList.end(); ++itr_RndNode )
			{
				GwVector vout1, vout2, vout3;
				float foutTime = FLT_MAX;

				IGwBaseMesh * pBaseMesh = dynamic_cast<IGwBaseMesh*>(*itr_RndNode);

				if( pBaseMesh != NULL && pBaseMesh->RayIntersectTriangle_SysMesh(vout1, vout2, vout3, foutTime, *vRayOrg, *vRayDir) == TRUE )
				{
					if( foutTime > 0 && foutTime < fDistanceTime )
					{
						fDistanceTime = foutTime;
						pPickingEntity = *itr;
						pPickingBaseMesh = pBaseMesh;
					}
				}
			}
		}

		if( pPickingEntity != NULL ) 
		{
			if( m_pCurrPickingEntity != NULL && m_pCurrPickingEntity != pPickingEntity && m_pCurrPickingBaseMesh != pPickingBaseMesh )
				m_pCurrPickingEntity->OnLeave(m_pCurrPickingBaseMesh);

			m_pCurrPickingEntity = pPickingEntity;
			m_pCurrPickingBaseMesh = pPickingBaseMesh;

			if( Gw::GetKeyMapper()->WasKeyDown(VK_LBUTTON) == TRUE )
				pPickingEntity->OnLButtonDown(pPickingBaseMesh);
			else if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE )
				pPickingEntity->OnLButtonUp(pPickingBaseMesh);
			else if( Gw::GetKeyMapper()->WasKeyDown(VK_RBUTTON) == TRUE )
				pPickingEntity->OnRButtonDown(pPickingBaseMesh);
			else if( Gw::GetKeyMapper()->IsKeyUp(VK_RBUTTON) == TRUE )
				pPickingEntity->OnRButtonUp(pPickingBaseMesh);
			else if( m_pCurrPickingEntity != pPickingEntity )
				pPickingEntity->OnOver(pPickingBaseMesh);
		}
		else 
		{
			if( m_pCurrPickingEntity != NULL && m_pCurrPickingBaseMesh != NULL )
				m_pCurrPickingEntity->OnLeave(m_pCurrPickingBaseMesh);

			m_pCurrPickingEntity = NULL;
			m_pCurrPickingBaseMesh = NULL;

			if( Gw::GetKeyMapper()->WasKeyDown(VK_LBUTTON) == TRUE )
			{
				EventArg_PickingEntity arg;
				arg.pPickingEntity = NULL;
				m_pEvent_NonePicking->OnEvent(arg);
			}
		}
	}
	else 
	{
		if( m_pCurrPickingEntity != NULL && m_pCurrPickingBaseMesh != NULL )
		{
			m_pCurrPickingEntity->OnLeave(m_pCurrPickingBaseMesh);
			m_pCurrPickingEntity = NULL;
			m_pCurrPickingBaseMesh = NULL;
		}
	}
}

CPickingEntity	*	CPickingMgr::AddPickingEntity(IGwScnObject * pScnObject, ITypeObject * pParameta)
{
	CPickingEntity * pPickingEntity = new CPickingEntity(pScnObject, pParameta);
	m_pPickingEntityList.push_back(pPickingEntity);

	return pPickingEntity;
}

void				CPickingMgr::RemovePickingEntity(IGwScnObject * pScnObject)
{
	vector<CPickingEntity*>::iterator itr = m_pPickingEntityList.begin();

	for( ; itr != m_pPickingEntityList.end(); ++itr )
	{
		if( (*itr)->GetScnObject() == pScnObject )
		{
			if( m_pCurrPickingEntity != NULL && m_pCurrPickingEntity->GetScnObject() == pScnObject )
			{
				m_pCurrPickingEntity = NULL;
			}

			(*itr)->Release();
			m_pPickingEntityList.erase(itr);
			break;
		}
	}
}

void				CPickingMgr::ClearAll_PickingEntity()
{
	vector<CPickingEntity*>::iterator itr = m_pPickingEntityList.begin();

	for( ; itr != m_pPickingEntityList.end(); ++itr )
	{
		(*itr)->Release();
	}

	m_pPickingEntityList.clear();

	m_pCurrPickingEntity = NULL;
}