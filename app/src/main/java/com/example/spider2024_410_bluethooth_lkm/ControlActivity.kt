package com.example.spider2024_410_bluethooth_lkm;

import android.app.Activity
import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.graphics.Color
import android.os.AsyncTask
import android.os.Bundle
import android.util.Log
import android.view.MotionEvent
import android.view.View
import android.widget.Button
import android.widget.ImageButton
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.control_layout.*
import java.io.IOException
import java.util.*
import android.widget.CompoundButton





class ControlActivity: AppCompatActivity(){

    companion object{
        var m_myUUID: UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB")
        var m_bluetoothSocket: BluetoothSocket? = null
        lateinit var m_progress: ProgressDialog
        lateinit var m_bluetoothAdapter: BluetoothAdapter
        var m_isConnected: Boolean = false
        lateinit var m_address: String
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.control_layout)
        m_address = intent.getStringExtra(MainActivity.EXTRA_ADDRESS).toString()

        btn_right.isEnabled = false

        ConnectToDevice(this).execute()

        setButtonTouchListener( btn_forward, 'W')
        setButtonTouchListener( btn_back, 'S')
        setButtonTouchListener( btn_left, 'D')
        setButtonTouchListener( btn_right, 'A')

        setButtonSpeedTouchListener( btn_speed_slow, 'Z')
        setButtonSpeedTouchListener( btn_speed_medium, 'Y')
        setButtonSpeedTouchListener( btn_speed_fast, 'X')


        btn_stand.setOnClickListener {
            sendCommand("F") // Начать выполнение команды при нажатии кнопки
            btn_forward.setBackgroundResource(android.R.drawable.btn_default);
            btn_back.setBackgroundResource(android.R.drawable.btn_default);
            btn_left.setBackgroundResource(android.R.drawable.btn_default);
            btn_right.setBackgroundResource(android.R.drawable.btn_default);
        }
        btn_sleep.setOnClickListener {
            sendCommand("R") // Начать выполнение команды при нажатии кнопки
        }

        btn_control_on.setOnClickListener {
            sendCommand("M") // Начать выполнение команды при нажатии кнопки
        }
        btn_control_off.setOnClickListener {
            sendCommand("P") // Начать выполнение команды при нажатии кнопки
        }

        on_right.setOnCheckedChangeListener { buttonView, isChecked ->
            btn_right.isEnabled = isChecked
        }

        btn_disconnect.setOnClickListener{ disconnect() }
        btn_connect.setOnClickListener{ ConnectToDevice(this).execute() }

    }
    private fun setButtonTouchListener(button: View, command: Char) {
        button.setOnClickListener { event ->
            sendCommand(command)
            btn_forward.setBackgroundResource(android.R.drawable.btn_default);
            btn_back.setBackgroundResource(android.R.drawable.btn_default);
            btn_left.setBackgroundResource(android.R.drawable.btn_default);
            btn_right.setBackgroundResource(android.R.drawable.btn_default);

            button.setBackgroundColor(Color.BLUE)
        }
    }

    private fun setButtonSpeedTouchListener(button: View, command: Char) {
        button.setOnClickListener { event ->
            sendCommand(command)
            btn_speed_slow.setBackgroundResource(android.R.drawable.btn_default);
            btn_speed_medium.setBackgroundResource(android.R.drawable.btn_default);
            btn_speed_fast.setBackgroundResource(android.R.drawable.btn_default);

            button.setBackgroundColor(Color.BLUE)
        }
    }


    private fun sendCommand(input: Char){
        if(m_bluetoothSocket != null){
            try{
                var ddd = input.toInt()
                m_bluetoothSocket!!.outputStream.write(ddd)
            }catch(e: IOException){
                Toast.makeText(this, "Error send command!", Toast.LENGTH_SHORT).show()
                e.printStackTrace()
            }
        }
    }

    private fun sendCommand(input: String){
        if(m_bluetoothSocket != null){
            try{
                m_bluetoothSocket!!.outputStream.write(input.toByteArray())
            }catch(e: IOException){
                Toast.makeText(this, "Error send command!", Toast.LENGTH_SHORT).show()
                e.printStackTrace()
            }
        }
    }

    private fun disconnect(){
        if(m_bluetoothSocket != null){
            try {
                m_bluetoothSocket!!.close()
                m_bluetoothSocket = null
                m_isConnected = false
            }catch(e: IOException){
                e.printStackTrace()
                Toast.makeText(this, "Error disconnect!", Toast.LENGTH_SHORT).show()
            }
        }
        finish()
    }

    private class ConnectToDevice(c: Context) : AsyncTask<Void, Void, String>(){
        private var connectSucess: Boolean = false
        private val context: Context

        init{
            this.context = c
        }

        override fun onPreExecute() {
            super.onPreExecute()
            m_progress = ProgressDialog.show(context, "Connecting...", "please wait")
        }

        override fun doInBackground(vararg p0: Void?): String {
            try{
                if(m_bluetoothSocket == null || !m_isConnected){
                    m_bluetoothAdapter = BluetoothAdapter.getDefaultAdapter()
                    val device: BluetoothDevice = m_bluetoothAdapter.getRemoteDevice(m_address)
                    m_bluetoothSocket = device.createInsecureRfcommSocketToServiceRecord(m_myUUID)
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery()
                    m_bluetoothSocket!!.connect()
                    connectSucess = true
                }
            }catch (e: IOException){
                connectSucess = false
                e.printStackTrace()
            }
            return null.toString()
        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)
            if(!connectSucess){
                Log.i("data", "couldn't connect")
                Toast.makeText(context, "couldn't connect", Toast.LENGTH_SHORT).show()
            }else{
                Toast.makeText(context, "Success!", Toast.LENGTH_SHORT).show()
                m_isConnected = true
            }
            m_progress.dismiss()
        }
    }
}