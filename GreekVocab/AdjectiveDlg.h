#pragma once
#include "Declension.h"
#include "GreekEdit.h"
class CGreekVocabDoc;
// CAdjectiveDlg dialog

class CAdjectiveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdjectiveDlg)

public:
	CAdjectiveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAdjectiveDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGADJECTIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CAdjective m_Adjective;
	CString m_strNomSM;

	void Save();
	void ClearAll();
	CGreekVocabDoc *GetDocument() const;
	void OnKillfocusEditdictionaryform();
	afx_msg void OnChangeEditRange(UINT ID);
	afx_msg void OnKillFocusEditRange(UINT ID);

	CEdit m_edtDeclension;
	CGreekEdit m_edtDictForm;

	CGreekEdit m_edtNomSM;
	CGreekEdit m_edtAccSM;
	CGreekEdit m_edtGenSM;
	CGreekEdit m_edtDatSM;
	CGreekEdit m_edtNomPM;
	CGreekEdit m_edtAccPM;
	CGreekEdit m_edtGenPM;
	CGreekEdit m_edtDatPM;
	CGreekEdit m_edtVocM;

	CGreekEdit m_edtNomSF;
	CGreekEdit m_edtAccSF;
	CGreekEdit m_edtGenSF;
	CGreekEdit m_edtDatSF;
	CGreekEdit m_edtNomPF;
	CGreekEdit m_edtAccPF;
	CGreekEdit m_edtGenPF;
	CGreekEdit m_edtDatPF;
	CGreekEdit m_edtVocF;

	CGreekEdit m_edtNomSN;
	CGreekEdit m_edtAccSN;
	CGreekEdit m_edtGenSN;
	CGreekEdit m_edtDatSN;
	CGreekEdit m_edtNomPN;
	CGreekEdit m_edtAccPN;
	CGreekEdit m_edtGenPN;
	CGreekEdit m_edtDatPN;
	CGreekEdit m_edtVocN;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonsave();
	afx_msg void OnKillfocusEditnomsm();
private:
	CString m_strTrans;
};
