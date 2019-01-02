#include <string>
#include <memory>

#include <jni.h>
#include <GLES2/gl2.h>

#include "engine.hpp"

Engine engine;

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_android_sonar_SonarJNI_surface_1created(JNIEnv* env, jclass type)
    {
        engine.init();
    }

    JNIEXPORT void JNICALL Java_com_example_android_sonar_SonarJNI_surface_1changed(JNIEnv* env, jclass type,
                                                                                    jint width, jint height)
    {
        engine.change(width, height);
    }

    JNIEXPORT void JNICALL Java_com_example_android_sonar_SonarJNI_draw(JNIEnv* env, jclass type,
                                                                        jfloat distance, jint angle)
    {
        engine.draw_frame(distance, angle);
    }
}

