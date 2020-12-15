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
