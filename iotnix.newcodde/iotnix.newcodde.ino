#define BLYNK_TEMPLATE_ID "TMPL3SvxvoPQL"
#define BLYNK_TEMPLATE_NAME "IOTNIXX"
#define BLYNK_AUTH_TOKEN "iDl8bxEF_4jpoK8_RXIvuDpr6oo-De52"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// WiFi Credentials
char ssid[] = "Praveen"; 
char pass[] = "96694092";

// Pin Definitions
#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ6PIN 34
#define RELAY_PIN 13 // Shifted to D13 for Boot Stability

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  
  // Relay Setup - High means OFF for Active Low modules
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); 

  // LCD Initialization
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TEAM: IOTNIX");
  lcd.setCursor(0, 1);
  lcd.print("System Booting..");

  dht.begin();

  // WiFi & Blynk Setup (Non-Blocking)
  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN);
  Blynk.connect(); // Background connection attempt

  delay(2000);
  lcd.clear();
}

void loop() {
  // Run Blynk only if connected to avoid lag
  if (Blynk.connected()) {
    Blynk.run();
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasVal = analogRead(MQ6PIN);

  // 1. LCD Display Update (Hamesha Chalega)
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print((int)t); lcd.print("C  H:"); lcd.print((int)h); lcd.print("%  ");
  
  lcd.setCursor(0, 1);
  lcd.print("G:"); lcd.print(gasVal); 

  // 2. Relay Control Logic (Active Low Fix)
  // Threshold testing ke liye 40% rakha hai, demo ke waqt 70 kar dena
  if(h > 40.0 || gasVal > 1500) { 
    digitalWrite(RELAY_PIN, LOW); // Relay ON (Peltier & Fan Start)
    lcd.setCursor(11, 1);
    lcd.print("!ON ");
    
    if (Blynk.connected()) {
      Blynk.virtualWrite(V3, 255); // Blynk LED/Indicator ON
    }
  } 
  else {
    digitalWrite(RELAY_PIN, HIGH); // Relay OFF
    lcd.setCursor(11, 1);
    lcd.print(" OK ");
    
    if (Blynk.connected()) {
      Blynk.virtualWrite(V3, 0); // Blynk LED/Indicator OFF
    }
  }

  // 3. Cloud Data Sync (Every 2 seconds)
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 2000 && Blynk.connected()) {
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, gasVal);
    lastUpdate = millis();
  }

  delay(500); // Faster Loop for Sensor Responsiveness
}
