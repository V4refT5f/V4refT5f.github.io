#include <HTTP_Method.h>
#include <Uri.h>
#include <WebServer.h>

#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>


#define BAUDRATE 99999999

#define GET_MESSAGE "GET /?controlInfo="
#define GET_MESSAGE_LEN 18

const char* ssid = "ESP32";
const char* password = "V4refT5f";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiServer server(80);

void setup() {

    Serial.begin(BAUDRATE);

    delay(100);

    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    delay(100);

    server.begin();

}

void loop() {

    // Listen for incoming clients.
    WiFiClient client = server.available();

    if (client) {

        // CurrentLine acts as a temperary holder for the current line of HTTP request.
        String currentLine = "";

        while (client.connected()) {

            // If there's character pending to be read, then do the following.
            if (client.available()) {
                char c = client.read();

                // If it's an End of Line:
                if (c == '\n') {
                    // If there's two '\n's in a row, that's the end of the client HTTP request.
                    if (currentLine.length() == 0) {

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.print("<!DOCTYPE html>\n<html lang=\"ja\">\n<head>\n\t<meta charset=\"Shift_JIS\">\n\t<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n\t<title>ESP32 Control</title>\n</head>\n<body>\n\n<script>\nfunction press(text) {\n\tdocument.getElementById(\"controlInfo\").value = text;\n\tdocument.getElementById(\"confirm\").click();\n}\n</script> \n\n<form name=\"input\" method=\"get\">\nText to be sent: <input type=\"text\" name=\"controlInfo\" id=\"controlInfo\">\n<input type=\"submit\" value=\"Confirm\" id='confirm'>\n</form>\n\n<button name=\"B\" onclick=\"press('B')\">B</button>\n<button name=\"F\" onclick=\"press('F')\">F</button>\n\n</body>\n</html>");
                        client.println();

                    }
                    // If it's one sole '\n' then deal with the data and clear the buffer.
                    else {
                        // Check if there's GET data coming.
                        if (currentLine.startsWith(GET_MESSAGE)) {
                            String toVex = currentLine.substring(GET_MESSAGE_LEN, currentLine.indexOf(' ') - GET_MESSAGE_LEN);
                            Serial.println(toVex.c_str());
                        }
                        
                        currentLine = "";
                    }
                }
                else if (c != '\r') {
                    currentLine += c;
                }
            }
        }

        client.stop();
    }
}
