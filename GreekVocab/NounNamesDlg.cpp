// NounNamesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "NounNamesDlg.h"
#include "afxdialogex.h"


// CNounNamesDlg dialog

IMPLEMENT_DYNAMIC(CNounNamesDlg, CDialogEx)

CNounNamesDlg::CNounNamesDlg(std::vector<CString> &NounNames,CWnd* pParent) :
	CDialogEx(CNounNamesDlg::IDD,pParent),m_vNounNames(NounNames)	{

}

CNounNamesDlg::~CNounNamesDlg()
{
}

void CNounNamesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTNOUNNAMES, m_lstNounNames);
}


BEGIN_MESSAGE_MAP(CNounNamesDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LISTNOUNNAMES, &CNounNamesDlg::OnSelchangeListNounnames)
END_MESSAGE_MAP()


// CNounNamesDlg message handlers


BOOL CNounNamesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for(auto item: m_vNounNames)	{
		m_lstNounNames.AddString(item);
	}	
	m_lstNounNames.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CNounNamesDlg::OnSelchangeListNounnames()
{
	int iSelection = m_lstNounNames.GetCurSel();
	if(iSelection != LB_ERR)	{
		m_lstNounNames.GetText(iSelection,m_strNounName);
	}
	else	{
		m_strNounName = _T("");
	}
}
