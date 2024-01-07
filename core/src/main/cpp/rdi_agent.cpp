//
// Created by imurluck on 2024/1/7.
//

#include "rdi_agent.h"
#include "jvmti_utils.h"

#define TAG "rdi_agent"

/**
 * singleton
 */
RdiAgent RdiAgent::Instance() {
    static RdiAgent instance;
    return instance;
}

void RdiAgent::OnMethodEnter(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    if (method != localVariableTest) {
        return;
    }
    LOG_I(TAG, "localVariableTest method enter");
    GetArguments(jvmti_env, jni_env, thread, method);
}