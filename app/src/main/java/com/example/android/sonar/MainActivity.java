package com.example.android.sonar;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;

public class MainActivity extends Activity {
    private SonarRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_layout);
        LinearLayout ll = findViewById(R.id.main_layout);
        this.renderer = new SonarRenderer();
        SonarGLSurfaceView glView = new SonarGLSurfaceView(this, this.renderer);
        Button btn = new Button(this);
        btn.setText("raise");
        btn.setLayoutParams(new LinearLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
        BtnOnClickListener clickListener = new BtnOnClickListener(this.renderer);
        btn.setOnClickListener(clickListener);
        ll.addView(btn);
        ll.addView(glView);

        this.renderer.setAngle(0);
        this.renderer.setDistance(0);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    private class BtnOnClickListener implements View.OnClickListener {
        private SonarRenderer renderer;
        private int angle;

        public BtnOnClickListener(SonarRenderer renderer)
        {
            this.renderer = renderer;
        }

        @Override
        public void onClick(View v) {
            this.renderer.setAngle(++this.angle);
            this.renderer.setDistance(this.angle % 2);
        }
    }
}