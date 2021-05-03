#pragma once

class CDX9DeviceInfo : public IGwRefObject
{
public:
	CDX9DeviceInfo(D3DDEVTYPE devType);
	~CDX9DeviceInfo(void);

	bool						CreateDeviceInfo(int nAdapterSerialNum, vector<D3DFORMAT> & AdapterFormatList);
	bool						FindDeviceInfo(D3DPRESENT_PARAMETERS &d3dpp, D3DDISPLAYMODE	&CurrDisplayMode, GwFrameWorksConstant::eDisplayMode eMode, D3DDEVTYPE &DevType, DWORD  &dwVertexProcessing);

	CDX9DeviceDesc			*	GetCurrDeviceDesc();

	D3DDEVTYPE					GetDevType();
	D3DCAPS9					GetD3DCaps();
private:

	D3DDEVTYPE					m_DevType;
	D3DCAPS9					m_d3dCaps;

	vector<CDX9DeviceDesc*>		m_DX9DeviceDescList[GwFrameWorksConstant::eDisplayMode_Max];

	CDX9DeviceDesc			*	m_pCurrDeviceDesc;
};
