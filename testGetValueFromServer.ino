#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#define TRIGGER_PIN 0
//char* server = "http://light-up-hope.sosvietnam.org/led";
//char* num = "500";
//char serverShow[40] = "http://light-up-hope.sosvietnam.org/led";
char serverShow[60] = "http://bksmartsocket.herokuapp.com/getLedsStatus";
char numShow[6] = "500";
int ledDefault = 0;
int currentDonator = 0;

void configWifimanager();
void getDonatorNum();

void setup() {
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT);
  Serial.println("Press and hold the button GPIO_0 for 5 seconds from now");
  delay(5000);
  configWifimanager();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" get number of donate");
    getDonatorNum();
  }
  delay(500);
}

void configWifimanager()
{
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManagerParameter custom_server("server", "ex:http://abc.com", serverShow, 60);
    WiFiManagerParameter custom_num_led("num", "ex: 500", numShow, 10);
    WiFiManager wifiManager;
    wifiManager.addParameter(&custom_server);
    wifiManager.addParameter(&custom_num_led);
    if (!wifiManager.startConfigPortal("SOS_LED_RGB_AP_7", "987654321")) {

      Serial.println("failed to connect and hit timeout");
      delay(3000);
      ESP.reset();
      delay(5000);
    }
    Serial.print("connected to :)");
    Serial.println(WiFi.localIP());
    strcpy(serverShow, custom_server.getValue());
    strcpy(numShow, custom_num_led.getValue());
    ledDefault = atoi(numShow);
    Serial.print("url-server : "); Serial.println(serverShow);
    Serial.print("number led default : "); Serial.println(ledDefault);
    delay(100);
  }
}

void getDonatorNum()
{
  HTTPClient http;
  Serial.print("connecting to ");
  Serial.println(serverShow);
  http.begin(serverShow);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    currentDonator = payload.toInt();
    Serial.println(currentDonator);
  }
  http.end();
}

