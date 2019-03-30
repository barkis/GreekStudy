#pragma once
#include "afxwin.h"
#include "Resource.h"

// CFileSelectDlg dialog

class CFileSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFileSelectDlg)

public:
	CFileSelectDlg(CWnd* pParent = NULL);   // standard constructor
	CFileSelectDlg(CString strFileName,CString strPath,CString strExtension,bool IsSaveDlg = true,CWnd* pParent = NULL);  
	virtual ~CFileSelectDlg();

// Dialog Data
	enum { IDD = IDD_FILEDIALOGUE };
private:
	CString m_strFileName;
	CString m_strExtension;
	CString m_strPath;
	bool m_IsSaveDlg;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void FillList();
	DECLARE_MESSAGE_MAP()

	CListBox m_lstFiles;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListfiles();
	CString GetFileName(){ return m_strFileName; }
};
