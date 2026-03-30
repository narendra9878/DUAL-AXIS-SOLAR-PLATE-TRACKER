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
const char* ssid = "Name";
const char* password = "Password";

WiFiClient client;

// ================= THINGSPEAK =================
unsigned long channelNumber = 3310713;
const char* writeAPIKey = "H0K6TCH7ZE9R1UVG";

// ================= LDR PINS =================
#define LDR1 34
#define LDR2 35
#define LDR3 32
#define LDR4 33

// ================= MOTOR DRIVER PINS =================
#define IN1 25 
#define IN2 26 
#define IN3 27 
#define IN4 14 

// ================= LIMIT SWITCHES =================
#define LIMIT_LEFT   18
#define LIMIT_RIGHT  19
#define LIMIT_UP     21
#define LIMIT_DOWN   22

// ================= SINGLE THRESHOLD =================
int threshold = 120;

// ================= TIMER =================
unsigned long lastThingSpeak = 0;
unsigned long interval = 15000;

// ================= LDR SMOOTH FUNCTION =================
int readLDR(int pin) {
  int sum = 0;
  for (int i = 0; i < 5; i++) {
    sum += analogRead(pin);
    delay(5);
  }
  return sum / 5;
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(LIMIT_LEFT, INPUT_PULLUP);
  pinMode(LIMIT_RIGHT, INPUT_PULLUP);
  pinMode(LIMIT_UP, INPUT_PULLUP);
  pinMode(LIMIT_DOWN, INPUT_PULLUP);

  stopAllMotors();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  ThingSpeak.begin(client);
}

// ================= MOTOR FUNCTIONS =================
void stopX() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void stopY() {
  digitalWrite(IN3, LOW);  
  digitalWrite(IN4, LOW);
}

void stopAllMotors() {
  stopX();
  stopY();
}

// ================= LOOP =================
void loop() {

  // ----------- LDR READINGS (NOW SMOOTHED) -----------
  int ldr1 = readLDR(LDR1);
  int ldr2 = readLDR(LDR2);
  int ldr3 = readLDR(LDR3);
  int ldr4 = readLDR(LDR4);

  int top = (ldr1 + ldr2) / 2;
  int bottom = (ldr3 + ldr4) / 2;
  int left = (ldr1 + ldr3) / 2;
  int right = (ldr2 + ldr4) / 2;

  Serial.print("L:");
  Serial.print(left);
  Serial.print(" R:");
  Serial.print(right);
  Serial.print(" T:");
  Serial.print(top);
  Serial.print(" B:");
  Serial.println(bottom);

  // ----------- PRIORITY CONTROL -----------

int diffX = left - right;
int diffY = top - bottom;

// 👉 PRIORITY: X AXIS FIRST
if (abs(diffX) > threshold) {

  stopY();  // 🔥 stop Y motor

  if (diffX > 0 && digitalRead(LIMIT_LEFT) == HIGH) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } 
  else if (diffX < 0 && digitalRead(LIMIT_RIGHT) == HIGH) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } 
  else {
    stopX();
  }

} 
// 👉 ONLY IF X IS STABLE → MOVE Y
else if (abs(diffY) > threshold) {

  stopX();  // 🔥 stop X motor

  if (diffY > 0 && digitalRead(LIMIT_UP) == HIGH) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } 
  else if (diffY < 0 && digitalRead(LIMIT_DOWN) == HIGH) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } 
  else {
    stopY();
  }

} 
else {
  stopAllMotors();  // perfect alignment
}
  // ----------- DHT + THINGSPEAK -----------
  if (millis() - lastThingSpeak > interval) {

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (isnan(temp) || isnan(hum)) {
      Serial.println("DHT Error!");
    } else {
      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print(" | Humidity: "); 
      Serial.println(hum);

      ThingSpeak.setField(1, temp);
      ThingSpeak.setField(2, hum);

      int x = ThingSpeak.writeFields(channelNumber, writeAPIKey);

      if (x == 200) {
        Serial.println("Data sent");
      } else {
        Serial.println("ThingSpeak Error");
      }
    }

    lastThingSpeak = millis();
  }

  delay(20);  // smooth motion
}
digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
