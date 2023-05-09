

void Mqtt_Publish(){
        Serial.print("UID: " + imei + ", ");
        Serial.print("Voltage: " + bat + " V" + ", ");
        Serial.printf("lat:%f lon:%f\n", lat, lon);
        Serial.println("Time:" + waktu + ".");
        mqtt.publish(topicImei, imei.c_str());
        mqtt.publish(topicTime,(imei + "," + jadwal).c_str());
        mqtt.publish(topicBattray, (imei + "," + bat).c_str());
        mqtt.publish(topicLocations, (imei + "," + message).c_str());
//        delay(2000);
}
