package com.imurluck.rdi.core.exception;

import androidx.annotation.NonNull;

/**
 * @author imurluck
 * @date 2024/1/14
 */
public class CapabilityRegisterException extends Exception {

   public CapabilityRegisterException(@NonNull String errorMsg) {
      super(errorMsg);
   }

   public CapabilityRegisterException(@NonNull String errorMsg, @NonNull Throwable cause) {
      super(errorMsg, cause);
   }
}
