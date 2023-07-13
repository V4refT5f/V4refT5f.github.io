#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>


#define BAUDRATE 99999999

const char* ssid = "ESP32";  
const char* password = "V4refT5f"; 
const String webpage = "<!DOCTYPE html>\n<html lang=\"ja\">\n<head>\n\t<meta charset=\"Shift_JIS\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\t<title>ESP32 Control</title>\n</head>\n<body>\n\n<script>\nfunction press(text) {\n\tdocument.getElementById(\"controlInfo\").value = text;\n\tdocument.getElementById(\"confirm\").click();\n}\n</script> \n\n<form name=\"input\" method=\"get\">\nText to be sent: <input type=\"text\" name=\"controlInfo\" id=\"controlInfo\">\n<input type=\"submit\" value=\"Confirm\" id='confirm'>\n</form>\n\n<button name=\"B\" onclick=\"press('B')\">B</button>\n<button name=\"F\" onclick=\"press('F')\">F</button>\n\n</body>\n</html>";
const String argname = "controlInfo";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

void handle_NotFound(){
    server.send(404, "text/plain", "Not found");
}

void handle_OnRequest() {
    if (server.hasArg(argname)) {
        Serial.print((server.arg(argname)).c_str());
    }
    server.send(200, "text/html", webpage);
}

void setup() {

    Serial.begin(BAUDRATE);

    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);
  
    server.on("/", handle_OnRequest);
    server.onNotFound(handle_NotFound);
  
    server.begin();

}

void loop() {
    server.handleClient();
    delay(2);
}