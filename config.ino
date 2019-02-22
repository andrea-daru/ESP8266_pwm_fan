
bool loadConfig() {
  Serial.println("Loading configuration");
  
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }
  parseJsonConfig(configFile);
}

/**
 * Parse the configuration JSON and Update the global instance of Configuration struct
 */
void parseJsonConfig(File configFile){
  String string_config = configFile.readString();
  configFile.close();
  Serial.println("CONFIG: ---> " + string_config);
  DynamicJsonBuffer jsonBuffer;
  JsonObject &root = jsonBuffer.parseObject(string_config);
  JsonArray &nodes = jsonBuffer.parseArray(string_config);
  JsonArray &fanSpeedConfig = root["fan_speed_config"];
  int length = fanSpeedConfig.size();
  for(int i = 0; i < length; i++){
    conf.fanSpeed[i].envTemp = double(fanSpeedConfig[i]["temp"]);
    conf.fanSpeed[i].fanSpeed = double(fanSpeedConfig[i]["speed"]);
//    Serial.println("[CONFIG] Env. temp: " + String(conf.fanSpeed[i].envTemp) + "°C --> Fan speed: " + String(conf.fanSpeed[i].fanSpeed)+"%");
  }
  qsort(conf.fanSpeed, length, sizeof(struct FanSpeed), compareFanSpeedConfig);
  for(int i = 0; i < length; i++){
    Serial.println("[CONFIG] SORT Env. temp: " + String(conf.fanSpeed[i].envTemp) + "°C --> Fan speed: " + String(conf.fanSpeed[i].fanSpeed)+"%");
  }
}

/**
 * fan speed comparing function for qsort
 */
int compareFanSpeedConfig(const void *fsc1, const void *fsc2)
{
  struct FanSpeed *fanSpeed1 = (struct FanSpeed *)fsc1;
  struct FanSpeed *fanSpeed2 = (struct FanSpeed *)fsc2;
  return fanSpeed1->envTemp - fanSpeed2->envTemp;
}
