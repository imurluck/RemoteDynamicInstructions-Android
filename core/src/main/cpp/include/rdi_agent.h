//
// Created by imurluck on 2024/1/7.
//

#ifndef RDI_RDI_AGENT_H
#define RDI_RDI_AGENT_H

#include "jvmti.h"
#include "log_utils.h"
#include "unordered_set"

using namespace std;

class RdiAgent {

private:
    RdiAgent() = default;

    ~RdiAgent() = default;

    unordered_set<jmethodID> on_method_enter_capabilities_;

    RdiAgent(const RdiAgent& other) = delete;
    RdiAgent& operator=(const RdiAgent& other) = delete;

public:

    /**
     * singleton
     */
    static RdiAgent& Instance() {
        static RdiAgent instance;
        return instance;
    }

    void OnMethodEnter(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method);

    void RegisterOnMethodEnterCapability(jmethodID method_id);
};

#endif //RDI_RDI_AGENT_H
