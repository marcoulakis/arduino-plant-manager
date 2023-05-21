#include <WiFi.h>    
#include <HTTPClient.h>
#include <UrlEncode.h>
#define RXD2 16
#define TXD2 17

const char* ssid = ""; // Name of the network
const char* password = ""; // Password of the network

// +international_country_code + phone number
// Portugal +351, example: +351912345678
String phoneNumber = "5511986469266";
String apiKey = "3627236";

void sendMessage(String message){

  // Data to send with HTTP POST
  String url = "https://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" + urlEncode(message);    
  HTTPClient http;
  http.begin(url);

  // Specify content-type header
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  
  // Send HTTP POST request
  int httpResponseCode = http.POST(url);
  if (httpResponseCode == 200){
    Serial.print("Message sent successfully");
  }
  else{
    Serial.println("Error sending the message");
    Serial.print("HTTP response code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);


}
void loop() {

    delay(7000);
    Serial.println("Message Received: ");
    Serial.println(Serial2.readString());
    sendMessage(Serial2.readString());
    delay(1800000);


}
