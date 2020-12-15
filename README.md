# iptools
提供一个获取Android系统所有网络接口IP的功能（仅限于已经存在IP信息的），因为Android系统未提供Wifi P2P连接GC端获取自身IP的功能，故此实现一个，希望能帮助需要的人.


#### 前因
在使用Wifi Direct功能时发现在作为客户端的时候无法获取到自己的IP地址，遂通过NetworkInterface 接口获取，发现没找到，其实后面发现自己弄错了，可以获取，因为P2P在没有IP地址的时候是获取不到的，所以。。。。。



这里是通过C语言实现的，既然实现了，那就分享出来吧，和Java的功能通过NetworkInterface结果一样  ，大家下载demo看吧!!!!



#### java ip 信息类
```
package com.jiangc.iptools;

public class IpInfo {
    String name;               // 网络接口名称
    String mac;                // 对应mac地址
    String address;            // 对应ip地址

    public IpInfo() {
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getAddress() {
        return address;
    }

    public void setAddress(String address) {
        this.address = address;
    }

    public String getMac() {
        return mac;
    }

    public void setMac(String mac) {
        this.mac = mac;
    }
}
```


#### jni 类
```
package com.jiangc.iptools;

public class IpTools {
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * TODO: 获取所有带IP信息的接口信息：如果接口存在，但没有ip信息是获取不到的，需要另外的方式，获取P2P的ip地址需要在连接成功之后调用该接口才可以，否在获取不到
     * 需要网络访问权限,高版本请注意权限问题
     * mac地址暂不支持，返回 "null"字符串
     * @return
     */

    public native IpInfo[] getAllIpInfoJNI();
}

```


#### C ip 信息类
```
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

```

#### C jni接口类

```
#include <jni.h>
#include <string>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include "ipinfo.h"
#include<android/log.h>

#define LOG_TAG "iptools"
#define LOG_D(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, fmt, ##args)
#define LOG_I(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)
#define LOG_W(fmt, args...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, fmt, ##args)
#define LOG_E(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#define LOG_F(fmt, args...) __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, fmt, ##args)

ipinfo sarray[20];

int get_local_ip() {
    int fd;
    int interface;
    int ret;
    memset(sarray, 0, sizeof(sarray));
    struct ifreq buf[INET_ADDRSTRLEN];
    struct ifconf ifc;
    int count = 0;   // 记录数组中有多少个节点信息
    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) >= 0) {
        ifc.ifc_len = sizeof(buf);
        ifc.ifc_buf = (caddr_t) buf;
        if (!ioctl(fd, SIOCGIFCONF, (char *) &ifc)) {
            interface = ifc.ifc_len / sizeof(struct ifreq);
            while (interface-- > 0) {
                if (!(ioctl(fd, SIOCGIFADDR, (char *) &buf[interface]))) {
                    sarray[interface].setAddress((inet_ntoa(((struct sockaddr_in *) (&buf[interface].ifr_addr))->sin_addr)));    // 设置IP地址
                    sarray[interface].setName(buf[interface].ifr_name);  // 设置节点名称
                    sarray[interface].setMac("null");             // 设置mac地址:暂不支持,后面再实现
                    count++;
                }
            }
        }
    }

    close(fd);
    return count;
}


/**
 * 获取所有ip信息
 * @param env
 * @param thiz
 * @return
 */
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_jiangc_iptools_IpTools_getAllIpInfoJNI(JNIEnv *env, jobject thiz) {
    jobjectArray jarray = NULL;
    int count = get_local_ip();   // 获取有多少个接口，计算申请空间的大小
    // 获取java中的对应的类 ipinfo
    jclass jipInfo = env->FindClass("com/jiangc/iptools/IpInfo");
    // 初始化返回的数组
    jarray = env->NewObjectArray(count, jipInfo, 0);
    // 获取类的构造函数
    jmethodID jmethodId = env->GetMethodID(jipInfo, "<init>", "()V");

    // 循环赋值
    for (int i = 0; i < count; i ++)
    {
        // 实例化一个对象
        jobject  ip = env->NewObject(jipInfo, jmethodId);
        // 获取字段
        jfieldID name = env->GetFieldID(jipInfo, "name", "Ljava/lang/String;");
        jfieldID mac = env->GetFieldID(jipInfo, "mac", "Ljava/lang/String;");
        jfieldID addree = env->GetFieldID(jipInfo, "address", "Ljava/lang/String;");
        // 设置字段的值
        env->SetObjectField(ip, name,  env->NewStringUTF(sarray[i].getName().c_str()));
        env->SetObjectField(ip, mac,  env->NewStringUTF(sarray[i].getMac().c_str()));
        env->SetObjectField(ip, addree,  env->NewStringUTF(sarray[i].getAddress().c_str()));
        env->SetObjectArrayElement(jarray, i, ip);
        env->DeleteLocalRef(ip);
    }
    return jarray;
}

```

####  主要的都在这里了，既然Java有，那就用Java的，这里就当练习NDK了
