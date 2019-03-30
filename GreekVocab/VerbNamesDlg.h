#pragma once

#include <vector>
#include "afxwin.h"
// CVerbNamesDlg dialog

class CVerbNamesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVerbNamesDlg)

public:
	CVerbNamesDlg(std::vector<CString> &VerbNames, CWnd* pParent = NULL);   // standard constructor
	virtual ~CVerbNamesDlg();
	CString GetVerbName(){ return m_strVerbName; }

// Dialog Data
	enum { IDD = IDD_DIALOGVERBNAMES };
private:
	std::vector<CString> &m_vVerbNames;
	CString m_strVerbName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lstVerbNames;
public:
	afx_msg void OnSelchangeListverbnames();
	virtual BOOL OnInitDialog();
};
