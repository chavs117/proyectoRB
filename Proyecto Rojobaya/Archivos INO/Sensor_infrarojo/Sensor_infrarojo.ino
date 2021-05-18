#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

// digital pin 2 tiene un pushbutton anlazado a el
int pushbutton = 4;

const char* ssid="IELCIBERC_2.4";
const char* password="1970019680";
String server = "http://194.62.97.54:8000/guardarData";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(pushbutton, INPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP:");
  Serial.println(WiFi.localIP());
}

int state = 0;

void loop() {
  // put your main code here, to run repeatedly:
  int buttonState = digitalRead(pushbutton);
  if(state == 0 && buttonState == 0){
    state = 1;
  }else if(state == 1){
    Serial.print(micros());
    Serial.print(',');
    Serial.println(buttonState);
   }
   post(buttonState);
   delay(2000);
   
}

void post(int estado) {
  HTTPClient http;
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["pulsaciones"] = estado;
  root.printTo(json);
  http.begin(server); //URL del microservicio
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  Serial.println(json);
  http.end();
}
