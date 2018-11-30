package com.example.android.sonar;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class SonarRenderer implements GLSurfaceView.Renderer {
    private int distance, angle;

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        SonarJNI.surface_created();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        SonarJNI.surface_changed(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        SonarJNI.draw(this.distance, this.angle);
    }

    public void setDistance(int distance) {
        this.distance = distance;
    }

    public void setAngle(int angle) {
        this.angle = angle;
    }
}
