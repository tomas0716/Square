#include "StdAfx.h"
#include "UIHelper.h"

HDC	CUIHelper::m_hDC = NULL;

CUIHelper::CUIHelper(void)
{
	m_hDC = CreateCompatibleDC(Gw::GetUIMgr()->GetMainDC());
}

CUIHelper::~CUIHelper(void)
{
	DeleteDC(m_hDC);
}


IGwUISprite		*	CUIHelper::CreateUISprite(const char * pszTex)
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwUIElement_MultiTex		tex;
	IGwUIImageInfo_Tex		*	pTexInfo;
	IGwUISprite				*	pSprite;

	IGwBillboardTex * pTex = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszTex);
	tex.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pTex );

	pTexInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex( tex );
	pSprite		= Gw::GetUIControlMgr()->CreateUISprite( pTexInfo );
	pTexInfo->Release();

	return pSprite;
}

IGwUIButton		*	CUIHelper::CreateUIButton(const char* strNormalTex, const char* strMouseOver, const char* strFocus, const char* strClick, const char* strDisable)
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwUIButton				*	pButton		= NULL;
	IGwUIElement_MultiTex		ElemTex;
	IGwBillboardTex			*	pBil		= NULL;

	pBil = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strNormalTex);
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pBil );

	pBil = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strMouseOver);
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_MOUSEOVER, pBil );

	pBil = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strFocus);
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_FOCUS, pBil );

	pBil = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strClick);
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_PRESSED, pBil );

	pBil = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strDisable);
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_DISABLED, pBil );

	IGwUIImageInfo_Tex	* pButtonImageInfo = NULL;
	pButtonImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemTex );
	pButton				= Gw::GetUIControlMgr()->CreateUIButton( pButtonImageInfo );

	pButtonImageInfo->Release();

	return pButton;
}

IGwUIButton		*	CUIHelper::CreateUIButton(const char* strTex)
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwUIButton				*	pButton		= NULL;
	IGwUIElement_MultiTex		ElemTex;
	IGwBillboardTex			*	pBil		= NULL;

	pBil = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strTex);
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pBil );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_MOUSEOVER, pBil );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_FOCUS, pBil );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_PRESSED, pBil );
	ElemTex.SetSpriteTex( GWUICONTROLSTATE_DISABLED, pBil );

	IGwUIImageInfo_Tex	* pButtonImageInfo = NULL;
	pButtonImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemTex );
	pButton				= Gw::GetUIControlMgr()->CreateUIButton( pButtonImageInfo );

	pButtonImageInfo->Release();

	return pButton;
}

IGwUIStatic		*	CUIHelper::CreateUIStatic(int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, bool IsShadow, int nShadowTick, GwColor shadowColor)
{
	IGwUIStatic			*	pStatic		= NULL;
	IGwUIFont			*   pFont		= NULL;
	IGwUIImageInfo_Font	*	pStaticImageInfo;
	IGwUIElement_FontExt	elemFont;

	pFont = Gw::GetUIMgr()->CreateUIFont(nFontSize,pszFontName,eWeightType, false);

	elemFont.SetFont(pFont);	pFont->Release();
	elemFont.SetColor( GWUICONTROLSTATE_NORMAL, fontColor );

	if (IsShadow == true)
	{
		elemFont.SetTextType(GWUITEXTTYPE_TABLETEXT_EXT);
		elemFont.SetShadowColor( GwColor(0.f, 0.f, 0.f, 1.f) );
		elemFont.SetColor(GWUICONTROLSTATE_NORMAL, fontColor, shadowColor);
		elemFont.SetOutlineSize(nShadowTick);
	}

	pStaticImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Font( elemFont );
	pStatic				= Gw::GetUIControlMgr()->CreateUIStatic( pStaticImageInfo );
	pStaticImageInfo->Release();

	return pStatic;
}
IGwUIStatic		*	CUIHelper::CreateUIStatic(int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor colorOutline, int iOutLineSize)
{
	IGwUIFont				*	pFont		= NULL;
	IGwUIElement_FontExt		elemFontExt;
	IGwUIImageInfo_Font		*	pFontImage	= NULL;

	pFont = Gw::GetUIMgr()->CreateUIFont(nFontSize,pszFontName,eWeightType, false);
	elemFontExt.SetFont( pFont );	pFont->Release();
	elemFontExt.SetColor(GWUICONTROLSTATE_NORMAL, fontColor ,colorOutline);

	elemFontExt.SetOutlineSize( iOutLineSize );

	pFontImage				= Gw::GetUIControlMgr()->CreateImageInfo_Font( elemFontExt );
	IGwUIStatic* pStatic	= Gw::GetUIControlMgr()->CreateUIStatic( pFontImage );
	pFontImage->Release();

	return pStatic;
}


IGwUIEditbox	*	CUIHelper::CreateUIEditBox(int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, GwColor SelectColor, GwColor ClrBoardColor)
{
	IGwUIImageInfo_Font*		pInfoFont;
	IGwUIImageInfo_Font*		pInfoSelFont;
	IGwUIImageInfo_ColorBoard*	pInfoSelBoard;
	IGwUIFont				*	pFont;

	pFont = Gw::GetUIMgr()->CreateUIFont(nFontSize,pszFontName,eWeightType, false);
	IGwUIElement_Font	elemFont;
	elemFont.SetFont(pFont);	pFont->Release();
	elemFont.SetColor(GWUICONTROLSTATE_NORMAL, fontColor);
	pInfoFont = Gw::GetUIControlMgr()->CreateImageInfo_Font(elemFont);

	elemFont.SetColor(GWUICONTROLSTATE_NORMAL, SelectColor );
	pInfoSelFont = Gw::GetUIControlMgr()->CreateImageInfo_Font(elemFont);

	IGwUIElement_ColorBoard	elemClrBoard; 
	elemClrBoard.SetColor(GWUICONTROLSTATE_NORMAL, ClrBoardColor );
	pInfoSelBoard = Gw::GetUIControlMgr()->CreateImageInfo_ColorBoard(elemClrBoard);

	IGwUIEditbox* pNewEditBox = Gw::GetUIControlMgr()->CreateUIEditbox(pInfoFont, pInfoSelFont, pInfoSelBoard);

	pInfoSelBoard->Release();
	pInfoSelFont->Release();
	pInfoFont->Release();

	return pNewEditBox;
}

IGwUICheckbox	*	CUIHelper::CreateUICheckBox(const char* strBaseTex, const char* strCheckTex)
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwBillboardTex			*	pBaseTex;
	IGwBillboardTex			*	pCheckTex;

	pBaseTex = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strBaseTex);
	pCheckTex = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, strCheckTex);

	IGwUIElement_MultiTex		ElemTexBase, ElemTexCheck;
	IGwUIImageInfo_Tex		*	pBaseImageInfo	= NULL;
	IGwUIImageInfo_Tex		*	pCheckImageInfo = NULL;
	IGwUICheckbox			*	pCheckBox		= NULL;

	ElemTexBase.SetSpriteTex(GWUICONTROLSTATE_NORMAL, pBaseTex);
	ElemTexCheck.SetSpriteTex(GWUICONTROLSTATE_NORMAL, pCheckTex);

	pBaseImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(ElemTexBase);
	pCheckImageInfo = Gw::GetUIControlMgr()->CreateImageInfo_Tex(ElemTexCheck);

	pCheckBox		= Gw::GetUIControlMgr()->CreateUICheckbox(pBaseImageInfo, pCheckImageInfo);

	pBaseImageInfo->Release();
	pCheckImageInfo->Release();

	return pCheckBox;
}

IGwUISlider_H	*	CUIHelper::CreateUISlider_H( const char * pszBase, const char * pszSliderThumb )
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwUISlider_H		*	pSlider		= NULL;
	IGwUIImageInfo_Tex	*	pInfoTrack	= NULL;
	IGwUIImageInfo_Tex	*	pInfoThumb	= NULL;

	IGwUIElement_MultiColor	elemSingle;

	IGwBillboardTex		* pTex[2]		= {NULL, };

	pTex[0] = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszBase);
	pTex[1] = CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszSliderThumb);

	elemSingle.SetSpriteTex( pTex[0] );	
	pInfoTrack = Gw::GetUIControlMgr()->CreateImageInfo_Tex(elemSingle);

	elemSingle.SetSpriteTex( pTex[1] );
	pInfoThumb = Gw::GetUIControlMgr()->CreateImageInfo_Tex(elemSingle);

	pSlider  = Gw::GetUIControlMgr()->CreateUISlider_H(pInfoTrack, pInfoThumb, true);
	pInfoTrack->Release();
	pInfoThumb->Release();

	return pSlider;
}

IGwUICombo		*	CUIHelper::CreateUIComboBox(  const char		* pszComboBaseTex, 
										  const char		* pszComboBoxBtnNormal,  
										  const char		* pszComboBoxBtnOver, 
										  const char		* pszComboBoxBtnClick,
										  const char		* pszListBaseTex,
										  const char		* pszTrackTex,
										  const char		* pszThumbTex,
										  const char		* pszPrevTex,
										  const char		* pszNextTex,
										  const GwVector2	& vComboBaseMargin,
										  float			      fCombobaseLength,
										  int				  iLineInterval,
										  bool                bComboBaseIncludeButton, 
										  bool				  bListIncludeScroll,
										  bool				  bTrackIncludeButton ) 
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwBillboardTex	* pComboBaseTex			=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszComboBaseTex);
	IGwBillboardTex	* pComboBoxBtnNormal	=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszComboBoxBtnNormal);
	IGwBillboardTex	* pComboBoxBtnOver		=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszComboBoxBtnOver);
	IGwBillboardTex	* pComboBoxBtnClick		=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszComboBoxBtnClick);
	IGwBillboardTex	* pListBaseTex			=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszListBaseTex);
	IGwBillboardTex	* pTrackTex				=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszTrackTex);
	IGwBillboardTex	* pThumbTex				=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszThumbTex);
	IGwBillboardTex	* pPrevTex				=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszPrevTex);
	IGwBillboardTex	* pNextTex				=  CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszNextTex);

	IGwUIElement_MultiTex	ElementMultiColor_ListBase;
	IGwUIElement_MultiTex	ElementMultiColor_ComboBase;
	IGwUIElement_MultiTex	multiButtonTex;
	IGwUIElement_MultiTex	ElemTrackTex,ElemThumb,ElemPrevBall,ElemNextBall;

	IGwUIImageInfo_Tex	* pImageInfo_ComboBase	= NULL;
	IGwUIImageInfo_Tex	* pImageInfo_Button		= NULL;
	IGwUIImageInfo_Tex	* pImageInfo_ListBase	= NULL;

	IGwUIImageInfo_Tex	* pTrackImageInfo		= NULL;
	IGwUIImageInfo_Tex	* pThumbImageInfo		= NULL; 
	IGwUIImageInfo_Tex	* pPrevBallImageInfo	= NULL;
	IGwUIImageInfo_Tex	* pNextBallImageInfo	= NULL;

	IGwUICombo			* pCombo = NULL;

	ElementMultiColor_ComboBase.SetSpriteTex( GWUICONTROLSTATE_NORMAL , pComboBaseTex );	pComboBaseTex->Release();
	pImageInfo_ComboBase = Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElementMultiColor_ComboBase);

	multiButtonTex.SetSpriteTex( GWUICONTROLSTATE_NORMAL,	pComboBoxBtnNormal );
	multiButtonTex.SetSpriteTex( GWUICONTROLSTATE_MOUSEOVER,pComboBoxBtnOver );
	multiButtonTex.SetSpriteTex( GWUICONTROLSTATE_PRESSED,  pComboBoxBtnClick );
	multiButtonTex.SetSpriteTex( GWUICONTROLSTATE_FOCUS,    pComboBoxBtnNormal );
	multiButtonTex.SetSpriteTex( GWUICONTROLSTATE_DISABLED, pComboBoxBtnNormal );


	pImageInfo_Button	= Gw::GetUIControlMgr()->CreateImageInfo_Tex( multiButtonTex );

	ElementMultiColor_ListBase.SetSpriteTex(GWUICONTROLSTATE_NORMAL, pListBaseTex );
	pListBaseTex->Release();

	pImageInfo_ListBase = Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElementMultiColor_ListBase);

	ElemTrackTex.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pTrackTex );
	ElemThumb.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pThumbTex );
	ElemPrevBall.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pPrevTex );
	ElemNextBall.SetSpriteTex( GWUICONTROLSTATE_NORMAL, pNextTex );

	pTrackImageInfo		= Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemTrackTex );
	pThumbImageInfo		= Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemThumb );
	pPrevBallImageInfo  = Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemPrevBall );
	pNextBallImageInfo  = Gw::GetUIControlMgr()->CreateImageInfo_Tex( ElemNextBall );

	pCombo = Gw::GetUIControlMgr()->CreateUICombo(	pImageInfo_ComboBase,pImageInfo_Button, pImageInfo_ListBase, 
													pTrackImageInfo, pThumbImageInfo, pPrevBallImageInfo, pNextBallImageInfo,
													vComboBaseMargin, fCombobaseLength, iLineInterval, 
													bComboBaseIncludeButton, bListIncludeScroll, bTrackIncludeButton );

	pImageInfo_ComboBase->Release();
	pImageInfo_Button->Release();
	pImageInfo_ListBase->Release();
	pTrackImageInfo->Release();
	pThumbImageInfo->Release();
	pPrevBallImageInfo->Release();
	pNextBallImageInfo->Release();

	return pCombo;
}

IGwUIScroll_V	*	CUIHelper::CreateUIScroll_V(const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext)
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwBillboardTex* pTrack	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszTrack);
	IGwBillboardTex* pThumb	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszThumb);
	IGwBillboardTex* pPrev	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszPrev);
	IGwBillboardTex* pNext	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszNext);

	IGwUIElement_MultiColor	elementTrack;
	IGwUIElement_MultiColor	elementThumb;
	IGwUIElement_MultiColor	elementPrev;
	IGwUIElement_MultiColor	elementNext;

	elementTrack.SetSpriteTex(pTrack);
	elementThumb.SetSpriteTex(pThumb);
	elementPrev.SetSpriteTex(pPrev);
	elementNext.SetSpriteTex(pNext);

	IGwUIImageInfo_Tex*	pTrackImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementTrack);
	IGwUIImageInfo_Tex*	pThumbImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementThumb);
	IGwUIImageInfo_Tex*	pPrevImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementPrev);
	IGwUIImageInfo_Tex*	pNextImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementNext);

	IGwUIScroll_V *	pScroll = Gw::GetUIControlMgr()->CreateUIScroll_V(pTrackImageInfo, pThumbImageInfo, pPrevImageInfo, pNextImageInfo);

	pNextImageInfo->Release();
	pPrevImageInfo->Release();
	pThumbImageInfo->Release();
	pTrackImageInfo->Release();

	return pScroll;
}

IGwUIScroll_H	*	CUIHelper::CreateUIScroll_H(const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext)
{
	const char * pszMediaPath = Gw::GetMtrlMgr()->GetMediaPath();

	IGwBillboardTex* pTrack	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszTrack);
	IGwBillboardTex* pThumb	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszThumb);
	IGwBillboardTex* pPrev	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszPrev);
	IGwBillboardTex* pNext	= CResourceManager::GetInstance()->CreateResource<IGwBillboardTex>(pszMediaPath, pszNext);

	IGwUIElement_MultiColor	elementTrack;
	IGwUIElement_MultiColor	elementThumb;
	IGwUIElement_MultiColor	elementPrev;
	IGwUIElement_MultiColor	elementNext;

	elementTrack.SetSpriteTex(pTrack);
	elementThumb.SetSpriteTex(pThumb);
	elementPrev.SetSpriteTex(pPrev);
	elementNext.SetSpriteTex(pNext);

	IGwUIImageInfo_Tex*	pTrackImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementTrack);
	IGwUIImageInfo_Tex*	pThumbImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementThumb);
	IGwUIImageInfo_Tex*	pPrevImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementPrev);
	IGwUIImageInfo_Tex*	pNextImageInfo	= Gw::GetUIControlMgr()->CreateImageInfo_Tex(elementNext);

	IGwUIScroll_H *	pScroll = Gw::GetUIControlMgr()->CreateUIScroll_H(pTrackImageInfo, pThumbImageInfo, pPrevImageInfo, pNextImageInfo);

	pNextImageInfo->Release();
	pPrevImageInfo->Release();
	pThumbImageInfo->Release();
	pTrackImageInfo->Release();

	return pScroll;
}

int					CUIHelper::CalcFirstItemIndexScroll_V(IGwUIScroll_V* pScroll, int iTotalCount, int iNumColumn, int iNumView)
{
	int	iFirstIdx = 0;
	int	iRange = iTotalCount;
	if( 0 == iNumColumn )
		return 0;

	if(iRange%iNumColumn != 0)
		iRange += iNumColumn - (iRange%iNumColumn);

	iRange -= iNumView;
	iRange /= iNumColumn;

	if(iRange <= 0)
	{
		iRange = 0;
	}

	pScroll->SetRange(0, iRange, 1);

	if(pScroll->GetCurrValue() < pScroll->GetRange_Max())
	{
		iFirstIdx = ((float)pScroll->GetCurrValue()/(float)pScroll->GetRange_Max()) * (iRange + 1);
	}
	else
	{
		iFirstIdx = iRange;
	}

	iFirstIdx = max(iFirstIdx, 0);
	iFirstIdx *= iNumColumn;

	return iFirstIdx;
}

int					CUIHelper::CalcFirstItemIndexScroll_H(IGwUIScroll_H* pScroll, int iTotalCount, int iNumColumn, int iNumView)
{
	int	iFirstIdx = 0;
	int	iRange = iTotalCount;
	if( 0 == iNumColumn )
		return 0;

	if(iRange%iNumColumn != 0)
		iRange += iNumColumn - (iRange%iNumColumn);

	iRange -= iNumView;
	iRange /= iNumColumn;

	if(iRange <= 0)
	{
		iRange = 0;
	}

	pScroll->SetRange(0, iRange, 1);

	if(pScroll->GetCurrValue() < pScroll->GetRange_Max())
	{
		iFirstIdx = ((float)pScroll->GetCurrValue()/(float)pScroll->GetRange_Max()) * (iRange + 1);
	}
	else
	{
		iFirstIdx = iRange;
	}

	iFirstIdx = max(iFirstIdx, 0);
	iFirstIdx *= iNumColumn;

	return iFirstIdx;
}

void				CUIHelper::FittingStringExtent(HFONT hFont, int iMaxExtent, IN const WCHAR* wstrOrig, OUT wstring& wstrResult, bool bMark)
{
	int		cchString	= wcslen(wstrOrig);
	int		iFit		= 0;
	SIZE	size;

	SelectObject(m_hDC, hFont);

	GetTextExtentExPointW(m_hDC, wstrOrig, cchString, iMaxExtent, &iFit, NULL, &size);

	if(bMark == true)
	{
		if(iFit > 2  &&  cchString != iFit)
		{
			wstrResult.clear();
			wstrResult.append(wstrOrig, iFit - 2);
			wstrResult.append(L"...");
		}
		else
		{
			wstrResult = wstrOrig;
		}
	}
	else
	{
		wstrResult.clear();
		wstrResult.append(wstrOrig, iFit);
	}
}

void				CUIHelper::FittingStringExtent(IGwUIFont* pFont, int iMaxExtent, IN const WCHAR* wstrOrig, OUT wstring& wstrResult, bool bMark)
{
	FittingStringExtent(pFont->GetHFont(), iMaxExtent, wstrOrig, wstrResult, bMark);
}