// AdjectiveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "AdjectiveDlg.h"
#include "afxdialogex.h"
#include "GreekVocabException.h"
#include "GreekVocabDoc.h"
#include "MainFrm.h"
// CAdjectiveDlg dialog

IMPLEMENT_DYNAMIC(CAdjectiveDlg, CDialogEx)

CAdjectiveDlg::CAdjectiveDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CAdjectiveDlg::IDD, pParent)
, m_strNomSM(_T(""))
, m_strTrans(_T(""))
{

}

CAdjectiveDlg::~CAdjectiveDlg()
{
}

void CAdjectiveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTRANSLATION, m_Adjective.m_strTranslation);
	DDX_Text(pDX, IDC_EDITNOMSM, m_Adjective.m_MascDecl.m_strNominativeS);
	DDX_Text(pDX, IDC_EDITACCSM, m_Adjective.m_MascDecl.m_strAccusativeS);
	DDX_Text(pDX, IDC_EDITGENSM, m_Adjective.m_MascDecl.m_strGenitiveS);
	DDX_Text(pDX, IDC_EDITDATSM, m_Adjective.m_MascDecl.m_strDativeP);
	DDX_Text(pDX, IDC_EDITNOMPM, m_Adjective.m_MascDecl.m_strNominativeP);
	DDX_Text(pDX, IDC_EDITACCPM, m_Adjective.m_MascDecl.m_strAccusativeP);
	DDX_Text(pDX, IDC_EDITGENPM, m_Adjective.m_MascDecl.m_strGenitiveP);
	DDX_Text(pDX, IDC_EDITDATPM, m_Adjective.m_MascDecl.m_strDativeP);
	DDX_Text(pDX, IDC_EDITVOCM, m_Adjective.m_MascDecl.m_strVocative);

	DDX_Text(pDX, IDC_EDITNOMSF, m_Adjective.m_FemDecl.m_strNominativeS);
	DDX_Text(pDX, IDC_EDITACCSF, m_Adjective.m_FemDecl.m_strAccusativeS);
	DDX_Text(pDX, IDC_EDITGENSF, m_Adjective.m_FemDecl.m_strGenitiveS);
	DDX_Text(pDX, IDC_EDITDATSF, m_Adjective.m_FemDecl.m_strDativeP);
	DDX_Text(pDX, IDC_EDITNOMPF, m_Adjective.m_FemDecl.m_strNominativeP);
	DDX_Text(pDX, IDC_EDITACCPF, m_Adjective.m_FemDecl.m_strAccusativeP);
	DDX_Text(pDX, IDC_EDITGENPF, m_Adjective.m_FemDecl.m_strGenitiveP);
	DDX_Text(pDX, IDC_EDITDATPF, m_Adjective.m_FemDecl.m_strDativeP);

	DDX_Text(pDX, IDC_EDITNOMSN, m_Adjective.m_NeutDecl.m_strNominativeS);
	DDX_Text(pDX, IDC_EDITACCSN, m_Adjective.m_NeutDecl.m_strAccusativeS);
	DDX_Text(pDX, IDC_EDITGENSN, m_Adjective.m_NeutDecl.m_strGenitiveS);
	DDX_Text(pDX, IDC_EDITDATSN, m_Adjective.m_NeutDecl.m_strDativeP);
	DDX_Text(pDX, IDC_EDITNOMPN, m_Adjective.m_NeutDecl.m_strNominativeP);
	DDX_Text(pDX, IDC_EDITACCPN, m_Adjective.m_NeutDecl.m_strAccusativeP);
	DDX_Text(pDX, IDC_EDITGENPN, m_Adjective.m_NeutDecl.m_strGenitiveP);
	DDX_Text(pDX, IDC_EDITDATPN, m_Adjective.m_NeutDecl.m_strDativeP);

	DDX_Control(pDX, IDC_EDITDICTIONARYFORM, m_edtDictForm);

	DDX_Control(pDX, IDC_EDITNOMSM, m_edtNomSM);
	DDX_Control(pDX, IDC_EDITACCSM, m_edtAccSM);
	DDX_Control(pDX, IDC_EDITGENSM, m_edtGenSM);
	DDX_Control(pDX, IDC_EDITDATSM, m_edtDatSM);
	DDX_Control(pDX, IDC_EDITNOMPM, m_edtNomPM);
	DDX_Control(pDX, IDC_EDITACCPM, m_edtAccPM);
	DDX_Control(pDX, IDC_EDITGENPM, m_edtGenPM);
	DDX_Control(pDX, IDC_EDITDATPM, m_edtDatPM);

	DDX_Control(pDX, IDC_EDITNOMSF, m_edtNomSF);
	DDX_Control(pDX, IDC_EDITACCSF, m_edtAccSF);
	DDX_Control(pDX, IDC_EDITGENSF, m_edtGenSF);
	DDX_Control(pDX, IDC_EDITDATSF, m_edtDatSF);
	DDX_Control(pDX, IDC_EDITNOMPF, m_edtNomPF);
	DDX_Control(pDX, IDC_EDITACCPF, m_edtAccPF);
	DDX_Control(pDX, IDC_EDITGENPF, m_edtGenPF);
	DDX_Control(pDX, IDC_EDITDATPF, m_edtDatPF);

	DDX_Control(pDX, IDC_EDITNOMSN, m_edtNomSN);
	DDX_Control(pDX, IDC_EDITACCSN, m_edtAccSN);
	DDX_Control(pDX, IDC_EDITGENSN, m_edtGenSN);
	DDX_Control(pDX, IDC_EDITDATSN, m_edtDatSN);
	DDX_Control(pDX, IDC_EDITNOMPN, m_edtNomPN);
	DDX_Control(pDX, IDC_EDITACCPN, m_edtAccPN);
	DDX_Control(pDX, IDC_EDITGENPN, m_edtGenPN);
	DDX_Control(pDX, IDC_EDITDATPN, m_edtDatPN);

	DDX_Control(pDX, IDC_EDITVOCM, m_edtVocM);
}


BEGIN_MESSAGE_MAP(CAdjectiveDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDITNOMPF, IDC_EDITDATSF, &CAdjectiveDlg::OnChangeEditRange)
	ON_BN_CLICKED(IDBUTTONSAVE, &CAdjectiveDlg::OnBnClickedButtonsave)
	ON_EN_KILLFOCUS(IDC_EDITNOMSM, &CAdjectiveDlg::OnKillfocusEditnomsm)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDITNOMPF, IDC_EDITDATSF, &CAdjectiveDlg::OnKillFocusEditRange)
END_MESSAGE_MAP()

void CAdjectiveDlg::Save()
{
	CGreekVocabDoc *pDoc = GetDocument();
	if (!m_Adjective.m_strFileName.GetLength())	{	//new entry
		if (pDoc->IsAdjectiveName(m_Adjective.m_strDescription))	{
			AfxMessageBox(_T("Please enter a unique declension name"));
			CEdit *pedtDescription = static_cast<CEdit*>(GetDlgItem(IDC_EDITDECLENSION));
			pedtDescription->SetFocus();
		}
		else	{
			FILE *fStream;
			CString strDefault = _T("*.stx");
			UpdateData(TRUE);

			CFileDialog fd(FALSE, _T("*.stx"), strDefault, OFN_HIDEREADONLY,
				_T("Gk. syntax files (*.stx)|*.stx|All files (*.*)|*.*||"));
			if (fd.DoModal() == IDOK)	{
				CString strFileName = fd.GetPathName();
				TCHAR szFname[1001];
				_tcscpy(szFname, (LPCTSTR)strFileName);
				errno_t e = _tfopen_s(&fStream,
					//szFname,_T("w,ccs=UNICODE"));
					szFname, _T("a,ccs=UTF-8"));
				if (e != 0)		{
					throw CVocabException(_T("_tfopen_s failed"), e);
				}
				CStdioFile fout(fStream);  // open the file from this stream
				m_Adjective.Write(fout);
				fout.Close();
				fclose(fStream);
				//		CGreekVocabDoc *pDoc = GetDocument();
				pDoc->LoadSyntaxLists();	//Refresh the lists so latest addition is included
			}
		}
	}
	else	{
		//copy changed details back to Adjective object in document
		CAdjective &docAdjective = pDoc->GetAdjective(m_Adjective.m_strDescription);
		docAdjective = m_Adjective;
		pDoc->WriteAllSyntax(m_Adjective.m_strFileName);
	}
}
CGreekVocabDoc *CAdjectiveDlg::GetDocument() const	{
	CWnd *pWnd = GetParent();
	CMainFrame* pTheFrame = (CMainFrame*)pWnd;
	CGreekVocabDoc *pDoc = static_cast<CGreekVocabDoc*>(pTheFrame->GetActiveDocument());
	return pDoc;
}

/*
deal with changes in edit boxes.
IDC_EDITNOMPF=1021, IDC_EDITDATSF=1046
ID: ID of edit box
*/
void CAdjectiveDlg::OnChangeEditRange(UINT ID)	{
	bool bAllEntered = true;
	CString strEditText;
	CEdit *pEditBox;
	ASSERT(ID >= IDC_EDITNOMPF && ID <= IDC_EDITDATSF);
	//Check all boxes to see if ready to save
	for (int i = IDC_EDITNOMPF; i < IDC_EDITDATSF; ++i)	{
		CWnd *pWnd = GetDlgItem(i);
		if (pWnd)	{
			pEditBox = dynamic_cast<CEdit*>(pWnd);
			if (!pEditBox)	{
				throw CVocabException(_T("Edit box ID not found"), i);
			}
			else	{
				pEditBox->GetWindowText(strEditText);
				if (strEditText.IsEmpty())	{
					bAllEntered = false;	//not ready to save
					break;
				}

			}
		}
	}
	CButton *pbtnSaveAndContinue = (CButton*)GetDlgItem(IDBUTTONSAVEANDCONTINUE);
	pbtnSaveAndContinue->EnableWindow(bAllEntered);
	CButton *pbtnSave = (CButton*)GetDlgItem(IDBUTTONSAVE);
	pbtnSave->EnableWindow(bAllEntered);
}




void CAdjectiveDlg::OnKillFocusEditRange(UINT ID)	{
	CEdit *pEditBox;
	CString strEditText;
	pEditBox = dynamic_cast<CEdit*>(GetDlgItem(ID));
	pEditBox->GetWindowText(strEditText);

	if (ID == IDC_EDITDICTIONARYFORM)	{
		if (!m_Adjective.m_MascDecl.m_strNominativeS.GetLength())	{
			m_Adjective.m_MascDecl.m_strNominativeS = strEditText;
		}
		if (ID == IDC_EDITGENSM)	{
			pEditBox = dynamic_cast<CEdit*>(GetDlgItem(IDC_EDITGENSN));
			if (!pEditBox->LineLength())	{
				pEditBox->SetWindowText(strEditText);
			}
		}
		else if (ID == IDC_EDITDATSM)	{
			pEditBox = dynamic_cast<CEdit*>(GetDlgItem(IDC_EDITDATSN));
			if (!pEditBox->LineLength())	{
				pEditBox->SetWindowText(strEditText);
			}
		}
		else if (ID == IDC_EDITGENPM)	{
			pEditBox = dynamic_cast<CEdit*>(GetDlgItem(IDC_EDITGENPN));
			if (!pEditBox->LineLength())	{
				pEditBox->SetWindowText(strEditText);
			}
		}
		else if (ID == IDC_EDITDATPM)	{
			pEditBox = dynamic_cast<CEdit*>(GetDlgItem(IDC_EDITDATPN));
			if (!pEditBox->LineLength())	{
				pEditBox->SetWindowText(strEditText);
			}
		}

	}
}

BOOL CAdjectiveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMainFrame *pFrame = (CMainFrame*)GetParent();
	CGreekEdit::SetFrame(pFrame);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CAdjectiveDlg::OnBnClickedButtonsave()
{
	Save();
	EndDialog(IDOK);
}

/*
	When leaving the masc nom sing box if the voc box is empty then copy to it
	*/
void CAdjectiveDlg::OnKillfocusEditnomsm()
{
	UpdateData(TRUE);
	if (m_Adjective.m_MascDecl.m_strNominativeS.GetLength() && !m_Adjective.m_MascDecl.m_strVocative.GetLength())	{
		m_Adjective.m_MascDecl.m_strVocative = m_Adjective.m_MascDecl.m_strNominativeS;
	}
	UpdateData(TRUE);
}

void CAdjectiveDlg::OnKillfocusEditdictionaryform()
{
	UpdateData(TRUE);
	if (m_Adjective.m_strDictionaryForm.GetLength() && !m_Adjective.m_MascDecl.m_strNominativeS.GetLength())	{
		m_Adjective.m_MascDecl.m_strNominativeS = m_Adjective.m_strDictionaryForm;
		if (!m_Adjective.m_MascDecl.m_strVocative.GetLength())	{
			m_Adjective.m_MascDecl.m_strVocative = m_Adjective.m_MascDecl.m_strNominativeS;
		}
		UpdateData(FALSE);
	}
}


