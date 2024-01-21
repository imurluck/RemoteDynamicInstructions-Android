//
// Created by imurluck on 2024/1/7.
//

#ifndef RDI_JVMTI_UTILS_H
#define RDI_JVMTI_UTILS_H

#include "jvmti.h"
#include "jni.h"
#include "vector"

using namespace std;

typedef enum {
    Z, // boolean
    C, // char
    B, // byte
    S, // short
    I, // int
    F, // float
    D, // double
    J, // long
    Object // object & array
} Type;

vector<jvmtiLocalVariableEntry*> FindArgumentEntries(
        jvmtiLocalVariableEntry* table_ptr, int total_entry_count);

jvmtiError GetArguments(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method,
                        int depth, jobjectArray* out);

#endif //RDI_JVMTI_UTILS_H
