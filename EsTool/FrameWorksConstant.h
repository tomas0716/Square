#pragma once


namespace	GwFrameWorksConstant
{
	const D3DFORMAT			g_D3DColorFormat[]		= {	D3DFMT_R8G8B8,
														D3DFMT_A8R8G8B8,
														D3DFMT_X8R8G8B8,
														D3DFMT_R5G6B5,
														D3DFMT_X1R5G5B5,
														D3DFMT_A1R5G5B5,
														D3DFMT_A4R4G4B4,
														D3DFMT_R3G3B2,
														D3DFMT_A8,
														D3DFMT_A8R3G3B2,
														D3DFMT_X4R4G4B4,
														D3DFMT_A2B10G10R10,
														D3DFMT_A8B8G8R8,
														D3DFMT_X8B8G8R8,
														D3DFMT_G16R16,
														D3DFMT_A2R10G10B10,
														D3DFMT_A16B16G16R16 };

	const unsigned char		g_nNumD3DColorFormat	= (unsigned char)(sizeof(g_D3DColorFormat) / sizeof(g_D3DColorFormat[0]));

	const D3DDEVTYPE		g_DeviceType[]			= { D3DDEVTYPE_HAL,
														D3DDEVTYPE_SW, 
														D3DDEVTYPE_REF };

	const unsigned char		g_nNumDeviceType		= (unsigned char)(sizeof(g_DeviceType) / sizeof(g_DeviceType[0]));

	const D3DFORMAT			g_DepthBitsFormat[]		= { D3DFMT_D16,
														D3DFMT_D15S1,
														D3DFMT_D24X8,
														D3DFMT_D24S8,
														D3DFMT_D24X4S4,
														D3DFMT_D32  };

	const unsigned char		g_nNumDepthBitsFormat	= (unsigned char)(sizeof(g_DepthBitsFormat) / sizeof(g_DepthBitsFormat[0]));

	const D3DMULTISAMPLE_TYPE	g_MultiSampleType[]	= { D3DMULTISAMPLE_NONE,
														D3DMULTISAMPLE_NONMASKABLE,
														D3DMULTISAMPLE_2_SAMPLES,
														D3DMULTISAMPLE_3_SAMPLES,
														D3DMULTISAMPLE_4_SAMPLES,
														D3DMULTISAMPLE_5_SAMPLES,
														D3DMULTISAMPLE_6_SAMPLES,
														D3DMULTISAMPLE_7_SAMPLES,
														D3DMULTISAMPLE_8_SAMPLES,
														D3DMULTISAMPLE_9_SAMPLES,
														D3DMULTISAMPLE_10_SAMPLES,
														D3DMULTISAMPLE_11_SAMPLES,
														D3DMULTISAMPLE_12_SAMPLES,
														D3DMULTISAMPLE_13_SAMPLES,
														D3DMULTISAMPLE_14_SAMPLES,
														D3DMULTISAMPLE_15_SAMPLES,
														D3DMULTISAMPLE_16_SAMPLES };

	const unsigned char		g_nNumMultiSampleType	= (unsigned char)(sizeof(g_MultiSampleType) / sizeof(g_MultiSampleType[0]));


	const DWORD				g_dwPresentIntervals_byWin[]	= { D3DPRESENT_INTERVAL_IMMEDIATE,
																D3DPRESENT_INTERVAL_ONE };

	const int				g_nNumPresentIntervals_byWin	= (sizeof(g_dwPresentIntervals_byWin) / sizeof(g_dwPresentIntervals_byWin[0]));

	const DWORD				g_dwPresentIntervals_byFull[]	= { D3DPRESENT_INTERVAL_IMMEDIATE,
																D3DPRESENT_INTERVAL_ONE,
																D3DPRESENT_INTERVAL_TWO,
																D3DPRESENT_INTERVAL_THREE,
																D3DPRESENT_INTERVAL_FOUR };

	const int				g_nNumPresentIntervals_byFull	= (sizeof(g_dwPresentIntervals_byFull) / sizeof(g_dwPresentIntervals_byFull[0]));
	

	enum	eDisplayMode
	{
		eDisplayMode_FullScreen = 0,
		eDisplayMode_Windows,

		eDisplayMode_Max,
	};

};