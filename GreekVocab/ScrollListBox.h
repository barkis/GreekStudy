#pragma once
#include "afxwin.h"
#define WM_LISTBOXVSCROLL WM_USER + 5
class CScrollListBox: public CListBox	{
	DECLARE_MESSAGE_MAP()
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};