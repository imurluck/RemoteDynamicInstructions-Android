package com.imurluck.rdi.core;

import android.app.Application;

import androidx.annotation.NonNull;

import java.io.File;

/**
 * @author imurluck
 */
public class Configurations {

   private static final String ROOT_DIR_NAME = "rdi";

   @NonNull
   public static File getRootDir(@NonNull Application application) {
      return new File(application.getFilesDir(), ROOT_DIR_NAME);
   }
}
