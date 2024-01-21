package com.imurluck.rdi;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;

import com.imurluck.rdi.api.IMethodCallContext;
import com.imurluck.rdi.api.IOnMethodEnterCapability;
import com.imurluck.rdi.core.Rdi;
import com.imurluck.rdi.core.exception.CapabilityRegisterException;
import com.imurluck.rdi.core.exception.InitializeException;

import java.lang.reflect.Array;
import java.util.Arrays;

public class OnMethodEnterActivity extends AppCompatActivity {

    private static final String TAG = "OnMethodEnterActivity";

    private OnDescendantInvalidatedEnter mOnDescendantInvalidatedEnter =
            new OnDescendantInvalidatedEnter();

    private OnTestPrimaryTypesEnter mOnTestPrimaryTypesEnter = new OnTestPrimaryTypesEnter();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_on_method_enter);

        findViewById(R.id.register_on_method_enter_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                try {
                    Rdi.initialize(getApplication());
                } catch (InitializeException e) {
                    throw new RuntimeException(e);
                }
                try {
                    Rdi.registerCapability(mOnDescendantInvalidatedEnter);
                    Rdi.registerCapability(mOnTestPrimaryTypesEnter);
                } catch (CapabilityRegisterException e) {
                    throw new RuntimeException(e);
                }
            }
        });

        findViewById(R.id.test_on_method_enter_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                new Thread(() -> {
                    v.invalidate();
                }).start();
            }
        });

        findViewById(R.id.test_primary_type_btn).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                testPrimaryTypes(true, 'c', (byte) 1, (short) 2, 3, 4.f, 5.d, 6L);
            }
        });
    }

    @Keep
    private void testPrimaryTypes(
            boolean b, char c, byte bt, short s, int i, float f, double d, long l) {

    }

    private static class OnDescendantInvalidatedEnter implements IOnMethodEnterCapability {

        @Override
        public String getDeclaringClass() {
            return "android.view.ViewRootImpl";
        }

        @Override
        public String getName() {
            return "onDescendantInvalidated";
        }

        @Override
        public String getSignature() {
            return "(Landroid/view/View;Landroid/view/View;)V";
        }

        @Override
        public void onMethodEnter(@NonNull IMethodCallContext context) {
            if (Looper.myLooper() != Looper.getMainLooper()) {
                new Throwable().printStackTrace();
            }
        }

        @Override
        public boolean collectStacktrace() {
            return false;
        }
    }

    private static class OnTestPrimaryTypesEnter implements IOnMethodEnterCapability {

        @Override
        public String getDeclaringClass() {
            return "com.imurluck.rdi.OnMethodEnterActivity";
        }

        @Override
        public String getName() {
            return "testPrimaryTypes";
        }

        @Override
        public String getSignature() {
            return "(ZCBSIFDJ)V";
        }

        @Override
        public void onMethodEnter(@NonNull IMethodCallContext context) {
            Log.d(TAG, "OnTestPrimaryTypesEnter: " + Arrays.toString(context.getArguments()));
        }

        @Override
        public boolean collectStacktrace() {
            return false;
        }
    }
}