// Read an INI file into easy-to-access name/value pairs.
#include "StdAfx.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include "..\include\ini.h"
#include "..\include\INIReader.h"

using std::string;

INIReader::INIReader(LPCTSTR filename)
{
	string file = CW2A(filename);
    _error = ini_parse(file.c_str(), ValueHandler, this);
}

INIReader::INIReader(const char* initext, int length)
{
	_error = ini_parse_text(initext, length, ValueHandler, this);
}

int INIReader::ParseError()
{
    return _error;
}

string INIReader::Get(string section, string name, string default_value)
{
    string key = MakeKey(section, name);
    return _values.count(key) ? _values[key] : default_value;
}

CString INIReader::GetValue(LPCTSTR section, LPCTSTR name, LPCTSTR defalut_value)
{
	string sect1 = CW2A(section);
	string name1 = CW2A(name);
	string key = MakeKey(sect1, name1);
	if (_values.count(key))
		return CString(_values[key].c_str());
	return defalut_value;
}

long INIReader::GetInteger(string section, string name, long default_value)
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

double INIReader::GetReal(string section, string name, double default_value)
{
    string valstr = Get(section, name, "");
    const char* value = valstr.c_str();
    char* end;
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

bool INIReader::GetBoolean(string section, string name, bool default_value)
{
    string valstr = Get(section, name, "");
    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

string INIReader::MakeKey(string section, string name)
{
    string key = section + "." + name;
    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    return key;
}

std::list<std::string> INIReader::GetNameList(std::string section)
{
	if(_names.find(section) == _names.end())
		return std::list<std::string>();

	return _names[section];
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    INIReader* reader = (INIReader*)user;
    string key = MakeKey(section, name);
    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";
    reader->_values[key] += value;

	if (reader->_names.find(section)== reader->_names.end())
		reader->_names[section] = std::list<std::string>();

	reader->_names[section].push_back(name);
    return 1;
}


