#include <Adafruit_NeoPixel.h>
#include <BlynkSimpleSerialBLE.h>
#include <Encoder.h>

//brightness button
const int buttonPin = 5;
int buttonState = 0;
bool bright = true;
int highBrightness = 255;
int lowBrightness = 17;
int btntimer = 0;

#define PIN 6
#define NUM_LEDS 32
int outline1[] = {16, 23, 24, 31, 30, 29, 28, 27, 20, 19, 18, 17};
int outline2[] = {0, 1, 2, 3, 4, 11, 12, 13, 14, 15, 8, 7};
int row[][4] = {{31, 32, 32, 32}, {24 , 30, 32, 32} , {23, 25 , 29, 32} , {16, 22, 26, 28}, {17 , 21 , 27, 32} , { 18, 20, 32, 32} , {19, 32, 32, 32}, {15, 32, 32, 32}, {8 , 14, 32, 32} , {7, 9 , 13, 32} , {0, 6, 10, 12}, {1 , 5 , 11, 32} , { 2, 4, 32, 32} , {3, 32, 32, 32}};
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
BlynkTimer timer;
Encoder myEnc(3, 4);
int rainbow, wipe, bot, constcyc;
int i = 0, y = 0;
boolean LR = true;
boolean RL = true;
void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.setBrightness(highBrightness);
  strip.show();
  myEnc.write(0);
  pinMode(buttonPin, INPUT_PULLUP);
  rainbow = timer.setInterval(5L, rainbowCycle);
  wipe = timer.setInterval(50L, MultiWipe);
  bot = timer.setInterval(50L, Cybot);
  constcyc = timer.setInterval(50L, Cycle);
  timer.disable(rainbow);
  timer.disable(wipe);
  timer.disable(bot);
  timer.disable(constcyc);
  Eyes();
}
void loop() {
  timer.run();
  brightness();
  long newPos = myEnc.read() / 4.0;
  if (newPos < 0) {
    myEnc.write(16);
    newPos = 4;
  }
  if (newPos > 4) {
    myEnc.write(0);
    newPos = 0;
  }
  if (newPos == 0) {
    timer.disable(wipe);
    timer.disable(rainbow);
    timer.disable(bot);
    timer.disable(constcyc);
    Eyes();
  }
  if (newPos == 1) {
    timer.disable(wipe);
    timer.disable(bot);
    timer.disable(constcyc);
    timer.enable(rainbow);
  }
  if (newPos == 2) {
    timer.disable(rainbow);
    timer.disable(bot);
    timer.disable(constcyc);
    timer.enable(wipe);
  }

  if (newPos == 3) {
    timer.disable(wipe);
    timer.disable(rainbow);
    timer.disable(constcyc);
    timer.enable(bot);
  }
  if (newPos == 4) {
    timer.disable(wipe);
    timer.disable(rainbow);
    timer.disable(bot);
    timer.enable(constcyc);
  }
}
void WipeEyes(uint32_t c, uint8_t wait) {
  int x = 0;
  while (x < 12) {
    strip.setPixelColor(outline1[x], c);
    strip.setPixelColor(outline2[x], c);
    strip.show();
    x++;
    delay(wait);
  }
}
void fillOutline(uint32_t c) {
  int x = 0;
  while (x < 12) {
    strip.setPixelColor(outline1[x], c);
    strip.setPixelColor(outline2[x], c);
    x++;
  }
  strip.show();
}
void fillLR(uint32_t c, uint8_t wait) {
  for (int q = 0; q < 7; q++) {
    for (int p = 0; p < 4; p ++) {
      if (row[q][p] != 32) {
        strip.setPixelColor(row[q][p], c);
      }
      strip.show();
    }
    delay(wait);
  }
  for (int q = 0; q < 7; q++) {
    for (int p = 0; p < 4; p ++) {
      if (row[q][p] != 32) {
        strip.setPixelColor(row[q][p] - 16, c);
      }
      strip.show();
    }
    delay(wait);
  }
}
void poleLR(uint32_t c, uint8_t wait) {
  for (int q = 0; q < 7; q++) {
    for (int p = 0; p < 4; p ++) {
      if (row[q][p] != 32) {
        strip.setPixelColor(row[q][p], c);
      }
      if (row[q - 1][p] != 32 && q != 0) {
        strip.setPixelColor(row[q - 1][p], strip.Color(0, 0, 0));
      }
    }
    strip.show();
    delay(wait);
  }
  strip.setPixelColor(row[6][0], strip.Color(0, 0, 0));
  strip.show();
  delay(wait);
  for (int q = 0; q < 7; q++) {
    for (int p = 0; p < 4; p ++) {
      if (row[q][p] != 32) {
        strip.setPixelColor(row[q][p] - 16, c);
      }
      if (row[q - 1][p] != 32 && q != 0) {
        strip.setPixelColor(row[q - 1][p] - 16, strip.Color(0, 0, 0));
      }
    }
    strip.show();
    delay(wait);
  }
  strip.setPixelColor(row[6][0] - 16, strip.Color(0, 0, 0));
  strip.show();
  delay(wait);
}
void poleRL(uint32_t c, uint8_t wait) {
  for (int q = 7; q > - 1; q--) {
    for (int p = 0; p < 4; p ++) {
      if (row[q][p] != 32) {
        strip.setPixelColor(row[q][p] - 16, c);
      }
      if (row[q + 1][p] != 32) {
        strip.setPixelColor(row[q + 1][p] - 16, strip.Color(0, 0, 0));
      }
    }
    strip.show();
    delay(wait);
  }
  strip.setPixelColor(row[0][0] - 16, strip.Color(0, 0, 0));
  strip.show();
  delay(wait);
  for (int q = 6; q > -1; q--) {
    for (int p = 0; p < 4; p ++) {
      if (row[q][p] != 32) {
        strip.setPixelColor(row[q][p], c);
      }
      if (row[q + 1][p] != 32) {
        strip.setPixelColor(row[q + 1][p], strip.Color(0, 0, 0));
      }
    }
    strip.show();
    delay(wait);
  }
  strip.setPixelColor(row[0][0], strip.Color(0, 0, 0));
  strip.show();
  delay(wait);
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void rainbowCycle() {
  uint8_t wait = 5;
  uint16_t i, j;
  for (j = 0; j < 256; j++) {
    for (i = 0; i < 14; i++) {
      for (int x = 0; x < 4; x++) {
        if (row[i][x] != 32) {
          strip.setPixelColor(row[i][x], Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
      }
    }
    strip.show();
    delay(wait);
  }
}
void MultiWipe() {
  if (LR == true) {
    WipeEyes(strip.Color(255, 0, 0), 50);
    poleLR(strip.Color(0, 255, 0), 50);
    LR = false;
  }
  else {
    WipeEyes(strip.Color(0, 255, 0), 50);
    poleRL(strip.Color(255, 0, 0), 50);
    LR = true;
  }
}
void Eyes() {
  for (int xz = 0; xz < 32; xz++) {
    strip.setPixelColor(xz, strip.Color(0, 0, 0));
  }
  fillOutline(strip.Color(255, 0, 0));
  strip.show();
}
void Cybot() {
  if (RL == true) {
    poleLR(strip.Color(0, 255, 0), 50);
    RL = false;
  }
  else {
    poleRL(strip.Color(255, 0, 0), 50);
    RL = true;
  }
}
void Cycle() {
  WipeEyes(strip.Color(0, 255, 0), 50);
  WipeEyes(strip.Color(255, 0, 0), 50);
}
void brightness() {
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    int math = btntimer + 750;
    if (millis() > math) { //
      changeBright();
    }
    btntimer = millis();
  }
}
void changeBright() {
  if (bright == true) {
    bright = false;
  } else {
    bright = true;
  }
  if(bright){
    strip.setBrightness(highBrightness);
  }else{
    strip.setBrightness(lowBrightness);
  }
}
