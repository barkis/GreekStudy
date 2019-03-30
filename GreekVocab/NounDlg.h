#pragma once
#include "Declension.h"
#include "greekedit.h"
#include "afxwin.h"

class CGreekVocabDoc;
// CNounDlg dialog

class CNounDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNounDlg)

public:
	CNounDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNounDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGNOUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CNoun *m_pNoun;
	CEdit m_edtDeclension;
	CGreekEdit m_edtDictForm;
	CGreekEdit m_edtNomS;
	CGreekEdit m_edtAccS;
	CGreekEdit m_edtGenS;
	CGreekEdit m_edtDatS;
	CGreekEdit m_edtNomP;
	CGreekEdit m_edtAccP;
	CGreekEdit m_edtGenP;
	CGreekEdit m_edtDatP;
	CGreekEdit m_edtVoc;
	CComboBox m_cboArticle;

	CString m_strFileName;

	void ClearAll();
	void Save();
	CGreekVocabDoc *GetDocument();
public:
	afx_msg void OnBnClickedSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditBox();
private:
	CButton m_btnSaveAndClose;
public:
	afx_msg void OnBnClickedClear();
private:
	CButton m_btnClear;
	CButton m_btnSaveAndContinue;
public:
	afx_msg void OnBnClickedButtonsaveandcontinue();
	afx_msg void OnKillfocusEditdictionaryform();
	afx_msg void OnBnClickedButtonedit();
	afx_msg void OnKillfocusEditnoms();
};
