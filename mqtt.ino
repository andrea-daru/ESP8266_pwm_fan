bool connectToMqtt(){
  mqtt_client.setServer(MQTT_SERVER, 1883);
}

void reconnect() {
  unsigned long startTime = millis();
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (mqtt_client.connect("ESP8266Client")) {
    if (mqtt_client.connect("ESP8266Client", MQTT_USER, MQTT_PWD)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
      if(millis() - startTime > 10000){
        ESP.restart();
      }
    }
  }
}
