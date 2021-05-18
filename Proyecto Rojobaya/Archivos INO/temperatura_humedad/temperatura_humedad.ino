#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <dht_nonblocking.h>
#define DHT_SENSOR_TYPE DHT_TYPE_11

int sensor = 5;
int led = 13;
static const int DHT_SENSOR_PIN = 4;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
const char* ssid="IELCIBERC_2.4";
const char* password="1970019680";

String server = "http://194.62.97.54:8000/guardarData";


void setup(){
  pinMode(led, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin( 9600);
  
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
  
void loop(){
  float temperatura;
  float humidity;
  
  int lectura = digitalRead(sensor);
  digitalWrite(led, lectura);

  if(lectura==true){
    if(dht_sensor.measure(&temperatura, &humidity)){
    //Serial.print( "T = " );
    //Serial.print( temperatura, 1 );
     post(temperatura, humidity);
    }
  }
}
void post(float temperatura, float humedad) {
  HTTPClient http;
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["temperatura"] = temperatura;
  root["humedad"] = humedad;
  root.printTo(json);
  http.begin(server); //URL del microservicio
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  Serial.println(json);
  http.end();
}
