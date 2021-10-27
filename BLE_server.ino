
#include <BleKeyboard.h>
BleKeyboard bleKeyboard("Volume Knob", "Nick-tz", 100);
const int ROTARY_A = 17;
const int ROTARY_B = 18;
const int TOGGLE = 5;
const int debounceDelay = 10;
unsigned long riseDetectTime = 0;
unsigned long riseDetectTimeBTN = 0;
int scala = 0;
bool button = false;
void IRAM_ATTR rotary() {
  unsigned long delta = millis() - riseDetectTime;
  if (delta < debounceDelay) {
    return;
  }
  riseDetectTime = millis();
  int b = digitalRead(ROTARY_B);
  int mod = delta > 90 ? 2 : 1;
  if (b == LOW) {
    scala -= 1 * mod ;
  } else if (b == HIGH) {
    scala += 1 * mod;
  }
}
void IRAM_ATTR toggle() {
  if (millis() - riseDetectTimeBTN < debounceDelay) {
    return;
  }
  riseDetectTimeBTN = millis();
  button = true;
}
void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  pinMode(ROTARY_A, INPUT_PULLUP);
  pinMode(ROTARY_B, INPUT_PULLUP);
  pinMode(TOGGLE, INPUT_PULLUP);
  attachInterrupt((ROTARY_A), rotary, FALLING);
  attachInterrupt((TOGGLE), toggle, RISING);
}
void loop() {
  if (bleKeyboard.isConnected()) {
    if (button) {
      bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
      bleKeyboard.release(KEY_MEDIA_PLAY_PAUSE);
      delay(100);
      button = false;
    }
    if (scala > 1) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      bleKeyboard.release(KEY_MEDIA_VOLUME_UP);
      delay(100);
      scala = 1;
    } else if (scala < -1) {
      bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      bleKeyboard.release(KEY_MEDIA_VOLUME_DOWN);
      delay(100);
      scala = -1;
    }
  }
}
