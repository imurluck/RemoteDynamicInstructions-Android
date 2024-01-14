package com.imurluck.rdi;

import android.app.Application;

import com.imurluck.rdi.core.Rdi;
import com.imurluck.rdi.core.exception.InitializeException;

/**
 * @author imurluck
 * @date 2024/1/14
 */
public class App extends Application {

   @Override
   public void onCreate() {
      super.onCreate();

//      try {
//         Rdi.initialize(this);
//      } catch (InitializeException e) {
//         throw new RuntimeException(e);
//      }
   }
}
