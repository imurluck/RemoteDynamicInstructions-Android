package com.imurluck.rdi.core;

import android.app.Application;
import android.os.Build;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Enumeration;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

/**
 * @author imurluck
 */
class NativeLibraryExtractor {

   private static final String ROOT_DIR_NAME = "rdi";

   /**
    * extract native lib from source apk to a file
    *
    * @return target file
    *
    * @throws IOException to many exceptions may occur of IO
    */
   @NonNull
   File extract(@NonNull Application application, @NonNull String libraryName) throws IOException {
      String sourceApkPath = application.getApplicationInfo().sourceDir;
      File targetFile = null;
      try (ZipFile zipFile = new ZipFile(sourceApkPath)) {
         String targetAbi = getTargetAbi(zipFile);
         if (targetAbi == null) {
            throw new IOException("There are no supported abi in source apk");
         }
         targetFile = getTargetFile(application, targetAbi, libraryName);
         // todo support cache by sdk version
         copyLibrary(targetFile, targetAbi, zipFile, libraryName);
      }

      return targetFile;
   }

   private void copyLibrary(
           @NonNull File targetFile,
           @NonNull String targetAbi,
           @NonNull ZipFile apkFile,
           @NonNull String libraryName) throws IOException {

      String libraryEntryName = "lib/" + targetAbi + "/" + libraryName;
      ZipEntry entry = apkFile.getEntry(libraryEntryName);
      if (entry == null) {
         throw new IOException(
                 String.format(Locale.ENGLISH, "No such library %s in source apk", libraryName));
      }

      // // todo temp code, need pack to file-util classes
      targetFile.getParentFile().mkdirs();
      boolean ignore = targetFile.createNewFile();
      int len = 0;
      byte[] buffer = new byte[4096];
      try (InputStream is = apkFile.getInputStream(entry);
           FileOutputStream fos = new FileOutputStream(targetFile)) {
         while ((len = is.read(buffer)) != -1) {
            fos.write(buffer, 0, len);
         }
      }
   }

   @NonNull
   private File getTargetFile(
           @NonNull Application application, @NonNull String abi, @NonNull String libraryName) {

      return new File(
              Configurations.getRootDir(application), abi + File.separator + libraryName);
   }
   @Nullable
   private String getTargetAbi(@NonNull ZipFile apkFile) throws IOException {
      Set<String> sourceApkAbiSet = getSourceApkAbiSet(apkFile);
      if (sourceApkAbiSet.isEmpty()) {
         throw new IOException("There are no so files in source apk");
      }

      final String[] supportedABIs = Build.SUPPORTED_ABIS;
      String targetAbi = null;
      for (String abi : supportedABIs) {
         if (sourceApkAbiSet.contains(abi)) {
            targetAbi = abi;
         }
      }
      return targetAbi;
   }

   @NonNull
   private Set<String> getSourceApkAbiSet(@NonNull ZipFile apkFile) {
      final Enumeration<? extends ZipEntry> entries = apkFile.entries();
      final Set<String> abiSet = new HashSet<>();
      while (entries.hasMoreElements()) {
         final ZipEntry entry = entries.nextElement();
         final String entryName = entry.getName();
         if (entryName.endsWith(".so")) {
            abiSet.add(entryName.split("/")[1]);
         }
      }
      return abiSet;
   }
}
