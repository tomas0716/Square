#include "StdAfx.h"
#include "DX9DeviceInfo.h"

CDX9DeviceInfo::CDX9DeviceInfo(D3DDEVTYPE devType) : m_DevType(devType), m_pCurrDeviceDesc(NULL)
{
}

CDX9DeviceInfo::~CDX9DeviceInfo(void)
{
	for( int nWindows = GwFrameWorksConstant::eDisplayMode_FullScreen; nWindows < GwFrameWorksConstant::eDisplayMode_Max; ++nWindows )
	{
		vector<CDX9DeviceDesc*>::iterator itr = m_DX9DeviceDescList[nWindows].begin();
		for(; itr != m_DX9DeviceDescList[nWindows].end(); ++itr)
		{
			(*itr)->Release();
		}
		m_DX9DeviceDescList[nWindows].clear();
	}
}

bool						CDX9DeviceInfo::CreateDeviceInfo(int nAdapterSerialNum, vector<D3DFORMAT> & AdapterFormatList)
{
	if( SUCCEEDED(CFrameWorks::GetD3D()->GetDeviceCaps(nAdapterSerialNum, m_DevType, &m_d3dCaps)) )
	{
		for( int i = 0; i < (int)AdapterFormatList.size(); ++i )
		{
			D3DFORMAT Format = AdapterFormatList[i];

			for( int j = 0; j < GwFrameWorksConstant::g_nNumD3DColorFormat; ++j )
			{
				for( int nWindows = GwFrameWorksConstant::eDisplayMode_FullScreen; nWindows < GwFrameWorksConstant::eDisplayMode_Max; ++nWindows )
				{
					if( SUCCEEDED( CFrameWorks::GetD3D()->CheckDeviceType(nAdapterSerialNum, m_DevType, AdapterFormatList[i], GwFrameWorksConstant::g_D3DColorFormat[j], (BOOL)nWindows)) )
					{
						CDX9DeviceDesc * pDeviceDesc = new CDX9DeviceDesc((BOOL)nWindows, AdapterFormatList[i], GwFrameWorksConstant::g_D3DColorFormat[j]);

						if( pDeviceDesc->CreateDeviceDesc(nAdapterSerialNum, m_DevType, m_d3dCaps) )
						{
							m_DX9DeviceDescList[nWindows].push_back(pDeviceDesc);
						}
						else
						{
							pDeviceDesc->Release();
						}
					}
				}
			}
		}

		return true;
	}

	return false;
}

bool						CDX9DeviceInfo::FindDeviceInfo(D3DPRESENT_PARAMETERS &d3dpp, D3DDISPLAYMODE	&CurrDisplayMode, GwFrameWorksConstant::eDisplayMode eMode, D3DDEVTYPE &DevType, DWORD  &dwVertexProcessing)
{
	if( m_DevType == DevType )
	{
		vector<CDX9DeviceDesc*>::iterator itr =	m_DX9DeviceDescList[eMode].begin();

		for( ; itr != m_DX9DeviceDescList[eMode].end(); ++itr )
		{
			CDX9DeviceDesc * pDeviceDesc = (*itr);

			if( pDeviceDesc && pDeviceDesc->FindDeviceInfo(d3dpp, CurrDisplayMode, DevType, dwVertexProcessing) )
			{
				m_pCurrDeviceDesc = pDeviceDesc;
				return true;
			}
		}
	}
	else
		return false;

	return true;
}

CDX9DeviceDesc			*	CDX9DeviceInfo::GetCurrDeviceDesc()
{
	return m_pCurrDeviceDesc;
}
D3DDEVTYPE					CDX9DeviceInfo::GetDevType()
{
	return m_DevType;
}

D3DCAPS9					CDX9DeviceInfo::GetD3DCaps()
{
	return m_d3dCaps;
}