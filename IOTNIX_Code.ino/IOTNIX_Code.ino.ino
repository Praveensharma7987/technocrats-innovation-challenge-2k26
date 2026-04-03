#define BLYNK_TEMPLATE_ID "TMPL3SvxvoPQL"
#define BLYNK_TEMPLATE_NAME "IOTNIXX"
#define BLYNK_AUTH_TOKEN "iDl8bxEF_4jpoK8_RXIvuDpr6oo-De52"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Praveen"; 
char pass[] = "96694092";

#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ6PIN 34
#define RELAY_PIN 12

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("--- IOTNIX CONNECTING ---");
  
  // Blynk connection with debug prints
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
}

void loop() {
  Blynk.run();
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int gasVal = analogRead(MQ6PIN);

  // Send data to Blynk
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, gasVal);

  Serial.print("T: "); Serial.print(t); 
  Serial.print(" | H: "); Serial.println(h);
  
  delay(2000); 
}
