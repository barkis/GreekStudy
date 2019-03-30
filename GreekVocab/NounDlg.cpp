// NounDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "NounDlg.h"
#include "afxdialogex.h"
#include "GreekVocabException.h"
#include "GreekVocabDoc.h"

#include "NounNamesDlg.h"
#include <iostream>

// CNounDlg dialog

IMPLEMENT_DYNAMIC(CNounDlg, CDialogEx)

	CNounDlg::CNounDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNounDlg::IDD, pParent)
{
	m_pNoun = new CNoun;
}

CNounDlg::~CNounDlg()
{
	if (m_pNoun->m_strFileName.IsEmpty())	{
		TRACE0("Deleting m_pNoun\n");
		delete m_pNoun;
	}
	else	{
		TRACE0("Not empty, not deleting m_pNoun\n");
	}
}

void CNounDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITDECLENSION, m_pNoun->m_strDescription);
	DDX_Text(pDX, IDC_EDITDICTIONARYFORM, m_pNoun->m_strDictionaryForm);
	DDX_Text(pDX, IDC_COMBOARTICLE, m_pNoun->m_strArticle);
	DDX_Text(pDX, IDC_EDITNOUNTRANS, m_pNoun->m_strTranslation);
	DDX_Text(pDX, IDC_EDITNOMS, m_pNoun->m_strNominativeS);
	DDX_Text(pDX, IDC_EDITACCS, m_pNoun->m_strAccusativeS);
	DDX_Text(pDX, IDC_EDITDATS, m_pNoun->m_strDativeS);
	DDX_Text(pDX, IDC_EDITNOMP, m_pNoun->m_strNominativeP);
	DDX_Text(pDX, IDC_EDITACCP, m_pNoun->m_strAccusativeP);
	DDX_Text(pDX, IDC_EDITGENP, m_pNoun->m_strGenitiveP);
	DDX_Text(pDX, IDC_EDITDATP, m_pNoun->m_strDativeP);
	DDX_Text(pDX, IDC_EDITGENS, m_pNoun->m_strGenitiveS);
	DDX_Text(pDX, IDC_EDITVOC, m_pNoun->m_strVocative);
	DDX_Control(pDX, IDC_EDITDECLENSION, m_edtDeclension);
	DDX_Control(pDX, IDC_EDITNOMS, m_edtNomS);
	DDX_Control(pDX, IDC_EDITDICTIONARYFORM, m_edtDictForm);
	DDX_Control(pDX, IDC_EDITACCS, m_edtAccS);
	DDX_Control(pDX, IDC_EDITGENS, m_edtGenS);
	DDX_Control(pDX, IDC_EDITDATS, m_edtDatS);
	DDX_Control(pDX, IDC_EDITNOMP, m_edtNomP);
	DDX_Control(pDX, IDC_EDITACCP, m_edtAccP);
	DDX_Control(pDX, IDC_EDITGENP, m_edtGenP);
	DDX_Control(pDX, IDC_EDITDATP, m_edtDatP);
	DDX_Control(pDX, IDC_EDITVOC, m_edtVoc);
	DDX_Control(pDX, IDSAVE, m_btnSaveAndClose);
	DDX_Control(pDX, IDBUTTONCLEAR, m_btnClear);
	DDX_Control(pDX, IDBUTTONSAVEANDCONTINUE, m_btnSaveAndContinue);
	DDX_Control(pDX, IDC_COMBOARTICLE, m_cboArticle);
}


BEGIN_MESSAGE_MAP(CNounDlg, CDialogEx)
	ON_EN_CHANGE(IDC_EDITDECLENSION, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITDICTIONARYFORM, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITNOMS, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITACCS, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITDATS, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITGENS, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITNOMP, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITACCP, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITGENP, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITDATP, &CNounDlg::OnChangeEditBox)
	ON_EN_CHANGE(IDC_EDITVOC, &CNounDlg::OnChangeEditBox)
	ON_BN_CLICKED(IDSAVE, &CNounDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDBUTTONCLEAR, &CNounDlg::OnBnClickedClear)
	ON_BN_CLICKED(IDBUTTONSAVEANDCONTINUE, &CNounDlg::OnBnClickedButtonsaveandcontinue)
	ON_EN_KILLFOCUS(IDC_EDITDICTIONARYFORM, &CNounDlg::OnKillfocusEditdictionaryform)
	ON_BN_CLICKED(IDBUTTONEDIT, &CNounDlg::OnBnClickedButtonedit)
	ON_EN_KILLFOCUS(IDC_EDITNOMS, &CNounDlg::OnKillfocusEditnoms)
END_MESSAGE_MAP()


// CNounDlg message handlers


void CNounDlg::OnBnClickedSave()	{
	Save();
	EndDialog(IDOK);
}

void CNounDlg::Save()
{
	FILE *fStream;
	UpdateData(TRUE);
	CGreekVocabDoc *pDoc = GetDocument();
	if(!m_pNoun->m_strFileName.GetLength())	{	//new entry
		if( pDoc->IsNounName(m_pNoun->m_strDescription))	{
			AfxMessageBox(_T("Please enter a unique declension name"));
			m_edtDeclension.SetFocus();
		}
		else	{
			CString strDefault = _T("*.stx");
			CString strNoun = _T("Noun\n");
			UpdateData(TRUE);

			CFileDialog fd(FALSE,_T("*.stx"),strDefault,OFN_HIDEREADONLY,
				_T("Gk. syntax files (*.stx)|*.stx|All files (*.*)|*.*||"));
			if(fd.DoModal() == IDOK)	{
				m_strFileName = fd.GetPathName(); 
				TCHAR szFname[1001];
				_tcscpy(szFname,(LPCTSTR)m_strFileName);
				errno_t e = _tfopen_s(&fStream,
					szFname,_T("a,ccs=UTF-8"));
				if (e != 0)		{
					throw CVocabException(_T("_tfopen_s failed"),e);
				}
				CStdioFile fout(fStream);  // open the file from this stream

				m_pNoun->Write(fout);
				fout.Close();
				fclose(fStream);
				pDoc->AddNoun(m_pNoun,m_strFileName);
			}
		}
	}
	else	{	//editing an existing entry
		//copy changed details back to noun object in document
		CNoun *docNoun = pDoc->GetNoun(m_pNoun->m_strDescription);
		*docNoun = *m_pNoun;
		pDoc->WriteAllSyntax(m_pNoun->m_strFileName);
	}
}


BOOL CNounDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CMainFrame *pFrame = (CMainFrame*)GetParent();
	CGreekEdit::SetFrame(pFrame);
	m_cboArticle.AddString(_T("ὁ"));
	m_cboArticle.AddString(_T("ἡ"));
	m_cboArticle.AddString(_T("το"));
	m_cboArticle.SetCurSel(0);
	m_edtDeclension.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
}


void CNounDlg::OnChangeEditBox()
{
	bool bSetSave, bSetClear;
	UpdateData(TRUE);
	bSetSave = (m_pNoun->m_strDescription.GetLength() && m_pNoun->m_strDictionaryForm.GetLength()
		&& m_pNoun->m_strNominativeS.GetLength() && m_pNoun->m_strAccusativeS.GetLength()
		&& m_pNoun->m_strGenitiveS.GetLength() && m_pNoun->m_strDativeS.GetLength()
		&& m_pNoun->m_strNominativeP.GetLength() && m_pNoun->m_strAccusativeP.GetLength()
		&& m_pNoun->m_strGenitiveP.GetLength() && m_pNoun->m_strDativeP.GetLength()
		&& m_pNoun->m_strVocative.GetLength());

	bSetClear = (m_pNoun->m_strDescription.GetLength() || m_pNoun->m_strDictionaryForm.GetLength()
		|| m_pNoun->m_strNominativeS.GetLength() || m_pNoun->m_strAccusativeS.GetLength()
		|| m_pNoun->m_strGenitiveS.GetLength() || m_pNoun->m_strDativeS.GetLength()
		|| m_pNoun->m_strNominativeP.GetLength() || m_pNoun->m_strAccusativeP.GetLength()
		|| m_pNoun->m_strGenitiveP.GetLength() || m_pNoun->m_strDativeP.GetLength() 
		||  m_pNoun->m_strVocative.GetLength() );

	m_btnSaveAndClose.EnableWindow(bSetSave);
	m_btnSaveAndContinue.EnableWindow(bSetSave);
	m_btnClear.EnableWindow(bSetClear);

}


void CNounDlg::OnBnClickedClear()
{
	if(AfxMessageBox(_T("Discard all entries?"),MB_OKCANCEL|MB_ICONQUESTION) == IDOK)	{
		ClearAll();
	}
}

void CNounDlg::ClearAll()	{
	m_pNoun->m_strNominativeS = m_pNoun->m_strAccusativeS = m_pNoun->m_strGenitiveS = m_pNoun->m_strDativeS = "";
	m_pNoun->m_strNominativeP = m_pNoun->m_strAccusativeP = m_pNoun->m_strGenitiveP = m_pNoun->m_strDativeP = m_pNoun->m_strVocative = "";
	m_pNoun->m_strDescription.Empty();
	m_pNoun->m_strDictionaryForm.Empty();
	m_pNoun->m_strFileName.Empty();
	m_cboArticle.SetCurSel(0);
	UpdateData(FALSE);
	m_edtDeclension.EnableWindow();
	m_edtDeclension.SetFocus();
}

void CNounDlg::OnBnClickedButtonsaveandcontinue()
{
	CGreekVocabDoc *pDoc = GetDocument();
	Save();
	ClearAll();
}


void CNounDlg::OnKillfocusEditdictionaryform()
{
	TRACE0("KillFocus dictionary form\n");
	if(m_pNoun->m_strDictionaryForm.GetLength() && !m_pNoun->m_strNominativeS.GetLength())	{
		m_pNoun->m_strNominativeS = m_pNoun->m_strDictionaryForm;
		if(!m_pNoun->m_strVocative.GetLength())	{
			m_pNoun->m_strVocative = m_pNoun->m_strNominativeS;
		}
		UpdateData(FALSE);
	}
}


void CNounDlg::OnBnClickedButtonedit()
{
	CGreekVocabDoc *pDoc = GetDocument();
	std::vector<CString> vNouns = pDoc->GetNounNames();
	CNounNamesDlg dlg(vNouns,this);
	if(dlg.DoModal() == IDOK)	{
		CString strName = dlg.GetNounName();
		if(strName.GetLength())	{
			CString strJustName = strName.SpanExcluding(_T("\t"));
			//make copy of noun object in document
			m_pNoun = pDoc->GetNoun(strJustName);
			m_edtDeclension.EnableWindow(FALSE);	
			UpdateData(FALSE);
			m_btnSaveAndContinue.EnableWindow();
			m_btnSaveAndClose.EnableWindow();
		}
	}	
}

CGreekVocabDoc *CNounDlg::GetDocument()	{
	CWnd *pWnd = GetParent();
	CMainFrame* pTheFrame = (CMainFrame*)pWnd;
	CGreekVocabDoc *pDoc = static_cast<CGreekVocabDoc*>(pTheFrame->GetActiveDocument());
	return pDoc;
}


void CNounDlg::OnKillfocusEditnoms()
{
	TRACE0("KillFocus noms\n");
		if (!m_pNoun->m_strVocative.GetLength())	{
		m_pNoun->m_strVocative = m_pNoun->m_strNominativeS;
		TRACE1("Set voc to %s\n",m_pNoun->m_strNominativeS);
	}
	UpdateData(FALSE);
}
