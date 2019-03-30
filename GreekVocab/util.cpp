// util.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "util.h"
#include <algorithm>


// util
void util::CentreRect(CWnd *pWindow,CRect &theRect)	{
	CRect clientRect;
	pWindow->GetClientRect(&clientRect);
	ASSERT(clientRect.right > clientRect.left && clientRect.bottom > clientRect.top);
	int iMidX = clientRect.left + (clientRect.right - clientRect.left) / 2;
	int iMidY = clientRect.top + (clientRect.bottom - clientRect.top) / 2;

	int iWidth = theRect.right - theRect.left + 1;
	int iHeight = theRect.bottom - theRect.top + 1;

	theRect.left = std::max<int>(0,iMidX - iWidth / 2);
	theRect.right = theRect.left + iWidth;

	theRect.top = std::max<int>(0,iMidY - iHeight / 2);
	theRect.bottom = theRect.top + iHeight;
	ASSERT(theRect.right > theRect.left && theRect.bottom > theRect.top);
}

std::string util::Tagify(std::string &theString,bool EndTag/*=false*/)	{
		std::string sRetVal;
		if(EndTag)	{
			sRetVal = "</>" + theString + '>';
		}
		else	{
			sRetVal = '<'+theString +'>';
		}
		return sRetVal;
}

std::string util::TagifyContents(std::string &Label,std::string Contents)	{
	std::string sRetVal;

	sRetVal = Tagify(Label) + Contents + Tagify(Label,true);
	return sRetVal;
}

/* remove "<" or "</" and ">" from string
theString: originally the string with tag markers, after processing the string without the markers
*/
void util::StripTag(std::string &theString)	{
	std::string sStripped;
	if(theString[0] == '<')	{
		if(theString[1] == '/')	{
			sStripped = theString.substr(2);
		}
		else	{
			sStripped = theString.substr(1);
		}
		if(sStripped[sStripped.size() - 1] == '>')	{
			theString = sStripped;
		}
	}
}


void util::TagContents(CString &theString, CString &theTag)	{
	theTag = "";
	int iStartTag = theString.Find(_T("<"));
	if (iStartTag != -1)	{
		theString = theString.Mid(iStartTag + 1);
		int iTagEnd = theString.Find(_T(">"), 0);
		CString strTag = theString.Left(iTagEnd);
		std::basic_string<TCHAR> sTag = (LPCTSTR)strTag;
		StripTag(sTag);
		theTag = sTag.c_str();	//text between '<' and '>' returned in theTag
		int iCloseTag = theString.Find(_T("<"), iTagEnd);
		if (iCloseTag == -1)	{
			theString = theString.Right(theString.GetLength() - iTagEnd - 1);
		}
		else		{
			theString = theString.Mid(iTagEnd + 1, iCloseTag - iTagEnd - 1);
		}

	}

}

tcstring util::Tagify(tcstring &theString,bool EndTag/*=false*/)	{
		tcstring sRetVal;
		if(EndTag)	{
			sRetVal = _T("</>") + theString + TCHAR('>');
		}
		else	{
			sRetVal = TCHAR('<')+theString + TCHAR('>');
		}
		return sRetVal;
}

tcstring util::TagifyContents(tcstring &Label,tcstring Contents)	{
	tcstring sRetVal;

	sRetVal = Tagify(Label) + Contents + Tagify(Label,true);
	return sRetVal;
}

void util::StripTag(tcstring &theString)	{
	tcstring sStripped;
	if(theString[0] == '<')	{
		if(theString[1] == TCHAR('/'))	{
			sStripped = theString.substr(2);
		}
		else	{
			sStripped = theString.substr(1);
		}
		if(sStripped[sStripped.size() - 1] == TCHAR('>'))	{
			theString = sStripped;
		}
	}
}


util::util()
{
}

util::~util()
{
}

CSize util::GetPaperSize(const int iFormat)	{
	CSize RetVal(0, 0);

	switch( iFormat){

	case DMPAPER_LETTER:
		RetVal = CSize(216, 279);
		break;
	case DMPAPER_LEGAL:
		RetVal = CSize(216, 326);
		break;

	case DMPAPER_A4:
		RetVal = CSize(210,297);
		break;

	case DMPAPER_CSHEET:  
		RetVal = CSize(559, 864);
		break;

	case DMPAPER_DSHEET:   
		RetVal = CSize(559, 864);
		break;

	case DMPAPER_ESHEET:   
		RetVal = CSize(864, 1118);
		break;

	case DMPAPER_LETTERSMALL:   
		RetVal = CSize(216, 279);
		break;

	case DMPAPER_TABLOID:   
		RetVal = CSize(279, 431);
		break;

	case DMPAPER_LEDGER:  
		RetVal = CSize(431, 279);
		break;

	case DMPAPER_STATEMENT:
		RetVal = CSize(114,216);
		break;

	case DMPAPER_EXECUTIVE:   
		RetVal = CSize(184, 267);
		break;

	case DMPAPER_A3:   
		RetVal = CSize(297, 420);
		break;

	case DMPAPER_A4SMALL:
		RetVal = CSize(210, 297);
			break;

	case DMPAPER_A5:
		RetVal = CSize(148, 210);
			break;

	case DMPAPER_B4:
		RetVal = CSize(250, 354);
		break;

	case DMPAPER_B5:
		RetVal = CSize(182, 257);
		break;

	case DMPAPER_FOLIO:
		RetVal = CSize(216, 330);
		break;

	case DMPAPER_QUARTO:
		RetVal = CSize(215, 275);
		break;

	case DMPAPER_10X14:
		RetVal = CSize(254, 356);
		break;

	case DMPAPER_11X17:   
		RetVal = CSize(279, 431);
		break;

	case DMPAPER_NOTE:
		RetVal = CSize(216, 279);
		break;

	case DMPAPER_ENV_9:
		RetVal = CSize(98, 225);
		break;

	case DMPAPER_ENV_10:
		RetVal = CSize(105, 241);
		break;

	case DMPAPER_ENV_11:
		RetVal = CSize(114, 264);
		break;

	case DMPAPER_ENV_12:
		RetVal = CSize(121, 279);
		break;

	case DMPAPER_ENV_14:
		RetVal = CSize(127, 292);
		break;

	case DMPAPER_ENV_DL:
		RetVal = CSize(110, 220);
		break;

	case DMPAPER_ENV_C5:
		RetVal = CSize(162, 229);
		break;

	case DMPAPER_ENV_C3:
		RetVal = CSize(324, 458);
		break;

	case DMPAPER_ENV_C4:
		RetVal = CSize(229, 324);
		break;

	case DMPAPER_ENV_C6:
		RetVal = CSize(114, 162);
		break;

	case DMPAPER_ENV_C65:
		RetVal = CSize(114, 229);
		break;

	case DMPAPER_ENV_B4:
		RetVal = CSize(250, 353);
		break;

	case DMPAPER_ENV_B5:
		RetVal = CSize(176, 250);
		break;

	case DMPAPER_ENV_B6:
		RetVal = CSize(176, 125);
		break;

	case DMPAPER_ENV_ITALY:
		RetVal = CSize(110, 230);
		break;

	case DMPAPER_ENV_MONARCH:
		RetVal = CSize(98, 191);
		break;

	case DMPAPER_ENV_PERSONAL:
		RetVal = CSize(92, 165);
		break;

	case DMPAPER_FANFOLD_US:
		RetVal = CSize(378, 279);
		break;

	case DMPAPER_FANFOLD_STD_GERMAN:
		RetVal = CSize(216, 305);
		break;

	case DMPAPER_FANFOLD_LGL_GERMAN:
		RetVal = CSize(216, 305);
		break;
	default:
		RetVal = CSize(210, 297);	//default to A4
		break;

	}
	return RetVal;
}

/*Use GetOpenFileName/GetSaveFileName API call to show file dialog
hWnd: handle to owning window
strDefault: default file extension
returns: name of file chosen
*/

CString util::GetFile(HWND hWnd, CString strDefault,TCHAR *aFilter,CString strPath,CString strFileName,bool IsSaving /*=true*/)	{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260];       // buffer for file name 
	CString strRetVal = _T("");

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	memset(ofn.lpstrFile, 0, sizeof(szFile));
	if (!strFileName.IsEmpty())	{
		_tcscpy(ofn.lpstrFile, (LPCTSTR)strFileName);
	}
	ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile); 
	ofn.lpstrFilter = aFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = (LPCTSTR)strPath;
	ofn.lpstrDefExt = (LPCTSTR)strDefault;
	// Display the Open dialog box. 
	if (IsSaving)	{
		if (GetSaveFileName(&ofn))	{
			strRetVal = ofn.lpstrFile;
		}
	}
	else if (GetOpenFileName(&ofn))	{
		strRetVal = ofn.lpstrFile;
	}
	return strRetVal;
}