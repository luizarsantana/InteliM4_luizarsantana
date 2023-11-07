const byte ledPins[] = {17, 19, 22, 23}; // Pinos dos LEDs
const byte buttonPins[] = {2, 4, 15, 18}; // Pinos dos botões 
const byte resetButtonPin = 21;  // Pino do botão de reset

#define MAX_SEQUENCE_LENGTH 20 // Valor máximo que a sequência pode ter

byte gameSequence[MAX_SEQUENCE_LENGTH] = {0};
byte sequenceIndex = 0;

void setup() {
  Serial.begin(9600);

  // Configura os pinos dos LEDs como saída e dos botões como entrada com pull-up
  for (byte i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
  pinMode(resetButtonPin, INPUT_PULLUP);// Configura o pino do botão de reset

  randomSeed(analogRead(A0)); // Inicializa a semente para geração de números aleatórios
}

void lightLed(byte pinIndex) {
  // Acende o LED no pino correspondente e aguarda um curto período antes de apagar
  digitalWrite(ledPins[pinIndex], HIGH);
  delay(300);
  digitalWrite(ledPins[pinIndex], LOW);
}

void playSequence() {
  // Reproduz a sequência de LEDs gerada até o momento
  for (int i = 0; i < sequenceIndex; i++) {
    byte currentLed = gameSequence[i];
    lightLed(currentLed);
    delay(50);
  }
}

// Função para ler os botões e determinar o botão pressionado
byte readButtons() {
  while (true) {
    for (byte i = 0; i < 4; i++) {
      byte buttonPin = buttonPins[i];
      if (digitalRead(buttonPin) == LOW) {
        return i; // Retorna o índice do botão pressionado
      }
    }
    delay(1);
  }
}

void endGame() {
  // Exibe a pontuação final quando o jogo termina
  Serial.print("Game over! Your score: ");
  Serial.println(sequenceIndex - 1);
  sequenceIndex = 0; // Reinicia o índice da sequência
  delay(200);
}

// Função para verificar se a sequência inserida pelo usuário está correta
bool checkUserInputSequence() {
  for (int i = 0; i < sequenceIndex; i++) {
    byte expectedButton = gameSequence[i];
    byte actualButton = readButtons();
    lightLed(actualButton);
    if (expectedButton != actualButton) {
      return false; // Retorna falso se a sequência estiver errada
    }
  }

  return true; // Retorna verdadeiro se a sequência estiver correta
}

void loop() {
  gameSequence[sequenceIndex] = random(0, 4); // Adiciona um novo LED à sequência
  sequenceIndex++;
  
  if (sequenceIndex >= MAX_SEQUENCE_LENGTH) {
    sequenceIndex = MAX_SEQUENCE_LENGTH - 1; // Limita o comprimento máximo da sequência
  }

  playSequence(); // Reproduz a sequência para o jogador
  if (!checkUserInputSequence()) {
    endGame(); // Se a sequência do jogador estiver errada, termina o jogo
  }

  // Verifica se o botão de reset foi pressionado para reiniciar o jogo
  if (digitalRead(resetButtonPin) == LOW) {
    while (digitalRead(resetButtonPin) == LOW) {} // Aguarda o botão ser solto
    sequenceIndex = 0; // Reinicia o jogo
  }
}

