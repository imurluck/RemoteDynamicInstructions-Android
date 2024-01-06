//
// Created by imurluck on 2024/1/6.
//

#ifndef RDI_LOG_UTILS_H
#define RDI_LOG_UTILS_H

#include <jni.h>
#include <android/log.h>

#define LOGE(tag, fmt, args...) __android_log_print(ANDROID_LOG_ERROR, tag, fmt, ##args)

#endif //RDI_LOG_UTILS_H
