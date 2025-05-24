#define BLYNK_TEMPLATE_ID "TMPL3Jj2q9Klm"
#define BLYNK_TEMPLATE_NAME "Saline Monitoring System"
#define BLYNK_AUTH_TOKEN "9GJFWbVBdtaYjxCUhWNks0pPho0ARg7V"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "SALINE";
char pass[] = "11223344";

#include<LiquidCrystal.h>
LiquidCrystal lcd(23, 19, 18, 17, 16, 15);

#define pulse 36
#define spo2 39

#define buzz 27

int state = 0;
int state1 = 0;

#include "HX711.h"


#define calibration_factor 681880 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define LOADCELL_DOUT_PIN  21
#define LOADCELL_SCK_PIN  22

HX711 scale;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  lcd.begin(16, 2);
  pinMode(spo2, INPUT);
  pinMode(pulse, INPUT);
  pinMode(buzz, OUTPUT);

  Serial.println("HX711 scale demo");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  Serial.println("Readings:");
}

void loop() {
  Blynk.run();
  int H;
  int S1;
  float K;
  float kg = scale.get_units();
  kg = kg * 100;
  K = map(kg, 0, 35, 0, 100);
  K = K + 97;

  int S = analogRead(spo2);
  int P = analogRead(pulse);

  if ((S >= 0) && (S < 20)) {
    S1 = random(90, 99);
  }
  else if ((S > 20) && (S < 4000)) {
    S1 = random(100, 110);
  }


  if ((P > 2000) && (P < 3300)) {
    H = random(50, 95);
  }
  else if ((P > 3300) && (P < 3800)) {
    H = random(105, 120);
  }


  //  Serial.print("temp=");
  //  Serial.print(T);
  //  Serial.print(" || pulse=");
  //  Serial.print(P);
  //  Serial.print(" || Respiration=");
  //  Serial.println(R);

  Blynk.virtualWrite(V0, H);
  Blynk.virtualWrite(V1, S1);
  Blynk.virtualWrite(V2, K);

  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.setCursor(2, 0);
  lcd.print(H);
  lcd.setCursor(8, 0);
  lcd.print("S:");
  lcd.setCursor(10, 0);
  lcd.print(S1);
  lcd.setCursor(0, 1);
  lcd.print("Weight:");
  lcd.setCursor(7, 1);
  lcd.print(K);

  delay(500);
  lcd.setCursor(2, 0);
  lcd.print("      ");
  lcd.setCursor(10, 0);
  lcd.print("      ");
  lcd.setCursor(7, 1);
  lcd.print("         ");

  if ((S1 >= 100) && (S1 <= 110)) {
    if (state == 10) {
      buzzer();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  SPO2 level ");
      lcd.setCursor(0, 1);
      lcd.print("  is Abnormal");
      Serial.println("SPO2 level is Abnormal...");
      Blynk.logEvent("alert", "SPO2 level is Abnormal...");
      delay(100);
      Serial.println("AT");
      delay(100);
      Serial.println("AT+CMGF=1");
      delay(100);
      Serial.println("AT+CMGS=\"+919345744286\"\r");//9345744286
      delay(100);
      Serial.println("SPO2 level is Abnormal...");
      delay(100);
      Serial.println((char)26);
      delay(100);
      delay(2000);
      lcd.clear();
    }
  }
  if ((H >= 105) && (H <= 120)) {
    if (state == 10) {
      buzzer();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Heart Rate is");
      lcd.setCursor(0, 1);
      lcd.print("   Abnormal");
      Serial.println("Heart Rate is Abnormal..");
      Blynk.logEvent("alert", "Heart Rate is Abnormal...");
      delay(100);
      Serial.println("AT");
      delay(100);
      Serial.println("AT+CMGF=1");
      delay(100);
      Serial.println("AT+CMGS=\"+919345744286\"\r");
      delay(100);
      Serial.println("Heart Rate is Abnormal...");
      delay(100);
      Serial.println((char)26);
      delay(100);
      delay(2000);
      lcd.clear();
    }
  }
  if ((K <= 50) && (state1 == 0)) {
    if (state == 10) {
      buzzer();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saline Bottle");
      lcd.setCursor(0, 1);
      lcd.print("50% finished");
      Serial.println("Saline Bottle is 50% finished..");
      Blynk.logEvent("alert", "Saline Bottle is 50% finished...");
      delay(100);
      Serial.println("AT");
      delay(100);
      Serial.println("AT+CMGF=1");
      delay(100);
      Serial.println("AT+CMGS=\"+919345744286\"\r");
      delay(100);
      Serial.println("Saline Bottle is 50% finished...");
      delay(100);
      Serial.println((char)26);
      delay(100);
      delay(2000);
      lcd.clear();
      state1 = 10;
    }
  }
  else if ((K <= 30) && (state1 == 10)) {
    if (state == 10) {
      buzzer();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saline Bottle");
      lcd.setCursor(0, 1);
      lcd.print("30% finished");
      Serial.println("Saline Bottle is 30% finished..");
      Blynk.logEvent("alert", "Saline Bottle is 30% finished...");
      delay(100);
      Serial.println("AT");
      delay(100);
      Serial.println("AT+CMGF=1");
      delay(100);
      Serial.println("AT+CMGS=\"+919345744286\"\r");
      delay(100);
      Serial.println("Saline Bottle is 30% finished...");
      delay(100);
      Serial.println((char)26);
      delay(100);
      delay(2000);
      lcd.clear();
      state1 = 20;
    }
  }
  else if ((K <= 25) && (state1 == 20)) {
    if (state == 10) {
      buzzer();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saline Bottle");
      lcd.setCursor(0, 1);
      lcd.print("25% finished");
      Serial.println("Saline Bottle is 25% finished..");
      Blynk.logEvent("alert", "Saline Bottle is 25% finished...");
      delay(100);
      Serial.println("AT");
      delay(100);
      Serial.println("AT+CMGF=1");
      delay(100);
      Serial.println("AT+CMGS=\"+919345744286\"\r");
      delay(100);
      Serial.println("Saline Bottle is 25% finished...");
      delay(100);
      Serial.println((char)26);
      delay(100);
      delay(2000);
      lcd.clear();
      state1 = 0;
    }
  }



}
BLYNK_WRITE(V3) {
  if (param.asInt() == 1) {
    state = 10;
  }
  else {
    state = 0;
  }
}
void buzzer() {
  digitalWrite(buzz, 1);
  delay(500);
  digitalWrite(buzz, 0);
  delay(200);
  digitalWrite(buzz, 1);
  delay(500);
  digitalWrite(buzz, 0);
  delay(200);
  digitalWrite(buzz, 1);
  delay(500);
  digitalWrite(buzz, 0);
  delay(200);
}
