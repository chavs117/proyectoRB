#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "IELCIBERC_2.4";
const char* password = "1970019680";

int ledPin = 5; // LED azul
int pirPin = 4; // Entrada de HC-SR501

int pirValue; // Variable para almacenar el valor del PIR

String server = "http://194.62.97.54:8000/guardarData";

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("WiFi contectado");
}

void loop() {
  // put your main code here, to run repeatedly:
  pirValue = digitalRead(pirPin);
  digitalWrite(ledPin, pirValue);
  if(pirValue == 0){
    Serial.println(pirPin);
  }
  postMov(pirValue);
  delay(2000);
}

void postMov(int mov) {
  HTTPClient http;
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["movimiento"] = mov;
  root.printTo(json);
  http.begin(server); //URL del microservicio
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  Serial.println(json);
  http.end();
}
