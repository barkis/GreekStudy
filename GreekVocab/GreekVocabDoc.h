
// GreekVocabDoc.h : interface of the CGreekVocabDoc class
//


#pragma once
#include <map>
#include <vector>
#include <fstream>
#include "shifts.h"
#include "Declension.h"
#include "Verb.h"

typedef std::map<CString,std::vector<CString>> StringVectorMap;

class CGreekVocabDoc : public CDocument
{
protected: // create from serialization only
	CGreekVocabDoc();
	DECLARE_DYNCREATE(CGreekVocabDoc)
	static const TCHAR* PATH;
	std::basic_string<TCHAR> m_sPath;
 // Attributes
public:
	void LoadVocabLists(){LoadLists(_T("*.gkv"),&CGreekVocabDoc::ReadVocabList);}
	void LoadSyntaxLists(){LoadLists(_T("*.stx"),&CGreekVocabDoc::ReadSyntaxList);}
	bool IsNounName(const CString &theName); 
	bool  IsAdjectiveName(const CString &theName);
	bool  IsVerbName(const CString &theName);
	void AddNoun(CNoun *pNoun, CString &FileName);
	void AddAdjective(CAdjective *pAdjective, CString &FileName);
	void AddVerb(CVerb *pVerb, CString &FileName);
	const bool GetChar(Shifts shift, const int charIn, int &codeFound);
	void WriteAllSyntax(CString &FileName);

	const std::vector<CString> GetVocabFileNames();
	std::vector<CString> *GetVocabFileList(CString strKey);
	const StringVectorMap *GetVocabListsMap();

	const std::vector<CString> CGreekVocabDoc::GetNounNames();
	const std::vector<CString> CGreekVocabDoc::GetVerbNames();
	std::basic_string<TCHAR> getPath(){ return m_sPath; }
//	const CDeclension &GetDeclension(CString &Description);
	CNoun *GetNoun(CString &Description);
	CVerb *GetVerb(CString &Description);
	CAdjective &GetAdjective(CString &Description);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	std::map<CString,CNoun*>m_NounsMap;
	std::multimap<CString,CGrammarExample*>m_FileGrammarMap;
	std::multimap<CString, CVerb*>m_FileVerbMap;
	std::map<CString, CAdjective*>m_AdjectivesMap;
	std::map<CString,CVerb*>m_VerbsMap;


	virtual void ReadVocabList(CString strFileName);
	virtual void ReadSyntaxList(CString strFileName);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGreekVocabDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	std::ifstream m_CharStreamEn, m_CharStreamEl;
	std::string m_FileNameEn,m_FileNameEl,m_FileNameUEn,m_FileNameUEl, m_FileNameAltEn, m_FileNameAltEl;
	std::string m_FileNameSmoothEn,m_FileNameSmoothEl,m_FileNameIotaEn,m_FileNameIotaEl,m_FileNameAltSmoothEn,
	m_FileNameAltSmoothEl,m_FileNameIotaSmoothEn,m_FileNameIotaSmoothEl;
	
	//maps of English WM_CHARs against Greek
	std::map<int,int> m_CharacterMap,m_AltSmoothMap;
	std::map<int,int> m_AltMap, m_SmoothMap,m_IotaMap,m_IotaSmoothMap;

	std::map<CString,std::vector<CString>> m_VocabListsMap;

	virtual void LoadCharacterMaps();
//	virtual void LoadVocabLists();
	
	typedef void (CGreekVocabDoc::*pReadMethod)(CString);
	virtual void LoadLists(CString strFileTemplate,pReadMethod Reader);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};

class StringCmp	{
private:
	CString m_strValue;
public:
	StringCmp(const CString &strValue):m_strValue(strValue){}
	bool operator()(std::pair<CString,CGrammarExample*> pair){return (pair.second->m_strFileName == m_strValue);}
};

