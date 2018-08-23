#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <Twitter.h>
#include <util.h>

#include <SPI.h>
#include <PubSubClient.h>

const int trig = 6;
const int echo = 7;
const int buzzer = 8;
const int levelOne = 5;
const int levelTwo = 3;
const int levelThree = 4;
const float monitorHeight = 0.7;
float time, dist, water;

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0x02};
IPAddress ip(10, 200, 180, 102);
IPAddress server(10, 200, 180, 7);
const char mqtt_topic[]="level";

float level=0;
String sLevel="";
char cLevel[10];
EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  while (!client.connected()) {
  
    if (client.connect("arduinoClient")) {
  
    }else {
      Serial.print(client.state());
  
      delay(5000);
    }
  }
}

void print_local_IP(){}

String float_to_string(float x){
  float y;
  String s;
  y=fmod(x,1);
  y=y*100;
  s=String(int(x))+"."+String(int(y));
  return (s);
}

void setup(){  
  Serial.begin(9600);
  pinMode(echo, INPUT);
  pinMode(trig, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(levelOne, OUTPUT);
  pinMode(levelTwo, OUTPUT);
  pinMode(levelThree, OUTPUT);
 
  client.setClient(ethClient);
  client.setServer(server, 1883);
  Ethernet.begin(mac, ip);
}


void loop(){ 
  if (!client.connected()){ 
    reconnect(); 
  }
  
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  time = pulseIn(echo, HIGH);
  dist = ((time/2) / 29.1) / 100.00;
  water = monitorHeight - dist;
  
  if(water >= 0.2 && water < 0.3) {
    Serial.println(water);
    tone(buzzer, 3000, 500);
    digitalWrite(levelOne, HIGH);
    digitalWrite(levelTwo, LOW);
    digitalWrite(levelThree, LOW);
  } else if(water >= 0.3 && water < 0.4) {
    Serial.println(water);
    tone(buzzer, 2000, 500);
    digitalWrite(levelOne, HIGH);
    digitalWrite(levelTwo, HIGH);
    digitalWrite(levelThree, LOW);
  } else if(water >= 0.4) {
    Serial.println(water);
    tone(buzzer, 500, 500);
    digitalWrite(levelOne, HIGH);
    digitalWrite(levelTwo, HIGH);
    digitalWrite(levelThree, HIGH);
  } else {
    Serial.println(water);
    digitalWrite(levelOne, LOW);
    digitalWrite(levelTwo, LOW);
    digitalWrite(levelThree, LOW);
  }

  Serial.println(water);
  sLevel=float_to_string(water);
  sLevel.toCharArray(cLevel, sLevel.length() + 1);
  client.publish(mqtt_topic,cLevel);
  
  delay(3000);
}

