//
// Created by imurluck on 2024/1/21.
//

#ifndef RDI_RDI_BASE_H
#define RDI_RDI_BASE_H

#include "jni.h"

extern jclass g_object_class;

jobject BoxingBoolean(JNIEnv* env, jboolean bool_value);

jobject BoxingChar(JNIEnv* env, jchar char_value);

jobject BoxingByte(JNIEnv* env, jbyte byte_value);

jobject BoxingShort(JNIEnv* env, jshort short_value);

jobject BoxingInt(JNIEnv* env, jint int_value);

jobject BoxingFloat(JNIEnv* env, jfloat float_value);

jobject BoxingDouble(JNIEnv* env, jdouble double_value);

jobject BoxingLong(JNIEnv* env, jlong long_value);

void InitializePrimaryTypes(JNIEnv* env);

#endif //RDI_RDI_BASE_H
