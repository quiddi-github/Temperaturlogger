#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "DHT.h"
int i = 0;
int resetPin = 16;

// Uncomment whatever type you're using!
#define DHTPIN 2      //DHT an Pin 2 konfigurieren
#define DHTTYPE DHT22   // DHT 11 
// init DHT; 3rd parameter = 16 works for ESP8266@80MHz
DHT dht(DHTPIN, DHTTYPE,16); 

/* Set these to your desired credentials. */
const char *ssid = "Newton";  //ENTER YOUR WIFI SETTINGS <<<<<<<<<
const char *password = "Password";

//Web address to read from
const char *host = "api.thingspeak.com";
String apiKey = "API-KEY";  //ENTER YOUR API KEY <<<<<<<<<<<
//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  //digitalWrite(resetPin, HIGH);
  delay(1000);
  //pinMode(resetPin, OUTPUT); 
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  //WiFi.mode(WIFI_AP_STA);   //Both hotspot and client are enabled
  //WiFi.mode(WIFI_AP);       //Only Access point
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
void(* resetFunc) (void) = 0; //declare reset function @ address 0

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop() {
String h = String(dht.readHumidity());
String t = String(dht.readTemperature());
Serial.println("Temperatur");
Serial.println(t);
Serial.println("Luftfeuchtigkeit");
Serial.println(h);
i++;


  
  WiFiClient client;          
  const int httpPort = 80; //Port 80 is commonly used for www
 //---------------------------------------------------------------------
 //Connect to host, host(web site) is define at top 
 if(!client.connect(host, httpPort)){
   Serial.println("Connection Failed");
   delay(300);
   return; //Keep retrying until we get connected
 }
 
//---------------------------------------------------------------------
  //Make GET request as pet HTTP GET Protocol format
  String Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
  Link = Link + t;
  Link = Link + "&field2=";  //Requeste webpage  
  Link = Link + h;
  Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
  client.print(Link);
  delay(100);
  
//---------------------------------------------------------------------
 //Wait for server to respond with timeout of 5 Seconds
 int timeout=0;
 while((!client.available()) && (timeout < 1000))     //Wait 5 seconds for data
 {
   delay(10);  //Use this with time out
   timeout++;
 }

//---------------------------------------------------------------------
 //If data is available before time out read it.
 if(timeout < 500)
 {
     while(client.available()){
        Serial.println(client.readString()); //Response from ThingSpeak       
     }
 }
 else
 {
     Serial.println("Request timeout..");
 }

//---------------------------------------------------------------------

 
delay(60000);
/*if(i>2){ //bei i=60 Reset durchführen
  i=0;
  digitalWrite(resetPin, LOW);
  delay(50);
  digitalWrite(resetPin, HIGH);
  //resetFunc();  //call reset
  delay(500);
  }*/
}
//=======================================================================
