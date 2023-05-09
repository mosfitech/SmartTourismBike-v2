#define TINY_GSM_MODEM_SIM7600
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define TINY_GSM_USE_GPRS true
#define GSM_PIN ""
#define UART_BAUD   115200
#define MODEM_TX      27
#define MODEM_RX      26
#define MODEM_PWRKEY  4
#define MODEM_FLIGHT  25
#define MODEM_STATUS  34
#define Dinamo        12
#define Buzzer        13
#define SW_UP         14
#define Battery       15

// Your GPRS credentials, if any
const char apn[]      = "telkomsel";
const char gprsUser[] = "";
const char gprsPass[] = "";

// MQTT details
const char *broker = "203.194.112.89";
int port =  1883 ;
const char *mqtt_user = "bikebikeaja";
const char *mqtt_pass = "Bikebike4ja";
const char *topicStatus        = "rental/Status";
const char *topicImei          = "rental/UUID";
const char *topicRental        = "rental/860371050882459";
const char *topicLocations     = "rental/location";
const char *topicBattray       = "rental/battray";
const char *topicWarning       = "rental/warning/860371050882459";
const char *topicTime          = "rental/time";


HardwareSerial SerialAT(1);
TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
PubSubClient  mqtt(client);
DynamicJsonDocument doc(500);

String imei = "860371050882459";

String ccid, imsi, cop, waktu, IPlocal, jadwal, message, bat;
float battery, lat, lon;
int csq;
volatile bool SW_UPState = LOW;

uint32_t lastReconnectAttempt = 0;
char receivedPayload[500];
//Mqtt Callback
void mqttCallback(char *topic, byte *payload, unsigned int len)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.write(payload, len);
    Serial.println();
//    memcpy(receivedPayload, payload, len);
//    receivedPayload[len] = '\0';

//    // Only proceed if incoming message's topic matches
  
    if (String(topic) == topicRental) { 
      if ((char)payload[0] == '1') {
        Serial.println("Sistem Sewa Aktif");
        mqtt.publish(topicStatus, "Sistem Sewa Aktif");
        digitalWrite(Dinamo, HIGH);
        digitalWrite(Buzzer, HIGH);
        delay(50);
        digitalWrite(Buzzer, LOW);
        delay(50);
        digitalWrite(Buzzer, HIGH);
        delay(50);
        digitalWrite(Buzzer, LOW);
        delay(50);
        delay(903);
        digitalWrite(Dinamo, LOW);
        digitalWrite(Buzzer, LOW);
      } else if ((char)payload[0] == '0') {
        digitalWrite(Dinamo, LOW);
        Serial.println("Dinamo turned off");
        mqtt.publish(topicStatus, "Waktu Habis");
        Buzzer_Blink();
      }
    } 

    else if (String(topic) == topicWarning) { 
      if ((char)payload[0] == '1') {
        Serial.println("Sistem Sewa Aktif");
        mqtt.publish(topicStatus, "Diluar Area");
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
        delay(100);
        digitalWrite(Buzzer, HIGH);
        delay(100);
        digitalWrite(Buzzer, LOW);
        delay(200);
      } else if ((char)payload[0] == '0') {
        digitalWrite(Buzzer, LOW);
        Serial.println("Dinamo turned off");
        mqtt.publish(topicStatus, "Didalam Area");
      }
    } 
    
}


boolean mqttConnect()
{
    Serial.print("Connecting to ");
    Serial.print(broker);

//     Connect to MQTT Broker
//    boolean status = mqtt.connect("Active");

    // Or, if you want to authenticate MQTT:
     boolean status = mqtt.connect("Active", mqtt_user, mqtt_pass);

    if (status == false) {
        Serial.println(" fail");
        return false;
    }
    Serial.println(" success");
  mqtt.publish(topicStatus,  imei.c_str());
  mqtt.subscribe(topicRental);
  mqtt.subscribe(topicWarning);
    return mqtt.connected();
}
 


void setup()
{
    // Set console baud rate
    Serial.begin(115200);
    delay(10);
    SerialAT.begin(UART_BAUD, SERIAL_8N1, MODEM_RX, MODEM_TX);
    analogReadResolution(12);
    analogSetAttenuation(ADC_11db);


    Control_Set();
    
    Serial.println("\nWait...");
    delay(1000);
    //Gsm setup
    Gsm_Set();

    // MQTT Broker setup
    mqtt.setServer(broker, port);
    mqtt.setCallback(mqttCallback);
//    Mqtt_Reconnect();

}

void loop()
{ 

    
    // Make sure we're still registered on the network
    if (!modem.isNetworkConnected()) {
        Serial.println("Network disconnected");
        if (!modem.waitForNetwork(180000L, true)) {
            Serial.println(" fail");
            delay(10000);
            return;
        }
        if (modem.isNetworkConnected()) {
            Serial.println("Network re-connected");
        }

        // and make sure GPRS/EPS is still connected
        if (!modem.isGprsConnected()) {
            Serial.println("GPRS disconnected!");
            Serial.print(F("Connecting to "));
            Serial.print(apn);
            if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
                Serial.println(" fail");
                delay(10000);
                return;
            }
            if (modem.isGprsConnected()) {
                Serial.println("GPRS reconnected");
            }
        }

    }

    if (!mqtt.connected()) {
        Serial.println("=== MQTT NOT CONNECTED ===");
        // Reconnect every 10 seconds
        uint32_t t = millis();
        if (t - lastReconnectAttempt > 10000L) {
            lastReconnectAttempt = t;
            if (mqttConnect()) {
                lastReconnectAttempt = 0;
            }
        }
        delay(100);
        return;
    }



       
//        // Jika variabel receivedPayload tidak kosong, maka proses pesan
//    if (topicWarning) { 
//      if ((char)receivedPayload[0] == '1') {
////        Data_Get();
//
//      }
//      else if ((char)receivedPayload[0] == '0') {
//      Gps_Get();
//      }
//    }
    Voltage_Get();
    Data_Get();
    Gps_Get();
    Mqtt_Publish();
    mqtt.loop();
}
