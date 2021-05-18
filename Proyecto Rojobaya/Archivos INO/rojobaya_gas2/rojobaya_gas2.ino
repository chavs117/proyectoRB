#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "IZZI-73B1";
const char* password = "chocolatin19";

int smokeA0 = A0;

String server = "http://194.62.97.54:8000/guardarData";

void setup() {
  Serial.begin(115200);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  pinMode(smokeA0, INPUT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("WiFi contectado");
}

void loop() {
  int analogSensor = analogRead(smokeA0);
  post(analogSensor);
  delay(2000);
}

void post(int gas) {
  HTTPClient http;
  String json;
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["concentracion_gas"] = gas;
  root.printTo(json);
  http.begin(server); //URL del microservicio
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  Serial.println(json);
  http.end();
}


