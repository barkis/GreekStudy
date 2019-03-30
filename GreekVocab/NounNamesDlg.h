#pragma once
#include <vector>
#include "afxwin.h"

// CNounNamesDlg dialog

class CNounNamesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNounNamesDlg)

public:
	CNounNamesDlg(std::vector<CString> &NounNames,CWnd* pParent = NULL);  
	virtual ~CNounNamesDlg();
	CString GetNounName(){return m_strNounName;}

// Dialog Data
	enum { IDD = IDD_DIALOGNOUNNAMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
private:
	std::vector<CString> &m_vNounNames;
	CString m_strNounName;
	CListBox m_lstNounNames;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListNounnames();
};
