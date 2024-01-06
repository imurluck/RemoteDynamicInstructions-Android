package com.imurluck.rdi;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;

import com.imurluck.rdi.core.Rdi;
import com.imurluck.rdi.core.exception.InitializeException;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.initialize_btn).setOnClickListener(v -> {
            long startTime = SystemClock.uptimeMillis();
            try {
                new Rdi().initialize(getApplication());
            } catch (InitializeException e) {
//                e.printStackTrace();
            }
            Log.e(TAG, "initialize cost " + (SystemClock.uptimeMillis() - startTime));
        });
    }
}