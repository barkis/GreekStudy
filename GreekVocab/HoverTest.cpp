// HoverTest.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "HoverTest.h"
#include "afxdialogex.h"
#include "GreekVocabDoc.h"
#include "MainFrm.h"
#include "GreekVocabException.h"
#include <algorithm>

// CHoverTest dialog

IMPLEMENT_DYNAMIC(CHoverTestDlg, CDialogEx)

	CHoverTestDlg::CHoverTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHoverTestDlg::IDD, pParent),m_szAllFiles(_T("All Files")),m_MaxWidth(0)
{

}

CHoverTestDlg::~CHoverTestDlg()
{
}

void CHoverTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOHOVER, m_cboHover);
	DDX_Control(pDX, IDC_LISTHOVER, m_lstGreek);
}


BEGIN_MESSAGE_MAP(CHoverTestDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBOHOVER, &CHoverTestDlg::OnSelchangeCombohover)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CHoverTestDlg::OnTtnNeedText)
END_MESSAGE_MAP()


// CHoverTestDlg message handlers

/*
Set pointer to document and populate filenames combo. 
Set focus to the combo.
*/
BOOL CHoverTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_lstGreek.SetDlg((CDialogEx*)this);
	CWnd* pWnd = GetParent();
	CMainFrame *pFrame = (CMainFrame*)pWnd;
	m_pDoc = (CGreekVocabDoc*)pFrame->GetActiveDocument();
	const std::vector<CString> vFileNames = m_pDoc->GetVocabFileNames();
	m_cboHover.AddString(m_szAllFiles);
	for(CString strFileName: vFileNames)	{
		m_cboHover.AddString((LPCTSTR)strFileName);
	}
	m_cboHover.SetCurSel(0);
	m_cboHover.SetFocus();
	return FALSE;  // return TRUE unless you set the focus to a control

	// EXCEPTION: OCX Property Pages should return FALSE
}


void CHoverTestDlg::OnSelchangeCombohover()
{
	CString strFileName;
	m_cboHover.GetLBText(m_cboHover.GetCurSel(),strFileName);
	if(strFileName == m_szAllFiles)	{
		ReadAllFileLists((StringVectorMap*)m_pDoc->GetVocabListsMap());
	}
	else	{
		std::vector<CString> *pvList = m_pDoc->GetVocabFileList(strFileName);
		ReadFileList(pvList,true);
	}
	//m_lstGreek.EnableToolTips();
}

/*
Get the strings which CGreekVocabDoc read from the named file,split into Greek and English strings which are put into 
list box and map 
pFileList: pointer to vector of strings in Doc
ClearListBox: true if list box is to be cleared in preparation for adding new strings
*/
void CHoverTestDlg::ReadFileList(std::vector<CString> *pFileList,bool ClearListBox/* = false*/)	{
	CDC* pDC = m_lstGreek.GetDC();
	pDC->SetMapMode(MM_TEXT);
	if(ClearListBox)	{
		m_lstGreek.ResetContent();
		m_EnMap.clear();
		m_MaxWidth = 0;
	}
	std::vector<CString>::iterator iter;
	for(iter = pFileList->begin();iter != pFileList->end();++iter)	{
		CString strNextEntry = *iter;
		int iTabPos = strNextEntry.Find('\t');
		CString strGreek = strNextEntry.Left(iTabPos);
		CString strEn = strNextEntry.Mid(iTabPos + 1);
		int iPos = m_lstGreek.AddString(strGreek);
		//DrawText gets length of unicode string better than GetTextExtent
		CRect rect(0,0,0,0);
		pDC->DrawText(strGreek, &rect, DT_CALCRECT);
		m_MaxWidth = std::max<long>(rect.right,m_MaxWidth);

		std::pair<int,CString> pairEn = std::make_pair(iPos,strEn);
		m_EnMap.insert(pairEn);
	}
	m_lstGreek.SetColumnWidth(m_MaxWidth * 2);
	UpdateData(FALSE);
}

/*
Get all the strings which were read by CGreekVocabDoc and call ReadFilesList on each list.
pFilesListMap: map of file names to vectors of greek-english strings
(StringVectorMap is typedef in GreekVocabDoc.h
*/
void CHoverTestDlg::ReadAllFileLists(StringVectorMap *pFilesListMap)	{
	CString strNextEntry;
	StringVectorMap::iterator iter;
	if(m_lstGreek.GetSafeHwnd())	{
		m_lstGreek.ResetContent();
	}

	for(iter = pFilesListMap->begin();iter != pFilesListMap->end();++iter)	{
		ReadFileList(&iter->second);
	}		
}
BOOL CHoverTestDlg::OnTtnNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult)	{

	NMTTDISPINFO *pTTT = (NMTTDISPINFO *) pNMHDR;
	UINT_PTR nID = pNMHDR->idFrom;
	BOOL bRet = FALSE;
	if (pTTT->uFlags & TTF_IDISHWND)
	{
		// idFrom is actually the HWND of the tool      
		nID = ::GetDlgCtrlID((HWND)nID);
		if(nID == IDC_LISTHOVER)
		{
			int iSelection = m_lstGreek.GetSelection();
			if(iSelection > -1)	{
				auto iter = m_EnMap.find(iSelection);
				if(iter == m_EnMap.end())	{
					throw CVocabException(_T("Item selection not in translation map"),iSelection);
				}
				else	{
					CString strTranslation = iter->second;
					//TCHAR *pBuff = (TCHAR*)malloc(64*sizeof(TCHAR));
					//TCHAR *pBuff = new TCHAR[64];
					TCHAR pBuff[64];
					_tcscpy(pBuff,(LPCTSTR)strTranslation);
					//memset(pBuff,0,64*sizeof(TCHAR));
					//_tcscpy(pBuff,(LPCTSTR)strTranslation);
					//pTTT->lpszText = pBuff;
					_stprintf_s(pTTT->szText, sizeof(pTTT->szText) / sizeof(TCHAR), 
						pBuff, nID);

					//pTTT->hinst = AfxGetResourceHandle();
					//TCHAR szBuff[64];
					//  _stprintf_s(szBuff, sizeof(szBuff) / sizeof(TCHAR),
					//        (LPCTSTR)strTranslation, nID);
					//     pTTT->lpszText = szBuff;				
					pTTT->hinst = NULL;
				//	pTTT->hinst = /*AfxGetResourceHandle();*/AfxGetInstanceHandle();
					bRet = TRUE;
				}
			}
			CString strTranslation = _T( "test");
		}
	}

	*pResult = 0;

	return bRet;

}


CString CHoverTestDlg::GetTranslation(int Index) const	{
	CString strRetVal;
	std::map<int,CString>::const_iterator iter;
	iter =  m_EnMap.find(Index);
	if(iter == m_EnMap.end())	{
		throw CVocabException(_T("Index not found in GetTranslation()"),Index);
	}
	else	{
		strRetVal = iter->second;
	}
	return strRetVal;
}
