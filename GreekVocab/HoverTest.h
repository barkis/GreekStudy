#pragma once
#include "afxwin.h"
#include "GreekVocabDoc.h"
#include <map>
#include <vector>
#include <string>
#include "MDLListBox.h"

// CHoverTest dialog

class CHoverTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHoverTestDlg)

public:
	CHoverTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHoverTestDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGHOVERTEST };

protected:
	afx_msg BOOL OnTtnNeedText(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	const TCHAR *m_szAllFiles;
	CGreekVocabDoc *m_pDoc;
	CComboBox m_cboHover;
	long m_MaxWidth;
	std::map<int,CString> m_EnMap;


	void ReadAllFileLists(StringVectorMap *pFilesListMap);
	void ReadFileList(std::vector<CString> *pFileList,bool ClearListBox = false);
public:
	CString GetTranslation(int Index) const;

	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombohover();
private:
	CMDLListBox m_lstGreek;
};
