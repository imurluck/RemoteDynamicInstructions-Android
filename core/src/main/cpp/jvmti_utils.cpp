//
// Created by imurluck on 2024/1/7.
//
#include "jvmti_utils.h"
#include "log_utils.h"
#include "vector"
#include "rdi_jni.h"
#include "rdi_base.h"

#define TAG "jvmti_utils"

Type signatureToType(const char* signature) {
    if (::strcmp(signature, "Z") == 0) {
        return Z;
    }

    if (::strcmp(signature, "C") == 0) {
        return C;
    }

    if (::strcmp(signature, "B") == 0) {
        return B;
    }

    if (::strcmp(signature, "S") == 0) {
        return S;
    }

    if (::strcmp(signature, "I") == 0) {
        return I;
    }

    if (::strcmp(signature, "F") == 0) {
        return F;
    }

    if (::strcmp(signature, "D") == 0) {
        return D;
    }

    if (::strcmp(signature, "J") == 0) {
        return J;
    }

    return Object;
}

 vector<jvmtiLocalVariableEntry*> FindArgumentEntries(
         jvmtiLocalVariableEntry* table_ptr, int total_entry_count) {

     vector<jvmtiLocalVariableEntry*> arguments;
    for (int i = 0; i < total_entry_count; ++i) {
        // only need judgement start_location
        if (table_ptr->start_location == 0) {
            arguments.push_back(table_ptr);
            table_ptr++;
        }
    }
    return arguments;
}

jvmtiError getLocalVariableValue(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread,
                      jvmtiLocalVariableEntry *entry, int depth, jobjectArray out, jsize index) {

    jvmtiError error_code;
    if (::strcmp("this", entry->name) == 0) {
        jobject instance;
        error_code = jvmti_env->GetLocalInstance(thread, depth, &instance);
        if (error_code != JVMTI_ERROR_NONE) {
            LOG_E(TAG, "Failed to GetLocalInstance, depth=%d, error_code=%d", depth, error_code);
            return error_code;
        }
        jni_env->SetObjectArrayElement(out, index, instance);
        return JVMTI_ERROR_NONE;
    }

    jobject argument_value = nullptr;
    switch (signatureToType(entry->signature)) {
        case Z: {
            jint value;
            error_code = jvmti_env->GetLocalInt(thread, 0, entry->slot, &value);
            argument_value = BoxingBoolean(jni_env, value);
            break;
        }
        case C: {
            jint value;
            error_code = jvmti_env->GetLocalInt(thread, 0, entry->slot, &value);
            argument_value = BoxingChar(jni_env, value);
            break;
        }
        case B: {
            jint value;
            error_code = jvmti_env->GetLocalInt(thread, 0, entry->slot, &value);
            argument_value = BoxingByte(jni_env, (jbyte) value);
            break;
        }
        case S: {
            jint value;
            error_code = jvmti_env->GetLocalInt(thread, 0, entry->slot, &value);
            argument_value = BoxingShort(jni_env, (jshort) value);
            break;
        }
        case I: {
            jint value;
            error_code = jvmti_env->GetLocalInt(thread, 0, entry->slot, &value);
            argument_value = BoxingInt(jni_env, value);
            break;
        }
        case F: {
            jfloat value;
            error_code = jvmti_env->GetLocalFloat(thread, 0, entry->slot, &value);
            argument_value = BoxingFloat(jni_env, value);
            break;
        }
        case D: {
            jdouble value;
            error_code = jvmti_env->GetLocalDouble(thread, 0, entry->slot, &value);
            argument_value = BoxingDouble(jni_env, value);
            break;
        }
        case J: {
            jlong value;
            error_code = jvmti_env->GetLocalLong(thread, 0, entry->slot, &value);
            argument_value = BoxingLong(jni_env, value);
            break;
        }

        case Object: {
            error_code = jvmti_env->GetLocalObject(thread, 0, entry->slot, &argument_value);
            break;
        }
    }

    if (error_code != JVMTI_ERROR_NONE) {
        LOG_E(TAG,
              "Failed to get local variable value, name=%s, error_code=%d", entry->name, error_code);
        return error_code;
    }

    jni_env->SetObjectArrayElement(out, index, static_cast<jobject>(argument_value));
    return JVMTI_ERROR_NONE;
}

jvmtiError getArgumentValues(jvmtiEnv* jvmti_env, JNIEnv *jni_env, jthread thread,
        vector<jvmtiLocalVariableEntry*> argument_entries, int depth, jobjectArray out) {

    for (size_t i = 0; i < argument_entries.size(); ++i) {
        jvmtiLocalVariableEntry* entry = argument_entries[i];
        jvmtiError error_code = getLocalVariableValue(
                jvmti_env, jni_env, thread, entry, depth, out, (jsize) i);
        if (error_code != JVMTI_ERROR_NONE) {
            return error_code;
        }
    }

    return JVMTI_ERROR_NONE;
}

jvmtiError GetArguments(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jthread thread, jmethodID method,
                        int depth, jobjectArray* out) {

    jint entry_count;
    jvmtiLocalVariableEntry* table_ptr;
    jvmtiError error_code = jvmti_env->GetLocalVariableTable(
            method, &entry_count, &table_ptr);
    if (error_code != JVMTI_ERROR_NONE) {
        LOG_E(TAG, "Failed to GetLocalVariableTable, %d", error_code);
        return error_code;
    }

    vector<jvmtiLocalVariableEntry*> argument_entries = FindArgumentEntries(
            table_ptr, entry_count);
    // no arguments, return a 0-length array
    if (argument_entries.empty()) {
        *out = jni_env->NewObjectArray(0, g_object_class, nullptr);
        return JVMTI_ERROR_NONE;
    }

    *out = jni_env->NewObjectArray(
            (jsize) argument_entries.size(), g_object_class, nullptr);

    return getArgumentValues(jvmti_env, jni_env, thread, argument_entries, depth, *out);
}