#pragma once


// CScrollEdit
#include <vector>
#include<string>

class CScrollEdit : public CEdit
{
	DECLARE_DYNAMIC(CScrollEdit)

public:
	CScrollEdit();
	virtual ~CScrollEdit();

protected:
	std::vector<std::basic_string<TCHAR>> m_vLines;	
	std::basic_string<TCHAR> tsCurrentLine;
	
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	DECLARE_MESSAGE_MAP()
};


