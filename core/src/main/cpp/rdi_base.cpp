//
// Created by imurluck on 2024/1/21.
//
#include "rdi_base.h"

jclass boolean_class;
jmethodID boolean_constructor;

jclass char_class;
jmethodID char_constructor;

jclass byte_class;
jmethodID byte_constructor;

jclass short_class;
jmethodID short_constructor;

jclass integer_class;
jmethodID integer_constructor;

jclass float_class;
jmethodID float_constructor;

jclass double_class;
jmethodID double_constructor;

jclass long_class;
jmethodID long_constructor;

jobject BoxingBoolean(JNIEnv* env, jboolean bool_value) {
    return env->NewObject(boolean_class, boolean_constructor, bool_value);
}

jobject BoxingChar(JNIEnv* env, jchar char_value) {
    return env->NewObject(char_class, char_constructor, char_value);
}

jobject BoxingByte(JNIEnv* env, jbyte byte_value) {
    return env->NewObject(byte_class, byte_constructor, byte_value);
}

jobject BoxingShort(JNIEnv* env, jshort short_value) {
    return env->NewObject(short_class, short_constructor, short_value);
}

jobject BoxingInt(JNIEnv* env, jint int_value) {
    return env->NewObject(integer_class, integer_constructor, int_value);
}

jobject BoxingFloat(JNIEnv* env, jfloat float_value) {
    return env->NewObject(float_class, float_constructor, float_value);
}

jobject BoxingDouble(JNIEnv* env, jdouble double_value) {
    return env->NewObject(double_class, double_constructor, double_value);
}

jobject BoxingLong(JNIEnv* env, jlong long_value) {
    return env->NewObject(long_class, long_constructor, long_value);
}

void InitializePrimaryTypes(JNIEnv* env) {
    boolean_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Boolean")));
    boolean_constructor = env->GetMethodID(boolean_class, "<init>", "(Z)V");

    char_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Character")));
    char_constructor = env->GetMethodID(char_class, "<init>", "(C)V");

    byte_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Byte")));
    byte_constructor = env->GetMethodID(byte_class, "<init>", "(B)V");

    short_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Short")));
    short_constructor = env->GetMethodID(short_class, "<init>", "(S)V");

    integer_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Integer")));
    integer_constructor = env->GetMethodID(integer_class, "<init>", "(I)V");

    float_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Float")));
    float_constructor = env->GetMethodID(float_class, "<init>", "(F)V");

    double_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Double")));
    double_constructor = env->GetMethodID(double_class, "<init>", "(D)V");

    long_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/Long")));
    long_constructor = env->GetMethodID(long_class, "<init>", "(J)V");
}

