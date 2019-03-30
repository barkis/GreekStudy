#pragma once
#include "afxwin.h"
#include <vector>

// FileNamesDlg dialog

class CFileNamesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileNamesDlg)

public:
	CFileNamesDlg(std::vector<CString> &vFileNames,CWnd *pParent = NULL);
	virtual ~CFileNamesDlg();
	CString GetFileName(){return m_strFileName;}

// Dialog Data
	enum { IDD = IDD_DIALOGFILENAMES };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CListBox m_lstFileNames;
	CString m_strFileName;
	std::vector<CString> &m_vFileNames;
public:
	afx_msg void OnSelchangeListfilenames();
	virtual BOOL OnInitDialog();
};
