//
// Created by imurluck on 2024/1/7.
//

#include "jvmti_registry.h"
#include "log_utils.h"
#include "rdi_agent.h"

#define TAG "jvmti_registry"

jint registerCapabilities(jvmtiEnv *jvmtiEnv);

void onMethodEnter(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method);

/**
 * supported capabilities
 */
const jvmtiCapabilities capabilities = {
        .can_access_local_variables = 1,
        .can_generate_method_entry_events = 1,
        .can_generate_method_exit_events = 1,
};

/**
 * supported event callbacks
 */
const jvmtiEventCallbacks eventCallbacks = {
        .MethodEntry = onMethodEnter
};

void onMethodEnter(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method) {
    RdiAgent::Instance().OnMethodEnter(jvmti_env, jni_env, thread, method);
}

jint JNICALL
Agent_OnAttach(JavaVM* vm, char* options, void* reserved) {
    jvmtiEnv* jvmtiEnv = nullptr;
    if (vm->GetEnv((void **) &jvmtiEnv, JVMTI_VERSION_1_2) != JNI_OK) {
        LOG_E(TAG, "Failed to get jvmti env of version %d", JVMTI_VERSION_1_2);
        return JNI_ERR;
    }

    return registerCapabilities(jvmtiEnv);
}

JNIEXPORT jint registerCapabilities(jvmtiEnv *jvmtiEnv) {
    jvmtiError error = jvmtiEnv->AddCapabilities(&capabilities);
    if (error != JVMTI_ERROR_NONE) {
        LOG_E(TAG, "Failed to add capabilities");
        return error;
    }

    error = jvmtiEnv->SetEventCallbacks(
            &eventCallbacks, sizeof(jvmtiEventCallbacks));
    if (error != JVMTI_ERROR_NONE) {
        LOG_E(TAG, "Failed to set event callback");
        return error;
    }

    error = jvmtiEnv->SetEventNotificationMode(
            JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, nullptr);
    if (error != JVMTI_ERROR_NONE) {
        LOG_E(TAG, "Failed to enable JVMTI_EVENT_METHOD_ENTRY");
        return error;
    }
    return JVMTI_ERROR_NONE;
}


