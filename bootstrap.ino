void loadBootstrap(){
  server.on("/bootstrap.min.css", bootstrap);
  server.on("bootstrap.min.css", bootstrap);
  server.on("/bootstrap.min.js", bootstrapjs);
  server.on("bootstrap.min.js", bootstrapjs);
  server.on("/jquery-3.3.1.min.js", jquery);
  server.on("jquery-3.3.1.min.js", jquery);
  server.on("/popper.min.js", popper);
  server.on("popper.min.js", popper);
}

void jquery(){
  File file = SPIFFS.open("/jquery-3.3.1.min.js.gz", "r");
  size_t sent = server.streamFile(file, "application/javascript");
}
void bootstrap()
{
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r");
  size_t sent = server.streamFile(file, "text/css");
}
void popper()
{
  File file = SPIFFS.open("/popper.min.js.gz", "r");
  size_t sent = server.streamFile(file, "application/javascript");
}
void bootstrapjs()
{
  File file = SPIFFS.open("/bootstrap.min.js.gz", "r");
  size_t sent = server.streamFile(file, "application/javascript");
}
