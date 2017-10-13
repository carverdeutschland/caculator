/******************************************************************************
**
** FILE NAME
**    sp_caculator_expvalue.h
**
** DESCRIPTION
**    
**
** History:
**  $Author:    caoqy
**  $Revision:  $
**  $Date:      2017.3.10
**  $Header:    $
*******************************************************************************/
#ifndef _SP_CACULATOR_EXPVALUE_H_
#define _SP_CACULATOR_EXPVALUE_H_

#include <string>
#include <stdint.h>

#include "sp_caculator_errorcode.h"

using namespace std;

// 定义操作符
#define   OP_ASN        '='
#define   OP_AND  		'~'     // &&
#define   OP_OR         '?'     // ||
#define   OP_GT         '>'
#define   OP_LT         '<'
#define   OP_EQ         '\\'    // ==
#define   OP_GTE        '`'     // >=
#define   OP_LTE        '%'     // <=
#define   OP_HAS        '@'
#define   OP_ADD        '+'
#define   OP_SUB        '-'
#define   OP_MUL        '*'
#define   OP_DIV        '/'
#define   OP_BITAND     '&'
#define   OP_BITOR      '|'
#define   OP_NE         '\17'

enum valueType
{
	value_bool,
	value_int32,
	value_int64,
	value_double,
	value_str,
	value_invalid = 255
};

class expValue
{
protected:
	valueType   nType;  
	union
	{
		bool        bValue;
		int32_t     nValue32;
		int64_t     nValue64;
		double      dValue;
	};
	string      strValue;

public:
	bool  isNumber()
	{
		return (nType == value_int32 || nType == value_int64 || nType == value_double );
	};
	
	ErrorCode negotiateType( expValue& v);
	ErrorCode switch2type( valueType type);
public:
	ErrorCode valueAdd( expValue& v);     // +
	ErrorCode valueSub( expValue& v);     // -
	ErrorCode valueMul( expValue& v);     // *
	ErrorCode valueDiv( expValue& v);     // 
	ErrorCode valueBitor( expValue& v);   // |
	ErrorCode valueBitand( expValue& v);  // &
	ErrorCode valueStrHas( expValue& v);  // has
	ErrorCode valueEqual( expValue& v);   // ==
	ErrorCode valueGt( expValue& v);      // >
	ErrorCode valueLt( expValue& v);      // <
	ErrorCode valueGte( expValue& v);     // >=
	ErrorCode valueLte( expValue& v);     // <=
	ErrorCode valueOr( expValue& v);      // ||
	ErrorCode valueAnd( expValue& v);     // &&
	ErrorCode valueNe( expValue& v);      // !=

	ErrorCode valueExec(char oper, expValue& v);
public:
	expValue()
	{
		nType = value_invalid;
	};
	expValue(const string& str){ setValue(str);};
	expValue(const char* szstr)
	{ 
		string str = szstr;
		setValue(str);
	};
	expValue(const expValue& o);
	expValue(const bool& o){ setValue(o);};
	expValue(const int32_t& o){ setValue(o);};
	expValue(const int64_t& o){ setValue(o);};
	expValue(const double& o) { setValue(o);};

	void setValue(const string& str);
	void setValue(const char* szstr)
	{ 
		string str = szstr;
		setValue(str);
	};
	void setValue(const bool& o);
	void setValue(const int32_t& o);
	void setValue(const int64_t& o);
	void setValue(const double& o);

	string getValueStr(ErrorCode& e)
	{
		( value_str == nType ) ? ( e = NGB_SUCCESS ):( e = NGB_EXPRESS_TYPE_NOTMATCH);
		return strValue;
	};

	bool getValueBool(ErrorCode& e)
	{
		( value_bool == nType ) ? ( e = NGB_SUCCESS ):( e = NGB_EXPRESS_TYPE_NOTMATCH);
		return bValue;
	};

	int32_t getValueInt32(ErrorCode& e)
	{
		( value_int32 == nType ) ? ( e = NGB_SUCCESS ):( e = NGB_EXPRESS_TYPE_NOTMATCH);
		return nValue32;
	};

	int64_t getValueInt64(ErrorCode& e)
	{
		( value_int64 == nType ) ? ( e = NGB_SUCCESS ):( e = NGB_EXPRESS_TYPE_NOTMATCH);
		return nValue64;
	};

	double getValueDouble(ErrorCode& e)
	{
		( value_double == nType ) ? ( e = NGB_SUCCESS ):( e = NGB_EXPRESS_TYPE_NOTMATCH);
		return dValue;
	};
	valueType getType(){ return nType;};
	virtual ~expValue(){};

	friend class expNode;
};


#endif

