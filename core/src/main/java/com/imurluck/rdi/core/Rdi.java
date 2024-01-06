package com.imurluck.rdi.core;

import android.annotation.SuppressLint;
import android.app.Application;
import android.os.Build;
import android.os.Debug;

import androidx.annotation.NonNull;

import com.imurluck.rdi.core.exception.InitializeException;

import java.io.File;
import java.io.IOException;

/**
 * @author imurluck
 */
public class Rdi {

   private static final String LIBRARY_NAME = "librdi.so";

   private boolean mInitialized = false;

   @SuppressLint("UnsafeDynamicallyLoadedCode")
   public synchronized void initialize(@NonNull Application application) throws InitializeException {
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

      String errorMsg = initializeNative();
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

   private static native String initializeNative();
}
