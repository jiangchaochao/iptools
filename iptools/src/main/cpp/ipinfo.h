//
// Created by 82039 on 2020/12/15.
//

#ifndef IPTOOLS_IPINFO_H
#define IPTOOLS_IPINFO_H

using namespace std;

#include "ipinfo.h"
#include <iostream>

class ipinfo {
    char name[16];               // 网络接口名称
    char mac[32];                // 对应mac地址
    char address[16];            // 对应ip地址
public:
    string getName();

    void setName(string name);

    string getAddress();

    void setAddress(string address);

    string getMac();

    void setMac(string mac);
};


#endif //IPTOOLS_IPINFO_H
