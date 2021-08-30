#pragma once


// CVerbEntryTestDlg dialog

#include "VerbDlg.h"
#include <unordered_map>
class CVerbEntryTestDlg : public CVerbDlg
{
	DECLARE_DYNAMIC(CVerbEntryTestDlg)
private:
	bool m_IsClearing;
	HBRUSH m_hGreenBrush;
//	COLORREF m_Red, m_Black, m_Green;
	COLORREF m_TextColor;
	CComboBox m_cboConjugation;
	CFont m_Font;
	const CString m_strFirstLineOriginal;
	const CString m_strFirstLineReplacement;
	COLORREF m_Red, m_Green, m_Black;
	//CVerb *m_pTheVerb;
	//CVerbForm *m_pTheVerbForm;
	CString m_strDictionaryForm;
	std::unordered_map<int, CString>m_mapTranslations;
	CVerb *m_pNewVerb;	//holds entries made by user
	CVerbForm *m_pNewVerbForm;	//current verbform for user entries
	void SetVerbForm();
	void EnableTextBoxes(bool Enabling);

public:
	CVerbEntryTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVerbEntryTestDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGVERB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnSelchangeComboConjugation();
	void SetTranslations();
	afx_msg void OnChangeEditRange(UINT ID);
	afx_msg void OnFocusEditRange(UINT ID);
	afx_msg void OnTcnSelchangeTabverb(NMHDR *pNMHDR, LRESULT *pResult);
	void ClearEditBoxes();
	void ClearTheEditBox(int ID);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// Enable or disable all the text boxes
	void EnableBoxes();
	// Get the select  verb form from the verb object
	void SetNewVerbForm();
};
