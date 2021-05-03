#pragma once

class CConstructureD3DPP : public IGwRefObject
{
public:
	CConstructureD3DPP(void);
	~CConstructureD3DPP(void);

	bool								CreatePresentParameters();

	D3DPRESENT_PARAMETERS			*	GetPresentParameters_Win();
	D3DPRESENT_PARAMETERS			*	GetPresentParameters_Full();

	CDX9DeviceAdapterInfo			*	GetCurrDeviceAdapterInfo();
	DWORD								GetCurrVertexProcessing();

	bool								BuildPresentParameters_Win(D3DDEVTYPE DevType);
	bool								BuildPresentParameters_Full(D3DDEVTYPE DevType);

private:

	bool								RecursiveBuildPresentParameters_Win(D3DDISPLAYMODE &CurrDisplayMode, D3DDEVTYPE DevType);
	bool								RecursiveBuildPresentParameters_Full(D3DDEVTYPE DevType);

	D3DPRESENT_PARAMETERS				m_d3dpp_Win;
	D3DPRESENT_PARAMETERS				m_d3dpp_Full;
	vector<CDX9DeviceAdapterInfo*>		m_DeviceAdapterInfoList;

	DWORD								m_dwVertexProcessing;

	CDX9DeviceAdapterInfo			*	m_pCurrDeviceAdapterInfo;
};
