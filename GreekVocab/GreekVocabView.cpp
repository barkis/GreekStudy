
// GreekVocabView.cpp : implementation of the CGreekVocabView class
//
#include <vld.h>
#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GreekVocab.h"
#endif

#include "GreekVocabDoc.h"
#include "GreekVocabView.h"
#include "MainFrm.h"
#include "EnterDlg.h"
#include "HoverTest.h"
#include "GreekVocabException.h"
#include "EntryTestDlg.h"
#include "NounDlg.h"
#include "NounEntryTestDlg.h"
#include "AdjectiveDlg.h"
#include "VerbDlg.h"
#include "VerbEntryTestDlg.h"
#include <fstream>
#include <algorithm>
#include <cmath>	//for ceil
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGreekVocabView

IMPLEMENT_DYNCREATE(CGreekVocabView, CScrollView)

BEGIN_MESSAGE_MAP(CGreekVocabView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_VOCABULARY_ENTRY, &CGreekVocabView::OnVocabularyEntry)
	ON_COMMAND(ID_VOCABULARY_ENTRYTEST, &CGreekVocabView::OnVocabularyTest)
	ON_COMMAND(ID_VOCABULARY_HOVERTEST, &CGreekVocabView::OnVocabularyHovertest)
	ON_COMMAND(ID_GRAMMAR_NOUNS, &CGreekVocabView::OnGrammarNouns)
	ON_COMMAND(ID_NOUNS_ENTRYTEST, &CGreekVocabView::OnNounsEntrytest)
	ON_COMMAND(ID_ADJECTIVES_ENTER, &CGreekVocabView::OnAdjectivesEnter)
	ON_COMMAND(ID_VERBS_ENTER, &CGreekVocabView::OnVerbsEnter)
	ON_COMMAND(ID_VERBS_ENTRYTEST, &CGreekVocabView::OnVerbsEntrytest)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CGreekVocabView construction/destruction

CGreekVocabView::CGreekVocabView() :m_pDlg(NULL), m_plblStatus(nullptr)
{
}

CGreekVocabView::~CGreekVocabView()
{
}

BOOL CGreekVocabView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CScrollView::PreCreateWindow(cs);
}

// CGreekVocabView drawing
void CGreekVocabView::OnDraw(CDC* pDC)
{
	CGreekVocabDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) {
		return;

	}

	//Display fullvocabulary list in columns
	RECT clientRect;
	GetClientRect(&clientRect);
	int iWindoweight = clientRect.bottom - STATUSHEIGHT;
	int iWindowWidth = clientRect.right;

	pDC->SetMapMode(MM_TEXT);
	pDC->SetTextColor(RGB(0, 0, 255));
	CFont fontTahoma;
	if (!fontTahoma.CreatePointFont(120, _T("Tahoma"))) {
		throw CVocabException(_T("Failed to create font in view OnDraw"));
	}
	CFont* pFontOld = pDC->SelectObject(&fontTahoma);
	m_xPos = 0;
	m_yPos = YSTART;
	int iLineCount = 0;
	long iMaxBottom = 0;
	int iColNo = 0;
	//m_xPos = m_vColumnStartsGk[iColNo];
	int iEnStart = m_vColumnStartsEn[iColNo];
	int iLineMax = m_vColumnLengths[iColNo];
	for (std::basic_string<TCHAR> sLine : m_vEntries) {
		//split on the tab
		std::string::size_type tabPos = sLine.find('\t');
		std::basic_string<TCHAR> sGreek = sLine.substr(0, tabPos);
		std::basic_string<TCHAR> sEn = sLine.substr(tabPos + 1);
		if (!pDC->TextOut(m_xPos, m_yPos, sGreek.c_str(), sGreek.size())) {
			throw CVocabException(_T("TextOut(sGreek) failed"));
		}
		if (!pDC->TextOutW(iEnStart, m_yPos, sEn.c_str(), sEn.size())) {
			throw CVocabException(_T("TextOut(sEn) failed"));
		}
		int iEnSize = sEn.size();
		const char* szEn = (const char*)sEn.data();
		CSize textSize = pDC->GetTextExtent((LPCTSTR)szEn, sLine.size());
		m_yPos += textSize.cy + 2;
		if (iLineCount++ == iLineMax) {
			iMaxBottom = m_yPos;
			m_yPos = YSTART;
			m_xPos = m_vColumnStartsGk[++iColNo];
			iEnStart = m_vColumnStartsEn[iColNo];
			iLineMax = m_vColumnLengths[iColNo];
			iLineCount = 0;
			int iLinePos = m_xPos - COLGAP * 1.5;
			pDC->MoveTo(iLinePos, YSTART);
			pDC->LineTo(iLinePos, iMaxBottom);
		}
	}
	pDC->SelectObject(pFontOld);
	fontTahoma.DeleteObject();
	//	SetScroll(iMaxBottom + 50);*/

}


// CGreekVocabView printing

BOOL CGreekVocabView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGreekVocabView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGreekVocabView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CGreekVocabView diagnostics

#ifdef _DEBUG
void CGreekVocabView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGreekVocabView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CGreekVocabDoc* CGreekVocabView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGreekVocabDoc)));
	return (CGreekVocabDoc*)m_pDocument;
}
#endif //_DEBUG


// CGreekVocabView message handlers

/*
	Show vocabulary entry dialogue, then update data and display
	*/
void CGreekVocabView::OnVocabularyEntry()
{
	CFrameWnd* pFrame = GetParentFrame();
	m_pDlg = new CEnterDlg;
	m_pDlg->DoModal();
	delete m_pDlg;
	m_pDlg = NULL;
	ReadFiles();
	SetSizes();

}

/*
	Set headings and read data files
	*/
void CGreekVocabView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	m_plblStatus = new CStatic();
	CRect clientRect;
	GetClientRect(&clientRect);
	CRect statusRect(clientRect.left, clientRect.bottom - STATUSTOP, clientRect.right, 
		clientRect.bottom - STATUSTOP + STATUSHEIGHT);
	m_plblStatus->Create(L"Status", SS_SUNKEN | WS_CHILD | WS_VISIBLE, statusRect, this, IDC_STATICSTATUS);
	GetParent()->SetWindowText(_T("Greek Vocabulary and Syntax Tester"));
	SetScroll(1000, 1000);

	SetWindowText(_T("Greek Vocabulary Tester"));
	EnableToolTips();
	ReadFiles();
}

/*
	Set the scroll sizes
	*/
void CGreekVocabView::SetScroll(int MaxSizeX, int MaxSizeY) {
	CSize sizeTotal(MaxSizeX, MaxSizeY);
	CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);
	CSize sizeLine(sizeTotal.cx / 50, sizeTotal.cy / 50);
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
}

/*
	Show dialogue for testing vocabulary knowledge
	*/
void CGreekVocabView::OnVocabularyTest()
{
	m_pDlg = new CVocabEntryTestDlg;
	m_pDlg->DoModal();
	delete m_pDlg;
}



/* Get vocabulary entries from the document object
*/
void CGreekVocabView::ReadFiles() {
	CWnd* pWnd = GetParentFrame();
	CMainFrame* pFrame = (CMainFrame*)pWnd;
	CGreekVocabDoc* pDoc = (CGreekVocabDoc*)pFrame->GetActiveDocument();

	const std::vector<CString> vFileNames = pDoc->GetVocabFileNames();
	for (CString strFileName : vFileNames) {
		std::vector<CString>* pvList = pDoc->GetVocabFileList(strFileName);
		m_vEntries.insert(m_vEntries.end(), pvList->begin(), pvList->end());
	}
}

/*
	Show dialogue allowing testing of voabulary knowledge using MouseOver
	*/

void CGreekVocabView::OnVocabularyHovertest()
{
	CRect rect(100, 100, 500, 700);
	CHoverTestDlg dlg;

	dlg.DoModal();

}

/*
	Show dialogue to enter/edit noun paradigms
	*/
void CGreekVocabView::OnGrammarNouns()
{
	CNounDlg dlg;
	dlg.DoModal();
}

/*
	Show dialogue for testing of knowledge of noun declensions
	*/
void CGreekVocabView::OnNounsEntrytest()
{
	CNounEntryTestDlg dlg;
	dlg.DoModal();
}

/*
Show dialogue for testing of knowledge of noun declensions
*/
void CGreekVocabView::OnAdjectivesEnter()
{
	CAdjectiveDlg dlg;
	dlg.DoModal();
}


/*
	Show dialogue for entering/editing verb paradigms
	*/
void CGreekVocabView::OnVerbsEnter()
{
	CVerbDlg dlg;
	dlg.DoModal();
}

/*
	Set the postitions and lengths of columns to be used in OnDraw()
	Called by OnDraw()
	*/
void CGreekVocabView::SetSizes() {
	TRACE0("SetSizes\n");
	CDC* pDC = GetDC();
	m_vLinesToDraw.clear();
	m_vColumnStartsEn.clear();
	m_vColumnStartsGk.clear();
	m_vColumnLengths.clear();

	CRect rectClient;
	GetClientRect(&rectClient);
	m_Columns = 0;
	//round up to avoid overspill
	//m_PerColumn = ceil(static_cast<double>(m_vEntries.size()) / m_Columns);
	CFont fontTahoma;
	if (!fontTahoma.CreatePointFont(120, _T("Tahoma"))) {
		throw CVocabException(_T("Failed to create font in view SetSizes"));
	}
	CFont* pFontOld = pDC->SelectObject(&fontTahoma);
	m_xPos = 0;
	m_yPos = YSTART;
	int iLineCount = 0;
	long iMaxRightGk = 0, iMaxRightEn = 0;
	long iMaxBottom = 0;
	long iDocWidth = 0;
	std::basic_string<TCHAR> sLongestGk, sLongestEn;
	//find end of longest text line to get column sizes
	for (std::basic_string<TCHAR> sLine : m_vEntries) {
		//split on the tab
		std::string::size_type tabPos = sLine.find('\t');
		std::basic_string<TCHAR> sGreek = sLine.substr(0, tabPos);
		if (sGreek.size() > sLongestGk.size()) {
			sLongestGk = sGreek;
		}
		std::basic_string<TCHAR> sEn = sLine.substr(tabPos + 1);
		unsigned int iEnSize = sEn.size();
		if (iEnSize > sLongestEn.size()) {
			sLongestEn = sEn;
		}
		const char* szEn = (const char*)sEn.data();
		//		const char* szGk = (const char*)sGreek.data();
		CSize textSize = pDC->GetTextExtent((LPCTSTR)szEn, sLine.size());
		m_yPos += textSize.cy + 2;

		//DrawText gets length of unicode string better than GetTextExtent. Note that this isn't actually drawing text.
		//	CRect rect(0, 0, 0, 0);
		//	pDC->DrawText(sEn.data(), &rect, DT_CALCRECT);
		//	iMaxRightEn = std::max<long>(rect.right, iMaxRightEn);

		//	pDC->DrawText(sGreek.data(), &rect, DT_CALCRECT);
		//	iMaxRightGk = std::max<long>(rect.right, iMaxRightGk);

		iLineCount++;
		if ((m_yPos + textSize.cy + 3) >= rectClient.bottom) {
			CRect rect(0, 0, 0, 0);
			pDC->DrawText(sLongestEn.data(), &rect, DT_CALCRECT);
			iMaxRightEn = std::max<long>(rect.right, iMaxRightEn);

			pDC->DrawText(sLongestGk.data(), &rect, DT_CALCRECT);
			iMaxRightGk = std::max<long>(rect.right, iMaxRightGk);
			m_vColumnLengths.push_back(iLineCount - 1);
			m_vColumnStartsGk.push_back(m_xPos);
			m_vColumnStartsEn.push_back(m_xPos + iMaxRightGk + COLGAP);
			int iOldTextPos = m_xPos;
			m_xPos += iMaxRightEn + COLGAP * 3 + iMaxRightGk;
			m_yPos = YSTART;
			iLineCount = 0;
			iMaxRightEn = iMaxRightGk = 0;

		}
	}
	iDocWidth = m_xPos;
	//If not at the end of a column record the values for the last column
	if (iLineCount) {
		m_vColumnLengths.push_back(iLineCount + 1);
		m_vColumnStartsGk.push_back(m_xPos);
		m_vColumnStartsEn.push_back(m_xPos + iMaxRightGk + COLGAP);
		iDocWidth += iMaxRightGk + iMaxRightEn + COLGAP;
		//m_vColumnStartsGk.push_back(iDocWidth);
	}
	pDC->SelectObject(pFontOld);
	fontTahoma.DeleteObject();
	SetScroll(iDocWidth, m_yPos);
}

/*
	Show dialogue for testing of knowledge of verb conjugations
	*/
void CGreekVocabView::OnVerbsEntrytest()
{
	CVerbEntryTestDlg dlg;
	dlg.DoModal();
}


void CGreekVocabView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_vEntries.size()) {
		SetSizes();	//arguments passed by reference
	}
	if (m_plblStatus) {
		CRect clientRect;
		GetClientRect(&clientRect);
		CRect statusRect(clientRect.left, clientRect.bottom - STATUSTOP, 
			clientRect.right, clientRect.bottom - STATUSTOP + STATUSHEIGHT);
		m_plblStatus->MoveWindow(statusRect);
	}
}

HBRUSH CGreekVocabView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CScrollView::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_STATICSTATUS) {
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}