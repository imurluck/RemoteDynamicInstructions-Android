//
// Created by imurluck on 2024/1/6.
//
#include "art_env.h"

static void (*SetJavaDebuggable)(void*, bool) = nullptr;

void* ArtHelper::runtime_ = nullptr;

bool InitializeArtEnv(JavaVM* vm, JNIEnv* jniEnv, char* error_msg) {
    void* art = enhance::dlopen(LIB_ART_NAME, RTLD_NOW);
    if (art == nullptr) {
        error_msg = "Failed to open lib art";
        return false;
    }

    SetJavaDebuggable = reinterpret_cast<void(*)(void*, bool )>(
            enhance::dlsym(art, SYM_SET_JAVA_DEBUGGABLE));
    if (SetJavaDebuggable == nullptr) {
        error_msg = "Failed to find symbol of SetJavaDebuggable";
        goto error;
    }

    error:
    {
        enhance::dlclose(art);
        return false;
    }
}