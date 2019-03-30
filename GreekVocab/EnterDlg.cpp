// EnterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "EnterDlg.h"
#include "GreekVocabException.h"
#include "afxdialogex.h"
#include "mainfrm.h"
#include "util.h"
#include "GreekVocabDoc.h"
#include "FileNamesDlg.h"
#include "FileSelectDlg.h"

#include <string>
#include <fstream>
#include <cstring>

// CEnterDlg dialog

IMPLEMENT_DYNAMIC(CEnterDlg, CDialogEx)

	CEnterDlg::CEnterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterDlg::IDD, pParent), m_Scrolls(0), m_lstFilesCreated(false), m_strFileName(_T("")), m_IsDirty(false)
{

}

CEnterDlg::~CEnterDlg()
{
}

void CEnterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITGK, m_edtGreek);
	DDX_Control(pDX, IDC_EDITEN, m_edtEn);
	DDX_Control(pDX, IDSAVE, m_btnSave);
}


BEGIN_MESSAGE_MAP(CEnterDlg, CDialogEx)
	ON_BN_CLICKED(IDSAVE, &CEnterDlg::OnClickedIdsave)
	ON_EN_CHANGE(IDC_EDITEN, &CEnterDlg::OnChangeEditenEditgk)
	ON_EN_CHANGE(IDC_EDITGK, &CEnterDlg::OnChangeEditenEditgk)
	ON_EN_SETFOCUS(IDC_EDITEN, &CEnterDlg::OnSetfocusEditen)
	ON_EN_KILLFOCUS(IDC_EDITEN, &CEnterDlg::OnKillfocusEditen)
	ON_EN_KILLFOCUS(IDC_EDITGK, &CEnterDlg::OnKillfocusEditgk)
	ON_EN_SETFOCUS(IDC_EDITGK, &CEnterDlg::OnSetfocusEditgk)
	ON_EN_VSCROLL(IDC_EDITGK, &CEnterDlg::OnVscrollEditgk)
	ON_LBN_SELCHANGE(IDC_LISTEDITFILES, &CEnterDlg::OnSelChangeEditList)
	ON_BN_CLICKED(IDC_BUTTONEDIT, &CEnterDlg::OnBnClickedButtonedit)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_EN_VSCROLL(IDC_EDITEN, &CEnterDlg::OnVscrollEditen)
	ON_BN_CLICKED(IDNEW, &CEnterDlg::OnBnClickedNew)
END_MESSAGE_MAP()


// CEnterDlg message handlers


BOOL CEnterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_edtGreek.SetFocus();
	CWnd* pWnd = GetParent();
	CMainFrame* pTheFrame = (CMainFrame*)pWnd;
	m_edtGreek.SetFrame(pTheFrame);
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CEnterDlg::OnClickedIdsave()
{
	Save();
}

/* save entered details
 Called by Save and New button handlers
 Returns false if user cancels save dialog or doesn't select a file.
*/
bool CEnterDlg::Save()	{
	
	FILE *fStream;
	bool bRetVal = false;

	char szBufferGk[101];
	char szBufferEn[101];
	int iIndex = 0;
	CString strDefault;
	strDefault = _T("*.gkv");
	CGreekVocabDoc *pDoc = GetDocument();
	std::basic_string<TCHAR> sPath(pDoc->getPath());
	sPath += _T("\\vocablists\\");

	memset(szBufferGk, 0, 101);
	memset(szBufferEn, 0, 101);
	TCHAR aFilter[] = _T("Vocabulary file(*.gkv)\0*.gkv\0");
	CFileSelectDlg dlg(m_strFileName, sPath.c_str(), strDefault, true, this);
	if (dlg.DoModal() == IDOK)	{
		m_strFileName = dlg.GetFileName();
	}
	if (!m_strFileName.IsEmpty())	{
		bRetVal = true;
		if (m_strFileName.Right(4).MakeLower() != ".gkv")	{
			m_strFileName += ".gkv";
		}
		TCHAR szFname[1001];
		_tcscpy(szFname, sPath.c_str());
		_tcscat(szFname, (LPCTSTR)m_strFileName);
		errno_t e = _tfopen_s(&fStream,
			szFname, _T("w,ccs=UNICODE"));
		if (e != 0)		{
			throw CVocabException(_T("_tfopen_s failed"), e);
		}
		CStdioFile f(fStream);  // open the file from this stream
		const int MAXLINE = 501 * sizeof(TCHAR);
		TCHAR szNextGreekLine[MAXLINE];
		TCHAR szNextEnLine[MAXLINE];
		TCHAR szFullLine[MAXLINE * 2];
		memset(szNextGreekLine, 0, MAXLINE);
		memset(szNextEnLine, 0, MAXLINE);
		memset(szFullLine, 0, MAXLINE * 2);
		int iLineNo = 0;
		int iGkBytesRead = m_edtGreek.GetLine(iLineNo, szNextGreekLine, MAXLINE - 2);
		int iEnBytesRead = m_edtEn.GetLine(iLineNo++, szNextEnLine, MAXLINE - 2);

		while (iGkBytesRead && iEnBytesRead)		{
			_tcscat(szNextGreekLine, _T("\t"));
			_tcscpy(szFullLine /*+ 1*/, szNextGreekLine);
			_tcscat(szNextEnLine, _T("\n"));
			_tcscat(szFullLine, szNextEnLine);
			int iToWrite = (iGkBytesRead + iEnBytesRead + 2) * sizeof(TCHAR);
			iToWrite += (iToWrite & 1);	//odd numbers not allowed for unicode write
			f.Write(szFullLine, iToWrite);
			//	f.Write(szNextGreekLine,(iGkBytesRead + 1) *sizeof(TCHAR));
			memset(szNextGreekLine, 0, MAXLINE);
			memset(szNextEnLine, 0, MAXLINE);
			memset(szFullLine, 0, MAXLINE * 2);
			iGkBytesRead = m_edtGreek.GetLine(iLineNo, szNextGreekLine, MAXLINE - 2);
			iEnBytesRead = m_edtEn.GetLine(iLineNo++, szNextEnLine, MAXLINE - 2);
		}
		f.Close();
		fclose(fStream);
		pDoc->ReadVocabList(szFname);
	}
	return bRetVal;
}

/* Save button is enabled if the boxes have equal numbers of entries */
void CEnterDlg::OnChangeEditenEditgk()
{

	TRACE0("OnChangeEditenEditGk\n");
	m_IsDirty = true;

	UpdateData(TRUE);
	int iGkLines = m_edtGreek.GetLineCount();
	if(LastLineIsEmpty(m_edtGreek))	{
		iGkLines--;
	}
	int iEnLines = m_edtEn.GetLineCount();
	//don't include empty line in line count
	if(LastLineIsEmpty(m_edtEn))	{
		iEnLines--;
	}
	//Some text has been entered so allow user to save it and/or start anew
	CWnd* pButton = GetDlgItem(IDSAVE);
	ASSERT(pButton);

	//Don't allow saving if there's an unmatched Greek entry
	pButton->EnableWindow(iGkLines > 0 &&(iGkLines == iEnLines));

	pButton = GetDlgItem(IDNEW);
	ASSERT(pButton);
	pButton->EnableWindow(TRUE);
	UpdateData(FALSE);
}

bool CEnterDlg::LastLineIsEmpty(const CEdit &EditBox)	{
	int iLineCount = EditBox.GetLineCount();
	char szLineText[6];
	memset(szLineText,0,6);
	int iBytesCopied = EditBox.GetLine(iLineCount - 1,(LPTSTR)szLineText,2);
	bool bRetVal = !iBytesCopied;	//true for empty line
	return bRetVal;
}

/* clear selection on getting focus */
void CEnterDlg::OnSetfocusEditen()
{
	//DoScroll(&m_edtGreek);
	m_edtEn.SetSel(-1,0,TRUE);
}

void CEnterDlg::OnSetfocusEditgk()
{
	//DoScroll(&m_edtGreek);
	m_edtGreek.SetSel(-1,TRUE);
}

/* if tabbing from the last line add a newline */
void CEnterDlg::OnKillfocusEditen()
{
	EditBoxLostFocus(&m_edtEn,_T("EditEn"));
}


void CEnterDlg::OnKillfocusEditgk()
{
	EditBoxLostFocus(&m_edtGreek,_T("EditGk"));
}

void CEnterDlg::EditBoxLostFocus(CEdit *pEdit,const TCHAR *szEdit)	{
	int iLineCount = pEdit->GetLineCount();
	int iCurrentLineIndex = pEdit->LineIndex();
	int iLineNo = pEdit->LineFromChar(iCurrentLineIndex);
	int iLineLength = pEdit->LineLength(iLineNo);
	//	TCHAR* szWindowText = new TCHAR[(iCurrentLineIndex + iLineLength + 2)*sizeof(TCHAR)];
	//	memset(szWindowText,0,sizeof(szWindowText));
	if(iLineNo == iLineCount - 1)	{
		//CString theText;
		//pEdit->GetWindowText(szWindowText,(iCurrentLineIndex + iLineLength)*sizeof(TCHAR));
		//_tcscat(szWindowText,_T("\r\n"));
		//pEdit->SetWindowText(szWindowText);
		if(AtEndOFLine(pEdit, iLineNo))	{
			pEdit->PostMessageW(WM_CHAR,'\n');
			if (!_tcscmp(szEdit, _T("EditEn")))	{
				pEdit->PostMessage(WM_VSCROLL, SB_VERT);
			}
			//DoScroll(pEdit,iLineNo,szEdit);
		}

	}

	//	delete[] szWindowText;
}


void CEnterDlg::DoScroll(CEdit *pEdit,int iLineNo,const TCHAR *szEdit)	{
	//if below bottom scroll down
	SCROLLINFO scrInfo;
	pEdit->GetScrollInfo(SB_VERT,&scrInfo);
	int iMax = scrInfo.nMax;
	int iPrev = pEdit->SetScrollPos(SB_VERT,iMax,1);
	pEdit->GetScrollInfo(SB_VERT,&scrInfo);
	pEdit->LineScroll(scrInfo.nPos - iLineNo + 1);
}

bool CEnterDlg::AtEndOFLine(const CEdit *pEditBox,int LineNo)	{
	int iLineStart = pEditBox->LineIndex(LineNo);
	int iLineEnd = iLineStart + pEditBox->LineLength(iLineStart) - 1;
	CPoint endCharPos = pEditBox->PosFromChar(iLineEnd);
	CPoint cursorPos = pEditBox->GetCaretPos();
	return(cursorPos.x >= endCharPos.x);
}



void CEnterDlg::OnBnClickedButtonedit()
{
	CGreekVocabDoc *pDoc = GetDocument();
	std::vector<CString> vFiles = pDoc->GetVocabFileNames();
	CFileNamesDlg dlg(vFiles,this);
	CString strGreekWindowText, strEnWindowText;
	if(dlg.DoModal() == IDOK)	{
		m_strFileName = dlg.GetFileName();
		m_edtGreek.Clear();
		m_edtEn.Clear();
		CGreekVocabDoc *pDoc = GetDocument();
		std::vector<CString>*pvLines  = pDoc->GetVocabFileList(m_strFileName);
		std::vector<CString>::iterator iter;
		for(iter = pvLines->begin();iter != pvLines->end();++iter)	{
			CString strLine = *iter;
			int iTabPos = strLine.Find('\t');
			CString strGreek  = strLine.Left(iTabPos);
			CString strEn = strLine.Mid(iTabPos + 1);
			strGreekWindowText += strGreek + _T("\r\n");
			strEnWindowText += strEn + _T("\r\n");
		}
		m_edtGreek.SetWindowText(strGreekWindowText);
		m_edtEn.SetWindowText(strEnWindowText);
		m_btnSave.EnableWindow();
		m_edtGreek.SetFocus();
		m_edtGreek.PutCaretAtEnd();
		m_IsDirty = false;
	}
}


int CEnterDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;

}

void CEnterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_lstFilesCreated)	{
		CRect rect;
		util::CentreRect((CWnd*)this,rect);

		m_lstFiles.SetWindowPos(&wndTopMost, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,SWP_SHOWWINDOW);
		m_lstFiles.SetWindowPos(&wndTop, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);
	}
}

/*
When user makes a selection of a file to edit read the entries from that file which are stored in the document
*/
void CEnterDlg::OnSelChangeEditList()	{
	CString strFileName;
	CString strGreekWindowText, strEnWindowText;
	m_lstFiles.GetText(m_lstFiles.GetCurSel(),strFileName);
	//m_lstFiles.DestroyWindow();
	//m_lstFilesCreated = false;
	m_edtGreek.Clear();
	m_edtEn.Clear();
	CGreekVocabDoc *pDoc = GetDocument();
	std::vector<CString>*pvLines  = pDoc->GetVocabFileList(strFileName);
	std::vector<CString>::iterator iter;
	for(iter = pvLines->begin();iter != pvLines->end();++iter)	{
		CString strLine = *iter;
		int iTabPos = strLine.Find('\t');
		CString strGreek  = strLine.Left(iTabPos);
		CString strEn = strLine.Mid(iTabPos + 1);
		strGreekWindowText += strGreek + _T("\r\n");
		strEnWindowText += strEn + _T("\r\n");
	}
	m_edtGreek.SetWindowTextW(strGreekWindowText);
	m_edtEn.SetWindowTextW(strEnWindowText);
}

CGreekVocabDoc *CEnterDlg::GetDocument()	{
	CWnd *pWnd = GetParent();
	CMainFrame* pTheFrame = (CMainFrame*)pWnd;
	CGreekVocabDoc *pDoc = static_cast<CGreekVocabDoc*>(pTheFrame->GetActiveDocument());
	return pDoc;
}

BOOL CEnterDlg::PreTranslateMessage(MSG* pMsg)
{

	return CDialogEx::PreTranslateMessage(pMsg);
}

/*
	When English edit box is scrolled adjust scrollbar on Greek edit box and scroll the contents
*/
void CEnterDlg::OnVscrollEditen()
{
	AdjustScroll(&m_edtEn, &m_edtGreek);
}

/*
	When Greek edit box is scrolled adjust scrollbar on English edit box and scroll the contents
*/
void CEnterDlg::OnVscrollEditgk()
{
	AdjustScroll(&m_edtGreek,&m_edtEn);
}

void CEnterDlg::AdjustScroll(CEdit *pScrolledBox,CEdit *pBoxToAdjust)	{
	SCROLLINFO scrollInfoScrolled,scrollInfoToAdjust;
	pScrolledBox->GetScrollInfo(SB_VERT,&scrollInfoScrolled);
	pBoxToAdjust->GetScrollInfo(SB_VERT,&scrollInfoToAdjust);
	pBoxToAdjust->LineScroll(scrollInfoScrolled.nPos - scrollInfoToAdjust.nPos);
	pBoxToAdjust->SetScrollInfo(SB_VERT,&scrollInfoScrolled);
	TRACE0("Invalidating\n");
	Invalidate();

}

void CEnterDlg::OnBnClickedNew()
{
	bool bContinue = true;
	if (!m_IsDirty)	{
		Clear();
	}
	else	{


		short iMessRet = AfxMessageBox(_T("Save changes?"), MB_YESNOCANCEL);

		if (iMessRet == IDYES)
		{
			if (Save())	{
				Clear();
			}
		}
		else if (iMessRet == IDNO)	{
			Clear();
		}
	}
}

void CEnterDlg::Clear()	{
	m_edtGreek.SetWindowText(_T(""));
	m_edtEn.SetWindowText(_T(""));
	m_IsDirty = false;
	CWnd* pButton = GetDlgItem(IDSAVE);
	ASSERT(pButton);
	pButton->EnableWindow(FALSE);
	pButton = GetDlgItem(IDNEW);
	ASSERT(pButton);
	pButton->EnableWindow(FALSE);
	m_edtGreek.SetFocus();
	UpdateData(FALSE);
}