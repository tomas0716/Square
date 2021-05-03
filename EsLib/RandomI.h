#pragma once

class IRandomI : public IGwRefObject
{
public:
	IRandomI();
	virtual ~IRandomI();

	void			SetRange(int Min, int Max);
	void			SetMin(int Min);
	void			SetMax(int Max);
	int				GetValue();

	int				GetMin();
	int				GetMax();

	BYTE		*	LoadFile(BYTE * pData);
	void			SaveFile(IFile & kFile);

private:
	int				m_nMin;
	int				m_nMax;
};