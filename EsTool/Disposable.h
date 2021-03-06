#pragma once

//////////////////////////////////////////////////////////////////////////
// 자원해제 인터페이스
//////////////////////////////////////////////////////////////////////////
class IDisposable
{
public:
	virtual	bool	IsDisposed() = 0;
	virtual	void	Dispose() = 0;
};
