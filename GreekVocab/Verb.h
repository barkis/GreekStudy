#pragma once
#include<iosfwd>
#include<map>
#include <memory>
#include "GrammarExample.h"

class CVerbForm 	{
public:
	CString m_strInfinitive;
	CString m_strPers1S, m_strPers2S, m_strPers3S;
	CString m_strPers1P, m_strPers2P, m_strPers3P;
	CString m_strImperativeS, m_strImperativeP;
	CString m_strParticiple;
	bool Read(CStdioFile &fileIn,CString strCloseTag);
	const virtual void Write(CStdioFile &FileOut);
	const bool HasEssentials();
	const bool IsEmpty();
};

class CVerb : public CGrammarExample	{
	friend bool operator==(const CString &str, const CVerb &verb){ return str == verb.m_strDescription; }
	friend bool operator==(const CVerb &verb, const CString &str){ return str == verb; }
public:
	CVerb();
	virtual ~CVerb();
	CVerbForm *m_pPresentActive, *m_pPresentMiddle;
	CVerbForm *m_pPresentActiveOpt, *m_pPresentMiddleOpt;
	CVerbForm *m_pPresentActiveSubj, *m_pPresentMiddleSubj;
	CVerbForm *m_pImperfectActive, *m_pImperfectMiddle;
	CVerbForm *m_pAoristActive, *m_pAoristMiddle;
	CVerbForm * m_pFutureActive, *m_pFutureMiddle;
	bool Read(CStdioFile &fileIn);
	const virtual void Write(CStdioFile &FileOut);
	friend CStdioFile &operator<<(CStdioFile &FileOut, CVerb &decl){ decl.Write(FileOut); return FileOut; }
	const bool IsEmpty();
};
