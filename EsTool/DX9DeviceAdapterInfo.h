#pragma once


class CDX9DeviceAdapterInfo : public IGwRefObject
{
public:
	CDX9DeviceAdapterInfo(int nAdapterSerialNum);
	~CDX9DeviceAdapterInfo(void);

	bool						CreateAdapterInfo();

	int							GetAdpterSerialNum();
	CDX9DeviceInfo			*	GetCurrDeviceInfo();
	D3DDISPLAYMODE				GetCurrDisplayMode();

	bool						FindDeviceInfo(D3DPRESENT_PARAMETERS &d3dpp, D3DDISPLAYMODE	&CurrDisplayMode, GwFrameWorksConstant::eDisplayMode eMode, D3DDEVTYPE &DevType, DWORD  &dwVertexProcessing);

private:

	bool						IsAdapterFormatList(D3DFORMAT d3dFormat);
	bool						CreateDeviceInfo();

	int							m_nAdapterSerialNum;
	D3DADAPTER_IDENTIFIER9		m_AdapterIndentifier;
	vector<D3DDISPLAYMODE>		m_DisplayModeList;
	vector<CDX9DeviceInfo*>		m_DX9DeviceInfoList;

	vector<D3DFORMAT>			m_AdapterFormatList;

	CDX9DeviceInfo			*	m_pCurrDeviceInfo;
	D3DDISPLAYMODE				m_CurrDisplayMode;
};
