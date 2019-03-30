
// GreekVocabView.cpp : implementation of the CGreekVocabView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GreekVocab.h"
#endif
#include <vld.h>
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
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CGreekVocabView construction/destruction

CGreekVocabView::CGreekVocabView() :m_pDlg(NULL), COLWIDTH(200)
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
	if (!pDoc)	{
		return;
	}
//	m_vLinesToDraw.clear();
	//Display fullvocabulary list in columns
	RECT clientRect;
	GetClientRect(&clientRect);
	int iWindoweight = clientRect.bottom;
	SetScroll(iWindoweight);
	int iWindowWidth = clientRect.right;
	//int iColumns = iWindowWidth / (COLWIDTH * 2);
	//round up to avoid overspill
	//int iPerColumn = ceil(static_cast<double>(m_vEntries.size()) / iColumns);

	pDC->SetMapMode(MM_TEXT);
	pDC->SetTextColor(RGB(0, 0, 255));
	CFont fontTahoma;
	if (!fontTahoma.CreatePointFont(120, _T("Tahoma")))	{
		throw CVocabException(_T("Failed to create font in view OnDraw"));
	}
	CFont* pFontOld = pDC->SelectObject(&fontTahoma);
	m_xPos = 0;
	m_yPos = YSTART;
	int iLineCount = 0;
	long iMaxRight = 0;	//find end of longest text line to position vertical line
	long iMaxBottom = 0;
	for (std::basic_string<TCHAR> sLine : m_vEntries)	{
		//split on the tab
		std::string::size_type tabPos = sLine.find('\t');
		std::basic_string<TCHAR> sGreek = sLine.substr(0, tabPos);
		std::basic_string<TCHAR> sEn = sLine.substr(tabPos + 1);
		if (!pDC->TextOut(m_xPos, m_yPos, sGreek.c_str(), sGreek.size()))	{
			throw CVocabException(_T("TextOut(sGreek) failed"));
		}
		if (!pDC->TextOutW(m_xPos + COLWIDTH, m_yPos, sEn.c_str(), sEn.size()))	{
			throw CVocabException(_T("TextOut(sEn) failed"));
		}
		int iEnSize = sEn.size();
		const char* szEn = (const char*)sEn.data();
		CSize textSize = pDC->GetTextExtent((LPCTSTR)szEn, sLine.size());
		m_yPos += textSize.cy + 2;

		//DrawText gets length of unicode string better than GetTextExtent
		CRect rect(0, 0, 0, 0);
		pDC->DrawText(sEn.data(), &rect, DT_CALCRECT);

		iMaxRight = std::max<long>(rect.right, iMaxRight);


		if (++iLineCount == m_PerColumn)	{
			iMaxBottom = std::max<long>(m_yPos, iMaxBottom);
			int iOldTextPos = m_xPos;
			m_xPos += COLWIDTH * 2;
			m_yPos = YSTART;
			iLineCount = 0;

			//draw vertical line to separate columns
			// right of 2nd column + 1/2 of difference between new left and right of 2nd column
			//int iRightEdge = iOldTextPos + COLWIDTH + iMaxRight;
			//int iLinePos = iRightEdge + (m_xPos - iRightEdge) / 2;
		//	m_vLinesToDraw.push_back(iLinePos);
		}
	}
	TRACE(_T("m_vLinesToDraw.size() = %d"), m_vLinesToDraw.size());
	for (int iLinePos : m_vLinesToDraw)	{
		pDC->MoveTo(iLinePos, YSTART);
		pDC->LineTo(iLinePos, iMaxBottom);
	}
	pDC->SelectObject(pFontOld);
	fontTahoma.DeleteObject();
	TRACE(_T("iMaxBottom = %d\n"), iMaxBottom);
	//SetScroll(iMaxBottom + 50);
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


void CGreekVocabView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	GetParent()->SetWindowText(_T("Greek Vocabulary and Syntax Tester"));
	SetScroll(1000);
	SetWindowText(_T("Greek Vocabulary Tester"));
	//	EnableToolTips();
	ReadFiles();
}

void CGreekVocabView::SetScroll(int MaxSize)	{
	CSize sizeTotal(MaxSize, 0);
	CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);
	CSize sizeLine(sizeTotal.cx / 50, sizeTotal.cy / 50);
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);
}

void CGreekVocabView::OnVocabularyTest()
{
	m_pDlg = new CVocabEntryTestDlg;
	m_pDlg->DoModal();
	delete m_pDlg;
}

/* Get vocabulary entries from the document 
*/
void CGreekVocabView::ReadFiles()	{
	CWnd* pWnd = GetParentFrame();
	CMainFrame *pFrame = (CMainFrame*)pWnd;
	CGreekVocabDoc *pDoc = (CGreekVocabDoc*)pFrame->GetActiveDocument();

	const std::vector<CString> vFileNames = pDoc->GetVocabFileNames();
	for (CString strFileName : vFileNames)	{
		std::vector<CString> *pvList = pDoc->GetVocabFileList(strFileName);
		m_vEntries.insert(m_vEntries.end(), pvList->begin(), pvList->end());
	}
}

//void CGreekVocabView::ReadLines(CStdioFile &file)	{
//	CString strNextLine;
//	while (file.ReadString(strNextLine))	{
//		std::basic_string<TCHAR> sNextLine((LPCTSTR)strNextLine);
//
//		m_vEntries.push_back(sNextLine);
//	}
//
//}


void CGreekVocabView::OnVocabularyHovertest()
{
	CRect rect(100, 100, 500, 700);
	CHoverTestDlg dlg;

	dlg.DoModal();

}


void CGreekVocabView::OnGrammarNouns()
{
	CNounDlg dlg;
	dlg.DoModal();
}


void CGreekVocabView::OnNounsEntrytest()
{
	CNounEntryTestDlg dlg;
	dlg.DoModal();
}


void CGreekVocabView::OnAdjectivesEnter()
{
	CAdjectiveDlg dlg;
	dlg.DoModal();
}


void CGreekVocabView::OnHelpHelptopics()
{
	//	HWND hwndHelp = HtmlHelp(
}


void CGreekVocabView::OnVerbsEnter()
{
	//std::unique_ptr<CVerbDlg> pVerbDlg(new CVerbDlg);
	CVerbDlg dlg;
	dlg.DoModal();
	//pVerbDlg->DoModal();
}

/*
When window is reszied calculate the number of lines per column and the pixel lengths use for scrolling
*/
void CGreekVocabView::OnSize(UINT nType, int cx, int cy)
{
	TRACE0("OnSize\n");
	CScrollView::OnSize(nType, cx, cy);
	if (m_vEntries.size())	{
		SetSizes();
	}
//	CDC *pDC = GetDC();
//	m_vLinesToDraw.clear();
//
////	m_Columns = cx / (COLWIDTH * 2);
//	//round up to avoid overspill
//	//m_PerColumn = ceil(static_cast<double>(m_vEntries.size()) / iColumns);
//	CFont fontTahoma;
//	if (!fontTahoma.CreatePointFont(120, _T("Tahoma")))	{
//		throw CVocabException(_T("Failed to create font in view OnDraw"));
//	}
//	CFont* pFontOld = pDC->SelectObject(&fontTahoma);
////	m_xPos = 0;
//	m_yPos = YSTART;
//	int iLineCount = 0;
//	long iMaxRight = 0;	//find end of longest text line to position vertical line
//	long iMaxBottom = 0;
//	for (std::basic_string<TCHAR> sLine : m_vEntries)	{
//		//split on the tab
//		std::string::size_type tabPos = sLine.find('\t');
//		std::basic_string<TCHAR> sGreek = sLine.substr(0, tabPos);
//		std::basic_string<TCHAR> sEn = sLine.substr(tabPos + 1);
//		int iEnSize = sEn.size();
//		const char* szEn = (const char*)sEn.data();
//		CSize textSize = pDC->GetTextExtent((LPCTSTR)szEn, sLine.size());
//		m_yPos += textSize.cy + 2;
//
//		//DrawText gets length of unicode string better than GetTextExtent
//		CRect rect(0, 0, 0, 0);
//		pDC->DrawText(sEn.data(), &rect, DT_CALCRECT);
//
//		iMaxRight = std::max<long>(rect.right, iMaxRight);
//
//
//		if (++iLineCount == m_PerColumn)	{
//			iMaxBottom = std::max<long>(m_yPos, iMaxBottom);
//			int iOldTextPos = m_xPos;
//			m_xPos += COLWIDTH * 2;
//			m_yPos = YSTART;
//			iLineCount = 0;
//
//			//draw vertical line to separate columns
//			// right of 2nd column + 1/2 of difference between new left and right of 2nd column
//		//	int iRightEdge = iOldTextPos + COLWIDTH + iMaxRight;
//			//int iLinePos = iRightEdge + (m_xPos - iRightEdge) / 2;
//			//m_vLinesToDraw.push_back(iLinePos);
//		}
//	}
//	pDC->SelectObject(pFontOld);
//	fontTahoma.DeleteObject();
//	SetScroll(iMaxBottom + 50);
}

void CGreekVocabView::SetSizes()	{
	CDC *pDC = GetDC();
	m_vLinesToDraw.clear();

	CRect rectClient;
	GetClientRect(&rectClient);
	m_Columns = rectClient.Width() / (COLWIDTH * 2);
	//round up to avoid overspill
	m_PerColumn = ceil(static_cast<double>(m_vEntries.size()) / m_Columns);
	CFont fontTahoma;
	if (!fontTahoma.CreatePointFont(120, _T("Tahoma")))	{
		throw CVocabException(_T("Failed to create font in view SetSizes"));
	}
	CFont* pFontOld = pDC->SelectObject(&fontTahoma);
	m_xPos = 0;
	m_yPos = YSTART;
	int iLineCount = 0;
	long iMaxRight = 0;	
	long iMaxBottom = 0;
	//find end of longest text line to position vertical line
	for (std::basic_string<TCHAR> sLine : m_vEntries)	{
		//split on the tab
		std::string::size_type tabPos = sLine.find('\t');
		std::basic_string<TCHAR> sGreek = sLine.substr(0, tabPos);
		std::basic_string<TCHAR> sEn = sLine.substr(tabPos + 1);
		int iEnSize = sEn.size();
		const char* szEn = (const char*)sEn.data();
		CSize textSize = pDC->GetTextExtent((LPCTSTR)szEn, sLine.size());
		m_yPos += textSize.cy + 2;

		//DrawText gets length of unicode string better than GetTextExtent. Note that this isn't actually drawing text.
		CRect rect(0, 0, 0, 0);
		pDC->DrawText(sEn.data(), &rect, DT_CALCRECT);

		iMaxRight = std::max<long>(rect.right, iMaxRight);


		if (++iLineCount == m_PerColumn)	{
			iMaxBottom = std::max<long>(m_yPos, iMaxBottom);
			int iOldTextPos = m_xPos;
			m_xPos += COLWIDTH * 2;
			m_yPos = YSTART;
			iLineCount = 0;

			//draw vertical line to separate columns
			// right of 2nd column + 1/2 of difference between new left and right of 2nd column
			int iRightEdge = iOldTextPos + COLWIDTH + iMaxRight;
			TRACE(_T("iRightEdge = %d, m_xPos=%d, iMaxRight = %d, COLWIDTH=%d\n"), iRightEdge,m_xPos,iMaxRight,COLWIDTH);
			int iLinePos = iRightEdge + ((m_xPos - iRightEdge) / 2);
			TRACE(_T("iLinePos = %d\n"), iLinePos);
			m_vLinesToDraw.push_back(iLinePos);
		}
	}
	pDC->SelectObject(pFontOld);
	fontTahoma.DeleteObject();
	SetScroll(iMaxBottom + 50);
}