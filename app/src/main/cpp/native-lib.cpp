#include <jni.h>
#include <string>

#include "engine.hpp"

extern "C" {
    JNIEXPORT void JNICALL Java_com_example_android_sonar_SonarJNI_surface_1created(JNIEnv* env, jclass type)
    {
        Engine::init();
    }

    JNIEXPORT void JNICALL Java_com_example_android_sonar_SonarJNI_surface_1changed(JNIEnv* env, jclass type,
                                                                                    jint width, jint height)
    {
        Engine::setup_graphics(width, height);
    }

    JNIEXPORT void JNICALL Java_com_example_android_sonar_SonarJNI_draw(JNIEnv* env, jclass type,
                                                                        jint distance, jint angle)
    {
        Engine::draw_frame(distance, angle);
    }
}

