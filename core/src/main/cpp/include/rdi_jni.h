//
// Created by imurluck on 2024/1/14.
//

#ifndef RDI_RDI_JNI_H
#define RDI_RDI_JNI_H
#include "jni.h"

extern jmethodID testMethodId;

void JniOnMethodEnter(JNIEnv *env, jmethodID method, jobjectArray pArray);

#endif //RDI_RDI_JNI_H
