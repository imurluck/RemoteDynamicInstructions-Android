package com.imurluck.rdi.core.context;

import androidx.annotation.NonNull;

import com.imurluck.rdi.api.IMethodCallContext;

/**
 * @author imurluck
 * @date 2024/1/14
 */
public class MethodCallContext implements IMethodCallContext {

   private final Object[] mArguments;

   public MethodCallContext(@NonNull Object[] arguments) {
      this.mArguments = arguments;
   }

   @NonNull
   @Override
   public Object[] getArguments() {
      return this.mArguments;
   }
}
