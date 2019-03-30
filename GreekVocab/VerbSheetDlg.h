#pragma once


// CVerbSheetDlg dialog

class CVerbSheetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVerbSheetDlg)

public:
	CVerbSheetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVerbSheetDlg();

// Dialog Data
	enum { IDD = IDD_DIALOGVERBSHEET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
