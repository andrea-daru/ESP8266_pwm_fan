double getCurrentFanSpeedPercent(){
  return  double(currentFanSpeed)/(double(1024)/double(100));
}

int setFanSpeed(double percentValue){
  double value = (double(1024)/double(100))*percentValue;
  if(currentFanSpeed != int(value)){
    currentFanSpeed = int(value);
    analogWrite(FANPIN, currentFanSpeed);
    Serial.print("Fan speed set to: ");
    Serial.print(percentValue);
    Serial.println("% PWM: " + String(currentFanSpeed));
    mqtt_client.publish(MQTT_FAN_TOPIC, String(percentValue).c_str(), true);
    // send mqtt data    
  }
}

void setFanSpeedByTemperature(double tCurr){
  int     index  = 0;
  double  tLow   = 0.00;
  double  fLow   = 0.00;
  double  tHigh  = 0.00;
  double  fHigh  = 0.00;
  while(tCurr > conf.fanSpeed[index].envTemp){
      index++;
  }

  if(index > 0){
    tLow = conf.fanSpeed[index - 1].envTemp;
    fLow = conf.fanSpeed[index - 1].fanSpeed;
    tHigh = conf.fanSpeed[index].envTemp;
    fHigh = conf.fanSpeed[index].fanSpeed;
    double dt = tHigh - tLow;
    double df = fHigh - fLow;
    
    double tCurrDevPerc = (tCurr - tLow) / (dt/100);
    double fanSpeed = fLow + ((df/100)*tCurrDevPerc);
    setFanSpeed(fanSpeed);
  }else{
    fHigh = conf.fanSpeed[index].fanSpeed;
    setFanSpeed(fHigh);
  }
}
