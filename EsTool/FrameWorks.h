#pragma once

class	CApplication : public IGwRefObject
{
public:
	CApplication(void)		{ }
	virtual ~CApplication(void)		{ }

	virtual bool		OnCreateDevice()			{ return true; }
	virtual bool		OnResetDevice()				{ return true; }
	virtual bool		OnLostDevice()				{ return true; }
	virtual bool		OnDeleteDevice()			{ return true; }

	virtual bool		Update(float fElapsedTime)	{ return true; }
	virtual bool		Render(float fElapsedTime)	{ return true; }

	virtual bool	 	WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )	{ return true; }
private:

};

class CFrameWorks : public TGwGetInstance<CFrameWorks>
{
public:
	CFrameWorks(void);
	virtual ~CFrameWorks(void);

	// WIN32 frameworks 위한 생성
	bool				OnCreateMainFrameWorks(	CApplication * pApplication, 
												HINSTANCE hIns, 
												DWORD dwWindowWidth,
												DWORD dwWindowHeight,
												TCHAR * strWindowTitle,
												GwFrameWorksConstant::eDisplayMode eMode = GwFrameWorksConstant::eDisplayMode_Windows,
												DWORD dwWindowStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE );

	void				OnFullScreenMode(bool bLost = true);
	void				OnWindowsMode(DWORD dwWindowWidth, DWORD dwWindowHeight, bool bLost = true);

	static	HWND				GetHwnd();
	static	LPDIRECT3D9			GetD3D();
	static	LPDIRECT3DDEVICE9	GetDevice();
	static	float				GetProcessTime();
	static	float				GetElapsedTime();
	static	DWORD				GetWindowWidth();
	static	DWORD				GetWindowHeight();

	void				OnSizeChange();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 이하 (MFC, windows 생성되어있는) 프레임워크
	bool				OnCreateMainFrameWorks_ForMFC(CApplication * pApplication, HWND hWnd, DWORD dwWindowWidth, DWORD dwWindowHeight);
	void				OnIdle();
	bool				ClearFrameWorks_ForMFC();

private:

	bool				OnCreateWindows(HINSTANCE hIns, TCHAR * strWindowTitle);

	bool				CreateProcess();
	bool				RunProcess();
	bool				RenderFrame();

	bool				CreateDevice();

	static LRESULT CALLBACK	WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

	static	HWND				m_hWnd;
	static	LPDIRECT3D9			m_pD3D;
	static	LPDIRECT3DDEVICE9	m_pd3dDevice;
	D3DPRESENT_PARAMETERS		m_d3dpp;

	static	DWORD				m_dwWindowWidth;
	static	DWORD				m_dwWindowHeight;

	bool						m_bMaxmized;
	bool						m_bMinimized;
	DWORD						m_dwPrevMaxmizedWidth;
	DWORD						m_dwPrevMaxmizedHeight;

	DWORD						m_dwWindowStyle;

	RECT						m_rcClient;

	class	IFrameTimer;
	static	IFrameTimer			m_FrameTimer;

	CApplication			*	m_pApplication;
	CConstructureD3DPP			m_ConstructureD3DPP;
	
	GwFrameWorksConstant::eDisplayMode		m_eDisplayMode;

	//GwEventManager			*	m_pEventManager;

	bool						m_bReSizeMsgBlock;

	enum eUpdateType
	{
		eUpdateType_None = 0,
		eUpdateType_Render,
		eUpdateType_Reset,

		eUpdateType_Max,
	};

	eUpdateType					m_eUpdateType;
	typedef		bool			(*UpdateFunc)(float fElpasedTime);

	UpdateFunc					m_UpdateFunc[eUpdateType_Max];

	static		bool			UpdateNone(float fElapsedTime);
	static		bool			UpdateRender(float fElapsedTime);
	static		bool			UpdateReset(float fElapsedTime);

	bool						m_bCreateDeviceCall;
};

#include "FrameWorks.inl"