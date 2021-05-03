#pragma once

enum ePlayType
{
	ePlay_Stop,
	ePlay_Pause,
	ePlay_Play,
	ePlay_Complete,
};

enum eEsRenderState
{
	eEsRenderState_AlphaBlending,		// Src : SrcAlpha,	Dest : InvScrAlpha
	eEsRenderState_Addtive,				// Src : SrcAlpha,	Dest : One
	eEsRenderState_SoftAddtive,			// Src : One,		Dest : InvDestColor
	eEsRenderState_Multiplicative,		// Src : DestColor, Dest : Zero
	eEsRenderState_2xMultiplicative,	// Src : DestColor,	Dest : SrcColor
};