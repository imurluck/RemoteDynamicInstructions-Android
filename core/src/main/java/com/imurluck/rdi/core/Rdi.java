//MIT License
//
//Copyright (c) 2024 imurluck
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in all
//copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//SOFTWARE.

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
