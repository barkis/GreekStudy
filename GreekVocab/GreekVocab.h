
// GreekVocab.h : main header file for the GreekVocab application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CGreekVocabApp:
// See GreekVocab.cpp for the implementation of this class
//

class CGreekVocabApp : public CWinApp
{
public:
	CGreekVocabApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGreekVocabApp theApp;
