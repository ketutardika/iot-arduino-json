#include <ArduinoJson.h>              // import DHT11 sensor
#include <ESP8266WiFi.h>        //import ESP8266 WiFi library
#include <ESP8266HTTPClient.h>  //import ESP8266 HTTP Client library
#include <WiFiClient.h>   
#include <SPI.h>   // Add Wifi Client
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int led1 = D7;
int led2 = D6;
const char* ssid = "Office";  // Enter your local network SSID
const char* password = "Moana123";   // Enter your local network Password
//Your Domain name with URL path or IP address with path       // Define Sensor location name to enter in database
void setup() {               // Start DHT11 Sensor
  Serial.begin(115200);        // Open Serial monitor at baud rate 115200
  WiFi.begin(ssid, password);  // Pass the credential for local wifi network to connect ESP8266 board
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP Address");
  Serial.println(WiFi.localIP());
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  myservo.attach(D5);  // attaches the servo on pin 9 to the servo object
}
void loop() {
// Connect to HTTP server
  WiFiClient client;
  client.setTimeout(10000);
  if (!client.connect("aquamonia.com", 80)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));

  // Send HTTP request
  client.println(F("GET /os/ex HTTP/1.1"));
  client.println(F("Host: aquamonia.com"));
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    client.stop();
    return;
  }

  


  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    client.stop();
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(6) + 60;
  DynamicJsonDocument doc(1024);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  switch (error.code()) {
      case DeserializationError::Ok:
          Serial.println(F("Error : Deserialization succeeded"));
          break;
      case DeserializationError::InvalidInput:
          Serial.println(F("Error : Invalid input!"));
          break;
      case DeserializationError::NoMemory:
          Serial.println(F("Error : Not enough memory"));
          break;
      default:
          Serial.println(F("Error : Deserialization failed"));
          break;
  }


  // Extract values
  int values1 = doc["data"]["data"][0]["value"];
  int values2 = doc["data"]["data"][1]["value"];
  int values3 = doc["data"]["data"][2]["value"];
  Serial.println(F("Response:"));
  Serial.println(values1);
  Serial.println(values2);
  Serial.println(values3);
    // put your main code here, to run repeatedly:
    // Disconnect
  if (values1==1){
    digitalWrite(led1, HIGH);
  }
  else{
    digitalWrite(led1, LOW);
  }
  if (values2==1){
    digitalWrite(led2, HIGH);
  }
  else{
    digitalWrite(led2, LOW);
  }
  if (values3==1){
    myservo.write(180);
  }
  else{
    myservo.write(0);
  }
  client.stop();
  delay(2000);
  
}