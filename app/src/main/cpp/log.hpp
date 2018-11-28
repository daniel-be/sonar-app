//
// Created by Daniel on 28.11.2018.
//

#ifndef SONAR_APP_LOG_HPP
#define SONAR_APP_LOG_HPP

#include <android/log.h>

#ifndef NDEBUG
#define LOG_I(...) __android_log_print(ANDROID_LOG_INFO, __VA_ARGS__)
#define LOG_W(...) __android_log_print(ANDROID_LOG_WARN, __VA_ARGS__)
#else
#define LOG_I(...)
    #define LOG_W(...)
#endif

#define LOG_E(...) __android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__)

#endif //SONAR_APP_LOG_HPP
