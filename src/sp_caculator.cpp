/******************************************************************************
**
** FILE NAME
**    sp_caculator.cpp
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
#include "sp_caculator.h"
#include "sp_caculator_utilites.h"
#include <iostream>


ErrorCode expNode::processExp(std::string  strExp)
{
	if( strExp.size() == 0 ) return  NGB_EXPRESS_INVALID_EXP;

	// 解析过一遍再解析，有内存泄露；
	if ( lchild != NULL ) { delete lchild; lchild = NULL; }
	if ( rchild != NULL ) { delete rchild; rchild = NULL; }

	while( calcStr::quotedByBrace(strExp.c_str()) )
	{
		strExp = strExp.substr( 1, strExp.size() - 2 );
	}
	
	nodeType_ = node_const;
 
	int64_t num;
	// 数字
	if( calcStr::isNumber(strExp, num ) )
	{
		if( num > 0xFFFFFFFF )
		{
			vAnalyzed.setValue( num );
		}
		else
		{
			vAnalyzed.setValue( int32_t(num & 0x7FFFFFFF ) );
		}
		return NGB_SUCCESS;
	}

	double  d;
	// 浮点数 
	if( calcStr::isDouble(strExp, d ) )
	{
		vAnalyzed.setValue( d );
		return NGB_SUCCESS;
	}

	//  字符串
	if( calcStr::isExpString(strExp.c_str() ) )
	{
		vAnalyzed.setValue( strExp.substr( 1, strExp.size() - 2 ) );
		return NGB_SUCCESS;
	}

	// 变量
	if( calcStr::isVar(strExp) )
	{
		nodeType_ = node_var;
		vAnalyzed.setValue( strExp.substr(1,strExp.size()) );
		return NGB_SUCCESS;
	}
	
	int nPos;
	ErrorCode nRtn = calcStr::findLastOperation(strExp, nPos);
	// 表达式
	if( NGB_SUCCESS == nRtn )
	{
		string strLeft = strExp.substr(0, nPos);
		string strRight = strExp.substr(nPos + 1, strExp.size() - nPos - 1 );

		( OP_ASN == strExp[nPos] ) ? ( nodeType_ = node_assign) : ( nodeType_ = node_exp);

		operType = strExp[nPos];

		lchild = new expNode();
		nRtn = lchild->processExp(strLeft);
		//cout << " strleft = " << strLeft << endl;
		if( nRtn != NGB_SUCCESS ) return nRtn;
		rchild = new expNode();
		//cout << " strright = " << strRight << endl;;
		nRtn = rchild->processExp(strRight);
		if( nRtn != NGB_SUCCESS ) return nRtn;

		// todo:赋值表达式，左边必须是变量类型 
		if( operType == OP_ASN && lchild->nodeType_ != node_var)
		{
			return NGB_EXPRESS_INVALID_ASN;
		}
		return NGB_SUCCESS;
	}
	else
	{
		return nRtn;
	}

	return NGB_SUCCESS;
}

ErrorCode expNode::analyseExp(string  strExp)
{
	string  strNew;

	ErrorCode nRtn = calcStr::formatExpress(strExp, strNew);
	//cout << " formated exp: " << strNew << endl;
	if( nRtn != NGB_SUCCESS ) return nRtn;

	return processExp(strNew);
}


expValue expNode::execute(dataExchange* data, ErrorCode& nRtn)
{
	ErrorCode lRtn = NGB_SUCCESS;

	expValue oResult;
	switch( nodeType_ )
	{
	case node_const:
		nRtn = lRtn;
		return vAnalyzed;
		break;
	case node_var:
		{
		expValue tmp;
		nRtn = data->getData( vAnalyzed.strValue, tmp );
		return tmp;
		}
		break;
	case node_exp:
		{
			expValue lvalue,rvalue;
			lvalue = lchild->execute( data, lRtn );
			if( lRtn != NGB_SUCCESS )
			{
				nRtn = lRtn;
				return lvalue;
			}
			rvalue = rchild->execute( data, lRtn );
			if( lRtn != NGB_SUCCESS )
			{
				nRtn = lRtn;
				return rvalue;
			}
			lRtn = lvalue.valueExec( operType, rvalue );
			if( lRtn != NGB_SUCCESS )
			{
				nRtn = lRtn;
				return lvalue;
			}
			vResult = lvalue;
			return lvalue;
		}
		break;
	case node_assign:
		{
			expValue oResult = rchild->execute( data, lRtn );
			if( lRtn != NGB_SUCCESS )
			{
				nRtn = lRtn;
				return oResult;
			}
			lRtn = data->SetData( lchild->vAnalyzed.strValue, oResult );

			nRtn = lRtn;
			vResult = oResult;
			return oResult;
		}
		break;
	default:
		nRtn = NGB_EXPRESS_UNKNOW_OPER;
		return oResult;
	}

}

void expNode::printTree(int nLevel)
{
	int i;
	ErrorCode nRtn;
	switch( nodeType_ )
	{
	case node_const:
		for( i = 0; i < nLevel; ++i)
		{
			cout << ' ';
		}
		if( vAnalyzed.isNumber() ) cout << vAnalyzed.getValueInt32(nRtn) << endl;
		if( vAnalyzed.getType() == value_str ) cout << vAnalyzed.getValueStr(nRtn) << endl;
		break;
	case node_var:
		for( i = 0; i < nLevel; ++i)
		{
			cout << ' ';
		}
		cout << vAnalyzed.getValueStr(nRtn) << endl;
		break;
	case node_exp:
		for( i = 0; i < nLevel; ++i)
		{
			cout << ' ';
		}
		cout << operType << endl;
		lchild->printTree(nLevel + 2);
		rchild->printTree(nLevel + 2);
		break;
	case node_assign:
		for( i = 0; i < nLevel; ++i)
		{
			cout << ' ';
		}
		cout << '=' << endl;
		lchild->printTree(nLevel + 2);
		rchild->printTree(nLevel + 2);
		break;
	default:
		nRtn = NGB_EXPRESS_UNKNOW_OPER;
		return ;
	}
}



