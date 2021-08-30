// NounEntryTest.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "GreekVocabDoc.h"
#include "NounEntryTestDlg.h"
//#include "afxdialogex.h"
#include "GreekVocabException.h"
#include "MainFrm.h"
#include <vector>
#include <typeinfo>
// CNounEntryTest dialog

IMPLEMENT_DYNAMIC(CNounEntryTestDlg, CDialogEx)

CNounEntryTestDlg::CNounEntryTestDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CNounEntryTestDlg::IDD, pParent), m_strFirstLineOriginal(_T("Select declension")),
	m_strFirstLineReplacement(_T("Clear all")), m_Red(RGB(255, 0, 0)), m_Black(RGB(0, 0, 0)),
	m_Green(RGB(0, 255, 0)), m_IsClearing(false)
{

}

CNounEntryTestDlg::~CNounEntryTestDlg()
{
	m_Font.DeleteObject();
}

void CNounEntryTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITDICTIONARYFORM, m_edtDictionaryForm);
	DDX_Control(pDX, IDC_EDITNOMS, m_edtNomS);
	DDX_Control(pDX, IDC_EDITACCS, m_edtAccS);
	DDX_Control(pDX, IDC_EDITGENS, m_edtGenS);
	DDX_Control(pDX, IDC_EDITDATS, m_edtDatS);
	DDX_Control(pDX, IDC_EDITNOMP, m_edtNomP);
	DDX_Control(pDX, IDC_EDITACCP, m_edtAccP);
	DDX_Control(pDX, IDC_EDITGENP, m_edtGenP);
	DDX_Control(pDX, IDC_EDITDATP, m_edtDatP);
	DDX_Control(pDX, IDC_EDITVOC, m_edtVocative);
	DDX_Text(pDX, IDC_EDITDICTIONARYFORM, m_strDictionaryForm);
	DDX_Text(pDX, IDC_EDITNOMS, m_strNominateveS);
	DDX_Text(pDX, IDC_EDITACCS, m_strAccusativeS);
	DDX_Text(pDX, IDC_EDITGENS, m_strGenitiveS);
	DDX_Text(pDX, IDC_EDITDATS, m_strDativeS);
	DDX_Text(pDX, IDC_EDITNOMP, m_strNominateveP);
	DDX_Text(pDX, IDC_EDITACCP, m_strAccusativeP);
	DDX_Text(pDX, IDC_EDITGENP, m_strGenitiveP);
	DDX_Text(pDX, IDC_EDITDATP, m_strDativeP);
	DDX_Text(pDX, IDC_EDITVOC, m_strVocative);
	DDX_Control(pDX, IDC_COMBOARTICLE, m_cboArticle);
}


BEGIN_MESSAGE_MAP(CNounEntryTestDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBODECLENSIONNAMES, &CNounEntryTestDlg::OnSelchangeComboDeclensionNames)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDITNOMS, IDC_EDITDATS, &CNounEntryTestDlg::OnChangeEditRange)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDITNOMS, IDC_EDITDATS, &CNounEntryTestDlg::OnFocusEditRange)
	//	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CNounEntryTest message handlers


BOOL CNounEntryTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//Hide all buttons except "Close"
	CWnd* pButton = GetDlgItem(IDSAVE);
	pButton->ShowWindow(SW_HIDE);
	pButton = GetDlgItem(IDBUTTONSAVEANDCONTINUE);
	pButton->ShowWindow(SW_HIDE);
	pButton = GetDlgItem(IDBUTTONCLEAR);
	pButton->ShowWindow(SW_HIDE);
	pButton = GetDlgItem(IDBUTTONEDIT);
	pButton->ShowWindow(SW_HIDE);

	//Move close button to middle
	pButton = GetDlgItem(IDCANCEL);
	CRect rectButton(144, 220, 213, 234);
	MapDialogRect(&rectButton);
	pButton->SetWindowPos(&wndTop, rectButton.left,rectButton.top,rectButton.Width(),rectButton.Height(),
		SWP_NOZORDER);

	CWnd *pWnd = GetParent();
	CMainFrame *pFrame = dynamic_cast<CMainFrame *>(pWnd);
	if (!pFrame)	{
		throw CVocabException(_T("Not a frame ptr in CNounEntryTestDlg::OnInitDialog()"));
	}
	CGreekEdit::SetFrame(pFrame);
	CWnd* pedtDeclension = GetDlgItem(IDC_EDITDECLENSION);
	CWnd *plblDeclension = GetDlgItem(IDC_STATICDECLENSION);

	pedtDeclension->ShowWindow(SW_HIDE);
	//plblDeclension->ShowWindow(SW_HIDE);
	plblDeclension->SetWindowText(_T("Declension"));

	m_cboArticle.AddString(_T("ὁ"));
	m_cboArticle.AddString(_T("ἡ"));
	m_cboArticle.AddString(_T("το"));
	m_cboArticle.SetCurSel(0);

	std::vector<CString> vNounNames;
	CGreekVocabDoc *pDoc = GetDocument();
	vNounNames = pDoc->GetNounNames();
	ASSERT(vNounNames.size());
	CRect rect;
	pedtDeclension->GetWindowRect(&rect);	//combo position and width of hidden edit box
	ScreenToClient(&rect);
	m_cboNounNames.Create(CBS_DROPDOWNLIST | WS_TABSTOP, rect, this, IDC_COMBODECLENSIONNAMES);
	// Creates a 12-point-Courier-font
	m_Font.CreatePointFont(120, _T("Courier New"));

	// With a member variable associated to the static control
	m_cboNounNames.SetFont(&m_Font);
	m_cboNounNames.AddString(m_strFirstLineOriginal);
	std::vector<CString>::iterator iter;
	for (iter = vNounNames.begin(); iter != vNounNames.end(); ++iter)	{
		m_cboNounNames.AddString(*iter);
	}

	m_cboNounNames.ShowWindow(SW_NORMAL);
	m_cboNounNames.SetCurSel(0);
	m_cboNounNames.SetFocus();
	EnableTextBoxes(false);
	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

CGreekVocabDoc *CNounEntryTestDlg::GetDocument()	{
	CGreekVocabDoc *pRetVal = NULL;
	CMainFrame *pFrame = dynamic_cast<CMainFrame*>(GetParent());
	if (pFrame)	{
		pRetVal = dynamic_cast<CGreekVocabDoc*>(pFrame->GetActiveDocument());
		if (!pRetVal)	{
			throw CVocabException(_T("Not a CGreekVocabDoc ptr in CNounEntryTestDlg::GetDocument()"));
		}
	}
	else	{
		throw CVocabException(_T("Not a CMainFrame ptr in CNounEntryTestDlg::GetDocument()"));
	}
	return pRetVal;
}
/*
	When selection is changed in combo clear existing selection and load new declension if one is selected
	*/
void CNounEntryTestDlg::OnSelchangeComboDeclensionNames()	{
	CString strDeclension;
	ClearEditBoxes();
	if (m_cboNounNames.GetCurSel() == 0)	{
		m_mapTranslations.clear();
	}
	else	{
		m_cboNounNames.GetLBText(0, strDeclension);
		if (strDeclension == m_strFirstLineOriginal)	{	//Set first line to "Clear all"
			m_cboNounNames.DeleteString(0);
			m_cboNounNames.InsertString(0, m_strFirstLineReplacement);
		}
		CString strSelection;
		m_cboNounNames.GetLBText(m_cboNounNames.GetCurSel(), strSelection);
		//Extract 1st part which is declension name
		CString strSelectedName = strSelection.SpanExcluding(_T("\t"));
		CGreekVocabDoc *pDoc = GetDocument();
		m_Noun = *pDoc->GetNoun(strSelectedName);
		m_strDictionaryForm = m_Noun.m_strDictionaryForm;
		if (!m_Noun.m_strArticle.IsEmpty())	{
			int iArticlePos = m_cboArticle.FindString(-1, m_Noun.m_strArticle);
			if (iArticlePos == CB_ERR)	{
				throw CVocabException(_T("Article not found in NounTestEntryDlg OnSelchangeComboDeclensionNames(): ") + m_Noun.m_strArticle);
			}
			else	{
				m_cboArticle.SetCurSel(iArticlePos);
			}
		}
		m_mapTranslations.clear();
		m_mapTranslations.insert(std::make_pair(IDC_EDITNOMS, m_Noun.m_strNominativeS));
		m_mapTranslations.insert(std::make_pair(IDC_EDITACCS, m_Noun.m_strAccusativeS));
		m_mapTranslations.insert(std::make_pair(IDC_EDITGENS, m_Noun.m_strGenitiveS));
		m_mapTranslations.insert(std::make_pair(IDC_EDITDATS, m_Noun.m_strDativeS));
		m_mapTranslations.insert(std::make_pair(IDC_EDITNOMP, m_Noun.m_strNominativeP));
		m_mapTranslations.insert(std::make_pair(IDC_EDITACCP, m_Noun.m_strAccusativeP));
		m_mapTranslations.insert(std::make_pair(IDC_EDITGENP, m_Noun.m_strGenitiveP));
		m_mapTranslations.insert(std::make_pair(IDC_EDITDATP, m_Noun.m_strDativeP));
		m_mapTranslations.insert(std::make_pair(IDC_EDITVOC, m_Noun.m_strVocative));
		EnableTextBoxes(true);
		m_edtNomS.SetFocus();
	}
	UpdateData(FALSE);
}

void CNounEntryTestDlg::ClearEditBoxes()	{
	m_IsClearing = true;
	for (int i = IDC_EDITDICTIONARYFORM; i <= IDC_EDITDATS; ++i)	{

		CWnd *pNextWnd = GetDlgItem(i);
		CString strType(typeid(*pNextWnd).name());
		CEdit *pEdit = dynamic_cast<CEdit*>(pNextWnd);
		if (pEdit)	{
			CString strText;
			pEdit->GetWindowText(strText);
			pEdit->SetWindowText(_T(""));
		}
		UpdateData(TRUE);
	}
	m_IsClearing = false;
}

void CNounEntryTestDlg::OnChangeEditRange(UINT ID)
{
	if (!m_IsClearing)	{
		ASSERT(ID >= IDC_EDITNOMS && ID <= IDC_EDITDATS);
		auto iter = m_mapTranslations.find(ID);
		if (iter == m_mapTranslations.end())	{
			throw CVocabException(_T("ID not found in CDeclensionEntryTestDlg::OnChangeEditRange"), ID);
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
			throw CVocabException(_T("Not a CGreekEdit in CNounEntryTestDlg::OnChangeEditRange"));
		}
		CString strTry;
		pGreekEdit->GetWindowText(strTry);
		strTry.MakeLower();
		if (strTry.GetLength() == strTranslation.GetLength()) {
			//if (strTry.Right(1) == "σ") {
			std::wstring sTry((LPCTSTR)strTry);
			if(sTry[sTry.size() - 1] ==  wchar_t(963))	{	//'σ'
				TRACE("Sigma last letter\n");
				sTry[sTry.size() - 1] = wchar_t(962);	//ς
				pGreekEdit->SetWindowText(sTry.c_str());
				TRACE("sTry is %s\n", sTry.c_str());
				strTry = strTry.Left(strTry.GetLength() - 1) + (CString)"";
				strTry = (CString)sTry.c_str();
			}
		}
		if (strTranslation == strTry)	{
			m_TextColor = m_Black;
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
//HBRUSH CNounEntryTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
//	CWnd *pFocusWnd = GetFocus();
//	if (pWnd == pFocusWnd)	{
//		pDC->SetTextColor(m_TextColor);
//	}
//	return hbr;
//}
/* enable or disable all the entry boxes*/
void CNounEntryTestDlg::EnableTextBoxes(bool Enable)	{
	for (int i = IDC_EDITDICTIONARYFORM; i <= IDC_EDITDATS; ++i)	{
		CWnd *pEdit = GetDlgItem(i);
		pEdit->EnableWindow(Enable);
	}
}

/* When text box gets focus look up translation and set box to set colour according to match
*/
void CNounEntryTestDlg::OnFocusEditRange(UINT ID)	{
	CGreekEdit *pGreekEdit = dynamic_cast<CGreekEdit*>(GetDlgItem(ID));
	if (pGreekEdit)	{
		ASSERT(ID >= IDC_EDITNOMS && ID <= IDC_EDITDATS);
		auto iter = m_mapTranslations.find(ID);
		if (iter == m_mapTranslations.end())	{
			throw CVocabException(_T("ID not found in CDeclensionEntryTestDlg::OnChangeEditRange"), ID);
		}
		CString strTranslation = iter->second;
		if (strTranslation.Right(1) == "\r")	{
			strTranslation = strTranslation.Left(strTranslation.GetLength() - 1);
		}
		strTranslation.MakeLower();
		pGreekEdit->SetEntryTest(strTranslation);
	}
}