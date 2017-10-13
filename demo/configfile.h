/******************************************************************************
**  Copyright (C) 2005 ~ 2015, Flaircomm Technologies, Inc.
**
** FILE NAME
**    configfile.h
**
** DESCRIPTION
**    declareration file of read/write configuration file
**
** History:
**  $Author:    caoqy
**  $Revision:  $
**  $Date:      2015.3.24
**  $Header:    $
*******************************************************************************/

///The format of conf file is as follows£º  
/************************************************* 
#this is comment 
name = Mesh BLE 
date = 2011.10.30 
cnt = 0
**************************************************/      

#ifndef _CONFIG_FILE_H_  
#define _CONFIG_FILE_H_  

#include <iostream>  
#include <fstream>  
#include <string.h>  
#include <stdlib.h>  
#include <map>  
#include <vector>  

using namespace std;  

class ConfigFile  
{  
public:  
    ConfigFile();  
    ~ConfigFile();  

    // open a file and read the content to buffer
    bool open(const string file_path);  

    //  save all information to disk
    bool save();  

    // get a string value with key
    string get_string(const string key);  

    // get a int value with key
    unsigned int get_int(const string key);  

    // set a string value with key
    bool set_string(const string key, const string value);  

    // set a int value with key
    bool set_int(const string, const unsigned int value);  

private:  
    void trim_left_right(string &str);  
    void set_vector_string(const string key, const string value);  

private:  
    string m_path;  
    vector<string> m_data;  
    map<string, string> m_map;  
};  

#endif // CONFIG_FILE_H

