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

        findViewById(R.id.local_variable_test_btn).setOnClickListener(v -> {
            localVariableTest(true, 'c', (byte) 1, (short) 1, 1, 2f, 3d, 1L, new Object(), new Object[1]);
        });
    }

    public void localVariableTest(boolean b, char c, byte bt, short s, int i, float f, double d, long l, Object o, Object[] array) {
//        int innerI = 11;
//        float innerF = 22f;
//        double innerD = 33d;
//        Object innerO = new MainActivity();
//        {
//            int shortLengthI = 111;
//        }
//        System.out.println();
    }
}