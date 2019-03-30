#pragma once
#include <string>

// util command target
typedef std::basic_string<TCHAR> tcstring;
class util 
{
public:
	static void CentreRect(CWnd *pWindow,CRect &theRect);
	static std::string Tagify(std::string &theString,bool EndTag = false);
	static std::string TagifyContents(std::string &Label,std::string Contents);
	static void StripTag(std::string &theString);
	static tcstring Tagify(tcstring &theString,bool EndTag = false);
	static tcstring TagifyContents(tcstring &Label,tcstring Contents);
	static void StripTag(tcstring &theString);
	static CSize GetPaperSize(const int iFormat); 
	static CString GetFile(HWND hWnd, CString strDefault, TCHAR *aFilter, CString strPath, CString strFileName, bool IsSaving = true);
	static void TagContents(CString &theString, CString &theTag);
private:
	util();
	~util();
};


