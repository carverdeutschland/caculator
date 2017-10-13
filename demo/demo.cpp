/******************************************************************************
**
** FILE NAME
**    calctest.cpp
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
#include "iostream"
#include <signal.h>
#include "../src/sp_caculator.h"
#include "configfile.h"


class dataExcDemo :public dataExchange
{
public:
    dataExcDemo(){};
    dataExcDemo(ConfigFile *f){m_f = f;};
    virtual ~dataExcDemo(){};
private:
    ConfigFile* m_f;
public:
    // 表达式从应用中获取数据
    virtual ErrorCode getData(const string& strVarName, expValue& expVal );
    // 表达式设置数据到应用中去
    virtual ErrorCode SetData(const string& strVarName, expValue& eVal );
};

string  toString(int n)
{
    char buf[16];
    sprintf(buf,"%d",n);
    return buf;
}

string  toString(double n)
{
    char buf[32];
    sprintf(buf,"%f",n);
    return buf;
}

string  toString(bool b)
{
    if( b ) return "true";

    return "false";
}


ErrorCode dataExcDemo::getData(const string& strVarName, expValue& expVal )
{
    if( strVarName.compare("popName1") == 0 )
    {
        expVal.setValue( m_f->get_string(strVarName));
        return NGB_SUCCESS;
    }
    if( strVarName.compare("popName2") == 0 )
    {
        expVal.setValue( m_f->get_string(strVarName));
        return NGB_SUCCESS;
    }
    if( strVarName.compare("popBandWidth") == 0 )
    {
        int32_t n = m_f->get_int(strVarName);
        expVal.setValue( n );
        return NGB_SUCCESS;
    }
    if( strVarName.compare("usage") == 0 )
    {
        int32_t n = m_f->get_int(strVarName);
        expVal.setValue( n );
        return NGB_SUCCESS;
    }
    return NGB_EXPRESS_DATAGET_NOTSUPPORTED;
}

ErrorCode dataExcDemo::SetData(const string& strVarName, expValue& eVal )
{
    ErrorCode nRtn = NGB_EXPRESS_DATASET_NOTSUPPORTED;
    if( value_bool == eVal.getType() )
    {
        m_f->set_string( strVarName, toString( eVal.getValueBool(nRtn)) );
        nRtn = NGB_SUCCESS;
    }

    if( value_int32 == eVal.getType() )
    {
        m_f->set_int( strVarName, eVal.getValueInt32(nRtn));
        nRtn = NGB_SUCCESS;
    }

    if( value_str == eVal.getType() )
    {
        m_f->set_string( strVarName, eVal.getValueStr(nRtn));
        nRtn = NGB_SUCCESS;
    }

    if( value_double == eVal.getType() )
    {
        m_f->set_string( strVarName, toString( eVal.getValueDouble(nRtn)) );
        nRtn = NGB_SUCCESS;
    }

    m_f->save();

    return nRtn;
}


void test_expNode()
{
    expNode  node;
    ErrorCode nRtn;

    string  strExp;

    //calcStr::formatExpress("$pop.bandwidth == (2+2)*3|0x11 && $pop.number < $totalnum/1000", strExp);
    //nRtn = calcStr::formatExpress("20 + (23+17)*4-14 * 4 ", strExp);  // 20 + (23+17)*4-14 * 4
    nRtn = node.analyseExp(strExp);

    cout << "analyze result = " << nRtn << endl;
   
    node.printTree(0);
    return;
    dataExchange* data = new dataExchange();

    expValue oResult = node.execute(data, nRtn);

    cout << "result " << oResult.getValueInt32(nRtn) << endl;
}

int main(int argc,char*argv[])
{

    ConfigFile f;

    if( f.open("./calcdemo.txt") ) cout << "open file calcdemo.txt succeed." << endl;

    string  strName = "exp";
    string  strExp;
    char c = f.get_string("from")[0];
    ErrorCode nRtn;

    dataExcDemo dd(&f);

    for( ; c <= f.get_string("to")[0]; c++ )
    {
        strExp = f.get_string(strName + c);
        cout << endl << strExp << endl;

        expNode  node;

        nRtn = node.analyseExp(strExp);

        if( nRtn == NGB_SUCCESS )
        {
            //cout << "analyze express succeed!" << endl;
            //node.printTree();
        }
        else
        {
            cout << "analyze express failed! errorcode = " << nRtn << endl;
        }

        expValue v = node.execute( &dd, nRtn);

        if( nRtn == NGB_SUCCESS )
        {
            //cout << "execute express succeed!" << endl;
        }
        else
        {
            cout << "execute express failed! errorcode = " << nRtn << endl;
            continue;
        }

        if( v.getType() == value_int32 )
        {
            cout << " result is int: " << v.getValueInt32(nRtn) << endl;
        }

        if( v.getType() == value_bool )
        {
            cout << " result is bool: " << toString( v.getValueBool(nRtn)) << endl;
        }

        if( v.getType() == value_str )
        {
            cout << " result is str: " << v.getValueStr(nRtn) << endl;
        }

        if( v.getType() == value_double )
        {
            cout << " result is double: " << v.getValueDouble(nRtn) << endl;
        }
    }

    
    
}

















