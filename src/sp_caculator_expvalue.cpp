/******************************************************************************
**
** FILE NAME
**    sp_caculator_expvalue.cpp
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

#include <stdlib.h>
#include <math.h>
#include "sp_caculator_expvalue.h"


expValue::expValue(const expValue& o)
{
	this->nType = o.nType;
	switch( this->nType )
	{
	case value_bool:
		this->bValue = o.bValue;
		break;
	case value_int32:
		this->nValue32 = o.nValue32;
		break;
	case value_int64:
		this->nValue64 = o.nValue64;
		break;
	case value_double:
		this->dValue = o.dValue;
		break;
	case value_str:
		this->strValue = o.strValue;
		break;
	default:
		// do nothing.
		break;
	}
}
void expValue::setValue(const string& str)
{
	this->nType = value_str;
	this->strValue = str;
}

void expValue::setValue(const bool& o)
{
	this->nType = value_bool;
	this->bValue = o;
}

void expValue::setValue(const int32_t& o)
{
	this->nType = value_int32;
	this->nValue32 = o;
}

void expValue::setValue(const int64_t& o)
{
	this->nType = value_int64;
	this->nValue64 = o;
}

void expValue::setValue(const double& o)
{
	this->nType = value_double;
	this->dValue = o;
}


ErrorCode expValue::switch2type( valueType type)
{
	switch( this->nType )
	{
		case value_int32:
			if( value_int64 == type )
			{
				nValue64 = (int64_t)nValue32;
			}
			else if( value_double == type )
			{
				dValue = (double)nValue32;
			}
			else
			{
				return NGB_EXPRESS_TYPE_CHANGE_FAILED;
			}
			break;
		case value_int64:
			if( value_double == type )
			{
				dValue = (double)nValue32;
			}
			else
			{
				return NGB_EXPRESS_TYPE_CHANGE_FAILED;
			}
			break;
		case value_double:
			return NGB_EXPRESS_TYPE_CHANGE_FAILED;
			break;
		default:
			return NGB_EXPRESS_TYPE_CHANGE_FAILED;
	}
	this->nType = type;
	return NGB_SUCCESS;
}

ErrorCode expValue::negotiateType( expValue& v)
{
	
	if( this->nType == value_invalid || v.nType == value_invalid )
	{
		return NGB_EXPRESS_INVALID_VALUE;
	}

	if( this->nType == v.nType ) return NGB_SUCCESS;

	ErrorCode nRtn = NGB_EXPRESS_TYPE_NOTMATCH; 

	if( this->isNumber() && v.isNumber() )
	{
		if(this->nType > v.nType)
		{
			nRtn = v.switch2type(this->nType);
		}
		else
		{
			nRtn = this->switch2type(v.nType);
		}
	}
	return nRtn;
}

ErrorCode expValue::valueAdd( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->nValue32 += v.nValue32;
		break;
	case value_int64:
		this->nValue64 += v.nValue64;
		break;
	case value_double:
		this->dValue += v.dValue;
		break;
	case value_str:
		this->strValue += v.strValue;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_ADD;
		this->nType = value_invalid;
	}
	return nRtn;
}

ErrorCode expValue::valueSub( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->nValue32 -= v.nValue32;
		break;
	case value_int64:
		this->nValue64 -= v.nValue64;
		break;
	case value_double:
		this->dValue -= v.dValue;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_SUB;
		this->nType = value_invalid;
	}
	return nRtn;
}

ErrorCode expValue::valueMul( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->nValue32 = this->nValue32 * v.nValue32;
		break;
	case value_int64:
		this->nValue64 = this->nValue64 * v.nValue64;
		break;
	case value_double:
		this->dValue = this->dValue * v.dValue;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_MUL;
		this->nType = value_invalid;
	}
	return nRtn;
}


ErrorCode expValue::valueDiv( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		if( v.nValue32 == 0 )
		{
			this->nType = value_invalid;
			nRtn = NGB_EXPRESS_DEVIDEBY_ZERO;
		}
		this->nValue32 = this->nValue32 / v.nValue32;
		break;
	case value_int64:
		if( v.nValue32 == 0 )
		{
			this->nType = value_invalid;
			nRtn = NGB_EXPRESS_DEVIDEBY_ZERO;
		}
		this->nValue64 = this->nValue64 / v.nValue64;
		break;
	case value_double:
		if( v.nValue32 == 0 )
		{
			this->nType = value_invalid;
			nRtn = NGB_EXPRESS_DEVIDEBY_ZERO;
		}
		this->dValue = this->dValue / v.dValue;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_DIV;
		this->nType = value_invalid;
	}
	return nRtn;
}


ErrorCode expValue::valueBitor( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->nValue32 |= v.nValue32;
		break;
	case value_int64:
		this->nValue64 |= v.nValue64;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_BOR;
		this->nType = value_invalid;
	}
	return nRtn;
}

ErrorCode expValue::valueBitand( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->nValue32 &= v.nValue32;
		break;
	case value_int64:
		this->nValue64 &= v.nValue64;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_BND;
		this->nType = value_invalid;
	}
	return nRtn;
}


ErrorCode expValue::valueStrHas( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	if( this->nType == value_str )
	{
		std::size_t found = this->strValue.find(v.strValue);
		this->nType = value_bool;
		this->bValue = ( found != std::string::npos );
	}
	else {
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_HAS;
		this->nType = value_invalid;
	}
	return nRtn;
}

ErrorCode expValue::valueEqual( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->bValue = ( this->nValue32 == v.nValue32 );
		break;
	case value_int64:
		this->bValue = ( this->nValue64 == v.nValue64 );
		break;
	case value_double:
		this->bValue = ( fabs(this->dValue - v.dValue) <= 0.000001 );
		break;
	case value_str:
		this->bValue = ( this->strValue.compare(v.strValue) == 0 ) ;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_EQL;
		this->nType = value_invalid;
		return nRtn;
	}
	this->nType = value_bool;
	return nRtn;
}


ErrorCode expValue::valueGt( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->bValue = (this->nValue32 > v.nValue32);
		break;
	case value_int64:
		this->bValue = (this->nValue64 > v.nValue64);
		break;
	case value_double:
		this->bValue = (this->dValue - v.dValue) > 0.000001 ;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_GT;
		this->nType = value_invalid;
		return nRtn;
	}

	this->nType = value_bool;
	return nRtn;
}

ErrorCode expValue::valueLt( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->bValue = (this->nValue32 < v.nValue32);
		break;
	case value_int64:
		this->bValue = (this->nValue64 < v.nValue64);
		break;
	case value_double:
		this->bValue = (this->dValue - v.dValue) < 0.000001 ;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_LT;
		this->nType = value_invalid;
		return nRtn;
	}
	this->nType = value_bool;
	return nRtn;
}


ErrorCode expValue::valueGte( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->bValue = (this->nValue32 >= v.nValue32);
		break;
	case value_int64:
		this->bValue = (this->nValue64 >= v.nValue64);
		break;
	case value_double:
		this->bValue = ( (this->dValue - v.dValue) >= 0.000001 );
		break;
	default:
		this->nType = value_invalid;
		return NGB_EXPRESS_RUNTIME_OP_INVALID_GTE;
	}
	this->nType = value_bool;
	return nRtn;
}

ErrorCode expValue::valueLte( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
		
	switch( this->nType )
	{
	case value_int32:
		this->bValue = (this->nValue32 <= v.nValue32);
		break;
	case value_int64:
		this->bValue = (this->nValue64 <= v.nValue64);
		break;
	case value_double:
		this->bValue = ((this->dValue - v.dValue) <= 0.000001)  ;
		break;
	default:
		this->nType = value_invalid;
		return NGB_EXPRESS_RUNTIME_OP_INVALID_LTE;
	}
	this->nType = value_bool;
	return nRtn;
}


ErrorCode expValue::valueOr( expValue& v)
{
   ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_bool:
		this->bValue = this->bValue || v.bValue;
		break;
	default:
		this->nType = value_invalid;
		return NGB_EXPRESS_RUNTIME_OP_INVALID_OR;
	}
	return nRtn;
}

ErrorCode expValue::valueAnd( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_bool:
		this->bValue = this->bValue && v.bValue;
		break;
	default:
		nRtn = NGB_EXPRESS_RUNTIME_OP_INVALID_AND;
		this->nType = value_invalid;
	}
	return nRtn;
}

ErrorCode expValue::valueNe( expValue& v)
{
	ErrorCode nRtn = negotiateType( v );
	if( NGB_SUCCESS != nRtn ) 
	{
		this->nType = value_invalid;
		return nRtn;
	}
	
	switch( this->nType )
	{
	case value_int32:
		this->bValue = (this->nValue32 != v.nValue32);
		break;
	case value_int64:
		this->bValue = (this->nValue64 != v.nValue64);
		break;
	case value_double:
		this->bValue = ( fabs(this->dValue - v.dValue ) >= 0.000001)  ;
		break;
	case value_bool:
		this->bValue = (this->bValue != v.bValue);
		break;
	default:
		this->nType = value_invalid;
		return NGB_EXPRESS_RUNTIME_OP_INVALID_LTE;
	}

	this->nType = value_bool;
	return nRtn;
}


ErrorCode expValue::valueExec(char oper, expValue& v)
{
	switch ( oper )
	{
	case OP_AND:
		return valueAnd( v );
		break;
	case OP_OR:
		return valueOr( v );
		break;
	case OP_GT:
		return valueGt( v );
		break;
	case OP_LT:
		return valueLt( v );
		break;
	case OP_EQ:
		return valueEqual( v );
		break;
	case OP_GTE:
		return valueGte( v );
		break;
	case OP_LTE:
		return valueLte( v );
		break;
	case OP_HAS:
		return valueStrHas( v );
		break;
	case OP_ADD:
		return valueAdd( v );
		break;
	case OP_SUB:
		return valueSub( v );
		break;
	case OP_MUL:
		return valueMul( v );
		break;
	case OP_DIV:
		return valueDiv( v );
		break;
	case OP_BITAND:
		return valueBitand( v );
		break;
	case OP_BITOR:
		return valueBitor( v );
		break;
	 case OP_NE:
		return valueNe( v );
		break;
	default:
		return NGB_EXPRESS_UNKNOW_OPER;
	}
}

