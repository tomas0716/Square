#include "StdAfx.h"
#include "PathObjectRender.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CPathObjectControllerRender

class CPathObjectRender::CPathObjectControllerRender : public IGwRefObject
{
public:
	CPathObjectControllerRender(CPathObjectRender * pPathObjectRender, IPathObject * pPathObject, sPathDotInfo * pPathDotInfo)
	: m_pPathObjectRender(pPathObjectRender), m_pPathObject(pPathObject), m_pPathDotInfo(pPathDotInfo), m_pPathDotInfo_Sel(NULL)
	{
		Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\Object\\Texture\\");

		for( int i = 0; i < 2; ++i )
		{
			m_pPolyLine[i] = Gw::GetPolylineMgr()->CreatePolyline();
			IGwBillboardTex * pPolyTex =	Gw::GetBillboardMgr()->CreateBillboardTexture_InPak(GwNullString, "control_line.png");
			m_pPolyLine[i]->SetBillboardTex(pPolyTex);	pPolyTex->Release();
			m_pPolyLine[i]->SetDestBlend(D3DBLEND_INVSRCALPHA);

			m_pPathDotInfo_Controller[i] = NULL;
		}

		if( pPathDotInfo->nIndex == 0 )
		{
			m_pPathDotInfo_Controller[1] = m_pPathObject->GetEvent(pPathDotInfo->nIndex + 1);
		}
		else if( pPathDotInfo->nIndex == m_pPathObject->GetNumEvent() - 1)
		{
			m_pPathDotInfo_Controller[0] = m_pPathObject->GetEvent(pPathDotInfo->nIndex - 1);
		}
		else
		{
			m_pPathDotInfo_Controller[0] = m_pPathObject->GetEvent(pPathDotInfo->nIndex - 1);
			m_pPathDotInfo_Controller[1] = m_pPathObject->GetEvent(pPathDotInfo->nIndex + 1);
		}

		ReLoad();
	}

	~CPathObjectControllerRender()
	{
		Clear();

		for( int i = 0; i < 2; ++i )
		{
			m_pPolyLine[i]->Release();
		}
	}

	void	Update(float fDeltaTime)
	{
		GwVector vCameraPos = CPerspectiveCamera::GetInstance()->GetCameraEye();

		map<IGwScnObject*,sPathDotInfo*>::iterator itr = m_PathDotInfoTable.begin();

		for( ; itr != m_PathDotInfoTable.end(); ++itr )
		{
			GwVector vDistance = vCameraPos - *itr->first->GetWorldPos();
			float fScale = D3DXVec3Length(&vDistance) / 360;
			fScale = max(fScale, 9.5f);

			GwMatA matWorld;
			D3DXMatrixScaling(&matWorld, fScale, fScale, fScale);
			matWorld._41 = itr->first->GetWorldPos()->x;
			matWorld._42 = itr->first->GetWorldPos()->y;
			matWorld._43 = itr->first->GetWorldPos()->z;

			itr->first->SetWorldMatrix(matWorld);

			itr->first->Update(fDeltaTime);
		}
	}

	void	Render(float fDeltaTime)
	{
		map<IGwScnObject*, sPathDotInfo*>::iterator itr = m_PathDotInfoTable.begin();

		for( ; itr != m_PathDotInfoTable.end(); ++itr )
		{
			Gw::GetRenderer()->AddScnObject(itr->first);
		}

		for( int i = 0; i < 2; ++i )
		{
			m_pPolyLine[i]->Render(fDeltaTime);
		}
	}

	void	ReLoad()
	{
		Clear();

		for( int i = 0; i < 2; ++i )
		{
			if( m_pPathDotInfo_Controller[i] != NULL )
			{
				GwMatA matWorld;
				D3DXMatrixScaling(&matWorld, 8.5f, 8.5f, 8.5f);
				matWorld._41 = m_pPathDotInfo_Controller[i]->vWorldPos.x;
				matWorld._42 = m_pPathDotInfo_Controller[i]->vWorldPos.y;
				matWorld._43 = m_pPathDotInfo_Controller[i]->vWorldPos.z;
				IGwScnObject * pScnObject = Gw::GetScnMgr()->CreateScnObject();
				pScnObject->SetWorldMatrix(matWorld);
				m_PathDotInfoTable.insert(map<IGwScnObject*,sPathDotInfo*>::value_type(pScnObject,m_pPathDotInfo_Controller[i]));
				IGwStaticObject * pStaticObject = IResourceHelper::CreateStaticObject("EsTool_Resources\\Object\\Mesh\\", "EsTool_Resources\\Object\\Texture\\", "path_tangentPoint.gb");
				pScnObject->AddStaticObject(pStaticObject);	pStaticObject->Release();

				CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(pScnObject);

				if( i == 0 )
					pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CPathObjectControllerRender", this, &CPathObjectControllerRender::OnLButtonDown_Picking_Prev);
				else if( i == 1 )
					pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CPathObjectControllerRender", this, &CPathObjectControllerRender::OnLButtonDown_Picking_Next);

				m_PosList[i].push_back(m_pPathDotInfo->vWorldPos);
				m_PosList[i].push_back(m_pPathDotInfo_Controller[i]->vWorldPos);


				m_pPolyLine[i]->ClearAll();

				GwVector vPos_01 = m_pPathDotInfo->vWorldPos;
				vPos_01.y += 5;
				GwVector vPos_02 = m_pPathDotInfo->vWorldPos;
				vPos_02.y -= 5;

				m_pPolyLine[i]->AddLine(vPos_01, vPos_02);

				vPos_01 = m_pPathDotInfo_Controller[i]->vWorldPos;
				vPos_01.y += 5;
				vPos_02 = m_pPathDotInfo_Controller[i]->vWorldPos;
				vPos_02.y -= 5;

				m_pPolyLine[i]->AddLine(vPos_01, vPos_02);
			}
		}
	}

	void	SetSelectController(int nPlusIndex)
	{
		sPathDotInfo * pPathDotInfo = m_pPathObject->GetEvent(m_pPathDotInfo->nIndex + nPlusIndex);
		
		if( pPathDotInfo != NULL )
		{
			m_pPathDotInfo_Sel = pPathDotInfo;
			CGizmoObject::GetInstance()->Show(pPathDotInfo->vWorldPos);
			CGizmoObject::GetInstance()->GetEvent_Move()->ClearAllEventOUT();
			CGizmoObject::GetInstance()->GetEvent_Move()->AddEventHandler("CPathObjectControllerRender", this, &CPathObjectControllerRender::OnGizmoObjectMove);
		}
	}

private:

	void	Clear()
	{
		map<IGwScnObject*,sPathDotInfo*>::iterator itr = m_PathDotInfoTable.begin();

		for( ; itr != m_PathDotInfoTable.end(); ++itr )
		{
			if( CPickingMgr::GetInstance() != NULL )
				CPickingMgr::GetInstance()->RemovePickingEntity(itr->first);

			itr->first->Release();
		}

		m_PathDotInfoTable.clear();
	}

	void	OnLButtonDown_Picking_Prev(const EventArg_PickingEntity& Arg)
	{
		sPathDotInfo * pPathDotInfo_Prev = m_pPathObject->GetEvent(m_pPathDotInfo->nIndex - 1);
		
		if( pPathDotInfo_Prev != NULL )
		{
			m_pPathDotInfo_Sel = pPathDotInfo_Prev;
			CGizmoObject::GetInstance()->Show(pPathDotInfo_Prev->vWorldPos);
			CGizmoObject::GetInstance()->GetEvent_Move()->ClearAllEventOUT();
			CGizmoObject::GetInstance()->GetEvent_Move()->AddEventHandler("CPathObjectControllerRender", this, &CPathObjectControllerRender::OnGizmoObjectMove);

			CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");
			if( pGameStage_Hierarchy != NULL )
			{
				pGameStage_Hierarchy->SetSelectGameObject(m_pPathObject);
			}
		}
	}

	void	OnLButtonDown_Picking_Next(const EventArg_PickingEntity& Arg)
	{
		sPathDotInfo * pPathDotInfo_Next = m_pPathObject->GetEvent(m_pPathDotInfo->nIndex + 1);

		if( pPathDotInfo_Next != NULL )
		{
			m_pPathDotInfo_Sel = pPathDotInfo_Next;
			CGizmoObject::GetInstance()->Show(pPathDotInfo_Next->vWorldPos);
			CGizmoObject::GetInstance()->GetEvent_Move()->ClearAllEventOUT();
			CGizmoObject::GetInstance()->GetEvent_Move()->AddEventHandler("CPathObjectControllerRender", this, &CPathObjectControllerRender::OnGizmoObjectMove);

			CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");
			if( pGameStage_Hierarchy != NULL )
			{
				pGameStage_Hierarchy->SetSelectGameObject(m_pPathObject);
			}
		}
	}

	void	OnGizmoObjectMove(const EventArg_GizmoMove& Arg)
	{
		m_pPathDotInfo_Sel->vLocalPos = Arg.vPos;

		m_pPathObjectRender->ReLoad();
		ReLoad();

		NotificationArg_InspectorComponent_PathObject_InfoChange * pArg = new NotificationArg_InspectorComponent_PathObject_InfoChange();
		pArg->m_pPathDotInfo = m_pPathDotInfo;

		CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_PathObject_InfoChange, pArg);
		pArg->Release();
	}

private:

	CPathObjectRender	*	m_pPathObjectRender;
	IPathObject			*	m_pPathObject;
	sPathDotInfo		*	m_pPathDotInfo;
	sPathDotInfo		*	m_pPathDotInfo_Sel;

	map<IGwScnObject*, sPathDotInfo*>	m_PathDotInfoTable;

	sPathDotInfo		*	m_pPathDotInfo_Controller[2];
	IGwPolyline			*	m_pPolyLine[2];
	vector<GwVector>		m_PosList[2];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CPathObjectRender

CPathObjectRender::CPathObjectRender(IPathObject * pPathObject)
: m_pPathObject(pPathObject), m_pPathObjectControllerRender(NULL), m_nActivePolyline(0), m_pPickingPathDotInfo(NULL), m_IsPathDotRender(true)
{
	m_pPathObject->AddRef();

	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\Object\\Texture\\");
	m_pPolyTex =	Gw::GetBillboardMgr()->CreateBillboardTexture_InPak(GwNullString, "path_line.png");

	m_pPopupMenu_AddDelete = new CPopupMenu(eObjectEditState_PathObject_Add|eObjectEditState_PathObject_Delete);
	m_pPopupMenu_AddDelete->GetEvent_Click()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnObjectEditState);

	m_pPopupMenu_Add = new CPopupMenu(eObjectEditState_PathObject_Add);
	m_pPopupMenu_Add->GetEvent_Click()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnObjectEditState);
	
	m_pPopupMenu_Delete = new CPopupMenu(eObjectEditState_PathObject_Delete);
	m_pPopupMenu_Delete->GetEvent_Click()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnObjectEditState);

	ReLoad();

	if( m_pPathObject != NULL && m_pPathObject->IsCreationAutoPlay() == true )
	{
		m_pPathObject->OnStop();
		m_pPathObject->OnPlay();
	}
}

CPathObjectRender::~CPathObjectRender()
{
	m_pPathObject->Release();
	m_pPolyTex->Release();
	m_pPopupMenu_AddDelete->Release();
	m_pPopupMenu_Add->Release();
	m_pPopupMenu_Delete->Release();

	Clear();
}

void					CPathObjectRender::Update(float fDeltaTime)
{
	m_pPopupMenu_AddDelete->Update(fDeltaTime);
	m_pPopupMenu_Add->Update(fDeltaTime);
	m_pPopupMenu_Delete->Update(fDeltaTime);

	GwVector vCameraPos = CPerspectiveCamera::GetInstance()->GetCameraEye();

	map<IGwScnObject*,sPathDotInfo*>::iterator itr = m_PathDotInfoTable.begin();

	for( ; itr != m_PathDotInfoTable.end(); ++itr )
	{
		GwVector vDistance = vCameraPos - *itr->first->GetWorldPos();
		float fScale = D3DXVec3Length(&vDistance) / 360;

		fScale = max(fScale, 4.5f);

		GwMatA matWorld;
		D3DXMatrixScaling(&matWorld, fScale, fScale, fScale);
		matWorld._41 = itr->first->GetWorldPos()->x;
		matWorld._42 = itr->first->GetWorldPos()->y;
		matWorld._43 = itr->first->GetWorldPos()->z;

		itr->first->SetWorldMatrix(matWorld);

		itr->first->Update(fDeltaTime);
	}

	map<IGwScnObject*,int>::iterator itr_middle = m_MiddleDotIndexTable.begin();

	for( ; itr_middle != m_MiddleDotIndexTable.end(); ++itr_middle )
	{
		GwVector vDistance = vCameraPos - *itr_middle->first->GetWorldPos();
		float fScale = D3DXVec3Length(&vDistance) / 360;

		fScale = max(fScale, 4.5f);

		GwMatA matWorld;
		D3DXMatrixScaling(&matWorld, fScale, fScale, fScale);
		matWorld._41 = itr_middle->first->GetWorldPos()->x;
		matWorld._42 = itr_middle->first->GetWorldPos()->y;
		matWorld._43 = itr_middle->first->GetWorldPos()->z;

		itr_middle->first->SetWorldMatrix(matWorld);

		itr_middle->first->Update(fDeltaTime);
	}

	m_nActivePolyline = 0;

	if( m_pPathObjectControllerRender != NULL )
		m_pPathObjectControllerRender->Update(fDeltaTime);
}

void					CPathObjectRender::Render(float fDeltaTime)
{
	if( m_IsPathDotRender == true )
	{
		map<IGwScnObject*, sPathDotInfo*>::iterator itr = m_PathDotInfoTable.begin();

		for( ; itr != m_PathDotInfoTable.end(); ++itr )
		{
			Gw::GetRenderer()->AddScnObject(itr->first);
		}

		map<IGwScnObject*,int>::iterator itr_middle = m_MiddleDotIndexTable.begin();

		for( ; itr_middle != m_MiddleDotIndexTable.end(); ++itr_middle )
		{
			Gw::GetRenderer()->AddScnObject(itr_middle->first);
		}
	}

	vector<IGwPolyline*>::iterator itr_poly = m_PolylineList.begin();

	for( ; itr_poly != m_PolylineList.end(); ++itr_poly )
	{
		(*itr_poly)->Render(fDeltaTime);
	}

	if( m_pPathObjectControllerRender != NULL && m_IsPathDotRender == true )
		m_pPathObjectControllerRender->Render(fDeltaTime);
}

void					CPathObjectRender::SetSelectDot(sPathDotInfo * pPathDotInfo, int nIndexPlus)
{
	if( pPathDotInfo == NULL )
		return;

	if( m_pPathObjectControllerRender != NULL )
		m_pPathObjectControllerRender->Release();

	m_pPathObjectControllerRender = new CPathObjectControllerRender(this, m_pPathObject, pPathDotInfo);

	m_pPickingPathDotInfo = pPathDotInfo;

	if( nIndexPlus == 0 )
	{
		CGizmoObject::GetInstance()->Show(m_pPickingPathDotInfo->vWorldPos);
		CGizmoObject::GetInstance()->GetEvent_Move()->ClearAllEventOUT();
		CGizmoObject::GetInstance()->GetEvent_Move()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnGizmoObjectMove);
	}
	else
	{
		m_pPathObjectControllerRender->SetSelectController(nIndexPlus);
	}
}

void					CPathObjectRender::RemovePathObjectControllerRender()
{
	if( m_pPathObjectControllerRender != NULL ) 
		m_pPathObjectControllerRender->Release();

	m_pPathObjectControllerRender = NULL;

	CGizmoObject::GetInstance()->Hide();
}

void					CPathObjectRender::ReLoad()
{
	Clear();
	m_pPathObject->ReLoad();

	int nNumEvent = m_pPathObject->GetNumEvent();

	for( int i = 0; i < nNumEvent; ++i )
	{
		sPathDotInfo * pPathDotInfo = m_pPathObject->GetEvent(i);

		if( pPathDotInfo != NULL ) 
		{
			if( pPathDotInfo->bController == false )
			{
				GwMatA matWorld;
				D3DXMatrixScaling(&matWorld, 4, 4, 4);
				matWorld._41 = pPathDotInfo->vWorldPos.x;
				matWorld._42 = pPathDotInfo->vWorldPos.y;
				matWorld._43 = pPathDotInfo->vWorldPos.z;

				IGwScnObject * pScnObject = Gw::GetScnMgr()->CreateScnObject();
				pScnObject->SetWorldMatrix(matWorld);
				m_PathDotInfoTable.insert(map<IGwScnObject*,sPathDotInfo*>::value_type(pScnObject,pPathDotInfo));
				IGwStaticObject * pStaticObject = IResourceHelper::CreateStaticObject("EsTool_Resources\\Object\\Mesh\\", "EsTool_Resources\\Object\\Texture\\", "path_point.gb");
				pScnObject->AddStaticObject(pStaticObject);	pStaticObject->Release();

				CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(pScnObject);
				pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnLButtonDown_Picking_Dot);
				pPickingEntity->GetEvent_RButtonDown()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnRButtonDown_Picking_Dot);
			}
		}
	}
	
	float fEntireTime = 0;
	if( m_pPathObject->GetBlockTime() == false )
	{
		fEntireTime = m_pPathObject->GetEntireTime();
	}
	else 
	{
		fEntireTime = m_pPathObject->GetEvent(m_pPathObject->GetNumEvent()-1)->fTime;
	}

	m_pPathObject->OnStop();
	m_pPathObject->OnPlay();

	float fFrameTime = 0.01666666f;

	int nCount = (int)(fEntireTime / fFrameTime);

	m_PosList.clear();

	for( int i = 0; i < nCount; ++i )
	{
		m_pPathObject->Update(fFrameTime);
		GwVector vPos = m_pPathObject->GetCurrPos();
		m_PosList.push_back(vPos);
	}

	m_pPathObject->OnStop();

	ClearPolyLine();

	if( m_PosList.empty() == false )
	{
		IGwPolyline * pPolyline = NULL;

		for( int i = 0; i < (int)m_PosList.size(); ++i )
		{			
			if( (i % 500) == 0 )
			{
				++m_nActivePolyline;
				pPolyline = Gw::GetPolylineMgr()->CreatePolyline();
				pPolyline->SetBillboardTex(m_pPolyTex);
				pPolyline->SetDestBlend(D3DBLEND_INVSRCALPHA);
				m_PolylineList.push_back(pPolyline);

				if( i != 0 )
				{
					GwVector vPos_01 = m_PosList[i-1];
					vPos_01.y += 5;
					GwVector vPos_02 = m_PosList[i-1];
					vPos_02.y -= 5;

					pPolyline->AddLine(vPos_01, vPos_02);
				}
			}

			GwVector vPos_01 = m_PosList[i];
			vPos_01.y += 5;
			GwVector vPos_02 = m_PosList[i];
			vPos_02.y -= 5;

			pPolyline->AddLine(vPos_01, vPos_02);
		}

		for( int i = 0; i < (int)m_PosList.size(); ++i )
		{			
			if( (i % 500) == 0 )
			{
				++m_nActivePolyline;
				pPolyline = Gw::GetPolylineMgr()->CreatePolyline();
				pPolyline->SetBillboardTex(m_pPolyTex);
				pPolyline->SetDestBlend(D3DBLEND_INVSRCALPHA);
				m_PolylineList.push_back(pPolyline);

				if( i != 0 )
				{
					GwVector vDir = m_PosList[i] - m_PosList[i-1];
					GwVector vCross;
					D3DXVec3Cross(&vCross, &vDir, &GwVector(0,1,0));
					D3DXVec3Normalize(&vCross, &vCross);
					vCross *= 5;

					GwVector vPos_01 = m_PosList[i];
					vPos_01 += vCross;
					GwVector vPos_02 = m_PosList[i];
					vPos_02 -= vCross;

					pPolyline->AddLine(vPos_01, vPos_02);
				}
			}

			if( i == 0 )
			{
				GwVector vDir = m_PosList[1] - m_PosList[0];
				GwVector vCross;
				D3DXVec3Cross(&vCross, &vDir, &GwVector(0,1,0));
				D3DXVec3Normalize(&vCross, &vCross);
				vCross *= 5;

				GwVector vPos_01 = m_PosList[i];
				vPos_01 += vCross;
				GwVector vPos_02 = m_PosList[i];
				vPos_02 -= vCross;

				pPolyline->AddLine(vPos_01, vPos_02);
			}
			else 
			{
				GwVector vDir = m_PosList[i] - m_PosList[i-1];
				GwVector vCross;
				D3DXVec3Cross(&vCross, &vDir, &GwVector(0,1,0));
				D3DXVec3Normalize(&vCross, &vCross);
				vCross *= 5;

				GwVector vPos_01 = m_PosList[i];
				vPos_01 += vCross;
				GwVector vPos_02 = m_PosList[i];
				vPos_02 -= vCross;

				pPolyline->AddLine(vPos_01, vPos_02);
			}
		}
	}

	CreateMiddleDot();
}

void					CPathObjectRender::CreateMiddleDot()
{
	int nNumEvent = m_pPathObject->GetNumEvent();
	int nNumBlock = (nNumEvent - 1) / 3;

	for( int i = 0; i < nNumBlock; ++i )
	{
		int nStartIndex = i * 3;

		IBezier4 * pBezier4 = new IBezier4();

		for( int j = 0; j < 4; ++j )
		{
			pBezier4->AddPos(m_pPathObject->GetEvent(nStartIndex + j)->vWorldPos);
			pBezier4->SetBlockTime(0, j * 4);
			
		}

		pBezier4->OnPlay();

		pBezier4->Update(6);

		GwVector vPos = pBezier4->GetCurrPos();

		GwMatA matWorld;
		D3DXMatrixScaling(&matWorld, 4, 4, 4);
		matWorld._41 = vPos.x;
		matWorld._42 = vPos.y;
		matWorld._43 = vPos.z;

		IGwScnObject * pScnObject = Gw::GetScnMgr()->CreateScnObject();
		pScnObject->SetWorldMatrix(matWorld);
		m_MiddleDotIndexTable.insert(map<IGwScnObject*,int>::value_type(pScnObject,nStartIndex));
		IGwStaticObject * pStaticObject = IResourceHelper::CreateStaticObject("EsTool_Resources\\Object\\Mesh\\", "EsTool_Resources\\Object\\Texture\\", "path_point_select.gb");
		pScnObject->AddStaticObject(pStaticObject);	pStaticObject->Release();

		CPickingEntity * pPickingEntity = CPickingMgr::GetInstance()->AddPickingEntity(pScnObject);
		pPickingEntity->GetEvent_LButtonDown()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnLButtonUp_Picking_MiddleDot);
	}
}

void					CPathObjectRender::Clear()
{
	map<IGwScnObject*,sPathDotInfo*>::iterator itr = m_PathDotInfoTable.begin();

	for( ; itr != m_PathDotInfoTable.end(); ++itr )
	{
		if( CPickingMgr::GetInstance() != NULL )
			CPickingMgr::GetInstance()->RemovePickingEntity(itr->first);
		itr->first->Release();
	}

	m_PathDotInfoTable.clear();

	map<IGwScnObject*, int>::iterator itr_middle = m_MiddleDotIndexTable.begin();

	for( ; itr_middle != m_MiddleDotIndexTable.end(); ++itr_middle )
	{
		if( CPickingMgr::GetInstance() != NULL )
			CPickingMgr::GetInstance()->RemovePickingEntity(itr_middle->first);
		itr_middle->first->Release();
	}

	m_MiddleDotIndexTable.clear();
}

void					CPathObjectRender::ClearPolyLine()
{
	vector<IGwPolyline*>::iterator itr = m_PolylineList.begin();

	for( ; itr != m_PolylineList.end(); ++itr )
	{
		(*itr)->Release();
	}

	m_PolylineList.clear();
}

void					CPathObjectRender::OnLButtonDown_Picking_Dot(const EventArg_PickingEntity& Arg)
{
	map<IGwScnObject*, sPathDotInfo*>::iterator itr = m_PathDotInfoTable.find(Arg.pPickingEntity->GetScnObject());

	if( itr != m_PathDotInfoTable.end() )
	{
		if( m_pPathObjectControllerRender != NULL )
			m_pPathObjectControllerRender->Release();

		m_pPathObjectControllerRender = new CPathObjectControllerRender(this, m_pPathObject, itr->second);

		m_pPickingPathDotInfo = itr->second;

		CGizmoObject::GetInstance()->Show(m_pPickingPathDotInfo->vWorldPos);
		CGizmoObject::GetInstance()->GetEvent_Move()->ClearAllEventOUT();
		CGizmoObject::GetInstance()->GetEvent_Move()->AddEventHandler("CPathObjectRender", this, &CPathObjectRender::OnGizmoObjectMove);
	}

	CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");
	if( pGameStage_Hierarchy != NULL )
	{
		pGameStage_Hierarchy->SetSelectGameObject(m_pPathObject);
	}
}

void					CPathObjectRender::OnRButtonDown_Picking_Dot(const EventArg_PickingEntity& Arg)
{
	POINT Point = *Gw::GetKeyMapper()->GetCursorPos();

	map<IGwScnObject*, sPathDotInfo*>::iterator itr = m_PathDotInfoTable.find(Arg.pPickingEntity->GetScnObject());

	if( itr != m_PathDotInfoTable.end() )
	{
		if( m_pPathObject->GetNumEvent() == 4 )
		{
			m_pPopupMenu_Add->ShowPopup(GwVector2(Point.x, Point.y));
		}
		else if( itr->second->nIndex == 0 || itr->second->nIndex == m_pPathObject->GetNumEvent() - 1 )
		{
			m_pPopupMenu_AddDelete->ShowPopup(GwVector2(Point.x, Point.y));
		}
		else
		{
			m_pPopupMenu_Delete->ShowPopup(GwVector2(Point.x, Point.y));
		}

		m_pPickingPathDotInfo = itr->second;
	}

	CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");
	if( pGameStage_Hierarchy != NULL )
	{
		pGameStage_Hierarchy->SetSelectGameObject(m_pPathObject);
	}
}

void					CPathObjectRender::OnLButtonUp_Picking_MiddleDot(const EventArg_PickingEntity& Arg)
{
	map<IGwScnObject*, int>::iterator itr = m_MiddleDotIndexTable.find(Arg.pPickingEntity->GetScnObject());

	if( itr != m_MiddleDotIndexTable.end() )
	{
		int nFirst = itr->second;
		int nEnd = itr->second + 3;

		sPathDotInfo * pPathDotInfo_Start = m_pPathObject->GetEvent(nFirst);
		sPathDotInfo * pPathDotInfo_End = m_pPathObject->GetEvent(nEnd);

		if( pPathDotInfo_Start != NULL && pPathDotInfo_End != NULL )
		{
			float fFirstTime = pPathDotInfo_Start->fTime;
			float fEndTime = pPathDotInfo_End->fTime;

			GwVector vDir = pPathDotInfo_End->vLocalPos - pPathDotInfo_Start->vLocalPos;
			GwVector vCross;
			D3DXVec3Cross(&vCross, &vDir, &GwVector(0,1,0));
			D3DXVec3Normalize(&vCross, &vCross);

			GwMatA matWorld = m_pPathObject->GetWorldMat();
			GwMatA matInverse;
			D3DXMatrixInverse(&matInverse, NULL, &matWorld);
			GwVector vMiddlePos = *itr->first->GetWorldPos();
			D3DXVec3TransformCoord(&vMiddlePos, &vMiddlePos, &matInverse);

			m_pPathObject->InsertEvent(nEnd-1, 0, vMiddlePos + vCross * 150, true);
			m_pPathObject->InsertEvent(nEnd-1, fFirstTime + (fEndTime-fFirstTime) * 0.5f, vMiddlePos, false);
			m_pPathObject->InsertEvent(nEnd-1, 0, vMiddlePos + vCross * -150, true);

			ReLoad();

			CGizmoObject::GetInstance()->Hide();
		}
	}

	CGameStage_Hierarchy * pGameStage_Hierarchy = CGameStageMgr::GetInstance()->FindGameStage<CGameStage_Hierarchy>("CGameStage_Hierarchy");
	if( pGameStage_Hierarchy != NULL )
	{
		pGameStage_Hierarchy->SetSelectGameObject(m_pPathObject);
	}
}

void					CPathObjectRender::OnGizmoObjectMove(const EventArg_GizmoMove& Arg)
{
	sPathDotInfo * pPathDotInfo_Prev = m_pPathObject->GetEvent(m_pPickingPathDotInfo->nIndex - 1);
	sPathDotInfo * pPathDotInfo_Next = m_pPathObject->GetEvent(m_pPickingPathDotInfo->nIndex + 1);

	if( pPathDotInfo_Prev != NULL )
	{
		GwVector vInterval_Move = Arg.vPos - m_pPickingPathDotInfo->vLocalPos;
		pPathDotInfo_Prev->vLocalPos += vInterval_Move;
	}

	if( pPathDotInfo_Next != NULL )
	{
		GwVector vInterval_Move = Arg.vPos - m_pPickingPathDotInfo->vLocalPos;
		pPathDotInfo_Next->vLocalPos += vInterval_Move;
	}

	m_pPickingPathDotInfo->vLocalPos = Arg.vPos;

	ReLoad();

	if( m_pPathObjectControllerRender != NULL )
		m_pPathObjectControllerRender->Release();

	m_pPathObjectControllerRender = new CPathObjectControllerRender(this, m_pPathObject, m_pPickingPathDotInfo);

	NotificationArg_InspectorComponent_PathObject_InfoChange * pArg = new NotificationArg_InspectorComponent_PathObject_InfoChange();
	pArg->m_pPathDotInfo = m_pPickingPathDotInfo;

	CNotificationCenter::GetInstance()->PostNotification(Notification_InspectorComponent_PathObject_InfoChange, pArg);
	pArg->Release();
}

void					CPathObjectRender::OnObjectEditState(const EventArg_PopupMenu_Click& Arg)
{
	if( m_pPickingPathDotInfo != NULL )
	{
		switch(Arg.eEditState)
		{
		case eObjectEditState_PathObject_Add:
			{
				if( m_pPickingPathDotInfo->nIndex == 0 )
				{
					sPathDotInfo * pPathDotInfo_Next = m_pPathObject->GetEvent(3);
					GwVector vDir = m_pPickingPathDotInfo->vLocalPos - pPathDotInfo_Next->vLocalPos;
					D3DXVec3Normalize(&vDir, &vDir);
					GwVector vCross;
					D3DXVec3Cross(&vCross, &vDir, &GwVector(0,1,0));
					D3DXVec3Normalize(&vCross, &vCross);

					m_pPathObject->InsertEvent(0, 0, m_pPickingPathDotInfo->vLocalPos + vDir * 100 + vCross * 100, true);
					m_pPathObject->InsertEvent(0, 0, m_pPickingPathDotInfo->vLocalPos + vDir * 200 + vCross * -100, true);
					m_pPathObject->InsertEvent(0, 0, m_pPickingPathDotInfo->vLocalPos + vDir * 300, false);

					int nNumEvent = m_pPathObject->GetNumEvent();

					for( int i = 1; i < nNumEvent; ++i )
					{
						sPathDotInfo * pPathDotInfo = m_pPathObject->GetEvent(i);

						if( pPathDotInfo->bController == false )
						{
							pPathDotInfo->fTime += 5;
						}
					}

					ReLoad();

					if( m_pPathObjectControllerRender != NULL )
					{
						m_pPathObjectControllerRender->Release();
						m_pPathObjectControllerRender = NULL;
					}
				}
				else 
				{
					sPathDotInfo * pPathDotInfo_Prev = m_pPathObject->GetEvent(m_pPickingPathDotInfo->nIndex - 3);
					GwVector vDir = m_pPickingPathDotInfo->vLocalPos - pPathDotInfo_Prev->vLocalPos;
					D3DXVec3Normalize(&vDir, &vDir);
					GwVector vCross;
					D3DXVec3Cross(&vCross, &vDir, &GwVector(0,1,0));
					D3DXVec3Normalize(&vCross, &vCross);

					m_pPathObject->AddEvent(0, m_pPickingPathDotInfo->vLocalPos + vDir * 100 + vCross * 100, true);
					m_pPathObject->AddEvent(0, m_pPickingPathDotInfo->vLocalPos + vDir * 200 + vCross * -100, true);
					m_pPathObject->AddEvent(m_pPickingPathDotInfo->fTime + 5, m_pPickingPathDotInfo->vLocalPos + vDir * 300, false);

					ReLoad();

					if( m_pPathObjectControllerRender != NULL )
					{
						m_pPathObjectControllerRender->Release();
						m_pPathObjectControllerRender = NULL;
					}
				}
			}
			break;
		case eObjectEditState_PathObject_Delete:
			{
				int nIndex = m_pPickingPathDotInfo->nIndex;

				if( m_pPathObject->GetNumEvent() != 4 )
				{
					if( nIndex == 0 )
					{
						m_pPathObject->DeleteEvent(0);
						m_pPathObject->DeleteEvent(0);
						m_pPathObject->DeleteEvent(0);

						ReLoad();

						if( m_pPathObjectControllerRender != NULL )
						{
							m_pPathObjectControllerRender->Release();
							m_pPathObjectControllerRender = NULL;
						}

						CGizmoObject::GetInstance()->Hide();
					}
					else if( nIndex == m_pPathObject->GetNumEvent() - 1 )
					{
						m_pPathObject->DeleteEvent(nIndex-2);
						m_pPathObject->DeleteEvent(nIndex-2);
						m_pPathObject->DeleteEvent(nIndex-2);

						ReLoad();

						if( m_pPathObjectControllerRender != NULL )
						{
							m_pPathObjectControllerRender->Release();
							m_pPathObjectControllerRender = NULL;
						}

						CGizmoObject::GetInstance()->Hide();
					}
					else
					{
						m_pPathObject->DeleteEvent(nIndex-1);
						m_pPathObject->DeleteEvent(nIndex-1);
						m_pPathObject->DeleteEvent(nIndex-1);

						ReLoad();

						if( m_pPathObjectControllerRender != NULL )
						{
							m_pPathObjectControllerRender->Release();
							m_pPathObjectControllerRender = NULL;
						}

						CGizmoObject::GetInstance()->Hide();
					}
				}
			}
			break;
		}
	}
}