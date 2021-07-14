#include "stdafx.h"
#include "GreekVocabException.h"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

extern "C"

const CString CVocabException::tcwhat() const {
	std::wostringstream ssRetVal(std::ios::app);;
	ssRetVal << static_cast<LPCTSTR>(text);
	if(code != 9999)	{
		ssRetVal <<": "<<code;
	}
	std::basic_string<TCHAR> sRetVal(ssRetVal.str());
	CString strFullText = ssRetVal.str().c_str();
	std::ofstream outfile(_T("\\\vocablists\\err.txt"));
	outfile << "code "<<code<<"File: "<< __FILE__<<" Line: "<<__LINE__;
	outfile <<": message "<<text;
	//AfxMessageBox(text);
	outfile.close();
	return strFullText;	
}

const char * CVocabException::what() const	{
	return("except");
}