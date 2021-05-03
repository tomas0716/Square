#include "StdAfx.h"
#include "DX9DeviceAdapterInfo.h"

CDX9DeviceAdapterInfo::CDX9DeviceAdapterInfo(int nAdapterSerialNum)	: m_nAdapterSerialNum(nAdapterSerialNum), m_pCurrDeviceInfo(NULL)
{
}

CDX9DeviceAdapterInfo::~CDX9DeviceAdapterInfo(void)
{
	vector<CDX9DeviceInfo*>::iterator itr = m_DX9DeviceInfoList.begin();
	for( ; itr != m_DX9DeviceInfoList.end(); ++ itr )
	{
		(*itr)->Release();
	}
	m_DX9DeviceInfoList.clear();
	m_DisplayModeList.clear();
	m_AdapterFormatList.clear();
}

bool						CDX9DeviceAdapterInfo::CreateAdapterInfo()
{
	m_AdapterFormatList.clear();

	CFrameWorks::GetD3D()->GetAdapterIdentifier(m_nAdapterSerialNum, 0, &m_AdapterIndentifier);

	for( int i = 0; i < GwFrameWorksConstant::g_nNumD3DColorFormat; ++i )
	{
		UINT nNumAdapterModes = CFrameWorks::GetD3D()->GetAdapterModeCount(m_nAdapterSerialNum, GwFrameWorksConstant::g_D3DColorFormat[i]);

		for( UINT nAdapterModes = 0; nAdapterModes < nNumAdapterModes; ++nAdapterModes )
		{
			D3DDISPLAYMODE d3dDisplayMode;
			CFrameWorks::GetD3D()->EnumAdapterModes(m_nAdapterSerialNum, GwFrameWorksConstant::g_D3DColorFormat[i], nAdapterModes, &d3dDisplayMode);

			if( d3dDisplayMode.Width >= 640 && d3dDisplayMode.Height >= 480 )
			{
				m_DisplayModeList.push_back(d3dDisplayMode);

				if( IsAdapterFormatList(d3dDisplayMode.Format) == false )	
				{
					m_AdapterFormatList.push_back(d3dDisplayMode.Format);
				}
			}
		}
	}

	return CreateDeviceInfo();
}

bool						CDX9DeviceAdapterInfo::IsAdapterFormatList(D3DFORMAT d3dFormat)
{
	vector<D3DFORMAT>::iterator itr = m_AdapterFormatList.begin();

	for( ; itr != m_AdapterFormatList.end(); ++itr )
	{
		if( *itr == d3dFormat )
			return true;
	}

	return false;
}

int							CDX9DeviceAdapterInfo::GetAdpterSerialNum()
{
	return m_nAdapterSerialNum;
}

CDX9DeviceInfo			*	CDX9DeviceAdapterInfo::GetCurrDeviceInfo()
{
	return m_pCurrDeviceInfo;
}

D3DDISPLAYMODE				CDX9DeviceAdapterInfo::GetCurrDisplayMode()
{
	return m_CurrDisplayMode;
}

bool						CDX9DeviceAdapterInfo::CreateDeviceInfo()
{
	bool bInclude = false;
	for( int i = 0; i < GwFrameWorksConstant::g_nNumDeviceType; ++i )
	{
		CDX9DeviceInfo * pDeviceInfo = new CDX9DeviceInfo(GwFrameWorksConstant::g_DeviceType[i]);

		if( pDeviceInfo->CreateDeviceInfo(m_nAdapterSerialNum, m_AdapterFormatList) )
		{
			m_DX9DeviceInfoList.push_back(pDeviceInfo);
			
			bInclude = true;
		}
		else
		{
			pDeviceInfo->Release();
		}
	}

	return bInclude;
}

bool						CDX9DeviceAdapterInfo::FindDeviceInfo(D3DPRESENT_PARAMETERS &d3dpp, D3DDISPLAYMODE	&CurrDisplayMode, GwFrameWorksConstant::eDisplayMode eMode, D3DDEVTYPE &DevType, DWORD  &dwVertexProcessing)
{
	bool bFind = false;
	vector<D3DDISPLAYMODE>::iterator itr = m_DisplayModeList.begin();

	for( ; itr != m_DisplayModeList.end(); ++itr )
	{
		D3DDISPLAYMODE d3dDisplayMode = (*itr);

		if( d3dDisplayMode.Format == CurrDisplayMode.Format && d3dDisplayMode.RefreshRate == CurrDisplayMode.RefreshRate && 
			( (eMode == GwFrameWorksConstant::eDisplayMode_FullScreen) ? (d3dDisplayMode.Width == CurrDisplayMode.Width && d3dDisplayMode.Height == CurrDisplayMode.Height) : 1) )
		{
			m_CurrDisplayMode = d3dDisplayMode;
			bFind = true;
		}
	}

	if( bFind == false )	return false;

	vector<CDX9DeviceInfo*>::iterator itr_Info = m_DX9DeviceInfoList.begin();

	for( ; itr_Info != m_DX9DeviceInfoList.end(); ++itr_Info )
	{
		CDX9DeviceInfo * pDeviceInfo = (*itr_Info);

		if( pDeviceInfo && pDeviceInfo->FindDeviceInfo(d3dpp, CurrDisplayMode, eMode, DevType, dwVertexProcessing ))
		{
			m_pCurrDeviceInfo = pDeviceInfo;
			return true;
		}
	}

	return true;
}
