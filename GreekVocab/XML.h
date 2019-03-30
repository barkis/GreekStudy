#pragma once

// CXML command target

#include <string>
#include <iosfwd>
class CXML : public CObject
{
public:
	CXML();
	virtual ~CXML();
	virtual bool FindTag(std::string sLabel,bool Fromtart = false);
	virtual bool FindTag(std::string sLabel,std::string sEndLabel);
	virtual bool FindPath(std::string sPath);
private:
	std::string m_sFileName;
	std::ostream &os;
	std::istream &is;
};


