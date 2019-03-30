#pragma once
#include<iosfwd>
#include<map>
#include "GrammarExample.h"

class CDeclension :	public CGrammarExample
{
	friend bool operator==(const CString &str, const CDeclension &decl){return str == decl.m_strDescription;}
	friend bool operator==(const CDeclension &decl,const CString &str){return str == decl;}
public:
	CString m_strArticle;
	CString m_strNominativeS, m_strAccusativeS, m_strGenitiveS, m_strDativeS;
	CString m_strNominativeP, m_strAccusativeP, m_strGenitiveP, m_strDativeP;
	CString m_strVocative;
	
	bool Read(CStdioFile &fileIn, CString strCloseTag);
	const virtual void Write( CStdioFile &FileOut);
//	friend CStdioFile &operator<<(CStdioFile &FileOut,CDeclension &decl){decl.Write(FileOut); return FileOut;}
	CDeclension(void){m_strPartOfSpeech=_T("declension");}
	virtual ~CDeclension(void);

};

class CNoun: public CDeclension		{
public:
	CNoun(){m_strPartOfSpeech=_T("noun");}

	bool Read(CStdioFile &FileIn);
	const virtual void Write(CStdioFile &FileOut);
};

class CAdjective: public CGrammarExample	{
public:
	CAdjective(){m_strPartOfSpeech=_T("adjective");}
	CDeclension m_MascDecl;
	CDeclension m_FemDecl;
	CDeclension m_NeutDecl;
	const virtual CStdioFile &operator<<(CStdioFile &FileOut){Write(FileOut); return FileOut;}

	bool Read(CStdioFile &fileIn);
	const virtual void Write( CStdioFile &FileOut);
};