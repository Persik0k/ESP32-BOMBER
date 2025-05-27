#include <BleKeyboard.h>
BleKeyboard keyboard("ESP32-BOMBER");

#define BOOT_BTN 0
#define LED_PIN 2
#define MAX_TEXT_LEN 40
#define CHAR_DELAY 35

char spamText[MAX_TEXT_LEN] = "BOMBER ACTIVATED";
bool spamming = false;
bool btnActive = false;
unsigned long lastSend = 0;

void setup() {
  pinMode(BOOT_BTN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  keyboard.begin();
}

void sendText() {
  if(!keyboard.isConnected()) return;
  
  for(int i = 0; i < strlen(spamText); i++) {
    keyboard.write(spamText[i]);
    delay(CHAR_DELAY);
  }
  keyboard.write(KEY_RETURN);
  delay(CHAR_DELAY * 2);
}

void loop() {
  if(Serial.available()){
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toCharArray(spamText, MAX_TEXT_LEN);
  }

  if(digitalRead(BOOT_BTN)) {
    if(!btnActive) {
      btnActive = true;
      spamming = !spamming;
      digitalWrite(LED_PIN, spamming);
      delay(300);
    }
  } else {
    btnActive = false;
  }

  if(spamming) {
    if(millis() - lastSend > 150) {
      sendText();
      lastSend = millis();
    }
  }
}