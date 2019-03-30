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
#include <string>
#include <fstream>
#include <cstring>

// CEnterDlg dialog

IMPLEMENT_DYNAMIC(CEnterDlg, CDialogEx)

	CEnterDlg::CEnterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEnterDlg::IDD, pParent),m_Scrolls(0),m_lstFilesCreated(false)
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
	DDX_Control(pDX, IDSAVE, btnSave);
	//DDX_Control(pDX, IDC_LISTEDITFILES, m_lstEditFiles);
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
	FILE *fStream;

	char szBufferGk[101];
	char szBufferEn[101];
	int iIndex = 0;
	CFileDialog fd(FALSE,_T("c:\\vocablists\\*.gkv"),_T("*.gkv"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Gk. vocab.files (*.gkv)|*.gkv|All files (*.*)|*.*||"));

	memset(szBufferGk,0,101);
	memset(szBufferEn,0,101);

	if(fd.DoModal() == IDOK)	{
		CString strFName = fd.GetPathName(); 
		//	sFname = szFName;
		//	LPCTSTR szFname = (LPCTSTR)strFName;
		TCHAR szFname[1001];
		_tcscpy(szFname,(LPCTSTR)strFName);
		errno_t e = _tfopen_s(&fStream,
			/**_T("C:\\Users\\Matthew\\Documents\\Visual Studio 2012\\Projects\\GreekVocab\\GreekVocab\\f2.gkv")*/
			szFname,_T("w,ccs=UNICODE"));


		if (e != 0)		{
			throw CVocabException(_T("_tfopen_s failed"),e);
		}
		CStdioFile f(fStream);  // open the file from this stream
		const int MAXLINE = 501*sizeof(TCHAR);
		TCHAR szNextGreekLine[MAXLINE];
		TCHAR szNextEnLine[MAXLINE];
		TCHAR szFullLine[MAXLINE * 2];
		memset(szNextGreekLine,0,MAXLINE);
		memset(szNextEnLine,0,MAXLINE);
		memset(szFullLine,0,MAXLINE * 2);
		int iLineNo = 0;
		int iGkBytesRead = m_edtGreek.GetLine(iLineNo,szNextGreekLine,MAXLINE-2);
		int iEnBytesRead = m_edtEn.GetLine(iLineNo++,szNextEnLine,MAXLINE-2);

		while (iGkBytesRead && iEnBytesRead)		{
			_tcscat(szNextGreekLine,_T("\t"));
			//	memmove(szFullLine,"\xff\xfe",2);
			_tcscpy(szFullLine /*+ 1*/,szNextGreekLine);
			_tcscat(szNextEnLine,_T("\n"));
			_tcscat(szFullLine,szNextEnLine);
			int iToWrite = (iGkBytesRead + iEnBytesRead + 2) * sizeof(TCHAR);
			iToWrite += (iToWrite & 1);	//odd numbers not allowed for unicode write
			f.Write(szFullLine,iToWrite);
			memset(szNextGreekLine,0,MAXLINE);
			memset(szNextEnLine,0,MAXLINE);
			memset(szFullLine,0,MAXLINE * 2);
			iGkBytesRead = m_edtGreek.GetLine(iLineNo,szNextGreekLine,MAXLINE-2);
			iEnBytesRead = m_edtEn.GetLine(iLineNo++,szNextEnLine,MAXLINE-2);
		} 
		f.Close();
		fclose(fStream);
	}
}


/* Save button is enabled if the boxes have equal numbers of entries */
void CEnterDlg::OnChangeEditenEditgk()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	int iGkLines = m_edtGreek.GetLineCount();
	if(LastLineIsEmpty(m_edtGreek))	{
		iGkLines--;
	}
	int iEnLines = m_edtEn.GetLineCount();
	if(LastLineIsEmpty(m_edtEn))	{
		iEnLines--;
	}
	CWnd* pSaveButton = GetDlgItem(IDSAVE);
	ASSERT(pSaveButton);
	pSaveButton->EnableWindow(iGkLines > 0 &&(iGkLines == iEnLines));
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
			pEdit->PostMessage(WM_VSCROLL,SB_VERT);
			//DoScroll(pEdit,iLineNo,szEdit);
		}

	}

	//	delete[] szWindowText;
}


void CEnterDlg::DoScroll(CEdit *pEdit,int iLineNo,const TCHAR *szEdit)	{
	//if below bottom scroll down
	TCHAR szClassName[101];
	GetClassName(pEdit->GetSafeHwnd(),szClassName,100);
	SCROLLINFO scrInfo;
	pEdit->GetScrollInfo(SB_VERT,&scrInfo);
	TRACE(_T("In %s, max:%d, Page:%d, Pos:%d\n"),szEdit,scrInfo.nMax,scrInfo.nPage,scrInfo.nPos);
	int iMax = scrInfo.nMax;
	//SetScrollRange(SB_VERT,0,iMax + 200,0);
	int iPrev = pEdit->SetScrollPos(SB_VERT,iMax,1);
	pEdit->GetScrollInfo(SB_VERT,&scrInfo);
	TRACE(_T("now max:%d, Page:%d, Pos:%d, was %d\n"),scrInfo.nMax,scrInfo.nPage,scrInfo.nPos,iPrev);
	pEdit->LineScroll(scrInfo.nPos - iLineNo + 1);
	TRACE(_T("Scrolled %d\n"),scrInfo.nMax - iLineNo + 1);
}

bool CEnterDlg::AtEndOFLine(const CEdit *pEditBox,int LineNo)	{
	int iLineStart = pEditBox->LineIndex(LineNo);
	int iLineEnd = iLineStart + pEditBox->LineLength(iLineStart) - 1;
	CPoint endCharPos = pEditBox->PosFromChar(iLineEnd);
	CPoint cursorPos = pEditBox->GetCaretPos();
	return(cursorPos.x >= endCharPos.x);
}

void CEnterDlg::OnVscrollEditgk()
{

}


void CEnterDlg::OnBnClickedButtonedit()
{
	CRect rect(0,0,500,100);
	CWnd *pWindow = (CWnd*)this;
	//	util::CentreRect(pWindow,rect);
	util::CentreRect(pWindow,rect);
	//	m_lstFiles.Create(ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN|WS_VSCROLL|WS_BORDER|WS_VISIBLE,
	//	rect,this,1);

	//m_lstFiles.m_hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, _T("LISTBOX"),_T( "Select File"), 
	//	WS_CHILD |WS_CAPTION | WS_VSCROLL | WS_HSCROLL|LBS_NOTIFY|WS_EX_TOPMOST , 
	//	rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, m_hWnd, 
	//	(HMENU)IDC_LISTEDITFILES, GetModuleHandle(NULL), NULL);
	//	m_lstFiles.SetActiveWindow();
	m_lstFiles.Create(/*WS_CHILD*/WS_OVERLAPPED |WS_CAPTION | WS_VSCROLL | WS_HSCROLL|LBS_NOTIFY|WS_EX_TOPMOST , 
		rect, this,IDC_LISTEDITFILES );
 	m_lstFiles.ShowWindow(SW_SHOW);
	m_lstFiles.SetForegroundWindow();
	m_lstFiles.SetFocus();
	m_lstFilesCreated = true;
	//m_lstFiles.SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//m_lstFiles.SetWindowPos(&wndTop,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//m_edtGreek.SetWindowPos(&m_lstFiles,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	//m_edtEn.SetWindowPos(&m_lstFiles,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	Invalidate();
	CGreekVocabDoc *pDoc = GetDocument();
	std::vector<CString> vFiles = pDoc->GetFileNames();
	std::vector<CString>::iterator iter;
	for(iter = vFiles.begin();iter != vFiles.end();++iter)	{
		m_lstFiles.AddString(*iter);
	}
	UpdateData(FALSE);

	//CListBox lstFileChoice;
	//RECT rect;
	//rect.top = 10;
	//rect.bottom = 200;
	//rect.left = 10;
	//rect.right = 300;
	//lstFileChoice.Create(ES_MULTILINE|ES_WANTRETURN|WS_CHILD|WS_VISIBLE,rect,this,1);
	//lstFileChoice.ShowWindow(SW_SHOW);
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
	/*	CRect rect;
	GetClientRect(&rect);
	m_rich.SetWindowPos(&wndTop,0,0,rect.right - rect.left,rect.bottom - rect.top,SWP_SHOWWINDOW);
	m_rich.SetWindowText(_T("Hello again world"));*/
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
	std::vector<CString>*pvLines  = pDoc->GetFileList(strFileName);
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
	if(pMsg->message == IDC_LISTEDITFILES||pMsg->wParam==IDC_LISTEDITFILES||pMsg->lParam==IDC_LISTEDITFILES)	{
		AfxMessageBox(_T("here"));
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
