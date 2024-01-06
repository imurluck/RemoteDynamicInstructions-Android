//
// Created by imurluck on 2024/1/6.
//

#ifndef RDI_ART_ENV_H
#define RDI_ART_ENV_H

#include <jni.h>
#include <dlfcn.h>
#include "EnhanceDlsym.h"

#define LIB_ART_NAME "libart.so"
#define SYM_SET_JAVA_DEBUGGABLE "_ZN3art7Runtime17SetJavaDebuggableEb"

bool InitializeArtEnv(JavaVM* javaVm, JNIEnv* jniEnv, char* error_msg);

namespace mirror {

    struct JavaVmExt {

        void* functions;

        void* runtime;
    };
}

class ArtHelper {

private:
    static void *runtime_;

public:
    static void Initialize(JavaVM* vm) {
        mirror::JavaVmExt* mirror_vm = (mirror::JavaVmExt*) vm;
        ArtHelper::runtime_ = mirror_vm->runtime;
    }

    static void *getRuntime() {
        return runtime_;
    }
};

#endif //RDI_ART_ENV_H
