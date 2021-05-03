#pragma once

class CDX9DeviceDesc : public IGwRefObject
{
public:
	CDX9DeviceDesc(BOOL bWindowed, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat);
	~CDX9DeviceDesc();

	bool							CreateDeviceDesc(int nAdapterSerialNum, D3DDEVTYPE devType, D3DCAPS9 &d3dCaps);

	void							SetAdapterFormat(D3DFORMAT AdapterFormat)		{ m_AdapterFormat = AdapterFormat; }
	void							SetBackBufferFormat(D3DFORMAT BackBufferFormat)	{ m_BackBufferFormat = BackBufferFormat; }

	bool							FindDeviceInfo(D3DPRESENT_PARAMETERS &d3dpp, D3DDISPLAYMODE	&CurrDisplayMode, D3DDEVTYPE &DevType, DWORD  &dwVertexProcessing);

	vector<D3DFORMAT>			*	GetDepthStencilFormatList()						{ return &m_DepthStencilFormatList; }
	vector<D3DMULTISAMPLE_TYPE> *	GetMultiSampleTypeList()						{ return &m_MultiSampleTypeList; }
	vector<DWORD>				*	GetVertexProcessingList()						{ return &m_VertexProcessingList; }
	vector<UINT>				*	GetPresentIntervalList()						{ return &m_PresentIntervalList; }

private:

	bool							CreateDepthStencilFormat(int nAdapterSerialNum, D3DDEVTYPE devType);
	bool							CreateMultiSampleType(int nAdapterSerialNum, D3DDEVTYPE devType);
	bool							CreateVertexProcessingType(D3DCAPS9 &d3dCaps);
	bool							CreatePresentInterval(D3DCAPS9 &d3dCaps);

	D3DFORMAT						m_AdapterFormat;
	D3DFORMAT						m_BackBufferFormat;
	BOOL							m_bWindowed;

	vector<D3DFORMAT>				m_DepthStencilFormatList;
	vector<D3DMULTISAMPLE_TYPE>		m_MultiSampleTypeList;
	vector<DWORD>					m_VertexProcessingList;
	vector<UINT>					m_PresentIntervalList;
};