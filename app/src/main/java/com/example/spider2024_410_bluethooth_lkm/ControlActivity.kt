package com.example.spider2024_410_bluethooth_lkm;

import android.app.AlertDialog
import android.app.ProgressDialog
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothSocket
import android.content.Context
import android.graphics.Color
import android.os.AsyncTask
import android.os.Bundle
import android.util.Log
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.control_layout.*
import java.io.IOException
import java.util.*


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

        setButtonClickListener( btn_forward, "W")
        setButtonClickListener( btn_back, "S")
        setButtonClickListener( btn_left, "D")
        setButtonClickListener( btn_right, "A")

        setButtonSpeedClickListener( btn_speed_slow, "Z")
        setButtonSpeedClickListener( btn_speed_medium, "Y")
        setButtonSpeedClickListener( btn_speed_fast, "X")


        btn_stand.setOnClickListener {
            sendCommand("F")
            btn_forward.setBackgroundResource(android.R.drawable.btn_default);
            btn_back.setBackgroundResource(android.R.drawable.btn_default);
            btn_left.setBackgroundResource(android.R.drawable.btn_default);
            btn_right.setBackgroundResource(android.R.drawable.btn_default);
        }
        btn_sleep.setOnClickListener {
            sendCommand("R")
        }

        btn_control_on.setOnClickListener {
            sendCommand("M")
        }
        btn_control_off.setOnClickListener {
            sendCommand("P")
        }

        on_right.setOnCheckedChangeListener { _, isChecked ->
            btn_right.isEnabled = isChecked
        }

        btn_disconnect.setOnClickListener{ disconnect() }
        btn_connect.setOnClickListener{ ConnectToDevice(this).execute() }

        about_authors.setOnLongClickListener{
            val alertDialogBuilder = AlertDialog.Builder(this)
            alertDialogBuilder.setTitle("Робота-паука разработали")
            alertDialogBuilder.setMessage("Студенты группы 410 з-аз:\nЛарионов;\nКорабельников;\nМякишев")
            alertDialogBuilder.setNegativeButton("OK") { dialogInterface, _ ->
                dialogInterface.dismiss()
            }
            val alertDialog = alertDialogBuilder.create()
            alertDialog?.show()
            return@setOnLongClickListener true
        }

    }
    private fun setButtonClickListener(button: View, command: String) {
        button.setOnClickListener {
            sendCommand(command)
            btn_forward.setBackgroundResource(android.R.drawable.btn_default);
            btn_back.setBackgroundResource(android.R.drawable.btn_default);
            btn_left.setBackgroundResource(android.R.drawable.btn_default);
            btn_right.setBackgroundResource(android.R.drawable.btn_default);

            button.setBackgroundColor(Color.BLUE)
        }
    }

    private fun setButtonSpeedClickListener(button: View, command: String) {
        button.setOnClickListener {
            sendCommand(command)
            btn_speed_slow.setBackgroundResource(android.R.drawable.btn_default);
            btn_speed_medium.setBackgroundResource(android.R.drawable.btn_default);
            btn_speed_fast.setBackgroundResource(android.R.drawable.btn_default);
            button.setBackgroundColor(Color.BLUE)
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