void Voltage_Get(){
  float batteryVoltage = analogRead(Battery) * (4.2 / 4095.0) - 0.37;
  battery = batteryVoltage*2;
  bat = String(battery, 2);
  // Menampilkan hasil bacaan tegangan baterai pada serial monitor
  //  Serial.print("Battery voltage: ");
  //  Serial.print(batteryVoltage);
  //  Serial.println(" V");
  //  Serial.println(battery);
  //  char volt[10];
  //  snprintf(volt, 10, "%.2f", battery);
  //    mqtt.publish(topicBattray, bat.c_str());
  //  delay(1000);
}
