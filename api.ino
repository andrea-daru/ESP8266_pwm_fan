void apiStatus(){
  String json;
  StaticJsonBuffer<300> JSONbuffer;
  JsonObject& root = JSONbuffer.createObject();
  root["current_temperature"] = currentT;
  root["current_humidity"] = currentH;
  root["current_fanspeed"] = getCurrentFanSpeedPercent();
  root.printTo(json);
  server.send(200, "application/json", json);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void apiConfig(){
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    server.send(404, "text/plain", "Configuration json not found. :(");
  }
  String json = configFile.readString();
  configFile.close();
  server.send(200, "application/json", json);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}
