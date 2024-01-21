package com.imurluck.rdi.core;

import android.annotation.SuppressLint;
import android.app.Application;
import android.os.Build;
import android.os.Debug;

import androidx.annotation.NonNull;

import com.imurluck.rdi.api.ICapability;
import com.imurluck.rdi.api.IOnMethodEnterCapability;
import com.imurluck.rdi.core.context.MethodCallContext;
import com.imurluck.rdi.core.exception.CapabilityRegisterException;
import com.imurluck.rdi.core.exception.InitializeException;

import java.io.File;
import java.io.IOException;
import java.util.List;

/**
 * @author imurluck
 * @date 2024/1/14
 */
class RdiAgent {
    private static final String LIBRARY_NAME = "librdi.so";

    private boolean mInitialized = false;

    private final CapabilityRegistry mCapabilityRegistry = new CapabilityRegistry(this);
    RdiAgent() {}

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    synchronized void initialize(@NonNull Application application) throws InitializeException {
        if (mInitialized) {
            return;
        }

        NativeLibraryExtractor extractor = new NativeLibraryExtractor();
        File targetLibraryFile = null;
        try {
            targetLibraryFile = extractor.extract(application, LIBRARY_NAME);
        } catch (IOException e) {
            throw new InitializeException("Failed to extract library", e);
        }

        System.load(targetLibraryFile.getAbsolutePath());

        String errorMsg = RdiJni.initializeNative();
        if (errorMsg != null) {
            throw new InitializeException("Failed to initialize in native. " + errorMsg);
        }

        try {
            loadAgent(targetLibraryFile);
        } catch (IOException e) {
            throw new InitializeException("Failed to load agent", e);
        }

        this.mInitialized = true;
    }

    private void loadAgent(@NonNull File libraryFile) throws IOException {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            Debug.attachJvmtiAgent(
                    libraryFile.getAbsolutePath(), null, getClass().getClassLoader());
        }
    }

    public void registerCapability(@NonNull ICapability capability) throws CapabilityRegisterException {
        mCapabilityRegistry.registerCapability(capability);
    }

    public void onMethodEnter(long methodId, @NonNull Object[] arguments) {
        synchronized (IOnMethodEnterCapability.class) {
            MethodCallContext context = new MethodCallContext(arguments);
            List<IOnMethodEnterCapability> capabilities =
                    mCapabilityRegistry.getOnMethodEnterCapabilities(methodId);
            if (capabilities == null || capabilities.size() == 0) {
                return;
            }
            for (IOnMethodEnterCapability capability : capabilities) {
                capability.onMethodEnter(context);
            }
        }
    }
}
