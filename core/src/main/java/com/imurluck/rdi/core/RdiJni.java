package com.imurluck.rdi.core;

import androidx.annotation.Keep;
import androidx.annotation.NonNull;

import com.imurluck.rdi.core.context.MethodCallContext;

/**
 * @author imurluck
 * @date 2024/1/14
 */
@Keep
class RdiJni {

   /**package*/static native String initializeNative();

   /**package*/static native long findMethod(
           @NonNull Class<?> declaringClass,
           @NonNull String name,
           @NonNull String signature);

   /**package*/static native void onMethodEnterRegistered(long methodId);

   /**
    * called from jni, if no arguments, will receive an empty array
    */
   private static void onMethodEnter(long methodId, @NonNull Object[] arguments) {
      RdiAgentHolder.agent.onMethodEnter(methodId, arguments);
   }
}
