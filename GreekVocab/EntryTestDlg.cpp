// TestDlg1.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "EntryTestDlg.h"
#include "afxdialogex.h"
#include "GreekVocabException.h"
#include <fstream>
#include <cstring>

// CVocabEntryTestDlg dialog

IMPLEMENT_DYNAMIC(CVocabEntryTestDlg, CDialogEx)

	CVocabEntryTestDlg::CVocabEntryTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVocabEntryTestDlg::IDD, pParent),m_Red(RGB(255,0,0)),m_Black(RGB(0,0,0)),m_Green(RGB(0,255,0)),m_szAllFiles (_T("All Files"))
	, m_strTry(_T(""))
{
	m_TextColor = m_Red;	
}

CVocabEntryTestDlg::~CVocabEntryTestDlg()
{
}

void CVocabEntryTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOFILES, m_cboFiles);
	DDX_Control(pDX, IDC_LISTGK, m_lstGreek);
	DDX_Control(pDX, IDC_LISTEN, m_lstEn);
	DDX_Text(pDX, IDC_TRY, m_strTry);
	DDX_Control(pDX, IDC_TRY, m_edtTry);
}


BEGIN_MESSAGE_MAP(CVocabEntryTestDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBOFILES, &CVocabEntryTestDlg::OnSelchangeCombofiles)
	ON_EN_CHANGE(IDC_TRY, &CVocabEntryTestDlg::OnChangeTry)
	ON_LBN_SELCHANGE(IDC_LISTGK, &CVocabEntryTestDlg::OnSelchangeListgk)
	ON_BN_CLICKED(IDC_BUTTONSHOW, &CVocabEntryTestDlg::OnBnClickedButtonshow)
	ON_BN_CLICKED(IDCLOSE, &CVocabEntryTestDlg::OnBnClickedClose)
	ON_EN_VSCROLL(IDC_LISTGK, &CVocabEntryTestDlg::OnVscrollListgk)
	ON_WM_VSCROLL()
	ON_MESSAGE(WM_LISTBOXVSCROLL,&CVocabEntryTestDlg::OnListBoxScroll)
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CVocabEntryTestDlg message handlers


BOOL CVocabEntryTestDlg::OnInitDialog()
{
	//	FILE *fStream;
	CString strGreekText, strEnText;
	CString strNextLine;
	//	TCHAR szNextFullLine[101];
	CDialogEx::OnInitDialog();

	CWnd* pWnd = GetParent();
	m_pFrame = (CMainFrame*)pWnd;
	m_pDoc = (CGreekVocabDoc*)m_pFrame->GetActiveDocument();

	const std::vector<CString> vFileNames = m_pDoc->GetVocabFileNames();
	m_cboFiles.AddString(m_szAllFiles);
	for(CString strFileName: vFileNames)	{
		m_cboFiles.AddString((LPCTSTR)strFileName);
	}
	m_cboFiles.SetCurSel(0);
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/*
	Set the colour on the entry box to red for no match, green for partial match, black for full match
*/
HBRUSH CVocabEntryTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_TRY)	{
		TRACE(_T("OnCtlColor for IDC_TRY\n"));
		pDC->SetTextColor(m_TextColor);
	}
	return hbr;
}

/*
Wnen a selection is made in the file names combo read the entries from the document, either those origintaing from
the selected file or all of them according to selection
*/
void CVocabEntryTestDlg::OnSelchangeCombofiles()
{
	CString strFileName;
	m_cboFiles.GetLBText(m_cboFiles.GetCurSel(),strFileName);
	if(strFileName == m_szAllFiles)	{
		ReadAllFileLists((StringVectorMap*)m_pDoc->GetVocabListsMap());
	}
	else	{
		std::vector<CString> *pvList = m_pDoc->GetVocabFileList(strFileName);
		m_lstGreek.ResetContent();
		m_lstEn.ResetContent();
		ReadFileList(pvList,true);
	}

}
/*
Get all the strings which were read by CGreekVocabDoc and call ReadFilesList on each list.
pFilesListMap: map of file names to vectors of greek-english strings
(StringVectorMap is typedef in GreekVocabDoc.h)
*/
void CVocabEntryTestDlg::ReadAllFileLists(StringVectorMap *pFilesListMap)	{
	CString strNextEntry;
	StringVectorMap::iterator iter;
	m_lstGreek.ResetContent();
	m_lstEn.ResetContent();

	for(iter = pFilesListMap->begin();iter != pFilesListMap->end();++iter)	{
		ReadFileList(&iter->second);
	}		
}

/*
Get the strings which CGreekVocabDoc read from the named file,split into Greek and English strings which are put into 
list box and map 
pFileList: pointer to vector of strings in Doc
ClearListBox: true if list box is to be cleared in preparation for adding new strings
*/
void CVocabEntryTestDlg::ReadFileList(std::vector<CString> *pFileList,bool ClearListBox/* = false*/)	{
	if(ClearListBox)	{
		m_lstGreek.ResetContent();
		m_EnMap.clear();
	}
	std::vector<CString>::iterator iter;
	for(iter = pFileList->begin();iter != pFileList->end();++iter)	{
		CString strNextEntry = *iter;
		int iTabPos = strNextEntry.Find('\t');
		CString strGreek = strNextEntry.Left(iTabPos);
		CString strEn = strNextEntry.Mid(iTabPos + 1);
		int iPos = m_lstGreek.AddString(strGreek);
		std::pair<int,CString> pairEn = std::make_pair(iPos,strEn);
		m_EnMap.insert(pairEn);
		m_lstEn.AddString(_T(""));	//blank spaces to be replaced as translations are achieved
	}
}

/*
	When the text box is modified check the entry against the correct translation and set the text colour value 
	accordingly.
*/
void CVocabEntryTestDlg::OnChangeTry()
{
	UpdateData(TRUE);
	int iSelection = m_lstGreek.GetCurSel();
	CString strTranslation = GetTranslation(iSelection);
	strTranslation.MakeLower();
	CString strTry = m_strTry;
	strTry.MakeLower();
	if(strTranslation == strTry)	{
		int iTopIndex = m_lstEn.GetTopIndex();
		m_TextColor = m_Black;
		m_lstEn.DeleteString(iSelection);

		m_lstEn.InsertString(iSelection ,m_strTry);
		m_lstEn.SetTopIndex(iTopIndex);	//restore scroll position
	}
	else	{
		if(strTranslation.Find(strTry) == 0)	{
			m_TextColor = m_Green;
		}
		else	{
			m_TextColor = m_Red;
		}
	}
	m_edtTry.Invalidate();
	UpdateData(FALSE);
}

/*
	When selection is changed in the Greek listbox copy the value of the English listbox to the entry box
*/
void CVocabEntryTestDlg::OnSelchangeListgk()
{
	int iCurSel = m_lstGreek.GetCurSel();
	m_lstEn.GetText(iCurSel, m_strTry);
	m_edtTry.SetFocus();
	UpdateData(FALSE);
}

/*
	When the "show translation" button is pressed put the translation in the entry box and the English list box
*/
void CVocabEntryTestDlg::OnBnClickedButtonshow()
{
	int iSelection = m_lstGreek.GetCurSel();
	CString strTranslation = GetTranslation(iSelection);
	m_strTry = strTranslation;
	m_TextColor = m_Black;
	int iTopIndex = m_lstEn.GetTopIndex();
	m_lstEn.DeleteString(iSelection);
	m_lstEn.InsertString(iSelection ,strTranslation);
	m_lstEn.SetTopIndex(iTopIndex);
	UpdateData(FALSE);
}


void CVocabEntryTestDlg::OnBnClickedClose()
{
	EndDialog(1);
}

CString CVocabEntryTestDlg::GetTranslation(int Key)	{
	CString strRetVal;
	std::map<int,CString>::iterator enIter;
	enIter = m_EnMap.find(Key);
	if(enIter == m_EnMap.end())	{
		throw CVocabException(_T("Invalid key to EnMap in CVocabEntryTestDlg::OnChangeTry"),Key);
	}
	else	{
		strRetVal = enIter->second;
	}
	ASSERT(strRetVal.GetLength() > 0);

	return strRetVal;
}

/*
	When Greek list box is scrolled adjust scrollbar on English edit box and scroll the contents
*/
void CVocabEntryTestDlg::OnVscrollListgk()
{
	AdjustScroll(&m_lstGreek,&m_lstEn);
}

void CVocabEntryTestDlg::LineScroll(CListBox *pLB,int LinesToScroll)	{
	int iTopIndex = pLB->GetTopIndex();
	pLB->SetTopIndex(iTopIndex + LinesToScroll);
}

void CVocabEntryTestDlg::AdjustScroll(CListBox *pScrolledBox, CListBox *pBoxToAdjust)	{
	SCROLLINFO scrollInfoScrolled, scrollInfoToAdjust;
	pScrolledBox->GetScrollInfo(SB_VERT, &scrollInfoScrolled);
	pBoxToAdjust->GetScrollInfo(SB_VERT, &scrollInfoToAdjust);
	LineScroll(pBoxToAdjust,scrollInfoScrolled.nPos - scrollInfoToAdjust.nPos);
	pBoxToAdjust->SetScrollInfo(SB_VERT, &scrollInfoScrolled);
	TRACE0("Invalidating\n");
	Invalidate();

}
//void CVocabEntryTestDlg::AdjustScroll(CListBox *pScrolledBox,CListBox *pBoxToAdjust)	{
//	SCROLLINFO scrollInfoScrolled,scrollInfoToAdjust;
//	pScrolledBox->GetScrollInfo(SB_VERT,&scrollInfoScrolled);
//	pBoxToAdjust->GetScrollInfo(SB_VERT,&scrollInfoToAdjust);
//	LineScroll(pBoxToAdjust,scrollInfoScrolled.nPos - scrollInfoToAdjust.nPos);
//	pBoxToAdjust->SetScrollInfo(SB_VERT,&scrollInfoScrolled);
//	Invalidate();
//}

LRESULT CVocabEntryTestDlg::OnListBoxScroll(WPARAM wParam,LPARAM lParam)	{
	AdjustScroll(&m_lstGreek,&m_lstEn);
	return 0;
}



BOOL CVocabEntryTestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN)	{
			CWnd *pWnd = GetFocus();
			CWnd *pTryEdit = GetDlgItem(IDC_TRY);
			if (pWnd == pTryEdit)	{
				HandleArrow(pMsg->wParam == VK_UP);
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
/*
	When up or down arrow pressed in the entry box change the selection in the list.
	Called by PreTranslateMessage
	IsUp: true if up arrow pressed
*/
void CVocabEntryTestDlg::HandleArrow(bool IsUp)	{
	int iListSel = m_lstGreek.GetCurSel();
	if (LB_ERR == iListSel)	{
		iListSel = 0;
	}
	iListSel += (IsUp ? -1 : 1);
	if (iListSel < 0)	{
		iListSel = 0;
	}
	if (iListSel >= m_lstGreek.GetCount())	{
		iListSel = m_lstGreek.GetCount() - 1;
	}
	m_lstGreek.SetCurSel(iListSel);
	m_lstEn.SetCurSel(iListSel);
	OnSelchangeListgk();
	UpdateData(FALSE);
}