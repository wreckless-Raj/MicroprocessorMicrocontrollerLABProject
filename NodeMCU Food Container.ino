#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient http;
String url;
String API = "J56NQSAOIQ87BEWJ"; // Replace with your API endpoint
String FieldNo = "1";

#define TRIG_PIN D7
#define ECHO_PIN D8
#define LED_PINGreen D4
#define LED_PINRed D3

void ConnectToWifi();
void SendGETRequest(float data);

int msg = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PINGreen, OUTPUT);
  pinMode(LED_PINRed, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  ConnectToWifi();
}

void loop() {
  

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  unsigned long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  
  Serial.print("Food level: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance > 10 && msg == 0) {
    msg=1;
    SendGETRequest(distance);
    digitalWrite(LED_PINRed, HIGH);
    digitalWrite(LED_PINGreen, LOW);
    
     
  } 
  else if (distance <= 10) {
    msg = 0;
    digitalWrite(LED_PINGreen, HIGH);
    digitalWrite(LED_PINRed, LOW);
  }
  
  delay(16000);
}

void SendGETRequest(float data) {
  url = "http://api.thingspeak.com/update?api_key=";
  url += API;
  url += "&field";  // <-- The missing '=' was added here
  url += FieldNo;
  url += "=";
  url += data;
  http.begin(client, url);
  Serial.println("Sending GET request...");
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    Serial.println("Data sent successfully");
  } else {
    Serial.print("Error in sending. HTTP code: ");
    Serial.println(httpCode);
    msg=0;
  }
  
  http.end();
}

void ConnectToWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin("Dexter_Net_By_Suborno", "918273645"); // Replace with your WiFi id and password
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(200);
  }
  
  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());
}
