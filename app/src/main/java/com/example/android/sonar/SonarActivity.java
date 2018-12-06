package com.example.android.sonar;

import android.app.Activity;
import android.os.Bundle;

public class SonarActivity extends Activity {
    private SonarRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.renderer = new SonarRenderer();
        SonarGLSurfaceView view = new SonarGLSurfaceView(this, this.renderer);
        this.renderer.setAngle(50);
        this.renderer.setDistance(10);
        setContentView(view);
    }
}
