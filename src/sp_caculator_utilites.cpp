/******************************************************************************
**
** FILE NAME
**    sp_caculator_utilities.cpp
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

#include <stdlib.h>

#include "sp_caculator_utilites.h"
#include "sp_caculator.h"
#include <iostream>

const int maxpriority = 6;
static string operlist[6] = {"=","~?","><\\`%@\17","+-","*/", "&|"};

ErrorCode calcStr::formatExpress(const string& strExp, string& strExpNew)
{
	unsigned int i;
	int iState = 0;     // 0:Normal
						// 1:in string, 
						// 2:&
						// 3:|
						// 4:=
						// 5:>
						// 6:<
	char c;
	string   strPre = "  &|=><";
	strExpNew = "";

	for(i = 0; i < strExp.size(); i++)
	{
		c = strExp[i];
		switch (iState){
		case 0:
			if( c == ' ' ) continue; 
			if( c == '\"' ) {  // enter string
				iState = 1;
				break;
			}
			if( c == '&' ) {  // begin and
				iState = 2;
				continue;
			}
			if( c == '|' ) {  // begin or 
				iState = 3;
				continue;
			}
			if( c == '=' ) {  // begin  equal
				iState = 4;
				continue;
			}
			if( c == '>' ) {  // begin great than
				iState = 5;
				continue;
			}
			if( c == '<' ) {  // begin less than
				iState = 6;
				continue;
			}
			if( c == '!' ) {  // begin not equal
				iState = 7;
				continue;
			}
			break;
		case 1:  // enter string
			if( c == '\"' ) {
				iState = 0;
			}
			strExpNew += c;
			continue;
		case 2:  // and
			if( c == '&' ) {
				strExpNew += OP_AND;
				iState = 0;
				continue;
			}
			else
			{
				strExpNew += strPre[iState];
				iState = 0;
				break;
			}
		case 3:  // or
			if( c == '|' ) {
				strExpNew += OP_OR;
				iState = 0;
				continue;
			}
			else
			{
				strExpNew += strPre[iState];
				iState = 0;
				break;
			}
		case 4:  // equal
			if( c == '=' ) {
				strExpNew += OP_EQ;
				iState = 0;
				continue;
			}
			else
			{
				strExpNew += strPre[iState];
				iState = 0;
				break;
			}
		case 5:  // great than
			if( c == '=' ) {
				strExpNew += OP_GTE;
				iState = 0;
				continue;
			}
			else
			{
				strExpNew += strPre[iState];
				iState = 0;
				break;
			}
		case 6:  // less than
			if( c == '=' ) {
				strExpNew += OP_LTE;
				iState = 0;
				continue;
			}
			else
			{
				strExpNew += strPre[iState];
				iState = 0;
				break;
			}
		case 7:  // less than
			if( c == '=' ) {
				strExpNew += OP_NE;
				iState = 0;
				continue;
			}
			else
			{
				strExpNew += strPre[iState];
				iState = 0;
				break;
			}
		default:
			continue;
		}

		if( c != ' '){
			strExpNew += c;
		}
	}

	if( iState == 0 )
	{
		return NGB_SUCCESS;
	}
	else if( iState == 1 )
	{
		return NGB_EXPRESS_QUOTE_NMATCH;
	}
	else
		return NGB_EXPRESS_INVALID_EXP;
}


// to check weather a string start with another string
bool calcStr::startWith(const char* szSource, const char* szCompare)
{
	if( strlen(szSource) < strlen(szCompare) )
	{
		return false;
	}

	unsigned int i = 0;
	for( ; i < strlen(szCompare) ; i++ )
	{
		if( *(szSource + i) != *(szCompare + i) )
		{
			return false;
		}
	}

	return true;
}

bool calcStr::split( string strSource, char splitor, vector<string> &target)
{
	if( strSource.length() == 0 || splitor == '\0' )
	{
		return false;
	}

	unsigned int i, nStart = 0;

	for( i = 0; i < strSource.length(); i++ )
	{
		if( strSource[i] == splitor )
		{
			target.push_back( strSource.substr(nStart, i - nStart ) );
			nStart = i + 1;
		}
	}

	target.push_back( strSource.substr(nStart, i - nStart ) );

	return true;

}

bool calcStr::quotedByBrace(const char* szSource)
{
	if ( NULL == szSource ) return false;

	int nLen = strlen(szSource);

	if( nLen <= 2 ) return false;
	if( szSource[0] != '(' || szSource[nLen - 1] != ')' ) 
	{
		return false;
	}
	//////////////////////////////////////////////////////
	int i = 0;
	int braceBalance = 0;
	bool inString = false;

	for( i = nLen - 1; i > 0; i-- )
	{
		char c = szSource[i];
		
		// 看看是否在字符串中
		if( '\"' == c )
		{
			inString = !inString;
		}
		if( inString ) continue;

		// 执行到这里表示在括号中，那么直到括号平衡为止
		if( '(' == c )
		{
			braceBalance --;
		}
		else if( ')' == c )
		{
			braceBalance ++;
		}
		if( 0 == braceBalance ) return false;
	}
	if( 1 == braceBalance ){
		return true;
	}
	else {
		return false;
	}
}

bool calcStr::isExpString(const char* szSource)
{
	if ( NULL == szSource ) return false;

	if( strlen(szSource) > 2 )
	{
		if( szSource[0] == '\"' && szSource[strlen(szSource) - 1] == '\"' ) {
			unsigned int i;
			for( i = 1; i < strlen(szSource) - 1; ++i )
			{
				if( szSource[i] == '\"' ) return false;
			}
			return true;
		}
	}
	return false;
}


bool calcStr::isNumber(const string& strExp, int64_t &num)
{
	num = 0;
	int bitnum = 0;
	unsigned int i = 0;
	char c;
	
	// 看看是不是十六进制
	if( startWith(strExp.c_str(), "0X") || startWith(strExp.c_str(), "0x") )
	{
		for( i = 2 ; i < strExp.size(); i++ )
		{
			c = strExp[i];
			
			if( c <= 'f' && c >= 'a' ) 
			{
				bitnum = c - 'a' + 10;
			}
			else if( c <= 'F' && c >= 'A' ) 
			{
				bitnum = c - 'A' + 10;
			}
			else if( c <= '9' && c >= '0' ) 
			{
				bitnum = c - '0';
			}
			else
			{
				return false;
			}
			num = ( num << 4 ) + bitnum;
		}
		return true;
	}
	
	// 看看是不是十进制数
	for( i = 0 ; i < strExp.size(); i++ )
	{
		c = strExp[i];
		if( c >= '0' && c <= '9' ) 
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	
	num = atol(strExp.c_str());
	return true;
}

bool calcStr::isDouble(const string& strExp, double& d)
{
	int dotnum = 0;
	unsigned int i;
	char c;

	for( i = 0; i < strExp.size(); i ++ )
	{
		c = strExp[i];
		if(c >= '0' && c <= '9')  continue;

		if( c == '.' ) {
			dotnum ++;
			if( dotnum > 1) 
				return false;
			else
				continue;
		}
		else {
			return false;
		}
	}
	if( dotnum == 0 ) return false;

	d = strtod( strExp.c_str(), NULL);
	return true;
}

bool calcStr::isVar(const string& strExp)
{
	if( strExp.size() == 0 ) return false;

	if( strExp[0] != '$' ) return false;

	unsigned int i;
	char c;

	for( i = 1; i < strExp.size(); i ++ )
	{
		c = strExp[i];
		if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_') || (c == '.') || (c == '[') || (c== ']') ) 
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool calcStr::isOneOf(char c, const string& strOper)
{
	unsigned int i;
	
	for( i = 0; i < strOper.size(); ++i )
	{
		if( c == strOper[i] )
		{
			return true;
		}
	}
	return false;
}

ErrorCode calcStr::findLastGivenOper(const string& strExp, const string& strOper, int& nPos)
{
	int i = 0;
	int braceBalance = 0;
	bool inString = false;

	nPos = -1;
	for( i = strExp.size(); i >= 0; i--)
	{
		char c = strExp[i];
		
		// 看看是否在字符串中
		if( '\"' == c )
		{
			inString = !inString;
		}
		if( inString ) continue;

		if( braceBalance < 0 )
		{
			//cout << "braceBalance < 0:" << braceBalance << " pos =" << i << endl;
			return NGB_EXPRESS_BRACE_ERROR;
		}
		
		// 没有在括号中，表示可以进行判断
		if( braceBalance == 0 )
		{
			if( isOneOf(c, strOper) )
			{
				nPos = i;
				return NGB_SUCCESS;
			}
		}
		// 执行到这里表示在括号中，那么直到括号平衡为止
		if( '(' == c )
		{
			braceBalance --;
		}
		else if( ')' == c )
		{
			braceBalance ++;
		}
	}

	if( braceBalance != 0 )
	{
		//cout << "final brace not balance " << endl;
		return NGB_EXPRESS_BRACE_ERROR;
	}

	return NGB_SUCCESS;
}

// 找到最后一个优先级最低的操作符
ErrorCode calcStr::findLastOperation(const string& strExp, int& nPos)
{
	int i;
	ErrorCode nRtn;

	for( i = 0; i < maxpriority; ++i )
	{
		nRtn = findLastGivenOper(strExp, operlist[i], nPos );
		if( NGB_SUCCESS != nRtn ) return nRtn;
		if( nPos > 0 ) return NGB_SUCCESS ;
	}

	return NGB_EXPRESS_INVALID_EXP;
}



