package com.example.android.sonar.ble;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.ParcelUuid;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import android.widget.Toast;

import com.example.android.sonar.R;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


public abstract class BlunoLibrary extends Activity {

    public static final String SerialPortUUID = "0000dfb1-0000-1000-8000-00805f9b34fb";

    public static final String CommandUUID = "0000dfb2-0000-1000-8000-00805f9b34fb";
    public static final String ModelNumberStringUUID = "00002a24-0000-1000-8000-00805f9b34fb";
    public static final String BLUNO_UUID = "0000dfb0-0000-1000-8000-00805f9b34fb";
    private static final int REQUEST_ENABLE_BT = 1;
    private final static String TAG = BlunoLibrary.class.getSimpleName();
    private static BluetoothGattCharacteristic mSCharacteristic, mModelNumberCharacteristic, mSerialPortCharacteristic, mCommandCharacteristic;
    private final String mPassword = "AT+PASSWOR=DFRobot\r\n";
    private final Context mainContext = this;
    private final Handler mHandler = new Handler();
    public connectionStateEnum mConnectionState = connectionStateEnum.isNull;
    public boolean mConnected = false;
    BluetoothLeService mBluetoothLeService;
    private final Runnable mConnectingOverTimeRunnable = () -> {
        if (BlunoLibrary.this.mConnectionState == connectionStateEnum.isConnecting)
            BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
        BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
        BlunoLibrary.this.mBluetoothLeService.close();
    };
    private final Runnable mDisonnectingOverTimeRunnable = () -> {
        if (BlunoLibrary.this.mConnectionState == connectionStateEnum.isDisconnecting)
            BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
        BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
        BlunoLibrary.this.mBluetoothLeService.close();
    };
    // Code to manage Service lifecycle.
    final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            System.out.println("mServiceConnection onServiceConnected");
            BlunoLibrary.this.mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!BlunoLibrary.this.mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                ((Activity) BlunoLibrary.this.mainContext).finish();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            System.out.println("mServiceConnection onServiceDisconnected");
            BlunoLibrary.this.mBluetoothLeService = null;
        }
    };
    AlertDialog mScanDeviceDialog;
    private int mBaudrate = 115200;    //set the default baud rate to 115200
    private String mBaudrateBuffer = "AT+CURRUART=" + this.mBaudrate + "\r\n";
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<>();
    // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @SuppressLint("DefaultLocale")
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();
            System.out.println("mGattUpdateReceiver->onReceive->action=" + action);
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                BlunoLibrary.this.mConnected = true;
                BlunoLibrary.this.mHandler.removeCallbacks(BlunoLibrary.this.mConnectingOverTimeRunnable);

            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                BlunoLibrary.this.mConnected = false;
                BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
                BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
                BlunoLibrary.this.mHandler.removeCallbacks(BlunoLibrary.this.mDisonnectingOverTimeRunnable);
                BlunoLibrary.this.mBluetoothLeService.close();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
                for (BluetoothGattService gattService : BlunoLibrary.this.mBluetoothLeService.getSupportedGattServices()) {
                    System.out.println("ACTION_GATT_SERVICES_DISCOVERED  " +
                            gattService.getUuid().toString());
                }
                BlunoLibrary.this.getGattServices(BlunoLibrary.this.mBluetoothLeService.getSupportedGattServices());
            } else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
                if (mSCharacteristic == mModelNumberCharacteristic) {
                    if (intent.getStringExtra(BluetoothLeService.EXTRA_DATA).toUpperCase().startsWith("DF BLUNO")) {
                        BlunoLibrary.this.mBluetoothLeService.setCharacteristicNotification(mSCharacteristic, false);
                        mSCharacteristic = mCommandCharacteristic;
                        mSCharacteristic.setValue(BlunoLibrary.this.mPassword);
                        BlunoLibrary.this.mBluetoothLeService.writeCharacteristic(mSCharacteristic);
                        mSCharacteristic.setValue(BlunoLibrary.this.mBaudrateBuffer);
                        BlunoLibrary.this.mBluetoothLeService.writeCharacteristic(mSCharacteristic);
                        mSCharacteristic = mSerialPortCharacteristic;
                        BlunoLibrary.this.mBluetoothLeService.setCharacteristicNotification(mSCharacteristic, true);
                        BlunoLibrary.this.mConnectionState = connectionStateEnum.isConnected;
                        BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);

                    } else {
                        Toast.makeText(BlunoLibrary.this.mainContext, "Please select DFRobot devices", Toast.LENGTH_SHORT).show();
                        BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
                        BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
                    }
                } else if (mSCharacteristic == mSerialPortCharacteristic) {
                    BlunoLibrary.this.onSerialReceived(intent.getIntExtra(BluetoothLeService.DISTANCE, -1), intent.getIntExtra(BluetoothLeService.ANGLE, -1));

                }


                System.out.println("displayData " + intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
            }
        }
    };
    private LeDeviceListAdapter mLeDeviceListAdapter = null;

    // Device scan callback.
    private final ScanCallback leScanCallback = new ScanCallback() {
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            super.onScanResult(callbackType, result);
            System.out.println("mLeScanCallback onLeScan run ");
            BluetoothDevice device = result.getDevice();
            BlunoLibrary.this.mLeDeviceListAdapter.addDevice(device);
            BlunoLibrary.this.mLeDeviceListAdapter.notifyDataSetChanged();
        }

        @Override
        public void onBatchScanResults(List<ScanResult> results) {
            super.onBatchScanResults(results);

            Log.d(TAG, "received batch result");
        }

        @Override
        public void onScanFailed(int errorCode) {
            super.onScanFailed(errorCode);

            Log.d(TAG, "scan failed " + errorCode);
        }
    };
    private BluetoothAdapter mBluetoothAdapter;
    private boolean mScanning = false;
    private String mDeviceName;
    private String mDeviceAddress;


    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        return intentFilter;
    }


    public abstract void onConectionStateChange(connectionStateEnum theconnectionStateEnum);

    protected abstract void onSerialReceived(int distance, int angle);

    public void serialSend(String theString) {
        if (this.mConnectionState == connectionStateEnum.isConnected) {
            mSCharacteristic.setValue(theString);
            this.mBluetoothLeService.writeCharacteristic(mSCharacteristic);
        }
    }

    public void serialBegin(int baud) {
        this.mBaudrate = baud;
        this.mBaudrateBuffer = "AT+CURRUART=" + this.mBaudrate + "\r\n";
    }

    public void onCreateProcess() {
        if (!this.initiate()) {
            Toast.makeText(this.mainContext, R.string.error_bluetooth_not_supported,
                    Toast.LENGTH_SHORT).show();
            ((Activity) this.mainContext).finish();
        }


        Intent gattServiceIntent = new Intent(this, BluetoothLeService.class);
        this.bindService(gattServiceIntent, this.mServiceConnection, Context.BIND_AUTO_CREATE);

        // Initializes list view adapter.
        this.mLeDeviceListAdapter = new LeDeviceListAdapter();
        // Initializes and show the scan Device Dialog
        this.mScanDeviceDialog = new AlertDialog.Builder(this.mainContext)
                .setTitle("BLE Device Scan...").setAdapter(this.mLeDeviceListAdapter, (dialog, which) -> {
                    final BluetoothDevice device = BlunoLibrary.this.mLeDeviceListAdapter.getDevice(which);
                    if (device == null)
                        return;
                    BlunoLibrary.this.scanLeDevice(false);

                    if (device.getName() == null || device.getAddress() == null) {
                        BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
                        BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
                    } else {

                        System.out.println("onListItemClick " + device.getName());

                        System.out.println("Device Name:" + device.getName() + "   " + "Device Name:" + device.getAddress());

                        BlunoLibrary.this.mDeviceName = device.getName();
                        BlunoLibrary.this.mDeviceAddress = device.getAddress();

                        if (BlunoLibrary.this.mBluetoothLeService.connect(BlunoLibrary.this.mDeviceAddress)) {
                            Log.d(TAG, "Connect request success");
                            BlunoLibrary.this.mConnectionState = connectionStateEnum.isConnecting;
                            BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
                            BlunoLibrary.this.mHandler.postDelayed(BlunoLibrary.this.mConnectingOverTimeRunnable, 10000);
                        } else {
                            Log.d(TAG, "Connect request fail");
                            BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
                            BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
                        }
                    }
                })
                .setOnCancelListener(arg0 -> {
                    System.out.println("mBluetoothAdapter.stopLeScan");

                    BlunoLibrary.this.mConnectionState = connectionStateEnum.isToScan;
                    BlunoLibrary.this.onConectionStateChange(BlunoLibrary.this.mConnectionState);
                    BlunoLibrary.this.mScanDeviceDialog.dismiss();

                    BlunoLibrary.this.scanLeDevice(false);
                }).create();


    }

    public void onResumeProcess() {
        System.out.println("BlUNOActivity onResume");
        // Ensures Bluetooth is enabled on the device. If Bluetooth is not
        // currently enabled,
        // fire an intent to display a dialog asking the user to grant
        // permission to enable it.
        if (!this.mBluetoothAdapter.isEnabled()) {
            if (!this.mBluetoothAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(
                        BluetoothAdapter.ACTION_REQUEST_ENABLE);
                ((Activity) this.mainContext).startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }
        }


        this.mainContext.registerReceiver(this.mGattUpdateReceiver, makeGattUpdateIntentFilter());

    }

    public void onPauseProcess() {
        System.out.println("BLUNOActivity onPause");
        this.scanLeDevice(false);
        this.mainContext.unregisterReceiver(this.mGattUpdateReceiver);
        this.mLeDeviceListAdapter.clear();
        this.mConnectionState = connectionStateEnum.isToScan;
        this.onConectionStateChange(this.mConnectionState);
        this.mScanDeviceDialog.dismiss();
        if (this.mBluetoothLeService != null) {
            this.mBluetoothLeService.disconnect();
            this.mHandler.postDelayed(this.mDisonnectingOverTimeRunnable, 10000);
        }
        mSCharacteristic = null;

    }

    public void onStopProcess() {
        System.out.println("MiUnoActivity onStop");
        if (this.mBluetoothLeService != null) {

            this.mHandler.removeCallbacks(this.mDisonnectingOverTimeRunnable);
            this.mBluetoothLeService.close();
        }
        mSCharacteristic = null;
    }

    public void onDestroyProcess() {
        this.mainContext.unbindService(this.mServiceConnection);
        this.mBluetoothLeService = null;
    }

    public void onActivityResultProcess(int requestCode, int resultCode, Intent data) {
        // User chose not to enable Bluetooth.
        if (requestCode == REQUEST_ENABLE_BT
                && resultCode == Activity.RESULT_CANCELED) {
            ((Activity) this.mainContext).finish();
        }
    }

    boolean initiate() {
        // Use this check to determine whether BLE is supported on the device.
        // Then you can
        // selectively disable BLE-related features.
        if (!this.mainContext.getPackageManager().hasSystemFeature(
                PackageManager.FEATURE_BLUETOOTH_LE)) {
            return false;
        }

        // Initializes a Bluetooth adapter. For API level 18 and above, get a
        // reference to
        // BluetoothAdapter through BluetoothManager.
        final BluetoothManager bluetoothManager = (BluetoothManager) this.mainContext.getSystemService(Context.BLUETOOTH_SERVICE);
        this.mBluetoothAdapter = bluetoothManager.getAdapter();

        // Checks if Bluetooth is supported on the device.
        return this.mBluetoothAdapter != null;
    }

    public void buttonScanOnClickProcess() {
        switch (this.mConnectionState) {
            case isNull:
                this.mConnectionState = connectionStateEnum.isScanning;
                this.onConectionStateChange(this.mConnectionState);
                this.scanLeDevice(true);
                this.mScanDeviceDialog.show();
                break;
            case isToScan:
                this.mConnectionState = connectionStateEnum.isScanning;
                this.onConectionStateChange(this.mConnectionState);
                this.scanLeDevice(true);
                this.mScanDeviceDialog.show();
                break;
            case isScanning:

                break;

            case isConnecting:

                break;
            case isConnected:
                this.mBluetoothLeService.disconnect();
                this.mHandler.postDelayed(this.mDisonnectingOverTimeRunnable, 10000);

//			mBluetoothLeService.close();
                this.mConnectionState = connectionStateEnum.isDisconnecting;
                this.onConectionStateChange(this.mConnectionState);
                break;
            case isDisconnecting:

                break;

            default:
                break;
        }


    }

    void scanLeDevice(final boolean enable) {
        if (enable) {
            // Stops scanning after a pre-defined scan period.

            System.out.println("mBluetoothAdapter.startLeScan");

            if (this.mLeDeviceListAdapter != null) {
                this.mLeDeviceListAdapter.clear();
                this.mLeDeviceListAdapter.notifyDataSetChanged();
            }

            if (!this.mScanning) {
                this.mScanning = true;

                ScanFilter scanFilter = new ScanFilter.Builder()
                        .setServiceUuid(ParcelUuid.fromString(BLUNO_UUID)).build();

                List<ScanFilter> filters = Collections.singletonList(scanFilter);
                ScanSettings scanSettings = new ScanSettings.Builder().build();
                this.mBluetoothAdapter.getBluetoothLeScanner().startScan(filters, scanSettings, leScanCallback);
            }
        } else {
            if (this.mScanning) {
                this.mScanning = false;
                this.mBluetoothAdapter.getBluetoothLeScanner().stopScan(leScanCallback);

            }
        }
    }

    private void getGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid;
        mModelNumberCharacteristic = null;
        mSerialPortCharacteristic = null;
        mCommandCharacteristic = null;
        this.mGattCharacteristics = new ArrayList<>();

        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            uuid = gattService.getUuid().toString();
            System.out.println("displayGattServices + uuid=" + uuid);

            List<BluetoothGattCharacteristic> gattCharacteristics =
                    gattService.getCharacteristics();
            ArrayList<BluetoothGattCharacteristic> charas =
                    new ArrayList<>();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                charas.add(gattCharacteristic);
                uuid = gattCharacteristic.getUuid().toString();
                switch (uuid) {
                    case ModelNumberStringUUID:
                        mModelNumberCharacteristic = gattCharacteristic;
                        System.out.println("mModelNumberCharacteristic  " + mModelNumberCharacteristic.getUuid().toString());
                        break;
                    case SerialPortUUID:
                        mSerialPortCharacteristic = gattCharacteristic;
                        System.out.println("mSerialPortCharacteristic  " + mSerialPortCharacteristic.getUuid().toString());
                        break;
                    case CommandUUID:
                        mCommandCharacteristic = gattCharacteristic;
                        System.out.println("mSerialPortCharacteristic  " + mSerialPortCharacteristic.getUuid().toString());
                        break;
                }
            }
            this.mGattCharacteristics.add(charas);
        }

        if (mModelNumberCharacteristic == null || mSerialPortCharacteristic == null || mCommandCharacteristic == null) {
            Toast.makeText(this.mainContext, R.string.please_select_DFRobot, Toast.LENGTH_SHORT).show();
            this.mConnectionState = connectionStateEnum.isToScan;
            this.onConectionStateChange(this.mConnectionState);
        } else {
            mSCharacteristic = mModelNumberCharacteristic;
            this.mBluetoothLeService.setCharacteristicNotification(mSCharacteristic, true);
            this.mBluetoothLeService.readCharacteristic(mSCharacteristic);
        }

    }

    public enum connectionStateEnum {isNull, isScanning, isToScan, isConnecting, isConnected, isDisconnecting}

    static class ViewHolder {
        TextView deviceName;
        TextView deviceAddress;
    }

    private class LeDeviceListAdapter extends BaseAdapter {
        private final ArrayList<BluetoothDevice> mLeDevices;
        private final LayoutInflater mInflator;

        public LeDeviceListAdapter() {
            super();
            this.mLeDevices = new ArrayList<>();
            this.mInflator = ((Activity) BlunoLibrary.this.mainContext).getLayoutInflater();
        }

        public void addDevice(BluetoothDevice device) {
            if (!this.mLeDevices.contains(device)) {
                this.mLeDevices.add(device);
            }
        }

        public BluetoothDevice getDevice(int position) {
            return this.mLeDevices.get(position);
        }

        public void clear() {
            this.mLeDevices.clear();
        }

        @Override
        public int getCount() {
            return this.mLeDevices.size();
        }

        @Override
        public Object getItem(int i) {
            return this.mLeDevices.get(i);
        }

        @Override
        public long getItemId(int i) {
            return i;
        }

        @Override
        public View getView(int i, View view, ViewGroup viewGroup) {
            ViewHolder viewHolder;
            // General ListView optimization code.
            if (view == null) {
                view = this.mInflator.inflate(R.layout.listitem_device, null);
                viewHolder = new ViewHolder();
                viewHolder.deviceAddress = view
                        .findViewById(R.id.device_address);
                viewHolder.deviceName = view
                        .findViewById(R.id.device_name);
                System.out.println("mInflator.inflate  getView");
                view.setTag(viewHolder);
            } else {
                viewHolder = (ViewHolder) view.getTag();
            }

            BluetoothDevice device = this.mLeDevices.get(i);
            final String deviceName = device.getName();
            if (deviceName != null && deviceName.length() > 0)
                viewHolder.deviceName.setText(deviceName);
            else
                viewHolder.deviceName.setText(R.string.unknown_device);
            viewHolder.deviceAddress.setText(device.getAddress());

            return view;
        }
    }
}
