void Gsm_Set(){
      Serial.print("Initializing Modem...");
    if (!modem.init()) {
        Serial.println("Reset");
        modem.restart();
        return;
    } 
    Serial.println("Succes");
    //
    //    // Restart takes quite some time
    //    // To skip it, call init() instead of restart()
    //    Serial.print("Initializing modem...");
    //    if (!modem.restart()) {
    //        Serial.print("Failed to restart modem, delaying 10s and retrying");
    //        // restart autobaud in case GSM just rebooted
    //        return;
    //    }


    /*  Preferred mode selection : AT+CNMP
          2 – Automatic
          13 – GSM Only
          14 – WCDMA Only
          38 – LTE Only
          59 – TDS-CDMA Only
          9 – CDMA Only
          10 – EVDO Only
          19 – GSM+WCDMA Only
          22 – CDMA+EVDO Only
          48 – Any but LTE
          60 – GSM+TDSCDMA Only
          63 – GSM+WCDMA+TDSCDMA Only
          67 – CDMA+EVDO+GSM+WCDMA+TDSCDMA Only
          39 – GSM+WCDMA+LTE Only
          51 – GSM+LTE Only
          54 – WCDMA+LTE Only
    */
    
    String ret;
    //    do {
    //        ret = modem.setNetworkMode(2);
    //        delay(500);
    //    } while (ret != "OK");
    ret = modem.setNetworkMode(2);
    Serial.println("setNetworkMode:"+ ret);

    String name = modem.getModemName();
    Serial.println("Modem Name:"+ name);

    String modemInfo = modem.getModemInfo();
    Serial.println("Modem Info:"+ modemInfo);

#if TINY_GSM_USE_GPRS
    // Unlock your SIM card with a PIN if needed
    if (GSM_PIN && modem.getSimStatus() != 3) {
        modem.simUnlock(GSM_PIN);
    }
#endif

#if TINY_GSM_USE_GPRS && defined TINY_GSM_MODEM_XBEE
    // The XBee must run the gprsConnect function BEFORE waiting for network!
    modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

    Serial.print("Waiting for network...");
    if (!modem.waitForNetwork()) {
        Serial.println(" fail");
        delay(10000);
        return;
    }
    Serial.println(" success");

    if (modem.isNetworkConnected()) {
        Serial.println("Network connected");

       
    }

#if TINY_GSM_USE_GPRS
    // GPRS connection parameters are usually set after network registration
    Serial.print(F("Connecting to "));
    Serial.print(apn);
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        Serial.println(" fail");
        delay(10000);
        return;
    }
    Serial.println(" success");

    if (modem.isGprsConnected()) {
        Serial.println("GPRS connected");
        delay(500);
      

        ccid = modem.getSimCCID();
        Serial.println("CCID:"+ ccid);
        delay(500);
      
//        imei = modem.getIMEI();
//        Serial.println("IMEI:"+ imei);
//        delay(500);
//      
        imsi = modem.getIMSI();
        Serial.println("IMSI:"+ imsi);
        delay(500);
      

        cop = modem.getOperator();
        Serial.println("Operator:" + cop);
        delay(500);
            
        csq = modem.getSignalQuality();
        Serial.print("Signal quality:");
        Serial.println(csq);
        delay(500);
  
        
        waktu = modem.getGSMDateTime(DATE_FULL);
        Serial.println("Current Network Time:" + waktu);
        delay(500);
      
        Serial.print("Getting local IP address...");
        IPlocal = modem.localIP().toString();
        if (IPlocal.length() == 0) {
          Serial.println("Failed to get local IP address!");
          return;
        }
        delay(500);
        Serial.println(IPlocal);

        
          }
#endif
}
