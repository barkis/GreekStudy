#include "stdafx.h"
#include "GreekEdit.h"
#include "EnterDlg.h"
#include "GreekVocabView.h"

CGreekEdit::CGreekEdit()	{
}

CMainFrame *CGreekEdit::m_pTheFrame = NULL;	//set to address of frame by dialogs using greek edits

BOOL CGreekEdit::PreTranslateMessage(MSG* pMsg)	{
	int iKey = pMsg->wParam;
	if(
	(pMsg->message == WM_CHAR || pMsg->message == WM_SYSCHAR) 
		&& ((iKey >= '0') && (iKey < 58)||(iKey >= 'A')) 
		 && iKey != 0xffff)	{
		int iNewChar = 0;
		TRACE(_T("WM_CHAR = %d\n"),pMsg->wParam);
		Shifts shiftDown = GetShift();
		if(m_pTheFrame->GetChar(shiftDown,pMsg->wParam,iNewChar))	{
			pMsg->wParam = iNewChar;
			pMsg->message = WM_CHAR;
			//return CDialogEx::PreTranslateMessage(pMsg);
			BOOL baseRet =  CEdit::PreTranslateMessage(pMsg);
			return baseRet;
		}
		else		{
			TRACE(_T("No match\n"));
			return TRUE;
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

const Shifts CGreekEdit::GetShift()	{
	Shifts RetVal = Shifts::enumShifts::NONE;
	TRACE(_T("GetShift\n"));
	if((GetAsyncKeyState(VK_LMENU)<0))	{	//alt key
		RetVal = Shifts::enumShifts::ALT;
		TRACE(_T("alt down"));
	}
	if((GetAsyncKeyState(VK_RMENU)<0))	{
		RetVal += Shifts::enumShifts::IOTA;
		TRACE(_T("altgr down"));
	}	
	if(GetAsyncKeyState(CKEY)<0)	{
		TRACE(_T("C down\n"));
		RetVal += Shifts::enumShifts::SMOOTH;
	}
	else if(GetAsyncKeyState(VKEY)<0)	{
		TRACE(_T("V down\n"));
		RetVal += Shifts::ROUGH;
	}
	
	//if(GetAsyncKeyState(VK_SHIFT)<0)	{
	//	RetVal += Shifts::enumShifts::SHIFT;
	//	TRACE(_T("Shift down"));
	//}
	if(GetAsyncKeyState(VK_CAPITAL)<0)	{
		RetVal += Shifts::enumShifts::SHIFT;
		TRACE(_T("Capslock on"));
	}
	return RetVal;
}
BEGIN_MESSAGE_MAP(CGreekEdit, CEdit)
	ON_WM_CHAR()
//	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void CGreekEdit::OnSetFocus()	{
	SetSel(-1);
}

void CGreekEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
void CGreekEdit::SetFrame(CMainFrame *pFrame)	{
	m_pTheFrame = pFrame;
}



