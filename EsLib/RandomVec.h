#pragma once

class IRandomVec : public IGwRefObject
{
public:
	IRandomVec();
	virtual ~IRandomVec();

	void			SetRange(GwVector Min, GwVector Max);
	void			SetMin(GwVector Min);
	void			SetMax(GwVector Max);
	GwVector		GetValue();

	GwVector		GetMin();
	GwVector		GetMax();

	BYTE		*	LoadFile(BYTE * pData);
	void			SaveFile(IFile & kFile);

private:
	GwVector		m_vMin;
	GwVector		m_vMax;
};