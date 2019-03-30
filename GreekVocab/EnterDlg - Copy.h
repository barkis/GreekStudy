#pragma once
#include "stdafx.h"
#include "greekedit.h"
#include "GreekVocabView.h"

#include "afxwin.h"
#include "resource.h"

// CEnterDlg dialog
class CEnterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnterDlg)

public:
	CEnterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnterDlg();

// Dialog Data
	enum { IDD = IDD_ENTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	CGreekEdit m_edtGreek;
//	CEdit m_edtGreek;
	CEdit m_edtEn;
//	CGreekEdit m_edtEn;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedIdsave();
	afx_msg void OnChangeEditenEditgk();
	afx_msg void OnSelChangeEditList();
private:
	int m_Scrolls;
	CRichEditCtrl m_rich;
	CListBox m_lstFiles;
	bool m_lstFilesCreated;
	CButton btnSave;
	void DoScroll(CEdit *pEdit,	int iLineNo,const TCHAR *szEdit);
	void EditBoxLostFocus(CEdit *pEdit,const TCHAR *szEdit);
	bool LastLineIsEmpty(const CEdit &EditBox);
	bool AtEndOFLine(const CEdit *pEditBox,int LineNo);
	CGreekVocabDoc *GetDocument();
public:
	afx_msg void OnSetfocusEditen();
	afx_msg void OnKillfocusEditen();
	afx_msg void OnKillfocusEditgk();
	afx_msg void OnSetfocusEditgk();
	afx_msg void OnVscrollEditgk();
	afx_msg void OnBnClickedButtonedit();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
