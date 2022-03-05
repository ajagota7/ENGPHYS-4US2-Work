
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>

Adafruit_BMP085 bmp;


#define SIGNAL_PIN 15
//Ultrasonic Range Finder
const int trigPin = 27;
const int echoPin = 25; 


const char* ssid = "BELL874";
const char* password = "6CD137D3264C";


const char* mqtt_server = "broker.hivemq.com"; 




WiFiClient espClient;
PubSubClient client(espClient);
long timerDelay = 0;
char msg[50];
int value = 0;

//float bmpPress;
int pir;
#define SOUND_SPEED 0.034
long duration;
float usd;

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
//  if (!bmp.begin()) {
//    Serial.println("Could not find a valid BMP085/BMP180 sensor, check wiring!");
//    while (1);
//  }
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(SIGNAL_PIN, INPUT); //setup motion sensor
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output ultrasonic
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input  ultrasonic
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("arjunesp32/output"); 
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
  if (String(topic) == "arjunesp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
}
}



void loop() {
  //wifi
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  if (millis() - timerDelay > 30000) {
    timerDelay = millis();
//    temperature = bmp.readTemperature();
//    pressure = bmp.readPressure();
    if(digitalRead(SIGNAL_PIN)==HIGH) {
      pir = 1;

    } else {
      pir = 0;
    }
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(echoPin, HIGH);
    // Calculate the distance
    usd = duration * SOUND_SPEED/2;
//    


//
//    // Convert the values to a char array
//    char pressSt[8];
//    dtostrf(bmpPress, 1, 2, presString);
//    Serial.print("Pressure: ");
//    Serial.println(presSt);
//    client.publish("arjunesp32/pressure", pressSt);

      Serial.println(pir);
    char pirSt[8];
    dtostrf(pir, 1, 2, pirSt);
    Serial.print("Movement: ");
    Serial.println(pirSt);
    client.publish("arjunesp32/pir", pirSt);

    char usdSt[8];
    dtostrf(usd, 1, 2, usdSt);
    Serial.print("Distance: ");
    Serial.println(usdSt);
    client.publish("arjunesp32/usd", usdSt);
   
  }
}
