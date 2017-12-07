#include <ESP8266WiFi.h>
#include <WiFiManager.h>        // https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>        // https://github.com/bblanchon/ArduinoJson
#include <LPD6803.h>		// https://github.com/area3001/esp8266-arduino-lpd6803
#include <Ticker.h>
#include "ets_sys.h"
#include "osapi.h"

#define USE_US_TIMER
#define TRIGGER_PIN 0
#define CLOCK_PIN  14
#define DATA_PIN   12
//char* server = "http://light-up-hope.sosvietnam.org/led";
//char* num = "500";
//char serverShow[40] = "http://light-up-hope.sosvietnam.org/led";
char serverShow[60] = "http://bksmartsocket.herokuapp.com/getLedsStatus";
char numShow[6] = "500";
int ledDefault = 0;
int currentDonator = 0;
int numled = 0;
static int state = 0;
unsigned long previousMillis_Blink = 0;
const long interval_Blink = 1000;

LPD6803 strip = LPD6803(numled, DATA_PIN, CLOCK_PIN);
Ticker flipper;

void configWifimanager();
void getDonatorNum();
void OnTimerOneInterrupt();
void blink_yellow(int numled);
void setState()
{
  state = ~state;
  Serial.println(state);
}
void setup() {
  Serial.begin(115200);
  Serial.println("\n Starting");
  pinMode(TRIGGER_PIN, INPUT);
  Serial.println("Press and hold the button GPIO_0 for 5 seconds from now");
  delay(5000);
  configWifimanager();
  strip.begin();
  //  strip.show();
  Serial.println("LEDS initalized");
  timer1_attachInterrupt (OnTimerOneInterrupt);
  timer1_write (800);
  timer1_enable (TIM_DIV1, TIM_EDGE, TIM_LOOP);
  flipper.attach(2, setState);
}

void loop() {
  if (state == 0) {
    Serial.println("state = 0");
    getDonatorNum();
  }
  else 
  Serial.println("state # 0");
  blink_yellow(numled);
}

void configWifimanager()
{
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    WiFiManagerParameter custom_server("server", "ex:http://abc.com", serverShow, 60);
    WiFiManagerParameter custom_num_led("num", "ex: 500", numShow, 10);
    WiFiManager wifiManager;
    wifiManager.addParameter(&custom_server);
    wifiManager.addParameter(&custom_num_led);
    if (!wifiManager.startConfigPortal("SOS_LED_RGB_AP", "987654321")) {

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
  //int sumDonator = 0;
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    numled = payload.toInt();
    Serial.println("numled");
    Serial.println(numled);
  }
  Serial.println("end");
  http.end();
}

void OnTimerOneInterrupt()
{
  strip.update ();
}

void blink_yellow(int numled)
{
  LPD6803::color_t color;
  unsigned long currentMillis_Blink = millis();
  if (currentMillis_Blink - previousMillis_Blink >= interval_Blink) {
    previousMillis_Blink = currentMillis_Blink;
    color.r = 0;
    color.g = 255;
    color.b = 255;
  }
  else {
    color.r = 0;
    color.g = 0;
    color.b = 0;
  }
  strip.setPixelColor (numled, color);
  strip.show();
}

