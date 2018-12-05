package com.example.android.sonar;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {
    private SonarRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.renderer = new SonarRenderer();
        SonarGLSurfaceView view = new SonarGLSurfaceView(this, this.renderer);

        /* Example of setting angle and distance values for drawing
        this.renderer.setAngle(50);
        this.renderer.setDistance(10);
        */

        setContentView(view);
    }

    // TODO: Acquire values from BLE and set them as shown in example above.
}
