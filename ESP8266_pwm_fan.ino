#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>;
#include "PubSubClient.h";

//Constants
#define BUILTIN_LED     2
#define FANPIN          5
#define DHTPIN          4
#define DHTTYPE         DHT22
#define TEMP_READ_DELAY 2000 //can only read digital temp sensor every ~750ms

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);

//Data structure
struct FanSpeed {
  double  envTemp;
  double  fanSpeed;
};

struct Configuration {
  FanSpeed fanSpeed[8];
};

//Variables
Configuration conf;
double h;
double t;
double currentH;
double currentT;
int currentFanSpeed;
unsigned long lastDhtRead;
int timeSinceLastRead = 0;
File fsUploadFile; // a File object to temporarily store the received file

// Parametri di connessione alla rete WiFi#################
const char* ssid = "xxxxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxxxx";
IPAddress ip(192, 168, 1, 250);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
// #########################################################

// MQTT parameters
#define MQTT_SERVER "192.168.1.102"
#define MQTT_PORT "1883"

#define MQTT_USER   "xxxxxxxxxx"
#define MQTT_PWD    "xxxxxxxxxx"

#define MQTT_HUM_TOPIC      "pwmfan/humidity"
#define MQTT_TEMP_TOPIC     "pwmfan/temperature"
#define MQTT_FAN_TOPIC      "pwmfan/fan"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void setup() {
  ESP.wdtDisable();
  ESP.wdtEnable(5000);
  Serial.begin(115200);
  // Wait for serial to initialize.
  while(!Serial) { }
  Serial.println("");
  pinMode(FANPIN, OUTPUT);
  setFanSpeed(100);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  analogWriteFreq(100000);
  if (!SPIFFS.begin()) {
      Serial.println("Failed to mount file system");
      return;
  }
  loadConfig();
  connectToWifi();
  connectToMqtt();
  webserverSetup();
}

bool connectToWifi(){
  WiFi.disconnect();
  unsigned long startTime = millis();
  WiFi.config(ip, gateway, subnet);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(BUILTIN_LED, LOW);
    // wait for a second
    delay(1000);
    Serial.print(".");
    digitalWrite(BUILTIN_LED, HIGH);
    // wait for a second
    delay(1000);
    if(millis() - startTime > 10000){
      ESP.restart();
    }
    yield();
  }
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
      connectToWifi();
  }
  if (!mqtt_client.connected()) {
    reconnect();
  }
  server.handleClient();
  if(millis() - lastDhtRead > 2000){
    //Leggo T e H e li immagazzino nelle variabili
    h = dht.readHumidity();
    t= dht.readTemperature();
    // Controllo se la lettrua ha avuto successo, altrimenti interrompo il loop
    if (isnan(h) || isnan(t)) {
      //Serial.println("Unable to read data from DHT sensor");
      timeSinceLastRead = 0;
      return;
    }
    //Controllo se la temperatura è variata
    if(t!=currentT){
      currentT = t;
      Serial.print("[CHANGE] Temperature: ");
      Serial.print(currentT);
      Serial.println("°C");
      mqtt_client.publish(MQTT_TEMP_TOPIC, String(currentT).c_str(), true);
      setFanSpeedByTemperature(currentT);
    }
    if(h!=currentH){
      currentH = h;
      mqtt_client.publish(MQTT_HUM_TOPIC, String(currentH).c_str(), true);
//      Serial.print("[CHANGE] Humidity: ");
//      Serial.print(currentH);
//      Serial.println("%");
    }
    lastDhtRead = millis();
  }
}
