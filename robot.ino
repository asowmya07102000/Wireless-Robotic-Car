#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define STASSID
#define STASSID "Redmi"
#define STAPSK  "asrithachitturi"

const char *ssid = STASSID;
const char *password = STAPSK;
ESP8266WebServer server(80);

const int led = 13;

void handleRoot() {
  digitalWrite(led, 1);
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  snprintf(temp, 400,

           "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",

           hr, min % 60, sec % 60
          );
  server.send(200, "text/html", temp);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void drawGraph() {
  String out;
  out.reserve(2600);
  char temp[70];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    y = y2;
    out += temp;
  out += "</g>\n</svg>\n";

  server.send(200, "image/svg+xml", out);
}

}
void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());
  pinMode(D1,OUTPUT);
  pinMode(D4,OUTPUT);
  pinMode(D5,OUTPUT);
  pinMode(D7,OUTPUT);

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/test.svg", drawGraph);
  server.on("/inline", []() {
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
  WiFiServer server(80);
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  Serial.println(F("new client"));

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
   int val;
  if (req.indexOf(F("/gpio/0")) != -1) {
    digitalWrite(D1,LOW);
    digitalWrite(D4,HIGH);
    digitalWrite(D5,HIGH);
    digitalWrite(D7,LOW);
  } 
  else if (req.indexOf(F("/gpio/1")) != -1) {
    digitalWrite(D1,LOW);
    digitalWrite(D4,HIGH);
    digitalWrite(D5,LOW);
    digitalWrite(D7,LOW);
  } 
  else if (req.indexOf(F("/gpio/2")) != -1) {
    digitalWrite(D1,LOW);
    digitalWrite(D4,LOW);
    digitalWrite(D5,HIGH);
    digitalWrite(D7,LOW);
  }
  else if (req.indexOf(F("/gpio/3")) != -1) {
    digitalWrite(D1,HIGH);
    digitalWrite(D4,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D7,HIGH);
  }
 else if(req.indexOf(F("/gpio/4")) != -1)
 {
  digitalWrite(D1,LOW);
    digitalWrite(D4,LOW);
    digitalWrite(D5,LOW);
    digitalWrite(D7,LOW);
 }
  // Set LED according to the request
  //digitalWrite(LED_BUILTIN, val);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\n"));
 
  //client.print((val) ? F("high") : F("low"));
  client.print(F("<br><br>ROBOT CONTROL<br><br><a href='http://"));
  client.print(WiFi.localIP());
  //client.print(F("<button>forward</button></a><br><br>"));
  //client.print(F("<button>backward</button></a><br><br>"));
  //client.print(F("<button>left</button></a><br><br>"));
  //client.print(F("<button>right</button></a><br><br>"));
  client.print(F("/gpio/0'><button>forward</button></a><br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'><button>right</button></a><br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/2'><button>left</button></a><br><br><a href='http://"));
   client.print(WiFi.localIP());
  client.print(F("/gpio/3'><button>backward</button></a><br><br><a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/4'><button>stop</button></a><br><br></html>"));

  
  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}