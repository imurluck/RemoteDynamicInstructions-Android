package com.imurluck.rdi.core.context;

import androidx.annotation.NonNull;

import com.imurluck.rdi.api.IMethodCallContext;

/**
 * @author imurluck
 * @date 2024/1/14
 */
public class MethodCallContext implements IMethodCallContext {

   @NonNull
   @Override
   public Object[] getArguments() {
      return new Object[0];
   }
}
