#pragma once
#include "afxwin.h"
#include "GreekEdit.h"
#include "ScrollListBox.h"
#include <vector>
// CVocabEntryTestDlg dialog

class CVocabEntryTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVocabEntryTestDlg)

public:
	CVocabEntryTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVocabEntryTestDlg();

// Dialog Data
	enum { IDD = IDD_ENTRYTEST };
private:
	const TCHAR * m_szAllFiles;
	CMainFrame *m_pFrame;
	CGreekVocabDoc *m_pDoc;
	HBRUSH m_hGreenBrush;
	COLORREF m_Red, m_Black, m_Green;
	COLORREF m_TextColor;
	std::map<int,CString> m_EnMap;

	//void FillBoxes();
	void ReadAllFileLists(StringVectorMap *pFilesListMap);
	void ReadFileList(std::vector<CString> *pFileList,bool ClearListBox = false);
	CString GetTranslation(int Key);
	void OnVscrollListgk();
	void AdjustScroll(CListBox *pScrolledBox,CListBox *pBoxToAdjust);
	void LineScroll(CListBox *pLB,int LinesToScroll);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::vector<CString> m_vGreekWords;
	std::vector<CString> m_vTranslations;
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnListBoxScroll(WPARAM wParam,LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
//	CEdit m_edtEn;
	CComboBox m_cboFiles;
public:
	afx_msg void OnSelchangeCombofiles();
private:
	//CListBox m_lstGreek;
	CScrollListBox m_lstGreek;
	CListBox m_lstEn;
public:
	afx_msg void OnChangeTry();
private:
	// Translation entered by user	
	CString m_strTry;

	void HandleArrow(bool IsUp);
public:
	//Entry box for user's translation
	CEdit m_edtTry;
	afx_msg void OnSelchangeListgk();
	afx_msg void OnBnClickedButtonshow();
	afx_msg void OnBnClickedClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
