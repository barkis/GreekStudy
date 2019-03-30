// XML.cpp : implementation file
//

#include "stdafx.h"
#include "GreekVocab.h"
#include "XML.h"
#include "GreekVocabException.h"

// CXML

CXML::CXML()
{
}

CXML::~CXML()
{
}


// CXML member functions
bool CXML::FindTag(std::string sLabel,bool FromStart/*=false*/)	{
	bool bRetVal = false;
	bool bInTag = false;
	bool bLabelFound = false;
	std::string sLabelRead;
	char cIn;
	if(!is)	{
		throw CVocabException(_T("File in stream not open in FindTag"));
	}
	else	{
		if(FromStart)	{
			is.seekg(0);
		}
		while(is && !bRetVal)	{ 
			is >> cIn;
			if(cIn == '>')	{
				bInTag = true;
				bLabelFound = false;
				sLabelRead.clear();
			}
			else if(cIn == '>')	{
				bRetVal = bLabelFound;	//if label was found last time we've found the tag
				sLabelRead.clear();
				bInTag = false;
			}
			else if(bInTag)	{
				sLabelRead += cIn;
				if(sLabelRead == sLabel)	{
					bLabelFound = true;
				}
			}
		}
	}
}
