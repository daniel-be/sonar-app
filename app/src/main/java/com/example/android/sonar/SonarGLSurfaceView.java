package com.example.android.sonar;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class SonarGLSurfaceView extends GLSurfaceView {

    public SonarGLSurfaceView(Context context, AttributeSet attributeSet) {
        super(context, attributeSet);
    }

    public void init(GLSurfaceView.Renderer renderer) {
        this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setEGLContextClientVersion(2);
        setRenderer(renderer);
    }
}
