void webserverSetup(){
  Serial.println("Setting up HTTP Server ...");
  server.on("/", HTTP_GET, []() {                 // if the client requests the upload page
    if (!handleFileRead("/index.html"))           // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });
  server.on("/", HTTP_POST,                       // if the client posts to the upload page
    [](){ server.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
    handleConfigUpload                                    // Receive and save the file
  );
  
  //JS & CSS
  loadBootstrap();
  
  server.on("/pwmfan.css", []() {
    if (!handleFileRead("/pwmfan.css"))
      server.send(404, "text/plain", "404: Not Found");
  });
  server.on("pwmfan.css", []() {
    if (!handleFileRead("/pwmfan.css"))
      server.send(404, "text/plain", "404: Not Found");
  });

  server.on("/pwmfan.js", []() {
    if (!handleFileRead("/pwmfan.js"))
      server.send(404, "text/plain", "404: Not Found");
  });
  server.on("pwmfan.js", []() {
    if (!handleFileRead("/pwmfan.js"))
      server.send(404, "text/plain", "404: Not Found");
  });
  server.on("/gauge.min.js", []() {
    if (!handleFileRead("/gauge.min.js"))
      server.send(404, "text/plain", "404: Not Found");
  });
  server.on("gauge.min.js", []() {
    if (!handleFileRead("/gauge.min.js"))
      server.send(404, "text/plain", "404: Not Found");
  });
  

  server.on("/api/status",apiStatus);
  server.on("/api/config",apiConfig);
  
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"
  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}



String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}

void handleConfigUpload(){ // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = "/config.json";
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleConfigUpload Size: "); Serial.println(upload.totalSize);
      //server.sendHeader("Location","/success.html");      // Redirect the client to the success page
      //server.send(303);
      ESP.restart();
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void handleRoot() {
  server.send(200, "text", "<p>Current temperature: "+String(currentT)+"</p><p>Current humidity: "+String(currentH)+"</p><p>Current fan speed: "+String(currentFanSpeed)+"%</p>");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
