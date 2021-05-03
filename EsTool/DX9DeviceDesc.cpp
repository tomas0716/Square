#include "StdAfx.h"
#include "DX9DeviceDesc.h"

CDX9DeviceDesc::CDX9DeviceDesc(BOOL bWindowed, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat)
: m_bWindowed(bWindowed), m_AdapterFormat(AdapterFormat), m_BackBufferFormat(BackBufferFormat)
{

}

CDX9DeviceDesc::~CDX9DeviceDesc()
{
	m_DepthStencilFormatList.clear();
	m_MultiSampleTypeList.clear();
	m_VertexProcessingList.clear();
	m_PresentIntervalList.clear();
}

bool							CDX9DeviceDesc::CreateDeviceDesc(int nAdapterSerialNum, D3DDEVTYPE devType, D3DCAPS9 &d3dCaps)
{
	if( CreateDepthStencilFormat(nAdapterSerialNum, devType) == false )
		return false;

	if( CreateMultiSampleType(nAdapterSerialNum, devType) == false )
		return false;

	if( CreateVertexProcessingType(d3dCaps) == false )
		return false;

	if( CreatePresentInterval(d3dCaps) == false )
		return false;

	return true;
}

bool							CDX9DeviceDesc::CreateDepthStencilFormat(int nAdapterSerialNum, D3DDEVTYPE devType)
{
	bool bInculde = false;
	for( int i = 0; i < GwFrameWorksConstant::g_nNumDepthBitsFormat; ++i )
	{
		if( SUCCEEDED( CFrameWorks::GetD3D()->CheckDeviceFormat(nAdapterSerialNum, devType, m_AdapterFormat, D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, GwFrameWorksConstant::g_DepthBitsFormat[i]) ) )
		{
			if( SUCCEEDED( CFrameWorks::GetD3D()->CheckDepthStencilMatch(nAdapterSerialNum, devType, m_AdapterFormat,m_BackBufferFormat, GwFrameWorksConstant::g_DepthBitsFormat[i] ) ) )
			{
				m_DepthStencilFormatList.push_back(GwFrameWorksConstant::g_DepthBitsFormat[i]);

				bInculde = true;
			}
		}
	}

	return bInculde;
}

bool							CDX9DeviceDesc::CreateMultiSampleType(int nAdapterSerialNum, D3DDEVTYPE devType)
{
	bool bInclude = false;
	for( int i = 0; i < GwFrameWorksConstant::g_nNumMultiSampleType; ++i )
	{
		DWORD dwQuality;
		if( SUCCEEDED( CFrameWorks::GetD3D()->CheckDeviceMultiSampleType(nAdapterSerialNum, devType, m_BackBufferFormat, m_bWindowed, GwFrameWorksConstant::g_MultiSampleType[i], &dwQuality ) ) )
		{
			m_MultiSampleTypeList.push_back(GwFrameWorksConstant::g_MultiSampleType[i]);
			bInclude = true;
		}
	}

	return bInclude;
}

bool							CDX9DeviceDesc::CreateVertexProcessingType(D3DCAPS9 &d3dCaps)
{
	if( (d3dCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0 )
	{
		if( ( d3dCaps.DevCaps & D3DDEVCAPS_PUREDEVICE ) != 0  )
		{
			m_VertexProcessingList.push_back(D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE);
		}
		
		m_VertexProcessingList.push_back(D3DCREATE_HARDWARE_VERTEXPROCESSING);
		m_VertexProcessingList.push_back(D3DCREATE_MIXED_VERTEXPROCESSING);
	}

	m_VertexProcessingList.push_back(D3DCREATE_SOFTWARE_VERTEXPROCESSING);

	return true;
}

bool							CDX9DeviceDesc::CreatePresentInterval(D3DCAPS9 &d3dCaps)
{
	bool bInclude = false;
	if( m_bWindowed )
	{
		for( int i = 0; i < GwFrameWorksConstant::g_nNumPresentIntervals_byWin; ++i )
		{
			if( d3dCaps.PresentationIntervals & GwFrameWorksConstant::g_dwPresentIntervals_byWin[i] )
			{
				m_PresentIntervalList.push_back(GwFrameWorksConstant::g_dwPresentIntervals_byWin[i]);

				bInclude = true;
			}
		}
	}
	else
	{
		for( int i = 0; i < GwFrameWorksConstant::g_nNumPresentIntervals_byFull; ++i )
		{
			if( d3dCaps.PresentationIntervals & GwFrameWorksConstant::g_dwPresentIntervals_byFull[i] )
			{
				m_PresentIntervalList.push_back(GwFrameWorksConstant::g_dwPresentIntervals_byFull[i]);

				bInclude = true;
			}
		}
	}

	if( bInclude == false )
	{
		m_PresentIntervalList.push_back(D3DPRESENT_INTERVAL_DEFAULT);
	}

	return true;
}

bool							CDX9DeviceDesc::FindDeviceInfo(D3DPRESENT_PARAMETERS &d3dpp, D3DDISPLAYMODE	&CurrDisplayMode, D3DDEVTYPE &DevType, DWORD  &dwVertexProcessing)
{
	if( CurrDisplayMode.Format == m_AdapterFormat && m_BackBufferFormat == m_AdapterFormat )
	{
		d3dpp.BackBufferFormat				= m_BackBufferFormat;
		d3dpp.MultiSampleType				= m_MultiSampleTypeList[(int)(m_MultiSampleTypeList.size() - 1)];
		d3dpp.MultiSampleQuality			= 0;
		d3dpp.AutoDepthStencilFormat		= m_DepthStencilFormatList[0];
		d3dpp.FullScreen_RefreshRateInHz	= 0;
		d3dpp.PresentationInterval			= m_PresentIntervalList[0];

		dwVertexProcessing					= m_VertexProcessingList[0];

		return true;
	}

	return false;
}
