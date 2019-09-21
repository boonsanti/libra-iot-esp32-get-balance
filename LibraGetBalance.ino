/**
 *  LibraGetBalance.ino clone from BasicHTTPClient.ino
 *
 *  Created on: 22/09/2019
 *
 *  Modify by: Mr.Boonsanti Tungisarawuttigul
 */

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

const String LIBRA_ADDRESS = "f86a39a0e53a111251e9769099d18cc04053c7813e4ebf0b0e554eb196a320f4";

const String GET_BALANCE_ADDRESS = "https://libraservice2.kulap.io/getBalance";

void setup() {

    Serial.begin(115200);

    setup_wifi();

    
    Serial.println();
    Serial.println("Getting Balance.. " + GET_BALANCE_ADDRESS + "/" + LIBRA_ADDRESS);

}


void setup_wifi() {
  delay(10);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.write("Wifi connecting.\n");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.write(".");
  }
  Serial.write("Wifi connected.\n");
}

void loop() {
    StaticJsonBuffer<100> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    char jsonMessage[100];
    root["address"] = LIBRA_ADDRESS;
    root.printTo(jsonMessage);

    // wait for WiFi connection
    if((WiFi.status() == WL_CONNECTED)) {

        HTTPClient http;

        // configure traged server and url
        http.begin(GET_BALANCE_ADDRESS); //HTTP
        http.addHeader("Content-Type", "application/json");

        // start connection and send HTTP header
        int httpCode = http.POST(jsonMessage);

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                
                char charBuf[100];
                payload.toCharArray(charBuf, 100);
                
                StaticJsonBuffer<100> jb;
                JsonObject& obj = jb.parseObject(charBuf);
                const String balance = obj["balance"];
                Serial.println("Balance: " + balance);
            }
        } else {
            Serial.printf("POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
    delay(1000);
}
