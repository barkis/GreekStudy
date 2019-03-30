
// GreekVocabDoc.cpp : implementation of the CGreekVocabDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GreekVocab.h"
#endif
#include "GreekVocabException.h"
#include "GreekVocabDoc.h"
#include <Shlobj.h>
#include <propkey.h>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGreekVocabDoc

IMPLEMENT_DYNCREATE(CGreekVocabDoc, CDocument)

	BEGIN_MESSAGE_MAP(CGreekVocabDoc, CDocument)
	END_MESSAGE_MAP()



	// CGreekVocabDoc construction/destruction
	/* set filenames for character-conversion files and load the file contents into maps.
	Load the vocabulary and syntax lists.
	*/
	CGreekVocabDoc::CGreekVocabDoc():m_FileNameEn("CharMaps\\charcodes.en"),m_FileNameEl("CharMaps\\charcodes.gk"),
		m_FileNameUEn("CharMaps\\charcodesU.en"),m_FileNameUEl("CharMaps\\charcodesU.gk"),m_FileNameAltEn("CharMaps\\charcodesalt.en"),
		m_FileNameAltEl("CharMaps\\charcodesalt.gk"),m_FileNameSmoothEn("CharMaps\\charcodessmooth.en"),m_FileNameSmoothEl("CharMaps\\charcodessmooth.gk"),
		m_FileNameIotaEn("CharMaps\\charcodesiota.en"),m_FileNameIotaEl("CharMaps\\charcodesiota.gk"),m_FileNameAltSmoothEn("CharMaps\\charcodesaltsmooth.en"),
		m_FileNameAltSmoothEl("CharMaps\\charcodesaltsmooth.gk"),m_FileNameIotaSmoothEn("CharMaps\\charcodesiotasmooth.en"),m_FileNameIotaSmoothEl("CharMaps\\charcodesiotasmooth.gk")
	{
#ifdef DEBUG
		m_sPath = _T(".");
#else
		LPWSTR pszPath = nullptr;
		HRESULT h = SHGetKnownFolderPath(
			FOLDERID_ProgramData,
			0,
			NULL,
			&pszPath
		);
		if(h != S_OK)	{
			throw CVocabException(_T("SHGetKnownFolderPath failed"),h);
		}
		m_sPath = pszPath;
		CoTaskMemFree(pszPath);
		//m_pCharMaps = new std::map<short,std::map<int,int> >;
#endif
		LoadCharacterMaps();
		LoadVocabLists();
		LoadSyntaxLists();
	}

	CGreekVocabDoc::~CGreekVocabDoc()
	{
		std::map<CString, CNoun*>::iterator iterNoun;
		for (iterNoun = m_NounsMap.begin(); iterNoun != m_NounsMap.end(); ++iterNoun)	{
			delete iterNoun->second;
		}
		std::map<CString, CVerb*>::iterator iterVerb;
		for (iterVerb = m_VerbsMap.begin(); iterVerb != m_VerbsMap.end(); ++iterVerb)	{
			delete iterVerb->second;
		}
	}

	BOOL CGreekVocabDoc::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		// TODO: add reinitialization code here
		// (SDI documents will reuse this document)

		return TRUE;
	}




	// CGreekVocabDoc serialization

	void CGreekVocabDoc::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			// TODO: add storing code here
		}
		else
		{
			// TODO: add loading code here
		}
	}

#ifdef SHARED_HANDLERS

	// Support for thumbnails
	void CGreekVocabDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
	{
		// Modify this code to draw the document's data
		dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

		CString strText = _T("TODO: implement thumbnail drawing here");
		LOGFONT lf;

		CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
		pDefaultGUIFont->GetLogFont(&lf);
		lf.lfHeight = 36;

		CFont fontDraw;
		fontDraw.CreateFontIndirect(&lf);

		CFont* pOldFont = dc.SelectObject(&fontDraw);
		dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
		dc.SelectObject(pOldFont);
	}

	// Support for Search Handlers
	void CGreekVocabDoc::InitializeSearchContent()
	{
		CString strSearchContent;
		// Set search contents from document's data. 
		// The content parts should be separated by ";"

		// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
		SetSearchContent(strSearchContent);
	}

	void CGreekVocabDoc::SetSearchContent(const CString& value)
	{
		if (value.IsEmpty())
		{
			RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
		}
		else
		{
			CMFCFilterChunkValueImpl *pChunk = NULL;
			ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
			if (pChunk != NULL)
			{
				pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
				SetChunkValue(pChunk);
			}
		}
	}

#endif // SHARED_HANDLERS

	// CGreekVocabDoc diagnostics

#ifdef _DEBUG
	void CGreekVocabDoc::AssertValid() const
	{
		CDocument::AssertValid();
	}

	void CGreekVocabDoc::Dump(CDumpContext& dc) const
	{
		CDocument::Dump(dc);
	}
#endif //_DEBUG



	// CGreekVocabDoc commands
	void CGreekVocabDoc::LoadCharacterMaps()	{
		int iEn, iEl;
		m_CharStreamEn.open(m_FileNameEn);
		m_CharStreamEl.open(m_FileNameEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_CharacterMap.insert(newPair);
			}
		} 

		m_CharStreamEl.close();
		m_CharStreamEn.close();

		m_CharStreamEn.open(m_FileNameUEn);
		m_CharStreamEl.open(m_FileNameUEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open upper CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_CharacterMap.insert(newPair);
			}
		} 
		m_CharStreamEl.close();
		m_CharStreamEn.close();

		m_CharStreamEn.open(m_FileNameAltEn);
		m_CharStreamEl.open(m_FileNameAltEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open Alt CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_AltMap.insert(newPair);
			}
		} 
		m_CharStreamEl.close();
		m_CharStreamEn.close();

		m_CharStreamEn.open(m_FileNameSmoothEn);
		m_CharStreamEl.open(m_FileNameSmoothEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open smooth CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_SmoothMap.insert(newPair);
			}
		} 
		m_CharStreamEl.close();
		m_CharStreamEn.close();

		m_CharStreamEn.open(m_FileNameIotaEn);
		m_CharStreamEl.open(m_FileNameIotaEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open iota CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_IotaMap.insert(newPair);
			}
		} 
		m_CharStreamEl.close();
		m_CharStreamEn.close();

		m_CharStreamEn.open(m_FileNameAltSmoothEn);
		m_CharStreamEl.open(m_FileNameAltSmoothEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open altsmooth CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_AltSmoothMap.insert(newPair);
			}
		} 
		m_CharStreamEl.close();
		m_CharStreamEn.close();

		m_CharStreamEn.open(m_FileNameIotaSmoothEn);
		m_CharStreamEl.open(m_FileNameIotaSmoothEl);
		if(!m_CharStreamEl||!m_CharStreamEn)	{
			throw CVocabException(_T("Failed to open iotamooth CharStreams"));
		}
		while(m_CharStreamEn >> iEn)	{
			if(m_CharStreamEl >> iEl)	{
				std::pair<int,int> newPair = std::make_pair(iEn,iEl);
				m_IotaSmoothMap.insert(newPair);
			}
		} 
		m_CharStreamEl.close();
		m_CharStreamEn.close();
	}

	const bool CGreekVocabDoc::GetChar(Shifts shift,const int charIn,int &codeFound)	{
		bool bRetVal = false;
		short iShiftVal = shift();
		std::map<int,int> *theMap = NULL;
		switch(iShiftVal)	{
		case Shifts::enumShifts::NONE:
		case Shifts::enumShifts::SHIFT:
			theMap = &m_CharacterMap;
			break;
		case Shifts::enumShifts::ALT:
			theMap = &m_AltMap;
			break;
		case Shifts::enumShifts::SMOOTH:
		case Shifts::enumShifts::ROUGH:
			theMap = &m_SmoothMap;
			break;
		case Shifts::enumShifts::IOTA:
			theMap = &m_IotaMap;
			break;
		case Shifts::enumShifts::ALT|Shifts::enumShifts::SMOOTH:
		case Shifts::enumShifts::ALT|Shifts::enumShifts::ROUGH:
			theMap = &m_AltSmoothMap;
			break;
		case Shifts::enumShifts::IOTA|Shifts::enumShifts::SMOOTH:
		case Shifts::enumShifts::IOTA|Shifts::enumShifts::ROUGH:
			theMap = &m_IotaSmoothMap;
			TRACE(_T("IotASmoothMap\n"));
			break;
		default:
			throw CVocabException(_T("Invalid shift code"),iShiftVal);
		}
		std::map<int,int>::iterator charIter;
		charIter = theMap->find(charIn);
		if(charIter != theMap->end())	{
			codeFound = charIter->second;
			if(iShiftVal & Shifts::enumShifts::ROUGH)	{
				codeFound++;
			}
			else if(codeFound == 8024)	{	//1 less than cap-upsilon-with-rough
				codeFound = 8170;
			}
			TRACE(_T("Code found = %d\n"),codeFound);
			bRetVal = true;
		}

		return bRetVal;
	}
	/*
		Called by ctor
		Load vocabulary or syntax from files depending on method pointer passed
		strFileTemplate: *.gkv for vocabulary, *.stx for syntax
		Reader: pointer to CGreekVocabDoc::ReadVocabList() orCGreekVocabDoc::ReadSyntaxList()
	*/
	void CGreekVocabDoc::LoadLists(CString strFileTemplate,pReadMethod Reader)	{
		WIN32_FIND_DATA FindStruct;
		bool bFileFound = true;
		//const std::basic_string<TCHAR> sPath(PATH);
		std::basic_string<TCHAR> sFullName = m_sPath + _T("\\vocablists\\") + (LPCTSTR)strFileTemplate;

		HANDLE hFile = FindFirstFile(sFullName.c_str(),&FindStruct);
		if(hFile == INVALID_HANDLE_VALUE)	{
			int iErr = GetLastError();
			if (iErr == ERROR_FILE_NOT_FOUND || iErr == ERROR_PATH_NOT_FOUND)	{
				bFileFound = false;
			}
			else	{
				throw CVocabException(_T("FindFirstFile failed"),iErr);
			}
		}
		while(bFileFound)	{
			CString strFileName = m_sPath.c_str();
			strFileName += _T("\\vocablists\\");
			strFileName += (LPCTSTR)FindStruct.cFileName;
			(this->* Reader)(strFileName);
			if(! FindNextFile(hFile,&FindStruct))	{
				int iErr = GetLastError();
				if(iErr == ERROR_NO_MORE_FILES)	{
					bFileFound = false;
				}
				else	{
					throw CVocabException(_T("FindNextFile failed in LoadLists"),iErr);
				}

			}
		}

	}

	/*
	read all the vocabulary lines from the specified file into a vector. Push the vector into a map with the file name as key.
	strFileName: the name of the file including path.
	*/
	void CGreekVocabDoc::ReadVocabList(CString strFileName)	{
		FILE *fStream;
		CString strNextLine;
		int iTabPos = strFileName.ReverseFind('\\');
		CString strJustName = strFileName.Mid(iTabPos + 1);
		std::vector<CString> vLines;
		errno_t e = _tfopen_s(&fStream,
			strFileName,_T("r,ccs=UNICODE"));
		if (e != 0)		{
			throw CVocabException(_T("_tfopen_s failed"),e);
		}
		CStdioFile fileIn(fStream);  // open the file from this stream
		bool bFound = fileIn.ReadString(strNextLine);
		while(bFound)	{
			vLines.push_back(strNextLine);
			bFound = fileIn.ReadString(strNextLine);
		}
		if(vLines.size())	{
			std::pair<CString,std::vector<CString>> thePair = std::make_pair(strJustName,vLines);
			m_VocabListsMap.insert(thePair);
		}
		fileIn.Close();
		fclose(fStream);

	}

	/*
	Read syntax paradigm (noun/adjective  declension, verb conjugation) from a file
	strFileName: full name of the file to read from
	*/
	void CGreekVocabDoc::ReadSyntaxList(CString strFileName)	{
		FILE *fStream;
		CString strNextLine;
		int iTabPos = strFileName.ReverseFind('\\');
		CString strJustName = strFileName.Mid(iTabPos + 1);
		errno_t e = _tfopen_s(&fStream,
			strFileName,_T("r,ccs=UNICODE"));
		if (e != 0)		{
			throw CVocabException(_T("_tfopen_s failed in ReadSyntaxList"),e);
		}
		CStdioFile fileIn(fStream);  // open the file from this stream
		bool bFound = fileIn.ReadString(strNextLine);
		while(bFound)	{
			if(strNextLine == _T("noun"))	{
				CNoun *theNoun = new CNoun;
				if(bFound = theNoun->Read(fileIn))	{
					AddNoun(theNoun,strFileName);
				}
				else	{
					delete theNoun;
				}
			}
			else if (strNextLine == _T("adjective"))	{
				CAdjective *theAdjective = new CAdjective;
				if (bFound = theAdjective->Read(fileIn))	{
					AddAdjective(theAdjective, strFileName);
				}
				else	{
					delete theAdjective;
				}
			}
			else if (strNextLine == _T("verb"))	{
				CVerb *theVerb = new CVerb;
				if (bFound = theVerb->Read(fileIn))	{
					AddVerb(theVerb, strFileName);
				}
				else	{
					delete theVerb;
				}
			}
			if (bFound)	{		//check return from Read() before continuing
				bFound = fileIn.ReadString(strNextLine);
			}
		}
		fileIn.Close();
		fclose(fStream);

	}
	/*Insert pointer to noun object in map indexed by description and in map indexed by filename
	*/
	void CGreekVocabDoc::AddNoun(CNoun *pNoun, CString &FileName)	{
		pNoun->m_strFileName = FileName;
		std::pair<CString,CNoun*> pairNoun = std::make_pair(pNoun->m_strDescription,pNoun);
		m_NounsMap.insert(pairNoun);

		int iSlashPos = FileName.ReverseFind(TCHAR('\\'));
		CString strJustFileName;
		if(iSlashPos != -1)	{
			strJustFileName = FileName.Mid(iSlashPos + 1);
		}
		else		{
			strJustFileName = FileName;
		}
		std::pair<CString,CGrammarExample*> pairFileDecl = std::make_pair(strJustFileName,pNoun);
		m_FileGrammarMap.insert(pairFileDecl);
	}

	/*Insert pointer to adjective object in map indexed by description and in map indexed by filename
	*/
	void CGreekVocabDoc::AddAdjective(CAdjective *pAdjective, CString &FileName)	{
		pAdjective->m_strFileName = FileName;
		std::pair<CString, CAdjective*> pairAdjective = std::make_pair(pAdjective->m_strDescription, pAdjective);
		m_AdjectivesMap.insert(pairAdjective);

		int iSlashPos = FileName.ReverseFind(TCHAR('\\'));
		CString strJustFileName;
		if (iSlashPos != -1)	{
			strJustFileName = FileName.Mid(iSlashPos + 1);
		}
		else		{
			strJustFileName = FileName;
		}
		std::pair<CString, CGrammarExample*> pairFileDecl = std::make_pair(strJustFileName, pAdjective);
		m_FileGrammarMap.insert(pairFileDecl);
	}

	/*Insert pointer to Verb object in map indexed by description and in map indexed by filename
	*/
	void CGreekVocabDoc::AddVerb(CVerb *pVerb, CString &FileName)	{
		pVerb->m_strFileName = FileName;
		std::pair<CString, CVerb*> pairVerb = std::make_pair(pVerb->m_strDescription, pVerb);
		m_VerbsMap.insert(pairVerb);

		int iSlashPos = FileName.ReverseFind(TCHAR('\\'));
		CString strJustFileName;
		if (iSlashPos != -1)	{
			strJustFileName = FileName.Mid(iSlashPos + 1);
		}
		else		{
			strJustFileName = FileName;
		}
		std::pair<CString, CVerb*> pairFileDecl = std::make_pair(strJustFileName, pVerb);
		m_FileVerbMap.insert(pairFileDecl);
	}

	/*
	returns a vector containing the (pathless) names of the files from which the vocabulary lines were read.
	These are the keys in the map holding the vectors of vocabulary lines.
	*/
	const std::vector<CString> CGreekVocabDoc::GetVocabFileNames()	{
		std::vector<CString> vFileNames;
		//		std::map<CString,std::vector<CString>>::iterator iter;
		StringVectorMap::iterator iter;
		for(iter = m_VocabListsMap.begin();iter != m_VocabListsMap.end();++iter)	{
			CString strNextName = iter->first;
			vFileNames.push_back(strNextName);
		}
		return vFileNames;
	}

	/*
	return a pointer to the vector containing the lines which were read from the specified file
	strKey: the file name, without path
	*/
	std::vector<CString> *CGreekVocabDoc::GetVocabFileList(CString strKey)	{
		std::vector<CString> *pRetVal = nullptr;
		StringVectorMap::iterator iter;
		iter = m_VocabListsMap.find(strKey);
		if(iter != m_VocabListsMap.end())	{
			pRetVal = &(iter->second);

		}
		else		{
			throw CVocabException(strKey + _T(" not found in lists map"));
		}
		return pRetVal;
	}

	const StringVectorMap *CGreekVocabDoc::GetVocabListsMap()	{
		return &m_VocabListsMap;
	}

	/*
	returns a vector containing the declensions (name and dictionary form) from which the user can select.
	The names are the keys in the map holding the declension objects.
	*/
	const std::vector<CString> CGreekVocabDoc::GetNounNames()	{
		std::vector<CString> vDeclensionNames;
		int iGapLength = 7;

		for(auto iter = m_NounsMap.begin();iter != m_NounsMap.end();++iter)	{
			CString strNextName = iter->first;
			int iStringLength = strNextName.GetLength();
			CString strGap(_T(' '),iGapLength - iStringLength);
			CDeclension declension = *iter->second;
			vDeclensionNames.push_back(strNextName + _T("\t") + strGap + declension.m_strDictionaryForm);
		}
		return vDeclensionNames;
	}

	/*
	returns a vector containing the conjugations (name and dictionary form) from which the user can select.
	Name and dictionary form are separated by a tab and aligned using spaces
	The names are the keys in the map holding the verb objects.
	*/
	const std::vector<CString> CGreekVocabDoc::GetVerbNames()	{
		std::vector<CString> vVerbNames;
		int iGapLength = 7;

		for (auto iter = m_VerbsMap.begin(); iter != m_VerbsMap.end(); ++iter)	{
			CString strNextName = iter->first;
			int iStringLength = strNextName.GetLength();
			CString strGap(_T(' '), iGapLength - iStringLength);
			//CVerb verb = *iter->second;
			CString strDictionaryForm = iter->second->m_strDictionaryForm;
			vVerbNames.push_back(strNextName + _T("\t") + strGap + strDictionaryForm);
		}
		return vVerbNames;
	}

	/* returns a reference to the Declension object referred to by the given Description in the Nouns map
	* Description: reference to the value used to look up the declension in the nouns map
	*/ 
	CNoun *CGreekVocabDoc::GetNoun(CString &Description)	{
		std::map<CString,CNoun*>::iterator iter;
		iter = m_NounsMap.find(Description);
		if(iter == m_NounsMap.end())	{
			throw CVocabException(_T("Invalid noun description ")+Description);
		}
		else	{
			return iter->second;
		}

	}

	/* returns a pointer to the Verb object referred to by the given Description in the Verbs map
	* Description: reference to the value used to look up the verb in the verbs map
	*/
	CVerb *CGreekVocabDoc::GetVerb(CString &Description)	{
		std::map<CString, CVerb*>::iterator iter;
		iter = m_VerbsMap.find(Description);
		if (iter == m_VerbsMap.end())	{
			throw CVocabException(_T("Invalid verb description ") + Description);
		}
		else	{
			return iter->second;
		}

	}

	/* returns a reference to the Adjective object referred to by the given Description in the Adjectives map
	* Description: reference to the value used to look up the declension in the adjectives map
	*/ 
	CAdjective &CGreekVocabDoc::GetAdjective(CString &Description)	{
		std::map<CString,CAdjective*>::iterator iter;
		iter = m_AdjectivesMap.find(Description);
		if(iter == m_AdjectivesMap.end())	{
			throw CVocabException(_T("Invalid Adjective description in CGreekVocabDoc::GetAdjective")+Description);
		}
		else	{
			return *iter->second;
		}

	}

	/* returns true if string exists as a name in the nouns map
	* theName: reference to string to be searched for
	*/
	bool  CGreekVocabDoc::IsNounName(const CString &theName)	{
		bool bRetVal = true;
		if(m_NounsMap.find(theName) == m_NounsMap.end())	{
			bRetVal = false;
		}
		return bRetVal;
	}

	/* returns true if string exists as a name in the adjectives map
	* theName: reference to string to be searched for
	*/
	bool  CGreekVocabDoc::IsAdjectiveName(const CString &theName)	{
		bool bRetVal = true;
		if(m_AdjectivesMap.find(theName) == m_AdjectivesMap.end())	{
			bRetVal = false;
		}
		return bRetVal;
	}

	/* returns true if string exists as a name in the verbs map
	* theName: reference to string to be searched for
	*/
	bool  CGreekVocabDoc::IsVerbName(const CString &theName)	{
		bool bRetVal = true;
		if (m_VerbsMap.find(theName) == m_VerbsMap.end())	{
			bRetVal = false;
		}
		return bRetVal;
	}

	/* Rewrite entire syntax file after entry has been edited
	* FileName: full name of file to be opened and rewritten
	*/
	void CGreekVocabDoc::WriteAllSyntax(CString &FileName)	{
		FILE *fStream;
		errno_t iOpenRet = _tfopen_s(&fStream,
			FileName,_T("w,ccs=UTF-8"));
		if (iOpenRet != 0)		{
			throw CVocabException(_T("_tfopen_s failed in CGreekVocabDoc::WriteAllSyntax"),iOpenRet);
		}
		CStdioFile fileOut(fStream);  // open the file from this stream
		//find all pairs where the filename is the same
		//StringCmp() is inline in GreekVocabDoc.h
		auto iter = std::find_if(m_NounsMap.begin(),m_NounsMap.end(),StringCmp(FileName));
		while(iter != m_NounsMap.end())	{
			//iter++->second.Write(fileOut);
			fileOut << *iter++->second;
			iter = std::find_if(iter,m_NounsMap.end(),StringCmp(FileName));
		}	
		auto itera = std::find_if(m_AdjectivesMap.begin(), m_AdjectivesMap.end(), StringCmp(FileName));
		while (itera != m_AdjectivesMap.end())	{
			//iter++->second.Write(fileOut);
			fileOut << *itera++->second;
			itera = std::find_if(itera, m_AdjectivesMap.end(), StringCmp(FileName));
		}
		auto iterv = std::find_if(m_VerbsMap.begin(), m_VerbsMap.end(), StringCmp(FileName));
		while (iterv != m_VerbsMap.end())	{
			//iter++->second.Write(fileOut);
			fileOut << *iterv++->second;
			iterv = std::find_if(iterv, m_VerbsMap.end(), StringCmp(FileName));
		}
		fileOut.Close();
		fclose(fStream);		
	}