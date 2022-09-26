#include "stdafx.h"
#include "..\include\CmdLineParser.h"

using namespace BwProj::toolkit;

CCmdLineParser::CCmdLineParser(LPCTSTR sCmdLine, bool bCaseSensitive)
: m_bCaseSensitive(bCaseSensitive)
{
	if(sCmdLine) 
	{
		Parse(sCmdLine);
	}
	else
	{
		m_sCmdLine = _T("");
	}
}

CCmdLineParser::~CCmdLineParser()
{
	m_ValsMap.clear();
}

bool CCmdLineParser::Parse(LPCTSTR sCmdLine)
{
	if(!sCmdLine) return false;

	m_sCmdLine = sCmdLine;
	m_ValsMap.clear();
	const CCmdLineParser_String sEmpty;

	//int nArgs = 0;
	LPCTSTR sCurrent = sCmdLine;
	while(true) {
		// /Key:"arg"
		if(_tcslen(sCurrent) == 0) { break; } // No data left
		LPCTSTR sArg = _tcspbrk(sCurrent, m_sDelimeters);
		if(!sArg) break; // No delimeters found
		sArg =  _tcsinc(sArg);
		// Key:"arg"
		if(_tcslen(sArg) == 0) break; // String ends with delimeter
		LPCTSTR sVal = _tcspbrk(sArg, m_sValueSep);
		if(sVal == NULL) { //Key ends command line
			CCmdLineParser_String csKey(sArg);
			if(!m_bCaseSensitive) {
				csKey.MakeLower();
			}
			m_ValsMap.insert(CValsMap::value_type(csKey, sEmpty));
			break;
		} else if(sVal[0] == _T(' ') || _tcslen(sVal) == 1 ) { // Key with no value or cmdline ends with /Key:
			CCmdLineParser_String csKey(sArg, (int)(sVal - sArg));
			if(!csKey.IsEmpty()) { // Prevent /: case
				if(!m_bCaseSensitive) {
					csKey.MakeLower();
				}
				m_ValsMap.insert(CValsMap::value_type(csKey, sEmpty));
			}
			sCurrent = _tcsinc(sVal);
			continue;
		} else { // Key with value
			CCmdLineParser_String csKey(sArg, (int)(sVal - sArg));
			if(!m_bCaseSensitive) {
				csKey.MakeLower();
			}

			sVal = _tcsinc(sVal);
			// "arg"
			LPCTSTR sQuote = _tcspbrk(sVal, m_sQuotes), sEndQuote(NULL);
			if(sQuote == sVal) { // Quoted String
				sQuote = _tcsinc(sVal);
				sEndQuote = _tcspbrk(sQuote, m_sQuotes);
			} else {
				sQuote = sVal;
				sEndQuote = _tcschr(sQuote, _T(' '));
			}

			if(sEndQuote == NULL) { // No end quotes or terminating space, take rest of string
				CCmdLineParser_String csVal(sQuote);
				if(!csKey.IsEmpty()) { // Prevent /:val case
					m_ValsMap.insert(CValsMap::value_type(csKey, csVal));
				}
				break;
			} else { // End quote or space present
				if(!csKey.IsEmpty()) {	// Prevent /:"val" case
					CCmdLineParser_String csVal(sQuote, (int)(sEndQuote - sQuote));
					m_ValsMap.insert(CValsMap::value_type(csKey, csVal));
				}
				sCurrent = _tcsinc(sEndQuote);
				continue;
			}
		}

	}

	return m_ValsMap.size() > 0;
	//return (nArgs > 0);
}

LPCTSTR CCmdLineParser::getCmdLine() const 
{ 
	return m_sCmdLine; 
}

void CCmdLineParser::setCaseSensitive(bool bSensitive) 
{ 
	m_bCaseSensitive = bSensitive; 
}
bool CCmdLineParser::getCaseSensitive() const { return m_bCaseSensitive; }
const CCmdLineParser::CValsMap& CCmdLineParser::getVals() const{ return m_ValsMap; }
// Start iterating through keys and values
CCmdLineParser::POSITION CCmdLineParser::getFirst() const{return m_ValsMap.begin();}
// Get next key-value pair, returns empty sKey if end reached
CCmdLineParser::POSITION CCmdLineParser::getNext(POSITION& pos, CCmdLineParser_String& sKey, CCmdLineParser_String& sValue) const
{
	if(isLast(pos)) {
		sKey.Empty();
		return pos;
	} else {
		sKey = pos->first;
		sValue = pos->second;
		pos ++;
		return pos;
	}
}

// TRUE if "Key" present in command line
bool CCmdLineParser::HasKey(LPCTSTR sKey) const
{
	CValsMap::const_iterator it = findKey(sKey);
	if(it == m_ValsMap.end()) return false;
	return true;
}
// Is "key" present in command line and have some value
bool CCmdLineParser::HasVal(LPCTSTR sKey) const
{
	CValsMap::const_iterator it = findKey(sKey);
	if(it == m_ValsMap.end()) return false;
	if(it->second.IsEmpty()) return false;
	return true;
}
// Returns value if value was found or NULL otherwise
LPCTSTR CCmdLineParser::GetVal(LPCTSTR sKey) const
{
	CValsMap::const_iterator it = findKey(sKey);
	if(it == m_ValsMap.end()) return _T("");
	return LPCTSTR(it->second);
}
// Returns true if value was found
bool CCmdLineParser::GetVal(LPCTSTR sKey, CCmdLineParser_String& sValue) const
{
	CValsMap::const_iterator it = findKey(sKey);
	if(it == m_ValsMap.end()) return false;
	sValue = it->second;
	return true;
}

bool CCmdLineParser::isLast(POSITION& pos) const
{
	return (pos == m_ValsMap.end());
}