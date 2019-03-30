#include "stdafx.h"
#include "ScrollListBox.h"
#include <stdexcept>

BEGIN_MESSAGE_MAP(CScrollListBox, CListBox)
	ON_WM_VSCROLL( )
END_MESSAGE_MAP()

afx_msg void CScrollListBox::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )	{
	ASSERT(nSBCode | SB_VERT);
	CListBox::OnVScroll(nSBCode,nPos,pScrollBar);
	CWnd *pParent = GetParent();
	int iID = GetDlgCtrlID();
	CDialog *pDlg = dynamic_cast<CDialog*>(pParent);
	if(pDlg)	{
		pDlg->SendMessage(WM_LISTBOXVSCROLL,iID,nPos);
	}
	else	{
		throw std::out_of_range("Not a dialog ptr in CScrollListBox::OnVScroll");
	}
}