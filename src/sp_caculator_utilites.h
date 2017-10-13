/******************************************************************************
**
** FILE NAME
**    sp_caculator_utilites.h
**
** DESCRIPTION
**    
**
** History:
**  $Author:    caoqy
**  $Revision:  $
**  $Date:      2017.3.9
**  $Header:    $
*******************************************************************************/
#ifndef _SP_CACULATOR_UTILITIES_H_
#define _SP_CACULATOR_UTILITIES_H_

#include <stdint.h>
#include <string>
#include <string.h>
#include <vector>
#include "sp_caculator_errorcode.h"

using namespace std;

class calcStr
{

public:
	calcStr(){};
	virtual ~calcStr(){};

	static ErrorCode formatExpress(const string& strExp, string& strExpNew);
	static bool startWith(const char* szSource, const char* szCompare);
	static bool quotedByBrace(const char* szSource);
	static bool isExpString(const char* szSource);
	static bool split( string strSource, char splitor, vector<string> &target);
	static bool isNumber(const string& strExp, int64_t &num);
	static bool isVar(const string& strExp);
	static bool isDouble(const string& strExp, double & d);
	static ErrorCode findLastOperation(const string& strExp, int& nPos);
	static bool isOneOf(char c, const string& strOper);
	static ErrorCode findLastGivenOper(const string& strExp, const string& strOper, int& nPos);
};

#endif


