#include<ESP8266WiFi.h>
#include "DHT.h"
#include <SFE_BMP180.h>
#include <Wire.h>
#define ALTITUDE 239.0
#define DHTPIN 0
#define DHTTYPE DHT11
SFE_BMP180 pressure;
DHT dht(DHTPIN, DHTTYPE);
const char* ssid = "nene"; // Your ssid
const char* password = "password"; // Your Password
char status;
double T, P, p0, a;
WiFiServer server(80);
void setup() {
Serial.begin(115200);
delay(100);
dht.begin();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi is connected");
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
if (pressure.begin())
Serial.println("BMP180 init success");
else {
Serial.println("BMP180 init fail\n\n");
while (1);
}
delay(1000);
10
}
void loop() {
status = pressure.getPressure(P, T);
if (status != 0) {
p0 = pressure.sealevel(P, ALTITUDE); 
Serial.print("relative (sea-level) pressure: ");
Serial.print(p0, 2);
Serial.print(" mb, ");
}
float h = dht.readHumidity();
// Read temperature as Celsius (the default)
float t = dht.readTemperature();
// Read temperature as Fahrenheit (isFahrenheit = true)
float f = dht.readTemperature(true);
WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");
client.println("Refresh: 10");
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<head>");
client.println("<meta name=\"viewport\" content=\"width=device-width, initialscale=1\">");
client.println("<link rel=\"stylesheet\" href=\"bootstrap.min.css\">");
client.println("<script src=\"jquery.min.js\"></script>");
client.println("<script src=\"bootstrap.min.js\"></script>");
client.println("<style>html { font-family: Cairo; display: block; margin: 0px 
auto; text-align: center;color: #333333; background-color: #222222;}");
client.println("body{margin-top: 50px;}");
client.println(".card {border: none; background-color: #333333; color: #ffffff; 
margin-bottom: 20px;}");
client.println(".card-header {background-color: lightgrey; color: #1a5fd7; fontsize: 24px; font-weight: bold;}");
client.println(".card-body {padding: 10px;}");
client.println(".humidity {color: #ff0000;}");
client.println(".temperature {color: #ffa500;}");
client.println(".pressure {color: #006400;}");
client.println(".title {color: #ffffff;}");
client.println(".footer {color: #f612a2; margin-top: 50px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div class=\"container\">");
11
client.println("<h1 class=\"title\">Weather Monitoring Station</h1>");
client.println("<div class=\"card-deck\">");
client.println("<div class=\"card col-xs-12 col-sm-4\">");
client.println("<div class=\"card-header\">Humidity</div>");
client.println("<div class=\"card-body\">");
client.print("<h2 class=\"card-title humidity\">");
client.print(h);
client.println("%</h2>");
client.println("</div>");
client.println("</div>");
client.println("<div class=\"card col-xs-12 col-sm-4\">");
client.println("<div class=\"card-header\">Temperature</div>");
client.println("<div class=\"card-body\">");
client.print("<h2 class=\"card-title temperature\">");
client.print(t);
client.println("*C</h2>");
client.print("<h2 class=\"card-title temperature\">");
client.print(f);
client.println("F</h2>");
client.println("</div>");
client.println("</div>");
client.println("<div class=\"card col-xs-12 col-sm-4\">");
client.println("<div class=\"card-header\">Pressure</div>");
client.println("<div class=\"card-body\">");
client.print("<h2 class=\"card-title pressure\">");
client.print(p0, 2);
client.println("mb</h2>");
client.println("</div>");
client.println("</div>");
client.println("</div>");
client.println("<div class=\"footer\">Project made by Vishwajith, Suba, Nithya, 
Ishika, Shivanjali, and Kavya</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
delay(4000);
}
