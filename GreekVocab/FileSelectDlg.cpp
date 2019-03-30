// FileSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileSelectDlg.h"
#include "afxdialogex.h"
#include "FileSelectDlg.h"


// CFileSelectDlg dialog

IMPLEMENT_DYNAMIC(CFileSelectDlg, CDialogEx)

CFileSelectDlg::CFileSelectDlg(CWnd* pParent /*= NULL*/)   // standard constructor
: CDialogEx(CFileSelectDlg::IDD, pParent)
{

}

CFileSelectDlg::CFileSelectDlg(CString strFileName, CString strPath, CString strExtension, bool IsSaveDlg/* = true*/,
	CWnd* pParent/* = NULL*/) : m_strFileName(strFileName), m_strPath(strPath), m_strExtension(strExtension), m_IsSaveDlg(IsSaveDlg),
	CDialogEx(CFileSelectDlg::IDD, pParent)
{
}

CFileSelectDlg::~CFileSelectDlg()
{
}

void CFileSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITFILENAME, m_strFileName);
	DDX_Text(pDX, IDC_EDITEXTENSION, m_strExtension);
	DDX_Control(pDX, IDC_LISTFILES, m_lstFiles);
}

void CFileSelectDlg::FillList()	{
	int iErrNo;
	CFileFind fileFind;
	int iSelection = -1;
	iErrNo = fileFind.FindFile(m_strPath + "\\" + m_strExtension, 0);
	if (!iErrNo)	{
		int iFindFileErr = GetLastError();
	}
	else	{
		while (iErrNo)	{
			iErrNo = fileFind.FindNextFile();
			if (!iErrNo)	{
				if (GetLastError() != ERROR_NO_MORE_FILES)	{
					break;
				}
			}
			CString strNext = fileFind.GetFileName();
			int iNextLine = m_lstFiles.AddString(strNext);
			if (strNext == m_strFileName)	{
				iSelection = iNextLine;
			}
		}
	}
	if (iSelection != -1)	{
		m_lstFiles.SetCurSel(iSelection);
	}
}


BEGIN_MESSAGE_MAP(CFileSelectDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LISTFILES, &CFileSelectDlg::OnSelchangeListfiles)
END_MESSAGE_MAP()


// CFileSelectDlg message handlers


BOOL CFileSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	FillList();
	if (m_strFileName.IsEmpty())	{
		m_lstFiles.SetFocus();
	}
	else	{
		CWnd *pWnd = GetDlgItem(IDC_EDITFILENAME);
		pWnd->SetFocus();
	}
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CFileSelectDlg::OnSelchangeListfiles()
{
	m_lstFiles.GetText(m_lstFiles.GetCurSel(), m_strFileName);
	UpdateData(FALSE);
}
