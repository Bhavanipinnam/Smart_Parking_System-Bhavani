#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include<Servo.h>

Servo myservo;




const char* ssid = "Nokia";
const char* password = "Manu2180";



#define ORG "pate1v"
#define DEVICE_TYPE "shruthi"
#define DEVICE_ID "2180"
#define TOKEN ")(vkOt6Z!Syqj*cP4q"
String command;


#define trigpin D2
#define echopin D3
#define green D5
#define red D6
#define light D7

int duration,distance;

char server[] = ORG ".messaging.internetofthings.ibmcloud.com";
char pubtopic[] = "iot-2/evt/Data/fmt/json";
char topic[] = "iot-2/cmd/home/fmt/String";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
//Serial.println(clientID);

WiFiClient wifiClient;
void callback(char* topic, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);
void setup() {
  Serial.begin(115200);
  myservo.attach(D1);
  pinMode(trigpin,OUTPUT);
  pinMode(echopin,INPUT);
  pinMode(green,OUTPUT);
  pinMode(red,OUTPUT);
  Serial.println();
  wifiConnect();
  mqttConnect();
}

void loop() {
  digitalWrite(trigpin,HIGH);
  delay(100);
  digitalWrite(trigpin,LOW);
  duration=pulseIn(echopin,HIGH);
  distance=(duration/2)*0.0343;

  if(distance<=10)
  {
    digitalWrite(red,HIGH);
    digitalWrite(green,LOW);
    myservo.write(180);
  }
  else
  {
    digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
    myservo.write(-180);
  }

  if (!client.loop()) {
    mqttConnect();
  }
  PublishData(distance);
delay(100);
}

void wifiConnect() {
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

void mqttConnect() {
  if (!client.connected()) {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!client.connect(clientId, authMethod, token)) {
      Serial.print(".");
      delay(500);
    }
    initManagedDevice();
    Serial.println();
  }
}
void initManagedDevice() {
  if (client.subscribe(topic)) {
    Serial.println("subscribe to cmd OK");
  } else {
    Serial.println("subscribe to cmd FAILED");
  }
}

void callback(char* topic, byte* payload, unsigned int payloadLength) {
  Serial.print("callback invoked for topic: "); Serial.println(topic);

  for (int i = 0; i < payloadLength; i++) {
    //Serial.println((char)payload[i]);
    command += (char)payload[i];
  }
Serial.println(command);
if(command == "LIGHTON"){
  digitalWrite(light,LOW);
  Serial.println("Light is ON");
  
}
else if(command == "LIGHTOFF"){
  Serial.println("Motor is OFF");
  digitalWrite(light,HIGH);
}
command ="";
}
void PublishData(float distance){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"distance\":";
  payload += distance;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(pubtopic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}
