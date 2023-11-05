const byte ledPins[] = {17, 19, 22, 23};
const byte buttonPins[] = {2, 4, 15, 18};

#define MAX_SEQUENCE_LENGTH 20

byte gameSequence[MAX_SEQUENCE_LENGTH] = {0};
byte sequenceIndex = 0;

void setup() {
  Serial.begin(9600);

  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  randomSeed(analogRead(A0));
}

void lightLed(byte pinIndex) {
  digitalWrite(ledPins[pinIndex], HIGH);
  delay(300);
  digitalWrite(ledPins[pinIndex], LOW);
}

void playSequence() {
  for (int i = 0; i < sequenceIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLed(currentLed);
    delay(50);
  }
}

byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        return i;
      }
    }
    delay(1);
  }
}

void endGame() {
  Serial.print("Game over! Your score: ");
  Serial.println(sequenceIndex - 1);
  sequenceIndex = 0;
  delay(200);
}

bool checkUserInputSequence() {
  for (int i = 0; i < sequenceIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    lightLed(actualButton);
    if (expectedButton != actualButton) {
      return false;
    }
  }

  return true;
}

void loop() {
  gameSequence[sequenceIndex] = random(0, 4);
  sequenceIndex++;
  
  if (sequenceIndex >= MAX_SEQUENCE_LENGTH) {
    sequenceIndex = MAX_SEQUENCE_LENGTH - 1;
  }

  playSequence();
  if (!checkUserInputSequence()) {
    endGame();
  }
}
