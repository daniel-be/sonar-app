//
// Created by Daniel on 29.11.2018.
//

#ifndef SONAR_APP_LOG_H
#define SONAR_APP_LOG_H

#include <android/log.h>

#ifndef NDEGUB
    #define LOG_I(...) ((void)__android_log_print(ANDROID_LOG_INFO, __VA_ARGS__))
    #define LOG_W(...) ((void)__android_log_print(ANDROID_LOG_WARN, __VA_ARGS__))
#else
    #define LOG_I(...) ((void)0)
    #define LOG_W(...) ((void)0)
#endif

#define LOG_E(...) (__android_log_print(ANDROID_LOG_ERROR, __VA_ARGS__))

#define GL_CALL(func) while (glGetError() != GL_NO_ERROR);\
    func;\
    while (unsigned int error = glGetError())\
    {\
        (void)__android_log_print(ANDROID_LOG_ERROR, "OpenGL function call",\
        "Error (%d) in %s %s:%d", error, #func, __FILE__, __LINE__);\
    }\

#endif //SONAR_APP_LOG_H
