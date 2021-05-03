#pragma once

class IRandomClr : public IGwRefObject
{
public:
	IRandomClr();
	virtual ~IRandomClr();

	void			SetRange(GwColor Min, GwColor Max);
	void			SetMin(GwColor Min);
	void			SetMax(GwColor Max);
	GwColor			GetValue();

	GwColor			GetMin();
	GwColor			GetMax();

	BYTE		*	LoadFile(BYTE * pData);
	void			SaveFile(IFile & kFile);

private:
	GwColor			m_clrMin;
	GwColor			m_clrMax;
};