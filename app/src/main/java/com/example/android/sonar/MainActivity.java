package com.example.android.sonar;

import android.Manifest;
import android.app.AlertDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.util.Log;
import android.widget.Button;

import com.example.android.sonar.ble.BlunoLibrary;


public class MainActivity extends BlunoLibrary {
    public static final int BAUD = 115200;
    private static final String TAG = "MainActivity";
    private static final int PERMISSION_REQUEST_COARSE_LOCATION = 1;
    private Button buttonScan;

    private SonarGLSurfaceView sonarGLSurfaceView;

    private SonarRenderer renderer;

    private int angle;
    private int distance;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        this.setContentView(R.layout.activity_sonar);


        this.angle = 0;
        this.distance = 0;

        this.onCreateProcess();                                                        //onCreate Process by BlunoLibrary

        this.getNeededPermissions();
        this.serialBegin(BAUD);                                                    //set the Uart Baudrate on BLE chip to 115200

        this.sonarGLSurfaceView = findViewById(R.id.glvSonarView);
        this.buttonScan = this.findViewById(R.id.ConnectButton);                    //initial the button for scanning the BLE device
        this.buttonScan.setOnClickListener(v -> {
            MainActivity.this.buttonScanOnClickProcess();                                        //Alert Dialog for selecting the BLE device
        });

        this.renderer = new SonarRenderer();
        this.sonarGLSurfaceView.init(this.renderer);
        this.renderer.setAngle(50);
        this.renderer.setDistance(10);

    }

    @Override
    public void onRequestPermissionsResult(int requestCode,
                                           @NonNull String permissions[],
                                           @NonNull int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_REQUEST_COARSE_LOCATION: {
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Log.d(TAG, "coarse location permission granted");
                } else {
                    final AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setTitle(getString(R.string.constrained_fuctionality));
                    builder.setPositiveButton(android.R.string.ok, null);
                    builder.setOnDismissListener(dialog -> {
                    });
                    builder.show();
                }
            }
        }
    }


    protected void onResume() {
        super.onResume();
        System.out.println("BlUNOActivity onResume");
        this.onResumeProcess();                                                        //onResume Process by BlunoLibrary
    }


    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        this.onActivityResultProcess(requestCode, resultCode, data);                    //onActivityResult Process by BlunoLibrary
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    protected void onPause() {
        super.onPause();
        this.onPauseProcess();                                                        //onPause Process by BlunoLibrary
    }

    protected void onStop() {
        super.onStop();
        this.onStopProcess();                                                        //onStop Process by BlunoLibrary
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        this.onDestroyProcess();                                                        //onDestroy Process by BlunoLibrary
    }

    @Override
    public void onConectionStateChange(connectionStateEnum theConnectionState) {//Once connection state changes, this function will be called
        switch (theConnectionState) {                                            //Four connection state
            case isConnected:
                this.buttonScan.setText(getString(R.string.connected));
                break;
            case isConnecting:
                this.buttonScan.setText(getString(R.string.connecting));
                break;
            case isToScan:
                this.buttonScan.setText(getString(R.string.scan));
                break;
            case isScanning:
                this.buttonScan.setText(getString(R.string.scanning));
                break;
            case isDisconnecting:
                this.buttonScan.setText(getString(R.string.isDisconnecting));
                break;
            default:
                break;
        }
    }


    // maybe move tp Sonar View
    @Override
    public void onSerialReceived(int distance, int angle) {                            //Once connection data received, this function will be called
        this.angle = angle;
        this.distance = distance;

        this.renderer.setAngle(angle);
        this.renderer.setDistance(distance);
    }


    public int getAngle() {
        return angle;
    }

    public void setAngle(int angle) {
        this.angle = angle;
    }

    public int getDistance() {
        return distance;
    }

    public void setDistance(int distance) {
        this.distance = distance;
    }

    private void getNeededPermissions() {
        if (this.checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            final AlertDialog.Builder builder = new AlertDialog.Builder(this);
            builder.setTitle(getString(R.string.loction_permission_required));
            builder.setMessage(getString(R.string.location_permission_required_message));
            builder.setPositiveButton(android.R.string.ok, null);
            builder.setOnDismissListener(dialog -> MainActivity.this.requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, PERMISSION_REQUEST_COARSE_LOCATION));
            builder.show();
        }
    }

}