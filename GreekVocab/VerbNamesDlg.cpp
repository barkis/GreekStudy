// VerbNamesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "VerbNamesDlg.h"
#include "afxdialogex.h"


// CVerbNamesDlg dialog

IMPLEMENT_DYNAMIC(CVerbNamesDlg, CDialogEx)

CVerbNamesDlg::CVerbNamesDlg(std::vector<CString> &VerbNames, CWnd* pParent/* = NULL */) :
CDialogEx(CVerbNamesDlg::IDD, pParent), m_vVerbNames(VerbNames)	{

}
CVerbNamesDlg::~CVerbNamesDlg()
{
}

void CVerbNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTVERBNAMES, m_lstVerbNames);
}


BEGIN_MESSAGE_MAP(CVerbNamesDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LISTVERBNAMES, &CVerbNamesDlg::OnSelchangeListverbnames)
END_MESSAGE_MAP()


// CVerbNamesDlg message handlers


void CVerbNamesDlg::OnSelchangeListverbnames()
{
	int iSelection = m_lstVerbNames.GetCurSel();
	if (iSelection != LB_ERR)	{
		m_lstVerbNames.GetText(iSelection, m_strVerbName);
	}
	else	{
		m_strVerbName = _T("");
	}
}


BOOL CVerbNamesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (auto item : m_vVerbNames)	{
		m_lstVerbNames.AddString(item);
	}
	m_lstVerbNames.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
