package com.example.spider2024_410_bluethooth_lkm;

import android.app.Activity
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.AdapterView
import android.widget.Toast
import androidx.annotation.RequiresApi
import com.example.spider2024_410_bluethooth_lkm.listViewAdapters.DevicesList
import com.example.spider2024_410_bluethooth_lkm.structurs.MyDevice
import kotlinx.android.synthetic.main.activity_main.*
import android.Manifest



class MainActivity : AppCompatActivity() {

    var m_bluetoothAdapter: BluetoothAdapter? = null
    lateinit var m_pairedDevices: Set<BluetoothDevice>
    val REQUEST_ENABLE_BLUETOOTH = 1
    val REQUEST_BLUETOOTH_PERMISSION = 2
    val REQUEST_LOCATION_PERMISSION = 3

    companion object{
        val EXTRA_ADDRESS: String = "Device_address"
    }

    @RequiresApi(Build.VERSION_CODES.M)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        m_bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
        if(m_bluetoothAdapter == null){
            Toast.makeText(this, "this device doesn't support bluetooth", Toast.LENGTH_SHORT).show()
            return
        }
        if(!m_bluetoothAdapter!!.isEnabled){
            val enableBluetoothIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            startActivityForResult(enableBluetoothIntent, REQUEST_ENABLE_BLUETOOTH)
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (checkSelfPermission(Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(arrayOf(Manifest.permission.ACCESS_FINE_LOCATION), REQUEST_LOCATION_PERMISSION)
            }
        }

        // Для Android 12 и выше
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.S) {
            if (checkSelfPermission(Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED ||
                checkSelfPermission(Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(arrayOf(
                    Manifest.permission.BLUETOOTH_SCAN,
                    Manifest.permission.BLUETOOTH_CONNECT), REQUEST_BLUETOOTH_PERMISSION)
            }
        }

        pairedDeviceList()
        select_device_refresh.setOnClickListener{ pairedDeviceList() }

    }

    private fun pairedDeviceList(){
        m_pairedDevices = m_bluetoothAdapter!!.bondedDevices
        val listDevices:ArrayList<MyDevice> = ArrayList()

        if(m_pairedDevices.isNotEmpty()){
            for(device: BluetoothDevice in m_pairedDevices){
                listDevices.add(MyDevice(device.address, device.name))
                // Log.i("device", ""+device)
            }
        }else{
            Toast.makeText(this, "no paired bluetooth devices found", Toast.LENGTH_SHORT).show()
        }
        val adapter = DevicesList(this, listDevices)
        select_device_list.adapter = adapter
        select_device_list.onItemClickListener = AdapterView.OnItemClickListener { parent, view, position, id ->
            val item = adapter.getItem(position)
            val intent = Intent(this, ControlActivity::class.java)
            intent.putExtra(EXTRA_ADDRESS, item.mac)
            startActivity(intent)
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if(requestCode == REQUEST_ENABLE_BLUETOOTH){
            if(resultCode == Activity.RESULT_OK){
                if(m_bluetoothAdapter!!.isEnabled){
                    Toast.makeText(this, "Bluetooth has been enabled.", Toast.LENGTH_SHORT).show()
                }else{
                    Toast.makeText(this, "Bluetooth has been disabled.", Toast.LENGTH_SHORT).show()
                }
            }else if(resultCode == Activity.RESULT_CANCELED){
                Toast.makeText(this, "Bluetooth enabling has been canceled.", Toast.LENGTH_SHORT).show()
            }
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        when (requestCode) {
            REQUEST_LOCATION_PERMISSION -> {
                if (grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    Toast.makeText(this, "REQUEST_LOCATION_PERMISSION Разрешение получено", Toast.LENGTH_SHORT).show()
                } else {
                    Toast.makeText(this, "REQUEST_LOCATION_PERMISSION Разрешение отклонено", Toast.LENGTH_SHORT).show()
                }
            }
            REQUEST_BLUETOOTH_PERMISSION -> {
                if (grantResults.isNotEmpty() && grantResults.all { it == PackageManager.PERMISSION_GRANTED }) {
                    Toast.makeText(this, "REQUEST_BLUETOOTH_PERMISSION Все Bluetooth-разрешения получены", Toast.LENGTH_SHORT).show()
                } else {
                    Toast.makeText(this, "REQUEST_BLUETOOTH_PERMISSION Разрешение отклонено", Toast.LENGTH_SHORT).show()
                }
            }
        }
    }

}

