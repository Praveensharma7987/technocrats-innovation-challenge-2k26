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
#define RELAY_PIN 13

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  // 1. LCD Pehle Chalu Hogi
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("TEAM: IOTNIX");
  
  dht.begin();

  // 2. WiFi Connection (Non-Blocking)
  Serial.println("Connecting WiFi...");
  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN); 
  
  lcd.setCursor(0, 1);
  lcd.print("System Ready...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // 3. Blynk tabhi chalega jab WiFi ho, warna skip karega
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasVal = analogRead(MQ6PIN);

  // 4. LCD Update (Hamesha chalega)
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print((int)t); lcd.print("C  H:"); lcd.print((int)h); lcd.print("%  ");
  lcd.setCursor(0, 1);
  lcd.print("G:"); lcd.print(gasVal); 


if(h > 40.0 || gasVal > 1500) { 
    digitalWrite(RELAY_PIN, LOW);   // Relay ON ho jayega agar humidity 40 se upar hai
    lcd.setCursor(11, 1);
    lcd.print("!ON ");
    if (WiFi.status() == WL_CONNECTED) Blynk.virtualWrite(V3, 255);
} else {
    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
    lcd.setCursor(11, 1);
    lcd.print(" OK ");
    if (WiFi.status() == WL_CONNECTED) Blynk.virtualWrite(V3, 0);
}


  // 6. Cloud Data Sync
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Blynk.virtualWrite(V2, gasVal);
  }

  delay(1000); 
}
