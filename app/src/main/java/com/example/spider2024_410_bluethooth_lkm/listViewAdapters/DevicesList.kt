package com.example.spider2024_410_bluethooth_lkm.listViewAdapters

import android.annotation.SuppressLint
import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.TextView
import com.example.spider2024_410_bluethooth_lkm.R
import com.example.spider2024_410_bluethooth_lkm.structurs.MyDevice


class DevicesList(internal var context: Context, private val data: ArrayList<MyDevice>) : BaseAdapter() {


    fun getDevices(): ArrayList<MyDevice> {
        return data
    }

    override fun getCount(): Int {
        return data.size
    }

    override fun getItem(num: Int): MyDevice {
        return data[num]
    }

    override fun getItemId(arg0: Int): Long {
        return 0
    }



    internal inner class ViewHolder {
        var name: TextView? = null
        var mac: TextView? = null
    }

    @SuppressLint("InflateParams")
    override fun getView(position: Int, view: View?, arg2: ViewGroup): View {
        var someView = view

        val holder: ViewHolder

        //   View rowView = someView;
        if (someView == null) {
            val inflater = LayoutInflater.from(context)
            someView = inflater.inflate(R.layout.row_device, null, false)
            holder = ViewHolder()
            //Обявляем Views и связываем их с разметкой
            holder.name = someView!!.findViewById<View>(R.id.name) as TextView
            holder.mac = someView.findViewById<View>(R.id.mac) as TextView

            someView.tag = holder
        } else {
            holder = someView.tag as ViewHolder
        }

        val client = data[position]
        holder.name?.text = client.name
        holder.mac?.text = client.mac

        return someView
    }
}