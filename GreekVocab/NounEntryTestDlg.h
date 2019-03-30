#pragma once
#include "afxwin.h"
#include "GreekEdit.h"
#include <unordered_map>
class CGreekVocabDoc;

// CNounEntryTest dialog

class CNounEntryTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNounEntryTestDlg)

public:
	CNounEntryTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNounEntryTestDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGNOUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeComboDeclensionNames();
	DECLARE_MESSAGE_MAP()
	std::unordered_map<int,CString>m_mapTranslations;
private:
	CString m_strDeclension;
	CString m_strDictionaryForm;
	CString m_strNominateveS;
	CString m_strAccusativeS;
	CString m_strGenitiveS;
	CString m_strDativeS;
	CString m_strNominateveP;
	CString m_strAccusativeP;
	CString m_strGenitiveP;
	CString m_strDativeP;
	CString m_strVocative;
	CGreekEdit m_edtNomS, m_edtAccS, m_edtGenS, m_edtDatS, m_edtVocative;
	CGreekEdit m_edtNomP, m_edtAccP, m_edtGenP, m_edtDatP;
	CGreekEdit m_edtDictionaryForm;

	bool m_IsClearing;
	COLORREF m_Red, m_Black, m_Green;
	COLORREF m_TextColor;
	CFont m_Font;

	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CNoun m_Noun;
	const CString m_strFirstLineOriginal;
	const CString m_strFirstLineReplacement;
	CComboBox m_cboNounNames;

	CGreekVocabDoc *GetDocument();
	void ClearEditBoxes();
	void EnableTextBoxes(bool Enable);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditRange(UINT ID);
	afx_msg void OnFocusEditRange(UINT ID);
private:
	CComboBox m_cboArticle;
};
