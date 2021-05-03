// CFrameWorks.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "FrameWorks.h"

#include <shellapi.h>

#define IDI_MAIN_ICON          101
#define IDR_MAIN_ACCEL         113

class	CFrameWorks::IFrameTimer
{
public:
	IFrameTimer(void) : m_fEntireTime(0.f), m_fElapsedTime(0.f), m_fInitTime(0.f), m_fPrevTime(0.f)		{ }
	~IFrameTimer(void)	{ }

	void		OnInit()
	{
		m_fEntireTime = m_fInitTime = m_fPrevTime = GetTickCount() * 0.001f;
	}

	void		Update()
	{
		m_fEntireTime = GetTickCount() * 0.001f;

		m_fElapsedTime = m_fEntireTime - m_fPrevTime;

		m_fPrevTime = m_fEntireTime;
	}

	float		GetEntireTime()
	{
		return m_fEntireTime-m_fInitTime;
	}

	float		GetElapsedTime()
	{
		return m_fElapsedTime;
	}

public:
	float		m_fEntireTime;
	float		m_fInitTime;
	float		m_fPrevTime;
	float		m_fElapsedTime;
};


HWND						CFrameWorks::m_hWnd			= NULL;
LPDIRECT3D9					CFrameWorks::m_pD3D			= NULL;
LPDIRECT3DDEVICE9			CFrameWorks::m_pd3dDevice		= NULL;
CFrameWorks::IFrameTimer	CFrameWorks::m_FrameTimer;

DWORD						CFrameWorks::m_dwWindowWidth	= 1900;
DWORD						CFrameWorks::m_dwWindowHeight	= 1050;

HWND				CFrameWorks::GetHwnd()
{
	return m_hWnd;
}
LPDIRECT3D9			CFrameWorks::GetD3D()
{
	return m_pD3D;
}

LPDIRECT3DDEVICE9	CFrameWorks::GetDevice()
{
	return m_pd3dDevice;
}

float				CFrameWorks::GetProcessTime()
{
	return m_FrameTimer.GetEntireTime();
}

float				CFrameWorks::GetElapsedTime()
{
	float fDeltaTime = m_FrameTimer.GetElapsedTime();
	return fDeltaTime;
}

DWORD				CFrameWorks::GetWindowWidth()
{
	return m_dwWindowWidth;
}

DWORD				CFrameWorks::GetWindowHeight()
{
	return m_dwWindowHeight;
}

CFrameWorks::CFrameWorks(void): m_eDisplayMode(GwFrameWorksConstant::eDisplayMode_Windows), m_eUpdateType(eUpdateType_Render), m_bMaxmized(false), m_bReSizeMsgBlock(false), m_bCreateDeviceCall(false)
{
	m_pApplication = new CApplication();

	m_UpdateFunc[eUpdateType_None]		= &UpdateNone;
	m_UpdateFunc[eUpdateType_Render]	= &UpdateRender;
	m_UpdateFunc[eUpdateType_Reset]		= &UpdateReset;
}

CFrameWorks::~CFrameWorks(void)
{
	int nRef;
	if( m_pD3D )		nRef = m_pD3D->Release();
	if( m_pd3dDevice )	nRef = m_pd3dDevice->Release();

	m_pD3D = NULL;
	m_pd3dDevice = NULL;

	m_pApplication->Release();
}

bool				CFrameWorks::OnCreateMainFrameWorks(CApplication * pApplication, HINSTANCE hIns, DWORD dwWindowWidth, DWORD dwWindowHeight, TCHAR * strWindowTitle, GwFrameWorksConstant::eDisplayMode eMode, DWORD dwWindowStyle)
{
	m_pApplication->Release();
	m_pApplication = pApplication;
	pApplication->AddRef();

	m_dwWindowWidth		= dwWindowWidth;
	m_dwWindowHeight	= dwWindowHeight;	
	m_dwWindowStyle		= dwWindowStyle;

	m_eDisplayMode		= eMode;

	OnCreateWindows(hIns, strWindowTitle);

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	m_ConstructureD3DPP.CreatePresentParameters();
	
	CreateProcess();

	return true;
}

bool				CFrameWorks::OnCreateWindows(HINSTANCE hIns, TCHAR * strWindowTitle)
{
	WCHAR szExePath[MAX_PATH];
	GetModuleFileName( NULL, szExePath, MAX_PATH );
	HICON hIcon = ExtractIcon( hIns, szExePath, 0 ); 

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc	= CFrameWorks::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hIns;
	wcex.hIcon			= hIcon;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= _T("");
	wcex.lpszClassName	= _T("CFrameWorks");
	wcex.hIconSm		= hIcon;

	RegisterClassEx(&wcex);

	SetRect( &m_rcClient, 0, 0, m_dwWindowWidth, m_dwWindowHeight );
	AdjustWindowRect( &m_rcClient, m_dwWindowStyle, false );

	m_hWnd = CreateWindow(	_T("CFrameWorks"), 
							strWindowTitle, 
							m_dwWindowStyle,
							0, 
							0,
							(m_rcClient.right-m_rcClient.left), 
							(m_rcClient.bottom-m_rcClient.top), 
							0,
							NULL, 
							hIns, 
							NULL );


	return true;
}

bool				CFrameWorks::CreateProcess()
{
	if( CreateDevice() == false )	
		return false;
	
	m_FrameTimer.OnInit();
	m_pApplication->OnCreateDevice();
	m_pApplication->OnResetDevice();

	m_bCreateDeviceCall = true;

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	if( RunProcess() == true )	
	{
		m_pApplication->OnLostDevice();
		m_pApplication->OnDeleteDevice();
	}

	return true;
}

bool				CFrameWorks::CreateDevice()
{
	if( m_pD3D == NULL )	return false;

	if( m_eDisplayMode == GwFrameWorksConstant::eDisplayMode_FullScreen )
	{
		m_d3dpp = *m_ConstructureD3DPP.GetPresentParameters_Full();

		m_d3dpp.hDeviceWindow			= m_hWnd;
		m_d3dpp.Windowed				= FALSE;
		m_d3dpp.BackBufferWidth			= m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode().Width;
		m_d3dpp.BackBufferHeight		= m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode().Height;
		m_d3dpp.FullScreen_RefreshRateInHz	= m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode().RefreshRate;
	}
	else if( m_eDisplayMode == GwFrameWorksConstant::eDisplayMode_Windows )
	{
		m_d3dpp = *m_ConstructureD3DPP.GetPresentParameters_Win();

		m_d3dpp.hDeviceWindow			= m_hWnd;
		m_d3dpp.Windowed				= TRUE;

		SetRect( &m_rcClient, 0, 0, m_dwWindowWidth, m_dwWindowHeight );
		AdjustWindowRect( &m_rcClient, m_dwWindowStyle, false );

		m_d3dpp.BackBufferWidth			= m_dwWindowWidth;
		m_d3dpp.BackBufferHeight		= m_dwWindowHeight;
	}

	if(FAILED( m_pD3D->CreateDevice(m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetAdpterSerialNum(), 
									m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDeviceInfo()->GetDevType(),
									m_hWnd, 
									m_ConstructureD3DPP.GetCurrVertexProcessing(),
									&m_d3dpp,
									&m_pd3dDevice) ) )
	{
		m_ConstructureD3DPP.BuildPresentParameters_Full(D3DDEVTYPE_REF);
		m_ConstructureD3DPP.BuildPresentParameters_Win(D3DDEVTYPE_REF);
		CreateDevice();
		return false;
	}

	return true;
}

bool				CFrameWorks::RunProcess()
{
	HACCEL hAccel = LoadAccelerators( NULL, MAKEINTRESOURCE(IDR_MAIN_ACCEL) );

	bool bGotMsg;
	MSG  msg;
	msg.message = WM_NULL;
	PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

	while( WM_QUIT != msg.message  )
	{
		bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );

		if( bGotMsg )
		{
			if( hAccel == NULL || m_hWnd == NULL || TranslateAccelerator( m_hWnd, hAccel, &msg ) == 0 )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			//Sleep(1);
			RenderFrame();
			m_FrameTimer.Update();
		}
	}
	if( hAccel != NULL )
		DestroyAcceleratorTable( hAccel );

	return true;
}

bool				CFrameWorks::RenderFrame()
{
	if( GetElapsedTime() != 0 )
		m_UpdateFunc[m_eUpdateType](GetElapsedTime());

	return true;
}

bool				CFrameWorks::UpdateNone(float fElapsedTime)
{
	CFrameWorks::GetInstance()->m_pApplication->Update(GetElapsedTime());
	return true;
}

bool				CFrameWorks::UpdateRender(float fElapsedTime)
{
	CFrameWorks::GetInstance()->m_pApplication->Update(CFrameWorks::GetInstance()->GetElapsedTime());

	if( SUCCEEDED(m_pd3dDevice->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(128, 128, 128), 1.0f, 0L)))
	{
		if( SUCCEEDED(m_pd3dDevice->BeginScene()))
		{
			CFrameWorks::GetInstance()->m_pApplication->Render(CFrameWorks::GetInstance()->GetElapsedTime());

			m_pd3dDevice->EndScene();
		}
	}

	if( m_pd3dDevice->Present(NULL,NULL,NULL,NULL) == D3DERR_DEVICELOST )
	{
		CFrameWorks::GetInstance()->m_eUpdateType = CFrameWorks::eUpdateType_Reset;
	}

	return true;
}

bool				CFrameWorks::UpdateReset(float fElapsedTime)
{
	HRESULT hr;
	if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel()) )
	{
		if( hr == D3DERR_DEVICELOST )
		{
			return false;
		}
		else if( hr == D3DERR_DEVICENOTRESET )
		{
			if( CFrameWorks::GetInstance()->m_eDisplayMode == GwFrameWorksConstant::eDisplayMode_Windows )
			{
				CFrameWorks::GetInstance()->OnWindowsMode(CFrameWorks::GetInstance()->m_dwWindowWidth, CFrameWorks::GetInstance()->m_dwWindowHeight, false);
			}
			else if( CFrameWorks::GetInstance()->m_eDisplayMode == GwFrameWorksConstant::eDisplayMode_FullScreen )
			{
				CFrameWorks::GetInstance()->OnFullScreenMode(false);
			}
		}
	}

	CFrameWorks::GetInstance()->m_eUpdateType = CFrameWorks::eUpdateType_Render;

	return true;
}

void				CFrameWorks::OnFullScreenMode(bool bLost)
{
	m_d3dpp = *m_ConstructureD3DPP.GetPresentParameters_Full();

	m_d3dpp.hDeviceWindow			= m_hWnd;
	m_d3dpp.Windowed				= FALSE;
	m_d3dpp.BackBufferWidth			= m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode().Width;
	m_d3dpp.BackBufferHeight		= m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode().Height;
	m_d3dpp.FullScreen_RefreshRateInHz	= m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode().RefreshRate;

	if( bLost )
		m_pApplication->OnLostDevice();

	m_bReSizeMsgBlock = true;

	m_dwWindowWidth		= m_d3dpp.BackBufferWidth;
	m_dwWindowHeight	= m_d3dpp.BackBufferHeight;

	SetRect( &m_rcClient, 0, 0, m_dwWindowWidth, m_dwWindowHeight );
	SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );

	m_bReSizeMsgBlock = true;

	::SetWindowPos(	m_hWnd, 
		HWND_NOTOPMOST,
		0,
		0,
		m_d3dpp.BackBufferWidth,
		m_d3dpp.BackBufferHeight,
		SWP_SHOWWINDOW );


	if( FAILED(m_pd3dDevice->Reset(&m_d3dpp)) )
	{
		m_eUpdateType = eUpdateType_Reset;
	}
	else
	{
		m_pApplication->OnResetDevice();
	}

	RECT rcClient;
	GetWindowRect(m_hWnd, &rcClient);
	ClipCursor(&rcClient);

	m_eDisplayMode = GwFrameWorksConstant::eDisplayMode_FullScreen;
	m_bReSizeMsgBlock = false;
}

void				CFrameWorks::OnWindowsMode(DWORD dwWindowWidth, DWORD dwWindowHeight, bool bLost)
{
	m_d3dpp.Windowed		= TRUE;
	m_dwWindowWidth			= dwWindowWidth;
	m_dwWindowHeight		= dwWindowHeight;

	m_d3dpp = *m_ConstructureD3DPP.GetPresentParameters_Win();

	m_d3dpp.hDeviceWindow			= m_hWnd;
	m_d3dpp.Windowed				= TRUE;
	m_d3dpp.BackBufferWidth			= m_dwWindowWidth;
	m_d3dpp.BackBufferHeight		= m_dwWindowHeight;

	if( bLost )
		m_pApplication->OnLostDevice();

	m_bReSizeMsgBlock = true;

	SetRect( &m_rcClient, 0, 0, m_dwWindowWidth, m_dwWindowHeight );
	AdjustWindowRect( &m_rcClient, m_dwWindowStyle, false );

	SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );

	D3DDISPLAYMODE	d3dDisplayMode = m_ConstructureD3DPP.GetCurrDeviceAdapterInfo()->GetCurrDisplayMode();

	POINT	point;
	point.x = (d3dDisplayMode.Width*0.5f) - ((m_rcClient.right - m_rcClient.left)*0.5f);
	point.y = (d3dDisplayMode.Height*0.5f) - ((m_rcClient.bottom - m_rcClient.top)*0.5f);

	m_bReSizeMsgBlock = true;

	::SetWindowPos(	m_hWnd, 
		HWND_NOTOPMOST,
		point.x,
		point.y,
		(m_rcClient.right - m_rcClient.left),
		(m_rcClient.bottom - m_rcClient.top),
		SWP_SHOWWINDOW );

	if( FAILED(m_pd3dDevice->Reset(&m_d3dpp)) )
	{
		m_eUpdateType = eUpdateType_Reset;
	}
	else
		m_pApplication->OnResetDevice();


	ClipCursor(NULL);

	m_eDisplayMode = GwFrameWorksConstant::eDisplayMode_Windows;
	m_bReSizeMsgBlock = false;
}

void				CFrameWorks::OnSizeChange()
{
	if( m_pd3dDevice == NULL ) return;

	RECT rc;
	GetClientRect(m_hWnd, &rc);

	if( rc.right - rc.left == 0 || rc.bottom - rc.top == 0 )	return;

	m_d3dpp.BackBufferWidth		= CFrameWorks::GetInstance()->m_dwWindowWidth	= rc.right - rc.left;
	m_d3dpp.BackBufferHeight	= CFrameWorks::GetInstance()->m_dwWindowHeight	= rc.bottom - rc.top;

	m_pApplication->OnLostDevice();

	if( FAILED(m_pd3dDevice->Reset(&m_d3dpp)) )
	{
		m_eUpdateType = eUpdateType_Reset;
	}
	else
	{
		m_pApplication->OnResetDevice();
	}
}

LRESULT CALLBACK	CFrameWorks::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	if( CFrameWorks::GetInstance()->m_bCreateDeviceCall )
		CFrameWorks::GetInstance()->m_pApplication->WndProc(hWnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_SIZE:
		{
			switch(wParam)
			{
			case SIZE_MINIMIZED:
				{
					CFrameWorks::GetInstance()->m_eUpdateType = CFrameWorks::eUpdateType_None;

					CFrameWorks::GetInstance()->m_bMinimized = true;
				}
				break;

			case SIZE_MAXIMIZED:
				{
					CFrameWorks::GetInstance()->m_dwPrevMaxmizedWidth	= CFrameWorks::GetInstance()->m_dwWindowWidth;
					CFrameWorks::GetInstance()->m_dwPrevMaxmizedHeight	= CFrameWorks::GetInstance()->m_dwWindowHeight;

					CFrameWorks::GetInstance()->m_bMaxmized = true;

					CFrameWorks::GetInstance()->OnSizeChange();
				}
				break;

			case SIZE_RESTORED:
				{
					if( CFrameWorks::GetInstance()->m_bMaxmized )
					{
						CFrameWorks::GetInstance()->OnWindowsMode(CFrameWorks::GetInstance()->m_dwPrevMaxmizedWidth, CFrameWorks::GetInstance()->m_dwPrevMaxmizedHeight);
						CFrameWorks::GetInstance()->m_bMaxmized = false;

						break;
					}
					else if( CFrameWorks::GetInstance()->m_bMinimized )
					{
						CFrameWorks::GetInstance()->m_eUpdateType = CFrameWorks::eUpdateType_Reset;
						CFrameWorks::GetInstance()->m_bMinimized = false;

						break;
					}
	
					if( CFrameWorks::GetInstance()->m_bReSizeMsgBlock == false )
						CFrameWorks::GetInstance()->OnSizeChange();
				}
				break;
			}
		}
		break;

	case WM_EXITSIZEMOVE:
		{
			CFrameWorks::GetInstance()->m_eUpdateType = CFrameWorks::eUpdateType_Reset;
			//CFrameWorks::GetInstance()->OnSizeChange();
		}
		break;

	case WM_CLOSE:
		{
			DestroyWindow( hWnd );
			PostQuitMessage(0);
			m_hWnd = NULL;

			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이하 (MFC, WIN HANDLE 이 생성 되어있는) 프레임워크
bool				CFrameWorks::OnCreateMainFrameWorks_ForMFC(CApplication * pApplication, HWND hWnd, DWORD dwWindowWidth, DWORD dwWindowHeight)
{
	m_pApplication->Release();
	m_pApplication = pApplication;
	pApplication->AddRef();

	m_dwWindowWidth		= dwWindowWidth;
	m_dwWindowHeight	= dwWindowHeight;	

	m_hWnd = hWnd;

	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	m_ConstructureD3DPP.CreatePresentParameters();

	if( CreateDevice() == false )	
		return false;

	m_FrameTimer.OnInit();
	m_pApplication->OnCreateDevice();
	m_pApplication->OnResetDevice();

	return true;
}

void				CFrameWorks::OnIdle()
{
	CFrameWorks::GetInstance()->RenderFrame();
	m_FrameTimer.Update();

	Sleep(1);
}

bool				CFrameWorks::ClearFrameWorks_ForMFC()
{
	m_pApplication->OnLostDevice();
	m_pApplication->OnDeleteDevice();

	return true;
}