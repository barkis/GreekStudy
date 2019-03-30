#ifndef GREEKEDIT
#define GREEKEDIT
#include "stdafx.h"
#include "GreekVocabDoc.h"
#include "GreekVocabView.h"
#include "MainFrm.h"
#include "ScrollEdit.h"

class CROEdit: public CEdit	{
public:
	CROEdit();
	static void SetFrame(CMainFrame *pFrame);
protected:	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static CMainFrame *m_pTheFrame;
private:
	static const short CKEY = 0x43;
	static const short VKEY = 0x56;

	const Shifts GetShift();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus();
};
#endif
