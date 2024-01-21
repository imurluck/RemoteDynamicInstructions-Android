//
// Created by imurluck on 2024/1/7.
//

#include "rdi_agent.h"
#include "jvmti_utils.h"
#include "rdi_jni.h"
#include "log_utils.h"

#define TAG "rdi_agent"

void RdiAgent::OnMethodEnter(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    if (on_method_enter_capabilities_.find(method) == on_method_enter_capabilities_.end()) {
        return;
    }
    jobjectArray argument_values;
    jvmtiError error_code = GetArguments(
            jvmti_env, jni_env, thread, method, 0, &argument_values);
    if (error_code != JVMTI_ERROR_NONE) {
        return;
    }
    JniOnMethodEnter(jni_env, method, argument_values);
}

void RdiAgent::RegisterOnMethodEnterCapability(jmethodID method_id) {
    on_method_enter_capabilities_.insert(method_id);
}