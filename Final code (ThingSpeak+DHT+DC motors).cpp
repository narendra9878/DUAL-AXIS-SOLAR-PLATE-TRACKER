/*
 * ⚠️ IMPORTANT NOTE:
 * This code is written for the Arduino framework running on ESP32.
 * It is not standard C++ code and requires the Arduino IDE or compatible
 * environment (such as PlatformIO) with ESP32 board support installed.
 */
#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// ================= DHT SETUP =================
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ================= WIFI =================
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WiFiClient client;

// ================= THINGSPEAK =================
unsigned long channelNumber = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";

// ================= LDR PINS =================
#define LDR1 34
#define LDR2 35
#define LDR3 32
#define LDR4 33

// ================= MOTOR DRIVER PINS =================
// Motor X (Horizontal)
#define IN1 18
#define IN2 19

// Motor Y (Vertical)
#define IN3 21
#define IN4 22

int threshold = 50;

// ================= TIMER =================
unsigned long lastThingSpeak = 0;
unsigned long interval = 15000;

// ================= SETUP =================
void setup() {
  Serial.begin(115200);

  dht.begin();

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Stop motors initially
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  ThingSpeak.begin(client);
}

// ================= LOOP =================
void loop() {

  // ----------- LDR READINGS -----------
  int ldr1 = analogRead(LDR1);
  int ldr2 = analogRead(LDR2);
  int ldr3 = analogRead(LDR3);
  int ldr4 = analogRead(LDR4);

  int top = (ldr1 + ldr2) / 2;
  int bottom = (ldr3 + ldr4) / 2;
  int left = (ldr1 + ldr3) / 2;
  int right = (ldr2 + ldr4) / 2;

  // ----------- X AXIS MOTOR -----------
  if (abs(left - right) > threshold) {
    if (left > right) {
      // rotate left
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else {
      // rotate right
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    }
  } else {
    // stop motor
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  // ----------- Y AXIS MOTOR -----------
  if (abs(top - bottom) > threshold) {
    if (top > bottom) {
      // move up
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else {
      // move down
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    }
  } else {
    // stop motor
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  // ----------- DHT + THINGSPEAK -----------
  if (millis() - lastThingSpeak > interval) {

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("DHT Error!");
      return;
    }

    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" | Humidity: ");
    Serial.println(hum);

    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, hum);

    int x = ThingSpeak.writeFields(channelNumber, writeAPIKey);

    if (x == 200) {
      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("ThingSpeak Error");
    }

    lastThingSpeak = millis();
  }

  delay(200);
}
