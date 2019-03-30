// MDLListBox.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "MDLListBox.h"
#include "HoverTest.h"
#include <typeinfo>

// CMDLListBox

IMPLEMENT_DYNAMIC(CMDLListBox, CListBox)

CMDLListBox::CMDLListBox()	{
	//xxxEnableToolTips();
}
CMDLListBox::~CMDLListBox()
{
}


BEGIN_MESSAGE_MAP(CMDLListBox, CListBox)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()
/*
	Get dialog to provide the text for a tooltip.
*/
INT_PTR CMDLListBox::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	BOOL iIsOutOfItem;
	int iItemUnder = ItemFromPoint(point,iIsOutOfItem);
	CWnd* pParent = GetParent();
	const char* szTypeName = typeid(*pParent).name();
	if(!iIsOutOfItem)	{
		pTI->hwnd = GetParent()->m_hWnd;
		pTI->uId      = (UINT)m_hWnd;

		pTI->uFlags  |= TTF_IDISHWND;

		pTI->lpszText = LPSTR_TEXTCALLBACK;

		return iItemUnder;
	}
	return CListBox::OnToolHitTest(point, pTI);
}


/*
Keep track of where cursor is for use of GetSelection()
*/
void CMDLListBox::OnMouseMove(UINT nFlags, CPoint point)
{
	m_CursorPos = point;

	CListBox::OnMouseMove(nFlags, point);
}

/*
Get index of item in listbox that cursor is over
*/
int CMDLListBox::GetSelection()	{
	BOOL bOutside;

	int iItem = ItemFromPoint(m_CursorPos,bOutside);
	if(bOutside)	{
		return -1;
	}
	else	{
		return iItem;
	}

}