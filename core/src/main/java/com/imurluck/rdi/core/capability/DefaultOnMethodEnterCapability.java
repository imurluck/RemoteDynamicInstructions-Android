package com.imurluck.rdi.core.capability;

import androidx.annotation.NonNull;

import com.imurluck.rdi.api.IMethodCallContext;
import com.imurluck.rdi.api.IOnMethodEnterCapability;

/**
 * @author imurluck
 * @date 2024/1/14
 */
public class DefaultOnMethodEnterCapability implements IOnMethodEnterCapability {

    private final String mDeclaringClass;

    private final String mName;

    private final String mSignature;

    private final boolean mCollectStacktrace;

    public DefaultOnMethodEnterCapability(
            @NonNull String declaringClass,
            @NonNull String methodName,
            @NonNull String methodSignature,
            boolean collectStacktrace) {

        this.mDeclaringClass = declaringClass;
        this.mName = methodName;
        this.mSignature = methodSignature;
        this.mCollectStacktrace = collectStacktrace;
    }
    @Override
    public String getDeclaringClass() {
        return mDeclaringClass;
    }

    @Override
    public String getName() {
        return mName;
    }

    @Override
    public String getSignature() {
        return mSignature;
    }

    @Override
    public void onMethodEnter(@NonNull IMethodCallContext context) {

    }

    @Override
    public boolean collectStacktrace() {
        return mCollectStacktrace;
    }
}
