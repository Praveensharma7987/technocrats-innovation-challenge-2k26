#define BLYNK_TEMPLATE_ID "TMPL3SvxvoPQL"
#define BLYNK_TEMPLATE_NAME "IOTNIXX"
#define BLYNK_AUTH_TOKEN "iDl8bxEF_4jpoK8_RXIvuDpr6oo-De52"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

char ssid[] = "Praveen"; 
char pass[] = "96694092";

#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ6PIN 34
#define RELAY_PIN 12

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  // LCD Startup
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TEAM: IOTNIX");
  lcd.setCursor(0, 1);
  lcd.print("Connecting...");

  dht.begin();
  
  // PURANA CONNECTION LOGIC (JO KAAM KAR RAHA THA)
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  lcd.clear();
  lcd.print("System Online!");
  delay(1000);
}

void loop() {
  Blynk.run();

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasVal = analogRead(MQ6PIN);

  // LCD Update
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print((int)t); lcd.print("C  H:"); lcd.print((int)h); lcd.print("%  ");
  lcd.setCursor(0, 1);
  lcd.print("G:"); lcd.print(gasVal); 

  // Relay & Blynk Sync
  if(h > 70.0 || gasVal > 1500) {
    digitalWrite(RELAY_PIN, LOW);
    lcd.setCursor(10, 1);
    lcd.print("[!ON]");
    Blynk.virtualWrite(V3, 255); 
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    lcd.setCursor(10, 1);
    lcd.print("[ OK]");
    Blynk.virtualWrite(V3, 0);
  }

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, gasVal);

  delay(1000); 
}
