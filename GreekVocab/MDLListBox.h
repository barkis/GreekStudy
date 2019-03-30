#pragma once


// CMDLListBox

class CMDLListBox : public CListBox
{
	DECLARE_DYNAMIC(CMDLListBox)

public:
	CMDLListBox();
	virtual ~CMDLListBox();
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;	
	void SetDlg(CDialogEx *pTheDlg){pDlg = pTheDlg;}
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	CPoint m_CursorPos;
	CDialogEx *pDlg;
public:
	int GetSelection();
};


