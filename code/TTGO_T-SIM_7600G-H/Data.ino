void Data_Get(){
  
  waktu = modem.getGSMDateTime(DATE_FULL);
  //      Serial.println("Current Network Time:" + waktu);
  jadwal = String(waktu);

}
