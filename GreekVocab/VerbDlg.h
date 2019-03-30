#pragma once
#include "afxcmn.h"
#include "GreekEdit.h"
#include "Verb.h"
// CVerbDlg dialog
#include <map>
class CVerbDlg : public CDialogEx
{
protected:
	DECLARE_DYNAMIC(CVerbDlg)
	enum enumSelection{PRESACT,PRESMID,IMPACT,IMPMID,FUTACT,FUTMID,AORACT,AORMID,PRESACTOPT,PRESMIDOPT,PRESACTSUBJ,PRESMIDSUBJ};
public:
	CVerbDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVerbDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGVERB };
private:

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void Save();
	DECLARE_MESSAGE_MAP()

	CTabCtrl m_TabSheet;
	CVerb *m_pTheVerb;
	CVerbForm *m_pCurrentVerbForm;
	enumSelection m_Selection;
	enumSelection m_aTabs[9];
	CGreekEdit m_edtPers1S, m_edtPers2S, m_edtPers3S;
	CGreekEdit m_edtPers1P, m_edtPers2P, m_edtPers3P;
	CGreekEdit m_edtInfinitive, m_edtImperativeS, m_edtImperativeP;
	CGreekEdit m_edtPart, m_edtDictionaryForm;
	CString m_strFileName;
	bool m_IsSavedVerb;

	void SetTabs();
	void Clear();
	CGreekVocabDoc *GetDocument();
	afx_msg void OnTcnSelchangeTabverb(NMHDR *pNMHDR, LRESULT *pResult);
public:
	virtual BOOL OnInitDialog();
	void CVerbDlg::OnChangeEditPers(UINT ID);
	afx_msg void OnBnClickedButtonclear();
	afx_msg void OnBnClickedButtonsave();
	afx_msg void OnBnClickedButtonsaveandcontinue();
	afx_msg void OnBnClickedButtonedit();
	afx_msg void OnBnClickedButtonnew();
};
