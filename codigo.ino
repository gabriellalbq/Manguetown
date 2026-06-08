#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- CONFIGURAÇÃO DO DISPLAY LCD ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// --- CONFIGURAÇÃO DOS PINOS ---
const int BTN[4] = {4, 10, 8, 12}; // Índices: 0=Vermelho, 1=Amarelo, 2=Verde, 3=Azul
const int LED[4] = {5, 9, 7, 11};  

// Botão que pausa o sorteio do D20
const int BTN_BATALHA = 2; 

// Nomes das cores na mesma ordem dos pinos
const String cores[4] = {"Vermelho", "Amarelo", "Verde", "Azul"};

// --- VARIÁVEIS DE CONTROLE DO JOGO ---
int jogadorDaVez = 0;  
int faseJogada = 0;    
int resultadoDado = 0;

// --- VARIÁVEIS PARA A BATALHA E VIDAS ---
int vidas[4] = {3, 3, 3, 3}; 
int contadorJogadas = 0;     
int limiteJogadasBatalha = 0; // Quantidade de rodadas aleatórias até a próxima batalha

void atualizarPainelELEDs();
void executarBatalhaAleatoria();

void setup() {
  Serial.begin(9600);
  Serial.println("--- Jogo Iniciado ---");

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Jogo Iniciado ");
  delay(2000);

  randomSeed(analogRead(0));
  
  for (int i = 0; i < 4; i++) {
    pinMode(BTN[i], INPUT_PULLUP); 
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW); 
  }

  pinMode(BTN_BATALHA, INPUT_PULLUP);

  // Sorteia o primeiro limite de rodadas para a primeira batalha (entre 3 e 6)
  limiteJogadasBatalha = random(3, 7); 

  atualizarPainelELEDs();
}

void loop() {
  int jogadorAtual = jogadorDaVez;

  if (digitalRead(BTN[jogadorAtual]) == LOW) {
    
    // --- FASE 1: RODAR O DADO DO TABULEIRO (D6) ---
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
    // --- FASE 2: PASSAR A VEZ / VERIFICAR BATALHA ---
    else if (faseJogada == 1) {
      // SE ATINGIU O LIMITE ALEATÓRIO, DISPARA A BATALHA
      if (contadorJogadas >= limiteJogadasBatalha) {
        executarBatalhaAleatoria();
        contadorJogadas = 0; 
        // Sorteia um novo limite para a próxima batalha (entre 3 e 6)
        limiteJogadasBatalha = random(3, 7); 
      }

      digitalWrite(LED[jogadorDaVez], LOW);
      
      do {
        jogadorDaVez = (jogadorDaVez + 1) % 4; 
      } while (vidas[jogadorDaVez] <= 0); 
      
      faseJogada = 0; 
      atualizarPainelELEDs();
    }

    while (digitalRead(BTN[jogadorAtual]) == LOW) {
      delay(10);
    }
    delay(200); 
  }
}

// --- FUNÇÃO: BATALHA COMPLETA COM PAUSA E DESISTÊNCIA NO BOTÃO PRÓPRIO ---
void executarBatalhaAleatoria() {
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

  // Apaga tudo e deixa ACESO fixo os dois da batalha
  for (int i = 0; i < 4; i++) { digitalWrite(LED[i], LOW); }
  digitalWrite(LED[p1], HIGH);
  digitalWrite(LED[p2], HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cores[p1].substring(0,3) + " VS " + cores[p2].substring(0,3));
  lcd.setCursor(0, 1);
  lcd.print("Pausar no Batalha");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("D20 RODANDO:");

  int valorD20 = 1;

  // --- PASSO 1: O DADO RODA ATÉ APERTAR O BOTÃO DE BATALHA (PINO 2) ---
  while (digitalRead(BTN_BATALHA) == HIGH) {
    valorD20 = random(1, 21); 
    
    lcd.setCursor(4, 1);
    lcd.print("D20: ");
    if (valorD20 < 10) lcd.print(" "); 
    lcd.print(valorD20);
    
    delay(50); 
  }

  // Debounce do botão de batalha e espera soltar
  delay(200);
  while (digitalRead(BTN_BATALHA) == LOW) { delay(10); }

  // --- PASSO 2: DADO TRAVADO. ESPERA ALGUÉM APERTAR O SEU BOTÃO NORMAL ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("D20 travado: " + String(valorD20));
  lcd.setCursor(0, 1);
  lcd.print("Quem perdeu?...");

  int perdedor = -1;

  while (true) {
    if (digitalRead(BTN[p1]) == LOW) {
      perdedor = p1;
      break;
    }
    if (digitalRead(BTN[p2]) == LOW) {
      perdedor = p2;
      break;
    }
    delay(10);
  }

  // Reduz a vida de quem apertou
  vidas[perdedor]--;

  // Mostra a mensagem de derrota
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(cores[perdedor] + " perdeu!");
  lcd.setCursor(0, 1);
  lcd.print("Vida: -1");
  Serial.println(cores[perdedor] + " assumiu a derrota no D20 valor " + String(valorD20) + " e perdeu 1 vida.");
  delay(4000);

  // Espera soltar o botão próprio
  while (digitalRead(BTN[perdedor]) == LOW) {
    delay(10);
  }
  delay(200); 

  // Verificação de eliminação definitiva
  if (vidas[perdedor] <= 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(cores[perdedor]);
    lcd.setCursor(0, 1);
    lcd.print("ELIMINADO!"); 
    delay(3000);
  }
}

void atualizarPainelELEDs() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED[i], LOW);
  }
  digitalWrite(LED[jogadorDaVez], HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vez: " + cores[jogadorDaVez]);
  lcd.setCursor(0, 1);
  lcd.print("Vidas:" + String(vidas[jogadorDaVez]));
}