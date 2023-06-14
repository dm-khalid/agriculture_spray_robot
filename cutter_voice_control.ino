#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define cutter        D1



#define WLAN_SSID       "khalid"           
#define WLAN_PASS       "123456789"        



#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "khalid_mahmud"            
#define AIO_KEY         "aio_koAA88gsggGM4MaMa8nxnUr17q3B"   


WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe cutter1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/cutter"); 



void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(cutter, OUTPUT);
 

 
 
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  mqtt.subscribe(&cutter1);

}

void loop() {
 
  MQTT_connect();
 

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &cutter1) {
      Serial.print(F("Got: "));
      Serial.println((char *)cutter1.lastread);
      int cutter1_State = atoi((char *)cutter1.lastread);
      digitalWrite(cutter, cutter1_State);
     
    }
    
  }

 
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
 
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
 
}
