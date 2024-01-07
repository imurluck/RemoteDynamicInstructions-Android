//
// Created by imurluck on 2024/1/7.
//

#ifndef RDI_RDI_AGENT_H
#define RDI_RDI_AGENT_H

#include "jvmti.h"
#include "log_utils.h"

class RdiAgent {

public:

    static RdiAgent Instance();

    void OnMethodEnter(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method);
};

#endif //RDI_RDI_AGENT_H
