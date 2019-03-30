#include "stdafx.h"
#include "Declension.h"
#include "util.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>

 

CDeclension::~CDeclension(void)
{
}

std::ostream& operator<<(std::ostream &os,CDeclension &declension)	{
	//os << "<declension>" << '\n';
	//os << util::TagifyContents(_T("dictionary form"),_T("dictionaryform")) /*<<_T('\n')*/;
	os << declension.m_strNominativeS<<'\n';
	return os;
}

std::istream& operator>>(std ::istream &is,CDeclension &declension)	{
	return is;
}

const void CGrammarExample::WriteHead(CStdioFile &FileOut)	{
	FileOut.WriteString(m_strPartOfSpeech + _T("\n"));
	FileOut.WriteString(_T("<DESCRIPT>") + m_strDescription + _T("</DESCRIPT>\n"));
	FileOut.WriteString(_T("<DICTFORM>")+ m_strDictionaryForm + _T("</DICTFORM>\n"));
	FileOut.WriteString(_T("<TRANSLATION>") + m_strTranslation + _T("</TRANSLATION>\n"));
}

const void CGrammarExample::WriteTail(CStdioFile &pFileOut)	{
	pFileOut.WriteString(_T("/") + m_strPartOfSpeech + _T("\n"));
}

bool CDeclension::Read(CStdioFile &fileIn, CString strCloseTag)	{
	CString strNext;
	CString strTag;
	bool bFound = fileIn.ReadString(strNext);
	while (bFound && (strNext != strCloseTag))	{
		util::TagContents(strNext, strTag);
		if (strTag == _T("DESCRIPT"))	{
			m_strDescription = strNext;
		}
		else if (strTag == _T("DICTFORM"))	{
			m_strDictionaryForm = strNext;
		}
		else if (strTag == _T("TRANSLATION"))	{
			m_strTranslation = strNext;
		}
		if (strTag == _T("ARTICLE"))	{
			m_strArticle = strNext;
		}
		if (strTag == _T("NOMS"))	{
			m_strNominativeS = strNext;
		}
		else if (strTag == _T("ACCS"))	{
			m_strAccusativeS = strNext;
		}
		else if (strTag == _T("GENS"))	{
			m_strGenitiveS = strNext;
		}
		else if (strTag == _T("DATS"))	{
			m_strDativeS = strNext;
		}
		else if (strTag == _T("NOMP"))	{
			m_strNominativeP = strNext;
		}
		else if (strTag == _T("ACCP"))	{
			m_strAccusativeP = strNext;
		}
		else if (strTag == _T("GENP"))	{
			m_strGenitiveP = strNext;
		}
		else if (strTag == _T("DATP"))	{
			m_strDativeP = strNext;
		}
		else if (strTag == _T("VOC"))	{
			m_strVocative = strNext;
		}
		bFound = fileIn.ReadString(strNext);
	}
	return bFound;
}

//
const void CDeclension::Write(CStdioFile &FileOut)	{
	if (!m_strNominativeS.IsEmpty())	{
		FileOut.WriteString(_T("<NOMS>") + m_strNominativeS + _T("</NOMS>") + _T("\n"));
	}
	if (!m_strAccusativeS.IsEmpty())	{
		FileOut.WriteString(_T("<ACCS>") + m_strAccusativeS + _T("</ACCS>") + _T("\n"));
	}
	if (!m_strGenitiveS.IsEmpty())	{
		FileOut.WriteString(_T("<GENS>") + m_strGenitiveS + _T("</GENS>") + _T("\n"));
	}
	if (!m_strDativeS.IsEmpty())	{
		FileOut.WriteString(_T("<DATS>") + m_strDativeS + _T("</DATS>") + _T("\n"));
	}
	if (!m_strNominativeP.IsEmpty())	{
		FileOut.WriteString(_T("<NOMP>") + m_strNominativeP + _T("</NOMP>") + _T("\n"));
	}
	if (!m_strAccusativeP.IsEmpty())	{
		FileOut.WriteString(_T("<ACCP>") + m_strAccusativeP + _T("</ACCP>") + _T("\n"));
	}
	if (!m_strGenitiveP.IsEmpty())	{
		FileOut.WriteString(_T("<GENP>") + m_strGenitiveP + _T("</GENP>") + _T("\n"));
	}
	if (!m_strDativeP.IsEmpty())	{
		FileOut.WriteString(_T("<DATP>") + m_strDativeP + _T("</DATP>") + _T("\n"));
	}
	if (!m_strVocative.IsEmpty())	{
		FileOut.WriteString(_T("<VOC>") + m_strVocative + _T("</VOC>") + _T("\n"));
	}
}
const void CNoun::Write(CStdioFile &FileOut)	{
	WriteHead(FileOut);
	if (!m_strArticle.IsEmpty())	{
		FileOut.WriteString(_T("<ARTICLE>") + m_strArticle + _T("</ARTICLE>\n"));
	}
	CDeclension::Write(FileOut);
	WriteTail(FileOut);
}

bool CNoun::Read(CStdioFile &FileIn)	{
	CDeclension::Read(FileIn,_T("\noun"));
	return true;
}

bool CAdjective::Read(CStdioFile &filein)	{
	bool bFound;
	bFound = m_MascDecl.Read(filein,_T("</MASCULINE>"));

	if(bFound)	{
		bFound = m_FemDecl.Read(filein,_T("</FEMININE>"));
	}

	if(bFound)	{
		bFound = m_NeutDecl.Read(filein,_T("</NEUTER>"));
	}

	return bFound;
}

const void CAdjective::Write(CStdioFile &fileOut)	{
	WriteHead(fileOut);
	fileOut.WriteString(_T("<MASCULINE>"));
	m_MascDecl.Write(fileOut);
	fileOut.WriteString(_T("</MASCULINE>"));
	fileOut.WriteString(_T("<FEMININE>"));
	m_FemDecl.Write(fileOut);
	fileOut.WriteString(_T("</FEMININE>"));
	fileOut.WriteString(_T("<NEUTER>"));
	m_NeutDecl.Write(fileOut);
	fileOut.WriteString(_T("</NEUTER>"));
	WriteTail(fileOut);
}