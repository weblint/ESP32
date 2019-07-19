// Schalte GPO0 mit Button und Java im Access Point modus  WLAN-Hotspot "astral" pass "12345678"
// http://esp8266-server.de/

//https://arduino-hannover.de/2014/12/11/wifi-kochbuch-mit-esp8266/
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
 
String html1 = "<!DOCTYPE html>\r\n<html>\r\n\
<head>\r\n<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"content-type\">\r\n\
<title>WebSchalter</title>\r\n\
<form action=\"";
// String((val)?"/1.html":"/0.html")
String html2 = "\">\r\n<input value=\"ON/OFF\" style=\"";
String html3 = " width:5em;height:3em; font-size: 16px;\" type=\"submit\">\
</form>\r\n</head>\r\n<body>\r\n</body>\r\n</html>";

ESP8266WebServer server(80);    // Server Port  hier einstellen
int val = 1;                    //Startzustand ausgeschaltet
String Temp = "";
 
void Ereignis_Schalte1ON()       // Wird ausgefuehrt wenn "http://<ip address>/1.html" aufgerufen wurde
{
  val = 0;                      // Relais Aus
  digitalWrite(0, val);         // GPIO0
  Temp = html1 + String((val) ? "/1.html" : "/0.html");
  Temp += html2 + String((val) ? "BACKGROUND-COLOR: DarkGray;" : "BACKGROUND-COLOR: Chartreuse;") + html3;
  server.send(200, "text/html", Temp);
}
 
void Ereignis_Schalte1OFF()  // Wird ausgefuehrt wenn "http://<ip address>//0.html" aufgerufen wurde
{
  val = 1;                      // Relais Ein
  digitalWrite(0, val);         // GPIO0
  Temp = html1 + String((val) ? "/1.html" : "/0.html");
  Temp += html2 + String((val) ? "BACKGROUND-COLOR: DarkGray;" : "BACKGROUND-COLOR: Chartreuse;") + html3;
  server.send(200, "text/html", Temp);
}

//-----------------------------------------------------------------------------------------------------------
  
void Ereignis_Index()           // Wird ausgeuehrt wenn "http://<ip address>/" aufgerufen wurde
{
  Temp = html1 + String((val) ? "/1.html" : "/0.html");
  Temp += html2 + String((val) ? "BACKGROUND-COLOR: DarkGray;" : "BACKGROUND-COLOR: Chartreuse;") + html3;
  server.send(200, "text/html", Temp);
}

void setup()
{
  digitalWrite(0, 1);           // Anfangszustand 1 (Relais ausgeschaltet)
  pinMode(0, OUTPUT);           // GPIO0 als Ausgang konfigurieren
  pinMode(2, INPUT_PULLUP);     // GPIO2 als Eingang mit Pullup konfigurieren
  digitalWrite(0, 1);           // Anfangszustand 1 (Relais ausgeschaltet)
 
  Serial.begin(115200);         // Serielle schnittstelle initialisieren
  Serial.println("");           // Lehere Zeile ausgeben
  Serial.println("Starte WLAN-Hotspot \"astral\"");
  WiFi.mode(WIFI_AP);           // access point modus
  WiFi.softAP("astral", "12345678");    // Name des Wi-Fi netzes
  delay(500);                   //Abwarten 0,5s
  Serial.print("IP Adresse ");  //Ausgabe aktueller IP des Servers
  Serial.println(WiFi.softAPIP());
 
  //  Bechandlung der Ereignissen anschlissen
  server.on("/", Ereignis_Index);
  server.on("/1.html", Ereignis_Schalte1ON);
  server.on("/0.html", Ereignis_Schalte1OFF);
  
 
  server.begin();               // Starte den Server
  Serial.println("HTTP Server gestartet");
}

void loop()
{
  server.handleClient();
  if (!digitalRead(2))          //Wenn Taster an GPIO2 betetigt wurde
  {
    val = !val;                 // Schaltzuschtand andern
    digitalWrite(0, val);
    while (!digitalRead(2))server.handleClient(); // Warten bis der Taster losgelassen wird.
  }
}
