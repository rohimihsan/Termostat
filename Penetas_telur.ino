#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTTYPE DHT11

const int Relay_pin = 14;
const int Dht_pin = 2;

DHT dht(Dht_pin, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

bool peak = false;

void setup() {
  pinMode(Relay_pin, OUTPUT);
  Serial.begin(9600);
  Serial.println("setup begin");

  dht.begin();
  lcd.begin();

  lcd.print("Memulai Perangkat");
}

void loop() {
  delay(2000);
  lcd.clear();

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.print("Kesalahan pada sensor suhu");
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  lcd.backlight();
  lcd.print(t);
  lcd.print(F("°C "));
  lcd.print(F("Humidity: "));
  lcd.print(h);

  if (t >= 38) {
    //relay off
    digitalWrite(Relay_pin, HIGH);
    delay(1000);
    peak = true;
  } else {
    if (peak == true) {
      if (t <= 36) {
        digitalWrite(Relay_pin, LOW);
        delay(1000);
        peak = false;
      }
    } else {
      //relay on
      digitalWrite(Relay_pin, LOW);
      delay(1000);
    }
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
}
