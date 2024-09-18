#include <FastLED.h>
#define NUM_LEDS 512
#define DATA_PIN 12
#define RELE_PIN 13

CRGB leds[NUM_LEDS];

int red = 0;
int green = 100;
int blue = 0;

int BRIGHTNESS = 50;
int DELAY = 20;  // delay in ms
int BETWEEN = 300;
bool RUNNING = false;
int BLINKCOUNT = 0;


void blinkArrow() {
  drawArrow();
  delay(BETWEEN);
  clearDisplay();
  delay(BETWEEN);
}

// *********************************************** PROCESS CMD
void processCMD(char *input) {
  int val = 0;
  int r;
  int xhour, xminute, xsecond;
  Serial.print("PROCESSING COMMAND:");
  Serial.println(input);
  char command[20];

  if (sscanf(input, "%[a-zA-Z]%d", command, &val) == 2) {

    if (strcmp(command, "BRIGHT") == 0) {
      BRIGHTNESS = val;
      FastLED.setBrightness(BRIGHTNESS);
      blinkArrow();
    }

    if (strcmp(command, "RED") == 0) {
      red = val;
      blinkArrow();
    }

    if (strcmp(command, "GREEN") == 0) {
      green = val;
      blinkArrow();
    }

    if (strcmp(command, "BLUE") == 0) {
      blue = val;
      blinkArrow();
    }

    if (strcmp(command, "BETWEEN") == 0) {
      BETWEEN = val;
      blinkArrow();
    }

    if (strcmp(command, "DELAY") == 0) {
      DELAY = val;
      blinkArrow();
    }

    if (strcmp(command, "BLINK") == 0) {
      BLINKCOUNT = val;
      blinkArrow();
    }
  }

  if (strcmp(input, "STOP") == 0) {
    RUNNING = false;
  }
  if (strcmp(input, "START") == 0) {
    RUNNING = true;
  }

  if (strcmp(input, "ON") == 0) {
    digitalWrite(RELE_PIN, HIGH);
  }

  if (strcmp(input, "OFF") == 0) {
    digitalWrite(RELE_PIN, LOW);
  }

  if (strcmp(input, "BLINK") == 0) {
    blinkArrow();
  }

  if (strcmp(input, "LOW") == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (strcmp(input, "HELP") == 0) {
    Serial.println("NAPOVEDA K PRIKAZUM");
    Serial.println("---------------------------");
    Serial.println("BLINK - zablika sipkou");
    Serial.println("BLINKNN - zablika sipkou NN krat (0-10)");
    Serial.println("START - zapne sipku");
    Serial.println("STOP - vypne sipku");
    Serial.println("ON - zapne svetla");
    Serial.println("OFF - vypne svetl");
    Serial.println("REDNNN - nastavi CERVENOU slozku barvy na hodnotu NNN (0-255)");
    Serial.println("GREENNNN - nastavi ZELENOU slozku barvy na hodnotu NNN (0-255)");
    Serial.println("BLUENNN - nastavi MODROU slozku barvy na hodnotu NNN (0-255)");
    Serial.println("DELAYNNN - nastavi pauzu mezi jednotlivymi sloupecky sipky na NNN milisekund. ");
    Serial.println("BETWEENNNN - nastavi dobu sviceni sipky na NNN milisekund. ");
    Serial.println("HELP - zobrazi tuto napovedu");
  }
}

// *********************************************** READ SERIAL
void readSerial() {

  if (Serial.available() > 0) {
    char myCmd[15] = "";
    char oneChar;
    int charPosition = 0;
    delay(100);

    while (Serial.available() > 0 && charPosition < (sizeof(myCmd) - 1)) {
      oneChar = Serial.read();
      if ((int)(oneChar) > 32) {
        myCmd[charPosition++] = oneChar;
      }
    }
    myCmd[charPosition] = '\0';

    processCMD(myCmd);
    sprintf(myCmd, "");
  }
}

// *********************************************** ARROW SHAPE
uint8_t arrow[32] = {
  0b00111100,  // Column 0
  0b00111100,  // Column 1
  0b00111100,  // Column 2
  0b00111100,  // Column 3
  0b00111100,  // Column 4
  0b00111100,  // Column 5
  0b00111100,  // Column 6
  0b00111100,  // Column 7
  0b00111100,  // Column 8
  0b00111100,  // Column 9
  0b00111100,  // Column 10
  0b00111100,  // Column 11
  0b00111100,  // Column 12
  0b00111100,  // Column 13
  0b00111100,  // Column 14
  0b00111100,  // Column 15
  0b00111100,  // Column 16
  0b00111100,  // Column 17
  0b00111100,  // Column 18
  0b00111100,  // Column 19
  0b00111100,  // Column 20
  0b00111100,  // Column 21
  0b00111100,  // Column 22
  0b00111100,  // Column 23
  0b00111100,  // Column 24
  0b00111100,  // Column 25
  0b00111100,  // Column 26
  0b00111100,  // Column 27
  0b11111111,  // Column 28
  0b01111110,  // Column 29
  0b00111100,  // Column 30
  0b00011000   // Column 31
};

// *********************************************** SETUP
void setup() {
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Kralovstvi zeleznic (c) 2024");
  Serial.println("-------------------------------");
  Serial.println("Zelena sipka by clown");
  pinMode(RELE_PIN, OUTPUT);
  digitalWrite(RELE_PIN, LOW);
  green = 0;
  red=255;
  blinkArrow();
  digitalWrite(RELE_PIN, HIGH);
  delay(3000);
  digitalWrite(RELE_PIN, LOW);
  Serial.println("-------------------------------");
  Serial.println("Setup done....");
  Serial.println("-------------------------------");
  green = 0;
  red=255;
}

// *********************************************** DRAW BOTH ARROWS
void drawArrow() {
  for (int col = 0; col < 32; col++) {
    for (int row = 0; row < 8; row++) {
      if (arrow[col] & (1 << row)) {
        leds[row + col * 8] = CRGB(green, red, blue);
        leds[256 + row + col * 8] = CRGB(green, red, blue);
      }
    }
    FastLED.show();
    delay(DELAY);
  }
}

// *********************************************** CLEAR / ERASE THE DISPLAY
void clearDisplay() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}

// *********************************************** LOOP ITSELF
void loop() {
  readSerial();
  if (RUNNING) {
    blinkArrow();
  }
  if (BLINKCOUNT > 0) {
    BLINKCOUNT--;
    blinkArrow();
  }
}
