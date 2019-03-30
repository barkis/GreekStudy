// FileNamesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "FileNamesDlg.h"
#include "afxdialogex.h"


// FileNamesDlg dialog

IMPLEMENT_DYNAMIC(CFileNamesDlg, CDialogEx)


CFileNamesDlg::CFileNamesDlg(std::vector<CString> &vFileNames,CWnd *pParent/* = NULL*/): CDialogEx(CFileNamesDlg::IDD, pParent),
	m_strFileName(_T("")),m_vFileNames(vFileNames)
{
}

CFileNamesDlg::~CFileNamesDlg()
{
}

void CFileNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTFILENAMES, m_lstFileNames);
}


BEGIN_MESSAGE_MAP(CFileNamesDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LISTFILENAMES, &CFileNamesDlg::OnSelchangeListfilenames)
END_MESSAGE_MAP()


// CFileNamesDlg message handlers


void CFileNamesDlg::OnSelchangeListfilenames()
{
	int iSelection = m_lstFileNames.GetCurSel();
	if(iSelection != LB_ERR)	{
		m_lstFileNames.GetText(iSelection,m_strFileName);
	}
	else	{
		m_strFileName = _T("");
	}
}


BOOL CFileNamesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for(auto iter: m_vFileNames)	{
		CString strFileName = static_cast<CString>(iter);
		m_lstFileNames.AddString(strFileName);
	}
	m_lstFileNames.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
