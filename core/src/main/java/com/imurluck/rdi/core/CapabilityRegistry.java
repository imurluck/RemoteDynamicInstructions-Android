package com.imurluck.rdi.core;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import com.imurluck.rdi.api.ICapability;
import com.imurluck.rdi.api.IOnMethodEnterCapability;
import com.imurluck.rdi.core.exception.CapabilityRegisterException;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author imurluck
 * @date 2024/1/14
 */
class CapabilityRegistry {

   private final Map<Long, List<IOnMethodEnterCapability>> mOnMethodEnterCapabilities = new HashMap<>();

   private final RdiAgent mRdiAgent;

   public CapabilityRegistry(@NonNull RdiAgent agent) {
      this.mRdiAgent = agent;
   }

   public void registerCapability(@NonNull ICapability capability) throws CapabilityRegisterException {
      if (capability instanceof IOnMethodEnterCapability) {
         registerOnMethodEnterCapability((IOnMethodEnterCapability) capability);
         return;
      }

      throw new CapabilityRegisterException("Unknown capability of " + capability.getClass().getName());
   }

   private void registerOnMethodEnterCapability(
           @NonNull IOnMethodEnterCapability capability) throws CapabilityRegisterException {

      // use host app class loader
      ClassLoader classLoader = this.getClass().getClassLoader();
      String declaringClassName = capability.getDeclaringClass();
      Class<?> declaringClass = null;
      try {
         declaringClass = Class.forName(declaringClassName, false, classLoader);
      } catch (ClassNotFoundException e) {
         throw new CapabilityRegisterException("Failed to find class " + declaringClassName, e);
      }
      long methodId = RdiJni.findMethod(
              declaringClass, capability.getName(), capability.getSignature());
      if (methodId == -1) {
         throw new CapabilityRegisterException(
                 String.format(
                         "Failed to find method %s%s decalred in %s",
                         capability.getName(),
                         capability.getSignature(),
                         declaringClassName));
      }
      // use class of IOnMethodEnterCapability as a lock to only lock about OnMethodEnter capability
      synchronized (IOnMethodEnterCapability.class) {
         List<IOnMethodEnterCapability> capabilities = mOnMethodEnterCapabilities.get(methodId);
         if (capabilities == null) {
            capabilities = new ArrayList<>();
            mOnMethodEnterCapabilities.put(methodId, capabilities);
            RdiJni.onMethodEnterRegistered(methodId);
         }
         capabilities.add(capability);
      }
   }

   @Nullable
   public List<IOnMethodEnterCapability> getOnMethodEnterCapabilities(long methodId) {
      return mOnMethodEnterCapabilities.get(methodId);
   }
}
