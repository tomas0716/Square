#include "StdAfx.h"
#include "ConstructureD3DPP.h"

CConstructureD3DPP::CConstructureD3DPP(void) : m_pCurrDeviceAdapterInfo(NULL)
{
	m_d3dpp_Full.BackBufferCount		= m_d3dpp_Win.BackBufferCount			= 1;
	m_d3dpp_Full.SwapEffect				= m_d3dpp_Win.SwapEffect				= D3DSWAPEFFECT_DISCARD;
	m_d3dpp_Full.EnableAutoDepthStencil	= m_d3dpp_Win.EnableAutoDepthStencil	= true;
	m_d3dpp_Full.Flags					= m_d3dpp_Win.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

	m_dwVertexProcessing	= D3DCREATE_HARDWARE_VERTEXPROCESSING;			
}

CConstructureD3DPP::~CConstructureD3DPP(void)
{
	vector<CDX9DeviceAdapterInfo*>::iterator itr = m_DeviceAdapterInfoList.begin();
	for( ; itr != m_DeviceAdapterInfoList.end(); ++itr)
	{
		(*itr)->Release();
	}
	m_DeviceAdapterInfoList.clear();
}

bool								CConstructureD3DPP::CreatePresentParameters()
{
	UINT	nNumAdapters = CFrameWorks::GetD3D()->GetAdapterCount();

	for( UINT i = 0; i < nNumAdapters; ++i )
	{
		CDX9DeviceAdapterInfo * pAdapterInfo = new CDX9DeviceAdapterInfo(i);

		if( pAdapterInfo->CreateAdapterInfo() )
		{
			m_DeviceAdapterInfoList.push_back(pAdapterInfo);
		}
		else
			pAdapterInfo->Release();
	}

	BuildPresentParameters_Win(D3DDEVTYPE_HAL);
	BuildPresentParameters_Full(D3DDEVTYPE_HAL);

	return true;
}

bool								CConstructureD3DPP::BuildPresentParameters_Win(D3DDEVTYPE DevType)
{
	D3DDISPLAYMODE	CurrDisplayMode;
	CFrameWorks::GetD3D()->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &CurrDisplayMode);

	if( RecursiveBuildPresentParameters_Win(CurrDisplayMode, DevType) == true )
	{
		return true;
	}
	vector<CDX9DeviceAdapterInfo*>::iterator itr =	m_DeviceAdapterInfoList.begin();

	for( ; itr != m_DeviceAdapterInfoList.end(); ++itr )
	{
		CDX9DeviceAdapterInfo * pInfo = (*itr);

		if( pInfo && pInfo->FindDeviceInfo(m_d3dpp_Win, CurrDisplayMode, GwFrameWorksConstant::eDisplayMode_Windows, DevType, m_dwVertexProcessing) )
		{
			m_pCurrDeviceAdapterInfo = pInfo;
			return true;
		}
	}

	return false;
}

bool								CConstructureD3DPP::RecursiveBuildPresentParameters_Win(D3DDISPLAYMODE	&CurrDisplayMode, D3DDEVTYPE DevType)
{
	vector<CDX9DeviceAdapterInfo*>::iterator itr =	m_DeviceAdapterInfoList.begin();

	for( ; itr != m_DeviceAdapterInfoList.end(); ++itr )
	{
		CDX9DeviceAdapterInfo * pInfo = (*itr);

		if( pInfo && pInfo->FindDeviceInfo(m_d3dpp_Win, CurrDisplayMode, GwFrameWorksConstant::eDisplayMode_Windows, DevType, m_dwVertexProcessing) )
		{
			m_pCurrDeviceAdapterInfo = pInfo;
			return true;
		}
		else 
		{
			switch(DevType)
			{
			case D3DDEVTYPE_HAL: DevType = D3DDEVTYPE_REF;	break;
			case D3DDEVTYPE_REF: DevType = D3DDEVTYPE_SW;	break;
			case D3DDEVTYPE_SW: return false;
			}

			RecursiveBuildPresentParameters_Win(CurrDisplayMode, DevType);
		}
	}

	return false;
}

bool								CConstructureD3DPP::BuildPresentParameters_Full(D3DDEVTYPE DevType)
{
	if( RecursiveBuildPresentParameters_Full(DevType) == true )
	{
		return true;
	}

	vector<CDX9DeviceAdapterInfo*>::iterator itr =	m_DeviceAdapterInfoList.begin();

	for( ; itr != m_DeviceAdapterInfoList.end(); ++itr )
	{
		CDX9DeviceAdapterInfo * pInfo = (*itr);

		int nAdapterSerialNum = pInfo->GetAdpterSerialNum();

		D3DDISPLAYMODE	DisplayMode;
		CFrameWorks::GetD3D()->GetAdapterDisplayMode(nAdapterSerialNum, &DisplayMode);

		if( pInfo && pInfo->FindDeviceInfo(m_d3dpp_Full, DisplayMode, GwFrameWorksConstant::eDisplayMode_FullScreen, DevType, m_dwVertexProcessing) )
		{
			m_pCurrDeviceAdapterInfo = pInfo;
			return true;
		}
	}

	return false;
}

bool								CConstructureD3DPP::RecursiveBuildPresentParameters_Full(D3DDEVTYPE DevType)
{
	vector<CDX9DeviceAdapterInfo*>::iterator itr =	m_DeviceAdapterInfoList.begin();

	for( ; itr != m_DeviceAdapterInfoList.end(); ++itr )
	{
		CDX9DeviceAdapterInfo * pInfo = (*itr);

		int nAdapterSerialNum = pInfo->GetAdpterSerialNum();

		D3DDISPLAYMODE	DisplayMode;
		CFrameWorks::GetD3D()->GetAdapterDisplayMode(nAdapterSerialNum, &DisplayMode);

		if( pInfo && pInfo->FindDeviceInfo(m_d3dpp_Full, DisplayMode, GwFrameWorksConstant::eDisplayMode_FullScreen, DevType, m_dwVertexProcessing) )
		{
			m_pCurrDeviceAdapterInfo = pInfo;
			return true;
		}
		else 
		{
			switch(DevType)
			{
			case D3DDEVTYPE_HAL: DevType = D3DDEVTYPE_REF;	break;
			case D3DDEVTYPE_REF: DevType = D3DDEVTYPE_SW;	break;
			case D3DDEVTYPE_SW: return false;
			}

			RecursiveBuildPresentParameters_Full(DevType);
		}
	}

	return true;
}

D3DPRESENT_PARAMETERS			*	CConstructureD3DPP::GetPresentParameters_Win()
{
	return &m_d3dpp_Win;
}

D3DPRESENT_PARAMETERS			*	CConstructureD3DPP::GetPresentParameters_Full()
{
	return &m_d3dpp_Full;
}

CDX9DeviceAdapterInfo			*	CConstructureD3DPP::GetCurrDeviceAdapterInfo()
{
	return m_pCurrDeviceAdapterInfo;
}

DWORD								CConstructureD3DPP::GetCurrVertexProcessing()
{
	return m_dwVertexProcessing;
}
