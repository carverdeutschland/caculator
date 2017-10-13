/******************************************************************************
**
** FILE NAME
**    sp_caculator.h
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
#ifndef _SP_CACULATOR_H_
#define _SP_CACULATOR_H_

#include <stdint.h>
#include <string>

#include "sp_caculator_errorcode.h"
#include "sp_caculator_expvalue.h"

using namespace std;


// 节点类型可以是一个值，也可以是一个变量，或者表达式，以及赋值操作
enum nodeType
{
	node_const,
	node_var,
	node_exp,
	node_assign
};

class dataExchange
{
public:
	dataExchange(){};
	virtual ~dataExchange(){};
	// 表达式从应用中获取数据
	virtual ErrorCode getData(const string& strVarName, expValue& expVal ){return NGB_EXPRESS_DATAGET_NOTSUPPORTED;};
	// 表达式设置数据到应用中去
	virtual ErrorCode SetData(const string& strVarName, expValue& eVal ){return NGB_EXPRESS_DATASET_NOTSUPPORTED;};
};


class expNode;

class expNode
{
public:
	expNode()
	{
		lchild = NULL;
		rchild = NULL;
	};
	virtual ~expNode()
	{
		if ( lchild != NULL ) {
			delete lchild;
			lchild = NULL;
		}
		if ( rchild != NULL ) {
			delete lchild;
			lchild = NULL;
		}
	};

private:
	nodeType   nodeType_;  
	// 这个存放表达式解释后的结果
	expValue   vAnalyzed;

	// 操作符 nodeType = node_exp 的时候，这个才需要
	char        operType;   
	// 这里只放计算结果
	expValue    vResult;
	
	expNode* lchild;
	expNode* rchild;
	ErrorCode processExp(std::string  strExp);
public:
	ErrorCode analyseExp(std::string  strExp);
	expValue execute(dataExchange* data, ErrorCode& nRtn);
	expValue getResult(){return vResult;};
	void printTree(int nLevel = 0);
};


#endif

