#pragma once

class IRandomF : public IGwRefObject
{
public:
	IRandomF();
	virtual ~IRandomF();

	void			SetRange(float Min, float Max);
	void			SetMin(float Min);
	void			SetMax(float Max);
	float			GetValue();

	float			GetMin();
	float			GetMax();

	BYTE		*	LoadFile(BYTE * pData);
	void			SaveFile(IFile & kFile);

private:
	float			m_fMin;
	float			m_fMax;
};