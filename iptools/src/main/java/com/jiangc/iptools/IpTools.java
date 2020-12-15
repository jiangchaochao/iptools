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
