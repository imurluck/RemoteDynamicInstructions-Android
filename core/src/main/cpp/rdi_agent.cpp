//
// Created by imurluck on 2024/1/7.
//

#include "rdi_agent.h"
#include "jvmti_utils.h"
#include "rdi_jni.h"
#include "log_utils.h"

#define TAG "rdi_agent"

/**
 * singleton
 */
RdiAgent RdiAgent::Instance() {
    static RdiAgent instance;
    return instance;
}

void RdiAgent::OnMethodEnter(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    char* name;
    char* signature;
    char* place_holder;
    jvmti_env->GetMethodName(method, &name, &signature, &place_holder);
    LOG_D(TAG, "onMethodEnter %s%s %lld", name, signature, method);
    if (on_method_enter_capabilities_.find(method) == on_method_enter_capabilities_.end() ||
        testMethodId == method) {
        return;
    }
    GetArguments(jvmti_env, jni_env, thread, method);
    JniOnMethodEnter(jni_env, method);
}

void RdiAgent::RegisterOnMethodEnterCapability(jmethodID methodId) {

    on_method_enter_capabilities_.insert(methodId);
}