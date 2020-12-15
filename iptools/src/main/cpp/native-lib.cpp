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
