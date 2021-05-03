#pragma once

class CUIHelper
{
public:
	CUIHelper(void);
	~CUIHelper(void);

	static	IGwUISprite		*	CreateUISprite(const char * pszTex);

	static	IGwUIButton		*	CreateUIButton(const char* strNormalTex, const char* strMouseOver, const char* strFocus,
											   const char* strClick, const char* strDisable);

	static	IGwUIButton		*	CreateUIButton(const char* strTex);

	static	IGwUIStatic		*	CreateUIStatic(int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor,
											   bool IsShadow = false, int nShadowTick = 0, GwColor shadowColor = GwColor(0,0,0,0));

	static	IGwUIStatic		*	CreateUIStatic(int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor,
											   GwColor colorOutline = GwColor(0,0,0,0), int iOutLineSize = 0);

	static	IGwUIEditbox	*	CreateUIEditBox(int nFontSize, const char * pszFontName, GwFontWeightType eWeightType, GwColor fontColor, 
											    GwColor SelectColor, GwColor ClrBoardColor = GwColor(0,0,0,0));

	static	IGwUICheckbox	*	CreateUICheckBox(const char* strBaseTex, const char* strCheckTex);

	static	IGwUISlider_H	*	CreateUISlider_H(const char * pszBase, const char * pszSliderThumb);
	static	IGwUIScroll_V	*	CreateUIScroll_V(const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext);
	static	IGwUIScroll_H	*	CreateUIScroll_H(const char* pszTrack, const char* pszThumb, const char* pszPrev, const char* pszNext);
	static	int					CalcFirstItemIndexScroll_V(IGwUIScroll_V* pScroll, int iTotalCount, int iNumColumn, int iNumView);
	static	int					CalcFirstItemIndexScroll_H(IGwUIScroll_H* pScroll, int iTotalCount, int iNumColumn, int iNumView);

	static	IGwUICombo		*	CreateUIComboBox(const char			* pszComboBaseTex, 
												 const char			* pszComboBoxBtnNormal,  
												 const char			* pszComboBoxBtnOver, 
												 const char			* pszComboBoxBtnClick,
												 const char			* pszListBaseTex,
												 const char			* pszTrackTex,
												 const char			* pszThumbTex,
												 const char			* pszPrevTex,
												 const char			* pszNextTex,
												 const GwVector2	& vComboBaseMargin,
												 float				  fCombobaseLength,
												 int				  iLineInterval,
												 bool                 bComboBaseIncludeButton, 
												 bool			      bListIncludeScroll,
												 bool			      bTrackIncludeButton );

	template<class CallbackClass,class UserClass>
	static void					BindUIScrollCallbackFunction( IGwUIScrollBase* pScroll, void* pUserData );

	// 영역에 맞게 문자열 축약. (뒤에 ... 붙음)
	static void					  FittingStringExtent(HFONT hFont, int iMaxExtent, IN const WCHAR* wstrOrig, OUT wstring& wstrResult, bool bMark = true);
	static void					  FittingStringExtent(IGwUIFont* pFont, int iMaxExtent, IN const WCHAR* wstrOrig, OUT wstring& wstrResult, bool bMark = true);

	template<class DerivedFromButtonCallback>
	static	DerivedFromButtonCallback*	CreateButtonCallback();
	template<class DerivedFromButtonCallback, typename P1>
	static	DerivedFromButtonCallback*	CreateButtonCallback(const P1& p1);
	template<class DerivedFromButtonCallback, typename P1, typename P2>
	static	DerivedFromButtonCallback*	CreateButtonCallback(const P1& p1, const P2& p2);
	template<class DerivedFromButtonCallback, typename P1, typename P2, typename P3>
	static	DerivedFromButtonCallback*	CreateButtonCallback(const P1& p1, const P2& p2, const P3& p3);
	template<class DerivedFromButtonCallback, typename P1, typename P2, typename P3, typename P4>
	static	DerivedFromButtonCallback*	CreateButtonCallback(const P1& p1, const P2& p2, const P3& p3, const P4& p4);
	template<class DerivedFromButtonCallback, typename P1, typename P2, typename P3, typename P4, typename P5>
	static	DerivedFromButtonCallback*	CreateButtonCallback(const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5);

private:

	static	HDC			  m_hDC;
};

#include "UIHelper.inl"
