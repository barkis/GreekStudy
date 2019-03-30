#pragma once

class CGrammarExample
{
private:
public:
	CString m_strDictionaryForm;
	CString m_strDescription;
	CString m_strFileName;
	CString m_strTranslation;
	virtual ~CGrammarExample(){};

protected:
	CString m_strPartOfSpeech;
	const void WriteHead(CStdioFile &FileOut);
	const void WriteTail(CStdioFile &pFileOut);
	const virtual void Write(CStdioFile &pFileOut) = 0;
	friend CStdioFile &operator<<(CStdioFile &FileOut, CGrammarExample &gramex){ gramex.Write(FileOut); return FileOut; }
};
