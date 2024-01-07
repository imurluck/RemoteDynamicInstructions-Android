//
// Created by imurluck on 2024/1/7.
//

#include "rdi_agent.h"

#define TAG "rdi_agent"

/**
 * singleton
 */
RdiAgent RdiAgent::Instance() {
    static RdiAgent instance;
    return instance;
}

void RdiAgent::OnMethodEnter(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
//    LOG_I(TAG, "onMethodEnter %p", method);
}