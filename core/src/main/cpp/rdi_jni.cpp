//
// Created by imurluck on 2024/1/6.
//

#include <jni.h>
#include "art_env.h"
#include "log_utils.h"
#include "rdi_agent.h"

#define TAG "rdi_jni"

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    ArtHelper::Initialize(vm);
    return JNI_VERSION_1_6;
}

jmethodID localVariableTest;
/**
 * temp test code
 */
void getLocalVariableTestID(JNIEnv* jniEnv) {

    jclass mainActivityClass = jniEnv->FindClass("com/imurluck/rdi/MainActivity");
    localVariableTest = jniEnv->GetMethodID(
            mainActivityClass, "localVariableTest",
            "(ZCBSIFDJLjava/lang/Object;[Ljava/lang/Object;)V");
    LOG_I(TAG, "find local variable test method success");
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_imurluck_rdi_core_Rdi_initializeNative(JNIEnv *env, jclass clazz) {
    LOG_I(TAG, "initialize native");
    JavaVM* vm = nullptr;
    env->GetJavaVM(&vm);

    char *error_msg = nullptr;
    if (!InitializeArtEnv(vm, env, error_msg)) {
        goto failed;
    }

    getLocalVariableTestID(env);

    // initialize success
    return nullptr;

    failed:
    {
        return env->NewStringUTF(error_msg);
    }
}