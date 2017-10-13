/******************************************************************************
**  Copyright (C) 2005 ~ 2015, Flaircomm Technologies, Inc.
**
** FILE NAME
**    configfile.cpp
**
** DESCRIPTION
**    This file contains implementation of read/write of configuration from a file.
**
** History:
**  $Author:    caoqy
**  $Revision:  $
**  $Date:      2015.3.24
**  $Header:    $
*******************************************************************************/

#include "configfile.h"  


#define LINE_LEN 256  

///public  
ConfigFile::ConfigFile()  
{  
}  

ConfigFile::~ConfigFile()  
{  
}  

/******************************************************************************
***
*** Function Name:  open
***
*** Function Description:
***		open a configuration file and read content to buffer.
***
*** Inputs:
***		file_path:  absolutly path with file name.
***
*** Returns:
***		bool:
******************************************************************************/
bool ConfigFile::open(const string file_path)  
{  
    bool ret = false;  

    if (file_path.empty())  
    {  
        return ret;  
    }  

    m_path = file_path;  
    fstream file_in(file_path.c_str(), ios_base::in);  

    if (file_in.is_open())  
    {  
        string str, left, right;  
        unsigned int len, pos;  
        char szLine[LINE_LEN] = {0};  

        while (file_in.getline(szLine, LINE_LEN))  
        {  
            len = strlen(szLine);  
            m_data.push_back(szLine);  

            str.assign(szLine, len);  
            trim_left_right(str);  

            if (str[0] != '#')  
            {  
                pos = str.find('=');  
                left = str.substr(0, pos);  
                right = str.substr(pos + 1);  
                trim_left_right(left);  
                trim_left_right(right);  
                m_map.insert(pair<string, string>(left, right));  
            }  

            bzero(szLine, LINE_LEN);  
        }  

        file_in.close();  
        ret = true;  
    }  

    return ret;  
}  

/******************************************************************************
***
*** Function Name:  save
***
*** Function Description:
***		save configuration to file.
***
*** Inputs: none
***
*** Returns:
***		bool:
******************************************************************************/
bool ConfigFile::save()  
{  
    bool ret = false;  

    fstream file_out(m_path.c_str(), ios_base::out);  

    if (file_out.is_open())  
    {  
        vector<string>::iterator iter;  

        for (iter = m_data.begin(); iter != m_data.end(); ++iter)  
        {  
            string str = (*iter);  
            file_out << str << endl;  
        }  

        file_out.close();  
        ret = true;  
    }  

    return ret;  
}  

string ConfigFile::get_string(const string key)  
{  
    string str = "";  

    map<string, string>::iterator  it;

    it = m_map.find(key);

    if (it != m_map.end())  
    {  
        str = it->second;  
    }

    trim_left_right(str);

    return str;  
}  

unsigned int ConfigFile::get_int(const string key)  
{  
    unsigned int value = 0;  

    string str = get_string(key);  

    if( str.length() == 0 )
    {
        value = 0xFFFFFFFF;
    }
    else
    {
        value = atoi(str.c_str());  
    }

    return value;  
}  

bool ConfigFile::set_string(const string key, const string value)  
{  
    bool ret = false;  

    map<string, string>::iterator  it;

    it = m_map.find(key);

    if (it != m_map.end())  
    {  
        it->second = value;  
        set_vector_string(key, value);  

        ret = true;  
    }  

    return ret;  
}  

bool ConfigFile::set_int(const string key, const unsigned int value)  
{  
    bool ret = false;  

    map<string, string>::iterator  it;

    it = m_map.find(key);

    if (it != m_map.end())  
    {  
        char szValue[32] = {0};  

        sprintf(szValue, "%d", value);  
        m_map[key] = szValue;  
        set_vector_string(key, szValue);  

        ret = true;  
    }  

    return ret;  
}  


void ConfigFile::trim_left_right(string &str)  
{  
    if (!str.empty())  
    {  
        int i;  
        int len = str.size();  

        for (i = 0; i < len; i++)  
        {  
            if (str[i] != ' ')  
            {  
                break;  
            }  
        }  

        str.erase( 0, i );  
        len = str.size();  

        for (i = len - 1; i > 0; i--)  
        {  
            if (str[i] != ' ')  
            {  
                break;  
            }  
        }  

        str.erase(i + 1, len);  
    }  
}  

/******************************************************************************
***
*** Function Name:  set_vector_string
***
*** Function Description:
***		replace relevant line.
***
*** Inputs: none
***
*** Returns:
***		void:
******************************************************************************/
void ConfigFile::set_vector_string(const string key, const string value)  
{  
    if (!m_data.empty() && !key.empty())  
    {  
        vector<string>::iterator iter;  
        string str;  
        for (iter = m_data.begin(); iter != m_data.end(); ++iter)  
        {  
            str = *iter;  
            trim_left_right(str);  
            int pos1 = str.find('=');  

            int pos2 = (*iter).find('=');  
            int len = (*iter).size();  

            string substr = str.substr(0, pos1);  
            trim_left_right(substr);  
            string value2 = " " + value;  

            if (str[0] != '#' && substr == key)  
            {  
                (*iter).replace(pos2 + 1, len - pos2 - 1, value2);  
            }  
        }  
    }     
}  



