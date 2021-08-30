#include "stdafx.h"
#include "GreekEdit.h"
#include "EnterDlg.h"
#include "GreekVocabView.h"
//#include "boost/regex.hpp"
#include <regex>
#undef max
#include <limits>

CGreekEdit::CGreekEdit() :m_Red(RGB(255, 0, 0)), m_Black(RGB(0, 0, 0)), m_Green(RGB(0, 255, 0)),m_IsEntryTest(false)	{
	m_Brush.CreateSolidBrush(RGB(255,255,255));
	m_TextColor = m_Black;
}

CGreekEdit::~CGreekEdit()	{
	DeleteObject((HGDIOBJ)m_Brush);
}

CMainFrame *CGreekEdit::m_pTheFrame = NULL;	//set to address of frame by dialogs using greek edits

BOOL CGreekEdit::PreTranslateMessage(MSG* pMsg)	{
	ASSERT(m_pTheFrame->GetSafeHwnd());
	int iKey = pMsg->wParam;
	if(pMsg->message == WM_CREATE)	{
		TRACE0("WM_CREATE\n");
	}
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
	ON_CONTROL_REFLECT_EX(EN_KILLFOCUS,OnKillFocus)
	ON_WM_CTLCOLOR_REFLECT()
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

BOOL CGreekEdit::OnKillFocus()	{
	Sigmify();
	return FALSE;
}

/* where a normal sigma is at the end of a word replace it with a final sigma.
called on KillFocus
*/
void CGreekEdit::Sigmify()	{
	TCHAR szLine[500];
	int iLineCount;
	std::wstring sLine;
	std::wstring sReplaced;
	std::wregex toMatch(_T("(σ)([\\s]|$)"));	//normal sigma at end of word or string

	//int iChars = GetLine(0,szLine,500);
	//szLine[iChars] = '\0';
	if(!m_bIsMultiline)	{
		iLineCount = 1;
	}
	else	{
		iLineCount = GetLineCount();
	}
	for(int i = 0;i < iLineCount;++i)	{
		int iChars = GetLine(i,szLine,500);
		szLine[iChars] = '\0';
		sLine = szLine;
		std::wstring sReplaced = regex_replace(sLine,toMatch,_T("ς"));
		//use select-replace to make changes (avoids Get/SetWindowText in KillFocus handler)
		if(sReplaced != sLine)	{	
			SetSel(LineIndex(i),LineIndex(i)+iChars);
			ReplaceSel(sReplaced.c_str());
		}
	}
}



void CGreekEdit::PreSubclassWindow()
{
	unsigned long dwStyle = GetStyle();
	if(dwStyle & ES_MULTILINE)	{
		m_bIsMultiline = true;
	}
	else	{
		m_bIsMultiline = false;
	}

	CEdit::PreSubclassWindow();
}

HBRUSH CGreekEdit::CtlColor(CDC* pDC, UINT nCtlColor)	{
	if (m_IsEntryTest)	{
		GetColor();
		pDC->SetTextColor(m_TextColor);
	}
	return HBRUSH(m_Brush);
}

void CGreekEdit::SetEntryTest(CString strTranslation)	{
	m_IsEntryTest = true;
	m_strTranslation = strTranslation;
}

void CGreekEdit::EndEntryTest()	{
	m_IsEntryTest = false;
}

void CGreekEdit::GetColor()	{
	CString strTry;
	GetWindowText(strTry);
	strTry.MakeLower();
	if (m_strTranslation == strTry)	{
		m_TextColor = m_Black;
	}
	else	{
		if (m_strTranslation.Find(strTry) == 0)	{
			m_TextColor = m_Green;
		}
		else	{
			m_TextColor = m_Red;
		}
	}
}

void CGreekEdit::PutCaretAtEnd()	{
	int iLineCount = GetLineCount();
	int iCharIndex = LineIndex(iLineCount - 1);
	SetSel(iCharIndex, iCharIndex);
}