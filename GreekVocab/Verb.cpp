#include "stdafx.h"
#include "Verb.h"
#include "util.h"

bool CVerbForm::Read(CStdioFile &fileIn, CString strCloseTag)	{
	bool bFound = false;
	CString strNext;
	CString strTag;
	bFound = fileIn.ReadString(strNext);
	while (bFound && (strNext != strCloseTag))	{
		util::TagContents(strNext, strTag);
		if (strTag == _T("PERS1S"))	{
			m_strPers1S = strNext;
		}
		else if (strTag == _T("PERS2S"))	{
			m_strPers2S = strNext;
		}
		else if (strTag == _T("PERS3S"))	{
			m_strPers3S = strNext;
		}
		else if (strTag == _T("PERS1P"))	{
			m_strPers1P = strNext;
		}
		else if (strTag == _T("PERS2P"))	{
			m_strPers2P = strNext;
		}
		else if (strTag == _T("PERS3P"))	{
			m_strPers3P = strNext;
		}
		else if (strTag == _T("INFINITIVE"))	{
			m_strInfinitive = strNext;
		}
		else if (strTag == _T("IMPS"))	{
			m_strImperativeS = strNext;
		}
		else if (strTag == _T("IMPP"))	{
			m_strImperativeP = strNext;
		}
		else if (strTag == _T("PART"))	{
			m_strParticiple = strNext;
		}
		bFound = fileIn.ReadString(strNext);
	}
	return bFound;
}

const bool CVerbForm::HasEssentials()	{
	bool bRetVal;
	bRetVal = !(m_strPers1S.IsEmpty() || m_strPers2S.IsEmpty() || m_strPers3S.IsEmpty()
		|| m_strPers1P.IsEmpty() || m_strPers2P.IsEmpty() || m_strPers3P.IsEmpty());
	return bRetVal;
}

const bool CVerbForm::IsEmpty()	{
	bool bRetVal;
	bRetVal = (m_strPers1S.IsEmpty() && m_strPers2S.IsEmpty() && m_strPers3S.IsEmpty()
		&& m_strPers1P.IsEmpty() && m_strPers2P.IsEmpty() && m_strPers3P.IsEmpty()
		&& m_strInfinitive.IsEmpty() && m_strImperativeS.IsEmpty() && m_strImperativeP.IsEmpty() && m_strParticiple.IsEmpty());
	return bRetVal;
}

const void CVerbForm::Write(CStdioFile &FileOut)	{
	FileOut.WriteString(_T("<INFINITIVE>") + m_strInfinitive + _T("</INFINITIVE>") + "\n");
	FileOut.WriteString(_T("<PERS1S>") + m_strPers1S + _T("</PERS1S>") + _T("\n"));
	FileOut.WriteString(_T("<PERS2S>") + m_strPers2S + _T("</PERS2S>") + _T("\n"));
	FileOut.WriteString(_T("<PERS3S>") + m_strPers3S + _T("</PERS3S>") + _T("\n"));
	FileOut.WriteString(_T("<PERS1P>") + m_strPers1P + _T("</PERS1P>") + _T("\n"));
	FileOut.WriteString(_T("<PERS2P>") + m_strPers2P + _T("</PERS2P>") + _T("\n"));
	FileOut.WriteString(_T("<PERS3P>") + m_strPers3P + _T("</PERS3P>") + _T("\n"));
	FileOut.WriteString(_T("<IMPS>") + m_strImperativeS + _T("</IMPS>") + _T("\n"));
	FileOut.WriteString(_T("<IMPP>") + m_strImperativeP + _T("</IMPP>") + _T("\n"));
	FileOut.WriteString(_T("<PART>") + m_strParticiple + _T("</PART>") + _T("\n"));
}

CVerb::CVerb()
{
	m_pPresentActive = new CVerbForm;
	m_pPresentMiddle = new CVerbForm;
	m_pPresentActiveOpt = new CVerbForm;
	m_pPresentMiddleOpt = new CVerbForm;
	m_pImperfectActive = new CVerbForm;
	m_pImperfectMiddle = new CVerbForm;
	m_pAoristActive = new CVerbForm;
	m_pAoristMiddle = new CVerbForm;
	m_pFutureActive = new CVerbForm;
	m_pFutureMiddle = new CVerbForm;
	m_strPartOfSpeech = _T("verb");
}

CVerb::~CVerb()	{
	delete m_pPresentActive;
	delete m_pPresentMiddle;
	delete m_pPresentActiveOpt;
	delete m_pPresentMiddleOpt;
	delete m_pImperfectActive;
	delete m_pImperfectMiddle;
	delete m_pAoristActive;
	delete m_pAoristMiddle;
	delete m_pFutureActive;
	delete m_pFutureMiddle;
}

const void CVerb::Write(CStdioFile &FileOut)	{
	WriteHead(FileOut);
	if (!m_pPresentActive->IsEmpty())	{
		FileOut.WriteString(_T("<PRESACT>\n"));
		m_pPresentActive->Write(FileOut);
		FileOut.WriteString(_T("</PRESACT>\n"));
	}
	if (!m_pPresentMiddle->IsEmpty())	{
		FileOut.WriteString(_T("<PRESMID>\n"));
		m_pPresentMiddle->Write(FileOut);
		FileOut.WriteString(_T("</PRESMID>\n"));
	}
	if (!m_pPresentActiveOpt->IsEmpty())	{
		FileOut.WriteString(_T("<PRESACTOPT>\n"));
		m_pPresentActiveOpt->Write(FileOut);
		FileOut.WriteString(_T("</PRESACTOPT>\n"));
	}
	if (!m_pPresentMiddleOpt->IsEmpty())	{
		FileOut.WriteString(_T("<PRESMIDOPT>\n"));
		m_pPresentMiddleOpt->Write(FileOut);
		FileOut.WriteString(_T("</PRESMIDOPT>\n"));
	}
	if (!m_pImperfectActive->IsEmpty())	{
		FileOut.WriteString(_T("<IMPACT>\n"));
		m_pImperfectActive->Write(FileOut);

		FileOut.WriteString(_T("</IMPACT>\n"));
	}
	if (!m_pImperfectMiddle->IsEmpty())	{
		FileOut.WriteString(_T("<IMPMID>\n"));
		m_pImperfectMiddle->Write(FileOut);
		FileOut.WriteString(_T("</IMPMID>\n"));
	}
	if (!m_pAoristActive->IsEmpty())	{
		FileOut.WriteString(_T("<AORACT>\n"));
		m_pAoristActive->Write(FileOut);
		FileOut.WriteString(_T("</AORACT>\n"));
	}
	if (!m_pAoristMiddle->IsEmpty())	{
		FileOut.WriteString(_T("<AORMID>\n"));
		m_pAoristMiddle->Write(FileOut);
		FileOut.WriteString(_T("</AORMID>\n"));
	}
	if (!m_pFutureActive->IsEmpty())	{
		FileOut.WriteString(_T("<FUTACT>\n"));
		m_pFutureActive->Write(FileOut);
		FileOut.WriteString(_T("</FUTACT>\n"));
	}
	if (!m_pFutureMiddle->IsEmpty())	{
		FileOut.WriteString(_T("<FUTMID>\n"));
		m_pFutureMiddle->Write(FileOut);
		FileOut.WriteString(_T("</FUTMID>\n"));
	}
	WriteTail(FileOut);
}

const bool CVerb::IsEmpty()	{
	bool bRetVal;
	bRetVal = !(m_pPresentActive->HasEssentials() || m_pPresentMiddle->HasEssentials()
		|| m_pPresentActiveOpt->HasEssentials() || m_pPresentMiddleOpt->HasEssentials()
		|| m_pImperfectActive->HasEssentials() || m_pImperfectMiddle->HasEssentials()
		|| m_pAoristActive->HasEssentials() || m_pAoristMiddle->HasEssentials()
		|| m_pFutureActive->HasEssentials() || m_pFutureMiddle);
	return bRetVal;
}

bool CVerb::Read(CStdioFile &fileIn)	{
	CString strNext;
	CString strTag;
	//bool bFound = fileIn.ReadString(m_strDescription);
	strNext = _T("");
	bool bFound = true;
	bFound = fileIn.ReadString(strNext);
	while (bFound && (strNext != "/verb"))	{
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
		else if (strTag == _T("PRESACT"))	{
			bFound = m_pPresentActive->Read(fileIn, _T("</PRESACT>"));
		}
		else if (strTag == _T("PRESMID"))	{
			bFound = m_pPresentMiddle->Read(fileIn, _T("</PRESMID>"));
		}
		else if (strTag == _T("PRESACTOPT"))	{
			bFound = m_pPresentActiveOpt->Read(fileIn, _T("</PRESACTOPT>"));
		}
		else if (strTag == _T("PRESMIDOPT"))	{
			bFound = m_pPresentMiddleOpt->Read(fileIn, _T("</PRESMIDOPT>"));
		}
		else if (strTag == _T("IMPACT"))	{
			bFound = m_pImperfectActive->Read(fileIn, _T("</IMPACT>"));
		}
		else if (strTag == _T("IMPMID"))	{
			bFound = m_pImperfectMiddle->Read(fileIn, _T("</IMPMID>"));
		}
		else if (strTag == _T("AORACT"))	{
			bFound = m_pAoristActive->Read(fileIn, _T("</AORACT>"));
		}
		else if (strTag == _T("AORMID"))	{
			bFound = m_pAoristMiddle->Read(fileIn, _T("</AORMID>"));
		}
		else if (strTag == _T("FUTACT"))	{
			bFound = m_pFutureActive->Read(fileIn, _T("</FUTACT>"));
		}
		else if (strTag == _T("FUTMID"))	{
			bFound = m_pFutureMiddle->Read(fileIn, _T("</FUTMID>"));
		}
		bFound = fileIn.ReadString(strNext);
	}
	return bFound;
}