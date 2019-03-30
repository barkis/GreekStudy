
// GreekVocabView.h : interface of the CGreekVocabView class
//

#pragma once
#include <map>
#include <vector>

class CGreekVocabView : public CScrollView
{
protected: // create from serialization only
	CGreekVocabView();
	DECLARE_DYNCREATE(CGreekVocabView);

// Attributes
public:
	CGreekVocabDoc* GetDocument() const;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGreekVocabView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
//xxx	static const int COLWIDTH = 500;
	static const int COLGAP = 25;
	static const int YSTART = 3;
	std::vector<std::basic_string<TCHAR>> m_vEntries;
	std::vector<int> m_vLinesToDraw;
	std::vector<int> m_vColumnStartsGk, m_vColumnStartsEn;
	std::vector<int> m_vColumnLengths;
	int m_xPos,m_yPos;
	int m_Columns;
	int m_PerColumn;

	void ReadFiles();
	void SetSizes();
		//void ReadLines(CStdioFile &file);
	void SetScroll(int MaxSizeX,int MaxSizeY);
protected:
	CDialogEx *m_pDlg;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnVocabularyEntry();
	virtual void OnInitialUpdate();
	afx_msg void OnVocabularyTest();
	afx_msg void OnVocabularyHovertest();
	afx_msg void OnGrammarNouns();
	afx_msg void OnNounsEntrytest();
	afx_msg void OnAdjectivesEnter();
	//afx_msg void OnHelpHelptopics();
	afx_msg void OnVerbsEnter();
	afx_msg void OnVerbsEntrytest();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // debug version in GreekVocabView.cpp
inline CGreekVocabDoc* CGreekVocabView::GetDocument() const
   { return reinterpret_cast<CGreekVocabDoc*>(m_pDocument); }
#endif

