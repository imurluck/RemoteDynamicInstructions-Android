package com.imurluck.rdi;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Looper;
import android.view.View;

import com.imurluck.rdi.api.IMethodCallContext;
import com.imurluck.rdi.api.IOnMethodEnterCapability;
import com.imurluck.rdi.core.Rdi;
import com.imurluck.rdi.core.exception.CapabilityRegisterException;
import com.imurluck.rdi.core.exception.InitializeException;

public class OnMethodEnterActivity extends AppCompatActivity {

    private OnDescendantInvalidatedEnter onDescendantInvalidatedEnter =
            new OnDescendantInvalidatedEnter();

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
                    Rdi.registerCapability(onDescendantInvalidatedEnter);
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
    }

    private class OnDescendantInvalidatedEnter implements IOnMethodEnterCapability {

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
}