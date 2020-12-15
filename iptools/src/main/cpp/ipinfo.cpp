//
// Created by 82039 on 2020/12/15.
//

#include "ipinfo.h"

string ipinfo::getName() {
    return string(name);
}

void ipinfo::setName(string name) {
    memcpy(this->name, name.c_str(), name.length());
}

string ipinfo::getAddress() {
    return string (address);
}

void ipinfo::setAddress(string address) {
    memcpy(this->address, address.c_str(), address.length());
}

string ipinfo::getMac() {
    return string (mac);
}

void ipinfo::setMac(string mac) {
    memcpy(this->mac, mac.c_str(), mac.length());
}



