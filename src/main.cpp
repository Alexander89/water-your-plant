#include <Arduino.h>

#include <WiFi.h>
#include <wlan.h>
#include <css.h>

#include "HTTPServer.hpp"

#define CSS "<style>" HTML BODY H1 BODY H1 UPTIME LI BUTTON LEVEL "</style>"
const char *HTML_HEADER = "<!DOCTYPE html><html>" CSS "<head><title>Water Sensor</title></head><body>";
const char *HTML_FOOTER = "</body></html>";

// Set web server port number to 80
httpsserver::HTTPServer httpServer;

void handleRoot(httpsserver::HTTPRequest *req, httpsserver::HTTPResponse *res);
void handleMetrics(httpsserver::HTTPRequest *req, httpsserver::HTTPResponse *res);

void setup() {
  pinMode(34, INPUT);
  Serial.begin(115200);

  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

    // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  auto *nodeRoot = new httpsserver::ResourceNode("/", "GET", &handleRoot);
  auto *metricsRoot = new httpsserver::ResourceNode("/metrics", "GET", &handleMetrics);

  httpServer.registerNode(nodeRoot);
  httpServer.registerNode(metricsRoot);
  httpServer.start();

  if (httpServer.isRunning())
    Serial.println("Server ready.");
  else
    Serial.println("Server failed to start.");
}

void loop() {
  httpServer.loop();
}

#define r(x) res->print(x)
#define rl(x) r(x);r("\n")

void handleRoot(httpsserver::HTTPRequest *req, httpsserver::HTTPResponse *res)
{
  int value = (int)analogRead(34);
  value = 100 - max(min((value - 800) / 16, 100), 0);

  // Status code is 200 OK by default.
  // We want to deliver a simple HTML page, so we send a corresponding content type:
  res->setHeader("Content-Type", "text/html");

  rl(HTML_HEADER);
  rl("<h1>Plant water sensor</h1>");

  r("<p>Navigation:</p><ul>");
  r("<li><a href=\"/metrics\">Metrics (current values)</a></li>");
  rl("</ul>");

  r("<div><p>Water level:</p>");
  if (value < 10) {
    r("<p class=\"lvl low\">");
  } else if (value < 30) {
    r("<p class=\"lvl medium\">");
  } else {
    r("<p class=\"lvl ok\">");
  }
  res->print(value, DEC);
  r("%</p>");
  rl("</div>");

  r("<p class=\"uptime\">Your sensor is running for ");
  res->print((int)(millis() / 1000), DEC);
  rl(" seconds.</p>");

  rl("<a class=\"button\" href=\"/\">refresh</a>");
  rl(HTML_FOOTER);
}

void handleMetrics(httpsserver::HTTPRequest *req, httpsserver::HTTPResponse *res)
{
  // Status code is 200 OK by default.
  // We want to deliver a simple TEXT, so we send a corresponding content type:
  res->setHeader("Content-Type", "text/plain");

  rl("# HELP plant_sensor_water The current value of the water sensor. ");
  rl("# TYPE plant_sensor_water gauge");
  r("plant_sensor_water ");
  // adding the analog value to the metrics
  res->print(analogRead(34), DEC);
  r("\n");
  rl("# HELP plant_sensor_uptime The current millis value, this will flip to 0 after a while.");
  rl("# TYPE plant_sensor_uptime gauge");
  r("plant_sensor_uptime ");
  // adding the uptime value to the metrics
  res->print(millis(), DEC);
  r("\n");
}