#pragma once

#ifndef TCONVERT_INCLUDED
#define TCONVERT_INCLUDED

#include "tcommon.h"
class TFilePath;

//
// Nota: il file tconvert.cpp esiste gia' in rop.
// l'implementazione di queste funzioni si trova in tstring.cpp
//

#undef DVAPI
#ifdef TNZCORE_EXPORTS //TNZCORE_DLL
#define DVAPI DV_EXPORT_API
#else
#define DVAPI DV_IMPORT_API
#endif

bool isInt(std::string s);
bool isDouble(std::string s);

std::string toString(int v);
std::string toString(unsigned long v);
std::string toString(unsigned long long v);
std::string toString(double v, int prec = -1);
std::string toString(std::wstring s);
//std::string toString(const TFilePath &fp);
std::string toString(void *p);

int toInt(std::string s);
double toDouble(std::string s);

bool isInt(std::wstring s);
bool isDouble(std::wstring s);

std::wstring toWideString(std::string s);
std::wstring toWideString(int v);
std::wstring toWideString(double v, int prec = -1);

int toInt(std::wstring s);
double toDouble(std::wstring s);

inline bool fromStr(int &v, std::string s)
{
	if (isInt(s)) {
		v = toInt(s);
		return true;
	} else
		return false;
}

inline bool fromStr(double &v, std::string s)
{
	if (isDouble(s)) {
		v = toDouble(s);
		return true;
	} else
		return false;
}

inline bool fromStr(std::string &out, std::string s)
{
	out = s;
	return true;
}

std::string toUpper(std::string a);
std::string toLower(std::string a);

std::wstring toUpper(std::wstring a);
std::wstring toLower(std::wstring a);

#ifndef TNZCORE_LIGHT
#include <QString>

inline bool fromStr(int &v, QString s)
{
	bool ret;
	v = s.toInt(&ret);
	return ret;
}

inline bool fromStr(double &v, QString s)
{
	bool ret;
	v = s.toDouble(&ret);
	return ret;
}

#endif

#endif
