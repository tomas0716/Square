#include "StdAfx.h"
#include "GameStage_Main.h"

CGameStage_Main::CGameStage_Main()
{
}

CGameStage_Main::~CGameStage_Main()
{
}

void		CGameStage_Main::OnCreateDevice()
{
	m_pUIHelper = new CUIHelper;

	m_pRndLight		=	Gw::GetLightMgr()->CreateLight();
	m_pRndLight->SetAmbient(GwColor(0.75f, 0.75f, 0.75f, 0.f));
	m_pRndLight->SetDiffuse(GwColor(1.f, 1.f, 1.f, 1.f));
	GwVector	vDirLight(-1.f, -1.f, 1.f);
	D3DXVec3Normalize(&vDirLight, &vDirLight);
	m_pRndLight->SetWorldAt(vDirLight);

	CAppEvent::CreateInstance();
	CNotificationCenter::CreateInstance();
	CGameStageMgr::CreateInstance();
	CResourceManager::CreateInstance();
	CUIDragObject::CreateInstance();

	CTableMgr::CreateInstance();
	IGameObjectMgr::CreateInstance();
	IGameObjectMgr::GetInstance()->LoadHierarchy();

	CPerspectiveCamera::CreateInstance();
	CPerspectiveCamera::GetInstance()->OnInitialize(CFrameWorks::CreateInstance()->GetWindowWidth(), CFrameWorks::CreateInstance()->GetWindowHeight());

	CPickingMgr::CreateInstance();
	CPickingMgr::GetInstance()->SetCamera(CPerspectiveCamera::GetInstance()->GetCamera());
	CPickingObjectWireFrameRender::CreateInstance();
	CPickingMeshColliderWireFrameRender::CreateInstance();
	CPathObjectRenderMgr::CreateInstance();
	CGizmoObject::CreateInstance();

	Gw::GetDynPolygonMgr()->SetFillMode(D3DFILL_WIREFRAME);

	Gw::GetDynPolygonMgr()->SetCamera(CPerspectiveCamera::GetInstance()->GetCamera());
	Gw::GetPolylineMgr()->SetCamera(CPerspectiveCamera::GetInstance()->GetCamera());

	CGameStageMgr::GetInstance()->CreateGameStage("CGameStage_Hierarchy");
	CGameStageMgr::GetInstance()->CreateGameStage("CGameStage_Project");
	CGameStageMgr::GetInstance()->CreateGameStage("CGameStage_Inspector");
	CGameStageMgr::GetInstance()->CreateGameStage("CGameStage_TimeLine");
	CGameStageMgr::GetInstance()->CreateGameStage("CGameStage_Menu");
}

void		CGameStage_Main::OnResetDevice()
{
	CPerspectiveCamera::GetInstance()->OnSizeChange(CFrameWorks::CreateInstance()->GetWindowWidth(), CFrameWorks::CreateInstance()->GetWindowHeight());
	CGameStageMgr::GetInstance()->OnResetDevice();
}

void		CGameStage_Main::OnLostDevice()
{
	CGameStageMgr::GetInstance()->OnLostDevice();
}

void		CGameStage_Main::OnDestroyDevice()
{
	delete m_pUIHelper;

	m_pRndLight->Release();
	CAppEvent::DestroyInstance();
	CNotificationCenter::DestroyInstance();
	CGameStageMgr::DestroyInstance();
	CResourceManager::DestroyInstance();
	CUIDragObject::DestroyInstance();
	CTableMgr::DestroyInstance();
	IGameObjectMgr::DestroyInstance();
	CPickingMgr::DestroyInstance();
	CPickingObjectWireFrameRender::DestroyInstance();
	CPickingMeshColliderWireFrameRender::DestroyInstance();
	CPathObjectRenderMgr::DestroyInstance();
	CGizmoObject::DestroyInstance();

	CPerspectiveCamera::DestroyInstance();
}

BOOL		CGameStage_Main::Update(float fDeltaTime)
{
	CGizmoObject::GetInstance()->Update(fDeltaTime);
	CPerspectiveCamera::GetInstance()->Update(fDeltaTime);
	IGameObjectMgr::GetInstance()->Update(fDeltaTime);
	CGameStageMgr::GetInstance()->Update(fDeltaTime);
	m_pRndLight->Update(fDeltaTime);
	EventArg_ElapsedTime arg;
	arg.fElapsedTime = fDeltaTime;
	CAppEvent::OnUpdate()->OnEvent(arg);
	CPickingObjectWireFrameRender::GetInstance()->Update(fDeltaTime);
	CPickingMeshColliderWireFrameRender::GetInstance()->Update(fDeltaTime);
	CPathObjectRenderMgr::GetInstance()->Update(fDeltaTime);
	CUIDragObject::GetInstance()->Update(fDeltaTime);
	//CGizmoObject::GetInstance()->Update(fDeltaTime);

	return TRUE;
}

void		CGameStage_Main::Render(float fDeltaTime)
{
	Gw::GetBillboardMgr()->SetCamera(CPerspectiveCamera::GetInstance()->GetCamera());

	CPerspectiveCamera::GetInstance()->Render(fDeltaTime);
	CGameStageMgr::GetInstance()->Render(fDeltaTime);
	IGameObjectMgr::GetInstance()->Render(fDeltaTime);
	CPickingObjectWireFrameRender::GetInstance()->Render(fDeltaTime);
	CPickingMeshColliderWireFrameRender::GetInstance()->Render(fDeltaTime);
	CPathObjectRenderMgr::GetInstance()->Render(fDeltaTime);
	//CGizmoObject::GetInstance()->Render(fDeltaTime);

	m_pRndLight->Render(fDeltaTime);
	m_pRndLight->SetEnable(FALSE);
	Gw::GetRenderer()->AddGlobalLight_OnActiveTech(0, m_pRndLight);

	Gw::GetRenderer()->Render(fDeltaTime);

	Gw::GetParticleMgr()->Render(fDeltaTime);
	Gw::GetPolylineMgr()->Render(fDeltaTime);
	Gw::GetBillboardMgr()->Render(fDeltaTime);

	CPerspectiveCamera::GetInstance()->Render(fDeltaTime);
	CGizmoObject::GetInstance()->Render(fDeltaTime);
	m_pRndLight->Render(fDeltaTime);
	Gw::GetRenderer()->AddGlobalLight_OnActiveTech(0, m_pRndLight);

	Gw::GetRenderer()->SetRenderState_ZEnable(FALSE);
	Gw::GetRenderer()->Render(fDeltaTime);
	Gw::GetRenderer()->SetRenderState_ZEnable(TRUE);

	Gw::GetBillboardMgr()->SetDefault2DCamera();
}