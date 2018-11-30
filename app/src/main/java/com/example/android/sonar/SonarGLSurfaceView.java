package com.example.android.sonar;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;

public class SonarGLSurfaceView extends GLSurfaceView {
    public SonarGLSurfaceView(Context context, GLSurfaceView.Renderer renderer) {
        super(context);
        init(renderer);
    }

    private void init(GLSurfaceView.Renderer renderer) {
        this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
        setRenderer(renderer);
    }
}
