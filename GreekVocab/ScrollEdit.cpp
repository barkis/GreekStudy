// ScrollEdit.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "ScrollEdit.h"


// CScrollEdit

IMPLEMENT_DYNAMIC(CScrollEdit, CEdit)

CScrollEdit::CScrollEdit()
{

}

CScrollEdit::~CScrollEdit()
{
}

BOOL CScrollEdit::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	return CEdit::PreCreateWindow(cs);
}


BEGIN_MESSAGE_MAP(CScrollEdit, CEdit)
END_MESSAGE_MAP()



// CScrollEdit message handlers


