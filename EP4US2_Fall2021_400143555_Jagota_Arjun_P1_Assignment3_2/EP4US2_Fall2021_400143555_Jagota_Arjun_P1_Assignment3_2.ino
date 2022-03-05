
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ThingSpeak.h>
#include <WiFi.h>
#include <BluetoothSerial.h>

//init class 
BluetoothSerial ESP_BT;


// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);



#include "DHT.h"

#define DHTPIN 5     // Digital pin connected to the DHT sensor


#define DHTTYPE DHT11   // DHT 11


DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "BELL874";
const char* password = "6CD137D3264C";


WiFiClient client;

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

unsigned long myChannelNumber = 1528179;
const char * myWriteAPIKey = "CHUA8D7XME4T1T03";

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));
  

  dht.begin();
  sensors.begin();

  WiFi.mode(WIFI_STA);   
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  if((millis() - lastTime) > timerDelay){
  // Wait a few seconds between measurements.
  delay(2000);

  // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }
  //DS18B20

    sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print("DS18B20: ");
  Serial.print(temperatureC);
  Serial.print(F("°C "));
  Serial.print(temperatureF);
  Serial.println(F("°F "));


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("DHT-11: "));
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.println(F("°F"));

  int sensorValue = analogRead(14); 
  Serial.print("Photoresistor: ");
  float voltage = sensorValue*0.001221;
  Serial.print(voltage ); // prints the value read
  Serial.print("V"); // prints the value read
  Serial.print(" \n"); // prints a space between the numbers

     // set the fields with the values
    ThingSpeak.setField(1, temperatureC);
    ThingSpeak.setField(2, temperatureF);
    ThingSpeak.setField(3, h);
    ThingSpeak.setField(4, t);
    ThingSpeak.setField(5, f);
    ThingSpeak.setField(6, voltage);

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();


  
  }
}
