#ifndef __CONFIG_H__
#define __CONFIG_H__
#include <iostream>
#include <string>
#include <fstream>

using std::fstream;
using std::string;
using std::cout;
using std::endl;


bool readConfigFile(const char * filepath, const string & key, string & value)
{
    fstream fs;
    fs.open(filepath);
    if(!fs.is_open()) 
    {
        cout << "can not open cfg file!" << endl;
        return false;
    }

    char temp[1000];
    while(!fs.eof()) 
    {
        fs.getline(temp, 1000);
        string line(temp);
        size_t pos = line.find('=');
        if(pos == string::npos) 
        {
          	return false; 
        }
        string temKey = line.substr(0, pos);
        if( key == temKey) 
        {
          	value = line.substr(pos + 1) ;
        }
    }
    return false;
}
#endif
