package com.example.android.sonar;

public class SonarJNI {
    static {
        System.loadLibrary("native-lib");
    }

    public static native void surface_created();
    public static native void surface_changed(int width, int height);
    public static native void draw(float distance, int angle);
}
