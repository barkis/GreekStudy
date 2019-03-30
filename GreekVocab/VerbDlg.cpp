// VerbDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "VerbDlg.h"
#include "afxdialogex.h"
#include "GreekVocabException.h"
#include "util.h"
#include "VerbNamesDlg.h"
// CVerbDlg dialog

IMPLEMENT_DYNAMIC(CVerbDlg, CDialogEx)

CVerbDlg::CVerbDlg(CWnd* pParent /*=NULL*/)
	:m_IsSavedVerb(false), CDialogEx(CVerbDlg::IDD, pParent)
{
	m_pTheVerb = new CVerb;
	m_pCurrentVerbForm = m_pTheVerb->m_pPresentActive;
}

CVerbDlg::~CVerbDlg()
{
	//if the verb hasn't been saved delete it
	if (m_pTheVerb->m_strFileName.IsEmpty())	{
		delete m_pTheVerb;
	}
}

void CVerbDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABVERB, m_TabSheet);
	DDX_Text(pDX, IDC_EDITPERS1S, m_pCurrentVerbForm->m_strPers1S);
	DDX_Text(pDX, IDC_EDITPERS2S, m_pCurrentVerbForm->m_strPers2S);
	DDX_Text(pDX, IDC_EDITPERS3S, m_pCurrentVerbForm->m_strPers3S);
	DDX_Text(pDX, IDC_EDITPERS1P, m_pCurrentVerbForm->m_strPers1P);
	DDX_Text(pDX, IDC_EDITPERS2P, m_pCurrentVerbForm->m_strPers2P);
	DDX_Text(pDX, IDC_EDITPERS3P, m_pCurrentVerbForm->m_strPers3P);
	DDX_Text(pDX, IDC_EDITINF, m_pCurrentVerbForm->m_strInfinitive);
	DDX_Text(pDX, IDC_EDITPART, m_pCurrentVerbForm->m_strParticiple);
	DDX_Text(pDX, IDC_EDITIMPS, m_pCurrentVerbForm->m_strImperativeS);
	DDX_Text(pDX, IDC_EDITIMPP, m_pCurrentVerbForm->m_strImperativeP);
	DDX_Text(pDX, IDC_EDITCONJUGATION, m_pTheVerb->m_strDescription);
	DDX_Text(pDX, IDC_EDITDICTIONARYFORM, m_pTheVerb->m_strDictionaryForm);
	DDX_Text(pDX, IDC_EDITTRANSLATION, m_pTheVerb->m_strTranslation);
	DDX_Control(pDX, IDC_EDITPERS1S, m_edtPers1S);
	DDX_Control(pDX, IDC_EDITPERS2S, m_edtPers2S);
	DDX_Control(pDX, IDC_EDITPERS3S, m_edtPers3S);
	DDX_Control(pDX, IDC_EDITPERS1P, m_edtPers1P);
	DDX_Control(pDX, IDC_EDITPERS2P, m_edtPers2P);
	DDX_Control(pDX, IDC_EDITPERS3P, m_edtPers3P);
	DDX_Control(pDX, IDC_EDITINF, m_edtInfinitive);
	DDX_Control(pDX, IDC_EDITIMPS, m_edtImperativeS);
	DDX_Control(pDX, IDC_EDITIMPP, m_edtImperativeP);
	DDX_Control(pDX, IDC_EDITPART, m_edtPart);
	DDX_Control(pDX, IDC_EDITDICTIONARYFORM, m_edtDictionaryForm);
}


BEGIN_MESSAGE_MAP(CVerbDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABVERB, &CVerbDlg::OnTcnSelchangeTabverb)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDITPERS1S, IDC_EDITPERS3P, &CVerbDlg::OnChangeEditPers)
	ON_BN_CLICKED(IDBUTTONCLEAR, &CVerbDlg::OnBnClickedButtonclear)
	ON_BN_CLICKED(IDBUTTONSAVE, &CVerbDlg::OnBnClickedButtonsave)
	ON_BN_CLICKED(IDBUTTONEDIT, &CVerbDlg::OnBnClickedButtonedit)
	ON_BN_CLICKED(IDBUTTONNEW, &CVerbDlg::OnBnClickedButtonnew)
END_MESSAGE_MAP()


// CVerbDlg message handlers


BOOL CVerbDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTabs();
	CMainFrame *pFrame = (CMainFrame*)GetParent();
	CGreekEdit::SetFrame(pFrame);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


/*when a new tab is chosen copy the current verbform to the appropriate member of the verb object
and clear the boxes*/
void CVerbDlg::OnTcnSelchangeTabverb(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	int iCurSel = m_TabSheet.GetCurSel();
	m_Selection = m_aTabs[iCurSel];
	switch (m_Selection)	{
	case PRESACT: m_pCurrentVerbForm = m_pTheVerb->m_pPresentActive;
		break;
	case PRESMID: m_pCurrentVerbForm = m_pTheVerb->m_pPresentMiddle;
		break;
	case PRESACTSUBJ: m_pCurrentVerbForm = m_pTheVerb->m_pPresentActiveSubj;
		break;
	case PRESMIDSUBJ: m_pCurrentVerbForm = m_pTheVerb->m_pPresentMiddleSubj;
		break;
	case IMPACT: m_pCurrentVerbForm = m_pTheVerb->m_pImperfectActive;
		break;
	case IMPMID: m_pCurrentVerbForm = m_pTheVerb->m_pImperfectMiddle;
		break;
	case FUTACT: m_pCurrentVerbForm = m_pTheVerb->m_pFutureActive;
		break;
	case FUTMID: m_pCurrentVerbForm = m_pTheVerb->m_pFutureMiddle;
		break;
	case AORACT: m_pCurrentVerbForm = m_pTheVerb->m_pAoristActive;
		break;
	case AORMID: m_pCurrentVerbForm = m_pTheVerb->m_pAoristMiddle;
		break;
	default:
		throw CVocabException(_T("Invalid tab selection in VerbDlg.cpp"), m_Selection);
	}
	//No infinitive, imperative or participle for imperfect
	int iWindowState;
	if ((m_Selection == IMPACT) || (m_Selection == IMPMID))	{
		iWindowState = SW_HIDE;
	}
	else	{
		iWindowState = SW_SHOW;
	}
	CWnd *pNextCtrl;
	pNextCtrl = GetDlgItem(IDC_STATICINF);
	pNextCtrl->ShowWindow(iWindowState);
	pNextCtrl = GetDlgItem(IDC_EDITINF);
	pNextCtrl->ShowWindow(iWindowState);
	pNextCtrl = GetDlgItem(IDC_STATICPART);
	pNextCtrl->ShowWindow(iWindowState);
	pNextCtrl = GetDlgItem(IDC_EDITPART);
	pNextCtrl->ShowWindow(iWindowState);
	pNextCtrl = GetDlgItem(IDC_STATICIMP);
	pNextCtrl->ShowWindow(iWindowState);
	pNextCtrl = GetDlgItem(IDC_EDITIMPS);
	pNextCtrl->ShowWindow(iWindowState);
	pNextCtrl = GetDlgItem(IDC_EDITIMPP);
	pNextCtrl->ShowWindow(iWindowState);
	UpdateData(FALSE);
	CWnd* pWnd = GetDlgItem(IDC_EDITPERS1S);
	pWnd->SetFocus();
	//	Clear();
	*pResult = 0;
}

void CVerbDlg::Clear()	{
	m_pCurrentVerbForm->m_strPers1S = m_pCurrentVerbForm->m_strPers2S = m_pCurrentVerbForm->m_strPers3S = "";
	m_pCurrentVerbForm->m_strPers1P = m_pCurrentVerbForm->m_strPers2P = m_pCurrentVerbForm->m_strPers3P = "";
	m_pCurrentVerbForm->m_strParticiple = m_pCurrentVerbForm->m_strImperativeP = m_pCurrentVerbForm->m_strImperativeS = "";
	UpdateData(FALSE);
}

void CVerbDlg::SetTabs()	{
	short iNext = 0;
	m_TabSheet.InsertItem(iNext, _T("Present Active"));
	m_aTabs[iNext++] = PRESACT;
	m_TabSheet.InsertItem(iNext, _T("Present Middle"));
	m_aTabs[iNext++] = PRESMID;
	m_TabSheet.InsertItem(iNext, _T("Imperfect Active"));
	m_aTabs[iNext++] = PRESACTOPT;
	m_TabSheet.InsertItem(iNext, _T("Present Active Optative"));
	m_aTabs[iNext++] = PRESACTSUBJ;
	m_TabSheet.InsertItem(iNext, _T("Present Active Subjunctive"));
	m_aTabs[iNext++] = IMPMID;
	m_TabSheet.InsertItem(iNext, _T("Future Active"));
	m_aTabs[iNext++] = FUTACT;
	m_TabSheet.InsertItem(iNext, _T("Future Middle"));
	m_aTabs[iNext++] = FUTMID;
	m_TabSheet.InsertItem(iNext, _T("Aorist Active"));
	m_aTabs[iNext++] = AORACT;
	m_TabSheet.InsertItem(iNext, _T("Aorist Middle"));
	m_aTabs[iNext++] = AORMID;
	m_Selection = PRESACT;
}

/*
deal with changes in edit boxes for 1st pers. s. to 3rd. pers. p.
ID: ID of edit box
*/
void CVerbDlg::OnChangeEditPers(UINT ID)	{
	UpdateData(TRUE);
	bool bEmpty = m_pTheVerb->IsEmpty();
	CWnd* pSave = GetDlgItem(IDBUTTONSAVE);
	pSave->EnableWindow(!bEmpty);
	CWnd* pNew = GetDlgItem(IDBUTTONNEW);
	pNew->EnableWindow(!bEmpty);
}

void CVerbDlg::OnBnClickedButtonclear()
{
	if (AfxMessageBox(_T("Discard all entries?"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)	{
		Clear();
	}
}
CGreekVocabDoc *CVerbDlg::GetDocument()	{
	CWnd *pWnd = GetParent();
	CMainFrame* pTheFrame = (CMainFrame*)pWnd;
	CGreekVocabDoc *pDoc = static_cast<CGreekVocabDoc*>(pTheFrame->GetActiveDocument());
	return pDoc;
}

void CVerbDlg::Save()	{
	FILE *fStream;
	UpdateData(TRUE);
	CGreekVocabDoc *pDoc = GetDocument();
	if (!m_pTheVerb->m_strFileName.GetLength())	{	//new entry
		if (pDoc->IsVerbName(m_pTheVerb->m_strDescription))	{
			AfxMessageBox(_T("Please enter a unique conjugation name"));
			CWnd *pWnd = GetDlgItem(IDC_EDITCONJUGATION);
			pWnd->SetFocus();
		}

		else	{
			CString strDefault = _T("*.stx");
			CString strVerb = _T("Verb\n");
			UpdateData(TRUE);
			std::basic_string<TCHAR> sPath(pDoc->getPath());
			sPath += _T("\\vocablists\\*.stx");
			TCHAR aFilter[] = _T("Syntax file(*.stx)\0*.stx\0");
			CString strFullFileName = _T("");
			if (!m_strFileName.IsEmpty())	{
				strFullFileName = sPath.c_str();
				strFullFileName += m_strFileName;
			}
			//strFullFileName = "";
			m_strFileName = util::GetFile(GetSafeHwnd(), strDefault, aFilter, sPath.c_str(), strFullFileName);
		//	m_strFileName = util::GetFile(GetSafeHwnd(), _T("*.stx"), aFilter, m_pTheVerb->m_strFileName, sPath.c_str());
			if (!m_strFileName.IsEmpty())	{
				TCHAR szFname[1001];
				_tcscpy(szFname, (LPCTSTR)m_strFileName);
				errno_t e = _tfopen_s(&fStream,
					szFname, _T("a,ccs=UTF-8"));
				if (e != 0)		{
					throw CVocabException(_T("_tfopen_s failed in VerbDlg"), e);
				}
				CStdioFile fout(fStream);  // open the file from this stream

				m_pTheVerb->Write(fout);
				fout.Close();
				fclose(fStream);
				pDoc->AddVerb(m_pTheVerb, m_strFileName);
			}
		}
	}
	else	{	//editing an existing entry
		//copy changed details back to noun object in document
		CVerb *docVerb = pDoc->GetVerb(m_pTheVerb->m_strDescription);
		docVerb = m_pTheVerb;
		pDoc->WriteAllSyntax(m_pTheVerb->m_strFileName);
	}
}

void CVerbDlg::OnBnClickedButtonsave()
{
	Save();
	CWnd *pWnd = GetDlgItem(IDBUTTONSAVE);
	pWnd->EnableWindow(FALSE);
//	EndDialog(IDOK);
}


void CVerbDlg::OnBnClickedButtonsaveandcontinue()
{
}

/*
	When "Edit Saved" button is pressed present list of conjugations. If selection is made copy the verb
	object from the document.
*/
void CVerbDlg::OnBnClickedButtonedit()
{
	CGreekVocabDoc *pDoc = GetDocument();
	std::vector<CString> vVerbs = pDoc->GetVerbNames();
	CVerbNamesDlg dlg(vVerbs, this);
	if (dlg.DoModal() == IDOK)	{
		CString strName = dlg.GetVerbName();
		if (strName.GetLength())	{
			CString strJustName = strName.SpanExcluding(_T("\t"));
			//make copy of Verb object in document
			if (m_pTheVerb)	{
				delete m_pTheVerb;
			}
			m_pTheVerb = pDoc->GetVerb(strJustName);
			m_Selection = PRESACT;
			m_TabSheet.SetCurSel(0);
			m_pCurrentVerbForm = m_pTheVerb->m_pPresentActive;
			m_IsSavedVerb = true;
			CWnd *pWnd = GetDlgItem(IDBUTTONSAVE);
			pWnd->EnableWindow();
			pWnd = GetDlgItem(IDBUTTONNEW);
			pWnd->EnableWindow();
			pWnd = GetDlgItem(IDC_EDITCONJUGATION);
			pWnd->EnableWindow(FALSE);
			UpdateData(FALSE);
		}
	}
}



void CVerbDlg::OnBnClickedButtonnew()
{
	Save();
	m_pTheVerb = new CVerb;
	Clear();
	m_Selection = PRESACT;
	m_TabSheet.SetCurSel(0);
	m_pCurrentVerbForm = m_pTheVerb->m_pPresentActive;
	m_IsSavedVerb = true;
	CWnd *pWnd = GetDlgItem(IDBUTTONSAVE);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDBUTTONNEW);
	pWnd->EnableWindow();
	pWnd = GetDlgItem(IDC_EDITCONJUGATION);
	pWnd->EnableWindow(TRUE);
	pWnd->SetFocus();
	UpdateData(FALSE);
}
