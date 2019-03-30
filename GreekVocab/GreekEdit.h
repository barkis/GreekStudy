#ifndef GREEKEDIT
#define GREEKEDIT
#include "stdafx.h"
#include "GreekVocabDoc.h"
#include "GreekVocabView.h"
#include "MainFrm.h"
#include "ScrollEdit.h"
#include <map>

class CGreekEdit: public CEdit	{
public:
	CGreekEdit();
	~CGreekEdit();
	static void SetFrame(CMainFrame *pFrame);
	void SetEntryTest(CString strTranslation);
	void EndEntryTest();
	void PutCaretAtEnd();
protected:	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static CMainFrame *m_pTheFrame;
	BOOL OnKillFocus();
	void Sigmify();

private:
	static const short CKEY = 0x43;
	static const short VKEY = 0x56;
	COLORREF m_Red, m_Black, m_Green;
	COLORREF m_TextColor;
	CBrush m_Brush;
	bool m_IsEntryTest;
	CString m_strTranslation;
	bool m_bIsMultiline;

	const Shifts GetShift();
	void GetColor();
public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus();
	afx_msg HBRUSH CtlColor(CDC* pDC,  UINT nCtlColor);
	virtual void PreSubclassWindow();
};
#endif
