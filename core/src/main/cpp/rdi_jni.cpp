//
// Created by imurluck on 2024/1/6.
//

#include <jni.h>
#include "art_env.h"
#include "log_utils.h"
#include "rdi_agent.h"
#include "rdi_jni.h"
#include "rdi_base.h"

jmethodID testMethodId = nullptr;

#define OBJECT_CLASS_NAME "java/lang/Object"
jclass g_object_class;

#define TAG "rdi_jni"
#define RDI_JNI_CLASS_NAME "com/imurluck/rdi/core/RdiJni"

static jclass rdi_jni_class = nullptr;
static jmethodID on_method_enterId = nullptr;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    ArtHelper::Initialize(vm);
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_imurluck_rdi_core_RdiJni_initializeNative(JNIEnv *env, jclass clazz) {
    LOG_I(TAG, "initialize native");
    JavaVM* vm = nullptr;
    env->GetJavaVM(&vm);

    char *error_msg = nullptr;
    if (!InitializeArtEnv(vm, env, error_msg)) {
        return env->NewStringUTF(error_msg);
    }

    rdi_jni_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass(RDI_JNI_CLASS_NAME)));
    on_method_enterId = env->GetStaticMethodID(
            rdi_jni_class,
            "onMethodEnter",
            "(J[Ljava/lang/Object;)V");

    g_object_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass(OBJECT_CLASS_NAME)));

    InitializePrimaryTypes(env);

    // initialize success
    return nullptr;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_imurluck_rdi_core_RdiJni_findMethod(
        JNIEnv *env, jclass, jclass declaring_class, jstring name, jstring signature) {
    const char* name_chars = env->GetStringUTFChars(name, nullptr);
    const char* signature_chars = env->GetStringUTFChars(signature, nullptr);

    jmethodID methodId = env->GetMethodID(declaring_class, name_chars, signature_chars);
    if (methodId == nullptr) {
        methodId = env->GetStaticMethodID(declaring_class, name_chars, signature_chars);
    }
    env->ReleaseStringUTFChars(name, name_chars);
    env->ReleaseStringUTFChars(signature, signature_chars);
    if (methodId == nullptr) {
        return -1;
    }
    return reinterpret_cast<jlong>(methodId);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_imurluck_rdi_core_RdiJni_onMethodEnterRegistered(JNIEnv*, jclass , jlong method_id) {
    RdiAgent::Instance().RegisterOnMethodEnterCapability(
            reinterpret_cast<jmethodID>(method_id));
}

void JniOnMethodEnter(JNIEnv *env, jmethodID methodId, jobjectArray argument_values) {
    env->CallStaticVoidMethod(
            rdi_jni_class, on_method_enterId, reinterpret_cast<jlong>(methodId), argument_values);
}