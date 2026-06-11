#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); 

#define pinRX 50
#define pinTX 53

SoftwareSerial serialMP3(pinRX, pinTX);
DFRobotDFPlayerMini moduloMP3;

const int BTN[4] = {4, 10, 8, 12};
const int LED_BOTOES[4] = {5, 9, 7, 11}; 

const int LED_VIDAS[4][3] = {
  {22, 24, 26},
  {28, 30, 32},
  {31, 33, 35},
  {36, 37, 38}
};

const int BTN_BATALHA = 2; 

const String cores[4] = {"Vermelho", "Amarelo", "Verde", "Azul"};

int jogadorDaVez = 0;  
int faseJogada = 0;    
int resultadoDado = 0;

int vidas[4] = {3, 3, 3, 3};
int contadorJogadas = 0;     
int limiteJogadasBatalha = 0;

void atualizarPainelELEDs();
void executarBatalhaAleatoria();
void atualizarTodosOsLEDsDeVida();
void verificarFimDeJogo();
void somBatalha();
void printRetorno(uint8_t type, int value);

void setup() {
  Serial.begin(9600);
  serialMP3.begin(9600);
  
  Serial.println("--- Jogo Iniciado ---");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Jogo Iniciado ");

  Serial.println("Inicializando DFPlayer...");
  if (!moduloMP3.begin(serialMP3, true, true)) {
    Serial.println("Falha ao iniciar DFPlayer.");
  } else {
    Serial.println("DFPlayer Online.");
    moduloMP3.volume(30);
    moduloMP3.EQ(DFPLAYER_EQ_NORMAL);
    
    moduloMP3.playFolder(1, 0); 
  }

  delay(4000);

  randomSeed(analogRead(0));
  
  for (int i = 0; i < 4; i++) {
    pinMode(BTN[i], INPUT_PULLUP); 
    pinMode(LED_BOTOES[i], OUTPUT);
    digitalWrite(LED_BOTOES[i], LOW);
  }

  for (int j = 0; j < 4; j++) {
    for (int v = 0; v < 3; v++) {
      pinMode(LED_VIDAS[j][v], OUTPUT);
    }
  }

  pinMode(BTN_BATALHA, INPUT_PULLUP);
  limiteJogadasBatalha = random(3, 7); 

  atualizarTodosOsLEDsDeVida();
  atualizarPainelELEDs();
}

void loop() {
  int jogadorAtual = jogadorDaVez;

  if (vidas[jogadorAtual] <= 0) {
    do {
      jogadorDaVez = (jogadorDaVez + 1) % 4;
    } while (vidas[jogadorDaVez] <= 0);
    atualizarPainelELEDs();
    return;
  }

  if (digitalRead(BTN[jogadorAtual]) == LOW) {
    if (faseJogada == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorteando...");
      delay(2000); 
      
      resultadoDado = random(1, 7); 
      contadorJogadas++; 
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resultado: ");
      lcd.setCursor(12, 0);
      lcd.print(resultadoDado);
      lcd.setCursor(0, 1);
      lcd.print("  Passar a vez ");
      
      Serial.println("Jogador " + cores[jogadorAtual] + " tirou " + String(resultadoDado));
      faseJogada = 1; 
    }
    else if (faseJogada == 1) {
      if (contadorJogadas >= limiteJogadasBatalha) {
        executarBatalhaAleatoria();
        contadorJogadas = 0; 
        limiteJogadasBatalha = random(3, 7); 
      }
      
      do {
        jogadorDaVez = (jogadorDaVez + 1) % 4; 
      } while (vidas[jogadorDaVez] <= 0); 
      
      faseJogada = 0; 
      atualizarPainelELEDs();
    }

    while (digitalRead(BTN[jogadorAtual]) == LOW) { delay(10); }
    delay(200); 
  }

  if (serialMP3.available()) {
    printRetorno(moduloMP3.readType(), serialMP3.read());
  }
}

void executarBatalhaAleatoria() {
  somBatalha(); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" !! HORA DA !!");
  lcd.setCursor(0, 1);
  lcd.print(" !! BATALHA !!");
  delay(2000);

  int p1 = -1;
  int p2 = -1;

  do { p1 = random(0, 4); } while (vidas[p1] <= 0);
  do { p2 = random(0, 4); } while (vidas[p2] <= 0 || p2 == p1);

  for (int j = 0; j < 4; j++) {
    digitalWrite(LED_BOTOES[j], LOW);
  }

  atualizarTodosOsLEDsDeVida();

  digitalWrite(LED_BOTOES[p1], HIGH);
  digitalWrite(LED_BOTOES[p2], HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cores[p1].substring(0,3) + " VS " + cores[p2].substring(0,3));
  lcd.setCursor(0, 1);
  lcd.print("Duelo de D20!");
  delay(3000);

  int valorD20 = 1;
unsigned long tempoGiro = 0;
int duelistaDaVez = p1;
bool flagInterrompido = false;
bool dadoTravado = false;

lcd.clear();

while (true) {

    if (!dadoTravado && millis() - tempoGiro > 80) {
      valorD20 = random(1, 21);
      tempoGiro = millis();
    }

    lcd.setCursor(0, 0);
    lcd.print("Vez do: " + cores[duelistaDaVez] + "      ");

    lcd.setCursor(0, 1);
    lcd.print("Resultado: " + String(valorD20) + "   ");

    if (digitalRead(BTN[0]) == LOW ||
        digitalRead(BTN[1]) == LOW ||
        digitalRead(BTN[2]) == LOW ||
        digitalRead(BTN[3]) == LOW) {

      flagInterrompido = true;
      break;
    }

    if (digitalRead(BTN_BATALHA) == LOW) {

      delay(50);

      while (digitalRead(BTN_BATALHA) == LOW) {
        delay(10);
      }

      if (!dadoTravado) {

        dadoTravado = true;

        lcd.setCursor(0, 1);
        lcd.print("Resultado: " + String(valorD20) + "   ");

      } 
      else {

        dadoTravado = false;

        if (duelistaDaVez == p1) {
          duelistaDaVez = p2;
        } else {
          duelistaDaVez = p1;
        }

        valorD20 = random(1, 21);

        lcd.clear();
      }

      delay(250);
    }
}

  delay(200);

  int perdedor = -1;

  while (true) {
    if (digitalRead(BTN[p1]) == LOW) { perdedor = p2; break; }
    if (digitalRead(BTN[p2]) == LOW) { perdedor = p1; break; }
    delay(10);
  }

  digitalWrite(LED_BOTOES[p1], LOW);
  digitalWrite(LED_BOTOES[p2], LOW);

  vidas[perdedor]--; 
  atualizarTodosOsLEDsDeVida(); 

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cores[perdedor] + " perdeu!");
  lcd.setCursor(0, 1);
  lcd.print("Vida: -1");
  Serial.println(cores[perdedor] + " perdeu 1 vida. Restam: " + String(vidas[perdedor]));
 
  for(int i = 0; i < 4; i++) {
    digitalWrite(LED_BOTOES[perdedor], HIGH);
    for (int v = 0; v < 3; v++) {
      if (v < vidas[perdedor]) digitalWrite(LED_VIDAS[perdedor][v], HIGH);
    }
    delay(250);
    
    digitalWrite(LED_BOTOES[perdedor], LOW);
    for (int v = 0; v < 3; v++) { 
      digitalWrite(LED_VIDAS[perdedor][v], LOW); 
    }
    delay(250);
  }

  atualizarTodosOsLEDsDeVida();

  while (digitalRead(BTN[perdedor]) == LOW) { delay(10); }
  delay(500); 

  if (vidas[perdedor] <= 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(cores[perdedor]);
    lcd.setCursor(0, 1);
    lcd.print("ELIMINADO!"); 
    delay(3000);
    verificarFimDeJogo();
  }
}

void atualizarTodosOsLEDsDeVida() {
  for (int j = 0; j < 4; j++) {
    for (int v = 0; v < 3; v++) {
      if (v < vidas[j]) {
        digitalWrite(LED_VIDAS[j][v], HIGH); 
      } else {
        digitalWrite(LED_VIDAS[j][v], LOW);  
      }
    }
  }
}

void atualizarPainelELEDs() {
  atualizarTodosOsLEDsDeVida();

  for (int i = 0; i < 4; i++) {
    digitalWrite(LED_BOTOES[i], LOW);
  }
  
  if(vidas[jogadorDaVez] > 0) {
    digitalWrite(LED_BOTOES[jogadorDaVez], HIGH);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vez: " + cores[jogadorDaVez]);
  lcd.setCursor(0, 1);
  lcd.print("Vidas:" + String(vidas[jogadorDaVez]));
}

void verificarFimDeJogo() {
  int jogadoresVivos = 0;
  int vencedor = -1;

  for (int i = 0; i < 4; i++) {
    if (vidas[i] > 0) {
      jogadoresVivos++;
      vencedor = i;
    }
  }

  if (jogadoresVivos == 1) {
    while(true) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("FIM DE JOGO!!");
      lcd.setCursor(0, 1);
      lcd.print("Vencedor: " + cores[vencedor]);
      
      digitalWrite(LED_BOTOES[vencedor], HIGH);
      delay(500);
      digitalWrite(LED_BOTOES[vencedor], LOW);
      delay(500);
    }
  }
}

void somBatalha() {
  moduloMP3.playFolder(1, 0);
}

void printRetorno(uint8_t type, int value) {
  switch (type) {
    case TimeOut: Serial.print('.'); break;
    case WrongStack: Serial.println(F("Pilha Invalida!")); break;
    case DFPlayerCardInserted: Serial.println(F("Cartao Inserido!")); break;
    case DFPlayerCardRemoved: Serial.println(F("Cartao Removido!")); break;
    case DFPlayerCardOnline: Serial.println(F("Cartao Online!")); break;
    case DFPlayerUSBInserted: Serial.println("USB Inserido!"); break;
    case DFPlayerUSBRemoved: Serial.println("USB Removido!"); break;
    case DFPlayerPlayFinished:
      Serial.print(F("Numero:"));
      Serial.print(value);
      Serial.println(F(" Reproducao Concluida!"));
      break;
    case DFPlayerError:
      Serial.print(F("Erro no modulo MP3:"));
      switch (value) {
        case Busy: Serial.println(F("Cartao nao Encontrado")); break;
        case Sleeping: Serial.println(F("Dormindo")); break;
        case SerialWrongStack: Serial.println(F("Pilha Incorreta Recebida")); break;
        case CheckSumNotMatch: Serial.println(F("Soma de Verificacao nao Confere")); break;
        case FileIndexOut: Serial.println(F("Indice de Arquivo Fora dos Limites")); break;
        case FileMismatch: Serial.println(F("Arquivo nao encontrado")); break;
        case Advertise: Serial.println(F("Em Publicidade")); break;
        default: break;
      }
      break;
    default: break;
  }
}
