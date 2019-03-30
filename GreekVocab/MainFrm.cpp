
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "GreekVocabDoc.h"
#include "MainFrm.h"
#include <map>
#include <htmlhelp.h>

#pragma comment(lib, "C:\\Program Files (x86)\\HTML Help Workshop\\lib\\htmlhelp")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_HELP_HELPTOPICS, &CMainFrame::OnHelpHelptopics)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | 
		CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	//EnableToolTips();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

bool CMainFrame::GetChar(Shifts shift,int charin,int &charout)	{
	bool bRetVal = false;
	int iFound;
	CMainFrame *p = this;
	CGreekVocabDoc* pTheDoc = (CGreekVocabDoc*)GetActiveDocument();
	if(pTheDoc->GetChar(shift,charin,iFound))	{
		bRetVal = true;
		charout = iFound;
	}
	return bRetVal;

}



void CMainFrame::OnHelpHelptopics()
{
		HWND hWndHelp =   ::HtmlHelp(0,
            //_T("C:\\Users\\Matthew\\Documents\\Visual Studio 2012\\Projects\\GreekVocab\\greekvocab.chm"),
            _T("greekvocab.chm"),
            HH_DISPLAY_TOC,
            NULL) ;	
}
