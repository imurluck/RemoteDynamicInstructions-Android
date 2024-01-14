package com.imurluck.rdi.api;

import android.os.Bundle;

import androidx.annotation.NonNull;

/**
 * @author imurluck
 * @date 2024/1/14
 */
public interface IOnMethodEnterCapability extends ICapability {

    /**
     * @return declaring class name, will use {@link Class#forName(String)} to find it
     */
   String getDeclaringClass();

    /**
     * @return method name
     */
   String getName();

    /**
     * @return method signature, arguments + return value
     *         for example: {@link android.app.Activity#onCreate(Bundle)} -> (Landroid/os/Bundle;)V
     */
   String getSignature();

   void onMethodEnter(@NonNull IMethodCallContext context);

   boolean collectStacktrace();
}
