#include<exception>
#include <string>

class CVocabException: public std::exception	{
public:
	CVocabException(CString theText, int theCode = 9999):text(theText),code(theCode){}
	const CString  __CLR_OR_THIS_CALL tcwhat() const;
	virtual const char * __CLR_OR_THIS_CALL what() const;
	std::wstring sFullText;
 private:
	CString text;
	int code;
};