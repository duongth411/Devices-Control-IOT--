
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Update these with values suitable for your network.

const char* ssid = "ENOGY T4.1";
const char* password = "12345678";
const char* mqtt_server = "broker.mqttdashboard.com";
 char* data_out ="abcd";
 int UARTINDEX =0 ;
 const int UARTDATA = 500;
 unsigned char *VrucUARTDATA;
 boolean stringComplete = false; 
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    const char* clientId = "5ccd00a2-955b-42f6-8bbd-36d0cc0df91e";
    const char* user = "abcd";
    const char* pw = "1234";
    // Attempt to connect
    if (client.connect(clientId,user,pw) ){
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("HOST");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  VrucUARTDATA = (unsigned char*) malloc (sizeof(unsigned char) * UARTDATA);
}
void serialEvent() {
  unsigned char inChar;
  unsigned char throwaway;
  while (Serial.available()) {
    inChar = (unsigned char)Serial.read();
    throwaway = inChar;
     if(throwaway <= 31){
     throwaway=0;}
     else if(throwaway>32){
    *(VrucUARTDATA+UARTINDEX) = inChar;
      UARTINDEX++;
    if (inChar == '$') {
      stringComplete = true;
    }
  }
 }
 yield();
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if(Serial.available()){serialEvent();}{
    if (stringComplete) {
      client.publish("CLIENT", VrucUARTDATA,UARTINDEX);
      free(VrucUARTDATA);
      stringComplete = false;
      UARTINDEX =0;
      yield();
    }
  }
}
