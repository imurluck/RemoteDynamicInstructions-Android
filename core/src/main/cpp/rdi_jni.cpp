//
// Created by imurluck on 2024/1/6.
//

#include <jni.h>
#include "art_env.h"
#include "log_utils.h"

#define TAG "rdi_jni"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    ArtHelper::Initialize(vm);
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_imurluck_rdi_core_Rdi_initializeNative(JNIEnv *env, jclass clazz) {
    JavaVM* vm = nullptr;
    env->GetJavaVM(&vm);

    char *error_msg = nullptr;
    if (!InitializeArtEnv(vm, env, error_msg)) {
        goto failed;
    }

    // initialize success
    return nullptr;

    failed:
    {
        return env->NewStringUTF(error_msg);
    }
}