//
// Created by imurluck on 2024/1/7.
//
#include "jvmti_utils.h"
#include "log_utils.h"
#include "vector"

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

void* getLocalVariableValue(jvmtiEnv* jvmti_env, jthread thread, jvmtiLocalVariableEntry* entry) {
    void* value_ptr = nullptr;
    jvmtiError error_code;
    switch (signatureToType(entry->signature)) {
        case Z:
        case C:
        case B:
        case S:
        case I: {
            jint value;
            error_code = jvmti_env->GetLocalInt(thread, 0, entry->slot, &value);
            LOG_D(TAG, "%s=%d", entry->name, value);
            value_ptr = &value;
            break;
        }
        case F: {
            jfloat value;
            error_code = jvmti_env->GetLocalFloat(thread, 0, entry->slot, &value);
            LOG_D(TAG, "%s=%f", entry->name, value);
            value_ptr = &value;
            break;
        }
        case D: {
            jdouble value;
            error_code = jvmti_env->GetLocalDouble(thread, 0, entry->slot, &value);
            LOG_D(TAG, "%s=%lld", entry->name, value);
            value_ptr = &value;
            break;
        }
        case J: {
            jlong value;
            error_code = jvmti_env->GetLocalLong(thread, 0, entry->slot, &value);
            LOG_D(TAG, "%s=%lld", entry->name, value);
            value_ptr = &value;
            break;
        }

        case Object: {
            jobject value;
            error_code = jvmti_env->GetLocalObject(thread, 0, entry->slot, &value);
            LOG_D(TAG, "%s=%p", entry->name, &value);
            value_ptr = &value;
            break;
        }
    }

    if (error_code != JVMTI_ERROR_NONE) {
        LOG_E(TAG, "Failed to get local variable value, name=%s, error_code=%d", entry->name, error_code);
        return nullptr;
    }

    return value_ptr;
}

jarray getArgumentValues(
        jvmtiEnv* jvmti_env,
        JNIEnv *jni_env,
        jthread thread,
        vector<jvmtiLocalVariableEntry*> argument_entries) {

    for (size_t i = 0; i < argument_entries.size(); ++i) {
        jvmtiLocalVariableEntry* entry = argument_entries[i];
        void* value = getLocalVariableValue(jvmti_env, thread, entry);

    }

    return nullptr;
}

jarray GetArguments(jvmtiEnv* jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method) {
    jint entry_count;
    jvmtiLocalVariableEntry* table_ptr;
    jvmtiError error_code;
    const char* error_msg;
    vector<jvmtiLocalVariableEntry*> argumentEntries;
    error_code = jvmti_env->GetLocalVariableTable(
            method, &entry_count, &table_ptr);
    if (error_code != JVMTI_ERROR_NONE) {
        error_msg = "Failed to GetLocalVariableTable";
        goto error;
    }

    argumentEntries = FindArgumentEntries(
            table_ptr, entry_count);
    if (argumentEntries.empty()) {
        return jni_env->NewObjectArray(0, nullptr, nullptr);
    }

    return getArgumentValues(jvmti_env, jni_env, thread, argumentEntries);

    error: {
        LOG_E(TAG, "error_code=%d, %s", error_code, error_msg);
        return nullptr;
    }
}