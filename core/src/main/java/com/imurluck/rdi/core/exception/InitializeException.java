package com.imurluck.rdi.core.exception;

import androidx.annotation.NonNull;

/**
 * @author imurluck
 */
public class InitializeException extends Exception {

   public InitializeException(@NonNull String errorMsg) {
      super(errorMsg);
   }

   public InitializeException(@NonNull String errorMsg, @NonNull Throwable cause) {
      super(errorMsg, cause);
   }
}
