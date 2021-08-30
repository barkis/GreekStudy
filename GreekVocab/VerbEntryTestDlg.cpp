// VerbEntryTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "VerbEntryTestDlg.h"
#include "afxdialogex.h"
#include "GreekVocabException.h"
#include "Verb.h"

// CVerbEntryTestDlg dialog

IMPLEMENT_DYNAMIC(CVerbEntryTestDlg, CDialogEx)

CVerbEntryTestDlg::CVerbEntryTestDlg(CWnd* pParent /*=NULL*/)
: CVerbDlg(pParent),m_strFirstLineOriginal(_T("Select verb")),
m_strFirstLineReplacement(_T("Clear all")), m_Red(RGB(255, 0, 0)), m_Black(RGB(0, 0, 0)),
m_Green(RGB(0, 255, 0))
{
	m_pNewVerb = new CVerb;
}

CVerbEntryTestDlg::~CVerbEntryTestDlg()
{
	delete m_pNewVerb;
}

void CVerbEntryTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABVERB, m_TabSheet);
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
//	DDX_Text(pDX, IDC_EDITCONJUGATION, m_pTheVerb->m_strDescription);
	DDX_Text(pDX, IDC_EDITDICTIONARYFORM, m_pTheVerb->m_strDictionaryForm);
	DDX_Text(pDX, IDC_EDITTRANSLATION, m_pTheVerb->m_strTranslation);
	DDX_Text(pDX, IDC_EDITPERS1S, m_pNewVerbForm->m_strPers1S);
	DDX_Text(pDX, IDC_EDITPERS2S, m_pNewVerbForm->m_strPers2S);
	DDX_Text(pDX, IDC_EDITPERS3S, m_pNewVerbForm->m_strPers3S);
	DDX_Text(pDX, IDC_EDITPERS1P, m_pNewVerbForm->m_strPers1P);
	DDX_Text(pDX, IDC_EDITPERS2P, m_pNewVerbForm->m_strPers2P);
	DDX_Text(pDX, IDC_EDITPERS3P, m_pNewVerbForm->m_strPers3P);
	DDX_Text(pDX, IDC_EDITINF, m_pNewVerbForm->m_strInfinitive);
	DDX_Text(pDX, IDC_EDITPART, m_pNewVerbForm->m_strParticiple);
	DDX_Text(pDX, IDC_EDITIMPS, m_pNewVerbForm->m_strImperativeS);
	DDX_Text(pDX, IDC_EDITIMPP, m_pNewVerbForm->m_strImperativeP);
	DDX_Text(pDX, IDC_EDITCONJUGATION, m_pTheVerb->m_strDescription);
}


BEGIN_MESSAGE_MAP(CVerbEntryTestDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBOCONJUGATION, &CVerbEntryTestDlg::OnSelchangeComboConjugation)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDITPERS1S, IDC_EDITIMPP, &CVerbEntryTestDlg::OnChangeEditRange)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDITPERS1S, IDC_EDITIMPP, &CVerbEntryTestDlg::OnFocusEditRange)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABVERB, &CVerbEntryTestDlg::OnTcnSelchangeTabverb)
END_MESSAGE_MAP()


// CVerbEntryTestDlg message handlers


BOOL CVerbEntryTestDlg::OnInitDialog()
{
	m_pNewVerbForm = m_pNewVerb->m_pPresentActive;
	CVerbDlg::OnInitDialog();

	CWnd *pConjEdit = GetDlgItem(IDC_EDITCONJUGATION);
	pConjEdit->ShowWindow(SW_HIDE);
	CRect rect;
	pConjEdit->GetWindowRect(&rect);	//combo position and width of hidden edit box
	ScreenToClient(&rect);
	m_cboConjugation.Create(CBS_DROPDOWNLIST | WS_TABSTOP, rect, this, IDC_COMBOCONJUGATION);
	// Creates a 12-point-Courier-font
	m_Font.CreatePointFont(120, _T("Courier New"));

	m_cboConjugation.SetFont(&m_Font);

	//Hide all buttons except "Close"
	CWnd* pButton = GetDlgItem(IDBUTTONSAVE);
	pButton->ShowWindow(SW_HIDE);
	pButton = GetDlgItem(IDBUTTONNEW);
	pButton->ShowWindow(SW_HIDE);
	pButton = GetDlgItem(IDBUTTONEDIT);
	pButton->ShowWindow(SW_HIDE);

	//Move close button to middle
	pButton = GetDlgItem(IDCANCEL);
	CRect rectButton(144, 220, 213, 234);
	MapDialogRect(&rectButton);
	pButton->SetWindowPos(&wndTop, rectButton.left, rectButton.top, rectButton.Width(), rectButton.Height(),
		SWP_NOZORDER);

	//Disable text boxes
	EnableTextBoxes(false);

	CWnd *pWnd = GetParent();
	CMainFrame *pFrame = dynamic_cast<CMainFrame *>(pWnd);
	if (!pFrame)	{
		throw CVocabException(_T("Not a frame ptr in CNounEntryTestDlg::OnInitDialog()"));
	}
	CGreekEdit::SetFrame(pFrame);

	m_cboConjugation.SetFont(&m_Font);
	m_cboConjugation.AddString(m_strFirstLineOriginal);
	std::vector<CString> vVerbNames;
	CGreekVocabDoc *pDoc = GetDocument();
	vVerbNames = pDoc->GetVerbNames();
	std::vector<CString>::iterator iter;
	for (iter = vVerbNames.begin(); iter != vVerbNames.end(); ++iter)	{
		m_cboConjugation.AddString(*iter);
	}

	m_cboConjugation.ShowWindow(SW_NORMAL);
	m_cboConjugation.SetCurSel(0);
	m_cboConjugation.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
/*
When selection is changed in combo clear existing selection and load new conjugation if one is selected
*/
void CVerbEntryTestDlg::OnSelchangeComboConjugation()	{
	CString strConjugation;
	if (m_cboConjugation.GetCurSel() == 0)	{
		m_pTheVerb = NULL;
	}
	else	{
		m_cboConjugation.GetLBText(0, strConjugation);
		if (strConjugation == m_strFirstLineOriginal)	{	//Set first line to "Clear all"
			m_cboConjugation.DeleteString(0);
			m_cboConjugation.InsertString(0, m_strFirstLineReplacement);
		}
		ClearEditBoxes();
		EnableTextBoxes(true);
		CString strSelection;
		m_cboConjugation.GetLBText(m_cboConjugation.GetCurSel(), strSelection);
		//Extract 1st part which is Conjugation name
		CString strSelectedName = strSelection.SpanExcluding(_T("\t"));
		CGreekVocabDoc *pDoc = GetDocument();
		m_pTheVerb = pDoc->GetVerb(strSelectedName);
		m_strDictionaryForm = m_pTheVerb->m_strDictionaryForm;
	//	m_pCurrentVerbForm = m_pTheVerb->m_pPresentActive;
		SetVerbForm();
		SetTranslations();
		m_edtPers1S.SetFocus();

//		EnableTextBoxes(true);
	}
	UpdateData(FALSE);
}

void CVerbEntryTestDlg::SetTranslations() {
	m_mapTranslations.clear();
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS1S, m_pCurrentVerbForm->m_strPers1S));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS2S, m_pCurrentVerbForm->m_strPers2S));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS3S, m_pCurrentVerbForm->m_strPers3S));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS1P, m_pCurrentVerbForm->m_strPers1P));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS2P, m_pCurrentVerbForm->m_strPers2P));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS3P, m_pCurrentVerbForm->m_strPers3P));
	m_mapTranslations.insert(std::make_pair(IDC_EDITINF, m_pCurrentVerbForm->m_strInfinitive));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPART, m_pCurrentVerbForm->m_strParticiple));
	m_mapTranslations.insert(std::make_pair(IDC_EDITIMPS, m_pCurrentVerbForm->m_strImperativeS));
	m_mapTranslations.insert(std::make_pair(IDC_EDITIMPP, m_pCurrentVerbForm->m_strImperativeP));
}
/*
	Remove any text from all the edit boxes
*/
void CVerbEntryTestDlg::ClearEditBoxes()	{
	m_IsClearing = true;
	ClearTheEditBox(IDC_EDITDICTIONARYFORM);
	ClearTheEditBox(IDC_EDITTRANSLATION);
	for (int i = IDC_EDITPERS1S; i <= IDC_EDITIMPP; ++i)	{
		ClearTheEditBox(i);
	}
	UpdateData(TRUE);
	m_IsClearing = false;
}

/*
	Clear specified edit box
	Called by ClearEditBoxes
	ID: identified of edit box
*/
void CVerbEntryTestDlg::ClearTheEditBox(int ID)	{
	CWnd *pNextWnd = GetDlgItem(ID);
	CEdit *pEdit = dynamic_cast<CEdit*>(pNextWnd);
	if (pEdit)	{
		CString strText;
		pEdit->GetWindowText(strText);
		pEdit->SetWindowText(_T(""));
	}

}

void CVerbEntryTestDlg::OnChangeEditRange(UINT ID)
{
	if (!m_IsClearing)	{
		ASSERT(ID >= IDC_EDITPERS1S && ID <= IDC_EDITIMPP);
		auto iter = m_mapTranslations.find(ID);
		if (iter == m_mapTranslations.end())	{
			throw CVocabException(_T("ID not found in CVerbEntryTestDlg::OnChangeEditRange"), ID);
		}
		CString strTranslation = iter->second;
		if (strTranslation.Right(1) == "\r")	{
			strTranslation = strTranslation.Left(strTranslation.GetLength() - 1);
		}
		UpdateData(TRUE);
		strTranslation.MakeLower();
		CWnd *pWnd = GetDlgItem(ID);
		CGreekEdit *pGreekEdit = dynamic_cast<CGreekEdit*>(pWnd);
		if (!pGreekEdit)	{
			throw CVocabException(_T("Not a CGreekEdit in CVerbEntryTestDlg::OnChangeEditRange"));
		}
		CString strTry;
		pGreekEdit->GetWindowText(strTry);
		CString strSigmaTermTry = strTry;
		strTry.MakeLower();
		if (strSigmaTermTry.Right(1) == _T("σ")) {
			strSigmaTermTry = strSigmaTermTry.Left(strSigmaTermTry.GetLength() - 1) + _T("ς");
		}
		if (strTry.Right(2) == _T("σ ")) {
			strTry = strSigmaTermTry.Left(strSigmaTermTry.GetLength() - 2) + _T("ς ");
			pGreekEdit->SetWindowText((LPCTSTR)strTry);
		}
		if (strTranslation == strTry || strTranslation == strSigmaTermTry)	{
			m_TextColor = m_Black;
			if (strTry != strTranslation) {
				pGreekEdit->SetWindowText((LPCTSTR)strSigmaTermTry);
			}
		}
		else	{
			if (strTranslation.Find(strTry) == 0)	{	//matches so far
				m_TextColor = m_Green;
			}
			else	{
				m_TextColor = m_Red;
			}
		}
		pGreekEdit->Invalidate();
		UpdateData(FALSE);
	}
}

/*
Set the colour on the entry box to red for no match, green for partial match, black for full match
*/
HBRUSH CVerbEntryTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CWnd *pFocusWnd = GetFocus();
	if (pWnd == pFocusWnd)	{
		pDC->SetTextColor(m_TextColor);
	}
	return hbr;
}
/* enable or disable all the entry boxes*/
void CVerbEntryTestDlg::EnableTextBoxes(bool Enable)	{
	CWnd *pEdit = GetDlgItem(IDC_EDITTRANSLATION);
	pEdit->EnableWindow(Enable);
	pEdit = GetDlgItem(IDC_EDITDICTIONARYFORM);
	pEdit->EnableWindow(Enable);
	for (int i = IDC_EDITPERS1S; i <= IDC_EDITIMPP; ++i)	{
		pEdit = GetDlgItem(i);
		if (dynamic_cast<CEdit*>(pEdit))	{
			pEdit->EnableWindow(Enable);
		}
	}
}

/* When text box gets focus look up translation and set box to set colour according to match
*/
void CVerbEntryTestDlg::OnFocusEditRange(UINT ID)	{
	CGreekEdit *pGreekEdit = dynamic_cast<CGreekEdit*>(GetDlgItem(ID));
	if (pGreekEdit)	{
		ASSERT(ID >= IDC_EDITPERS1S && ID <= IDC_EDITIMPP);
		auto iter = m_mapTranslations.find(ID);
		if (iter == m_mapTranslations.end())	{
			throw CVocabException(_T("ID not found in CVerbEntryTestDlg::OnChangeEditRange"), ID);
		}
		CString strTranslation = iter->second;
		if (strTranslation.Right(1) == "\r")	{
			strTranslation = strTranslation.Left(strTranslation.GetLength() - 1);
		}
		strTranslation.MakeLower();
		pGreekEdit->SetEntryTest(strTranslation);
	}
}

/*
	Update the data in the current verb form of the verb object and switch to the selected verb form
*/
void CVerbEntryTestDlg::OnTcnSelchangeTabverb(NMHDR *pNMHDR, LRESULT *pResult)	{
	ClearEditBoxes();
	UpdateData(TRUE);
	CVerbDlg::OnTcnSelchangeTabverb(pNMHDR, pResult);
	SetVerbForm();
	SetTranslations();
}

void CVerbEntryTestDlg::SetVerbForm()	{

	int iCurSel = m_TabSheet.GetCurSel();
	m_Selection = m_aTabs[iCurSel];
	switch (m_Selection)	{
	case PRESACT: m_pCurrentVerbForm = m_pTheVerb->m_pPresentActive;
		break;
	case PRESMID: m_pCurrentVerbForm = m_pTheVerb->m_pPresentMiddle;
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
	}
	UpdateData(FALSE);
	m_mapTranslations.clear();
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS1S, m_pCurrentVerbForm->m_strPers1S));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS2S, m_pCurrentVerbForm->m_strPers2S));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS3S, m_pCurrentVerbForm->m_strPers3S));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS1P, m_pCurrentVerbForm->m_strPers1P));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS2P, m_pCurrentVerbForm->m_strPers2P));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPERS3P, m_pCurrentVerbForm->m_strPers3P));
	m_mapTranslations.insert(std::make_pair(IDC_EDITINF, m_pCurrentVerbForm->m_strInfinitive));
	m_mapTranslations.insert(std::make_pair(IDC_EDITPART, m_pCurrentVerbForm->m_strParticiple));
	m_mapTranslations.insert(std::make_pair(IDC_EDITIMPS, m_pCurrentVerbForm->m_strImperativeS));
	m_mapTranslations.insert(std::make_pair(IDC_EDITIMPP, m_pCurrentVerbForm->m_strImperativeP));
	m_edtPers1S.SetFocus();

}