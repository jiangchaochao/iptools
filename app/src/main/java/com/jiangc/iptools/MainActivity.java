package com.jiangc.iptools;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import javax.security.auth.login.LoginException;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";
    // Used to load the 'native-lib' library on application startup.
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        IpTools ipTools = new IpTools();
        IpInfo[] ipInfos = ipTools.getAllIpInfoJNI();

        for (IpInfo ipInfo : ipInfos) {
            Log.e(TAG, "onCreate: interface: " + ipInfo.getName() + "  ipaddress:"+ ipInfo.address);
        }
    }
}