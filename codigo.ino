#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinoBotao = 8;
const int pinoLed = 7; 

void setup() {
  pinMode(pinoBotao, INPUT_PULLUP);
  pinMode(pinoLed, OUTPUT);
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Aperte o botao");
  // ver o acento!!!
}

void loop() {
  if (digitalRead(pinoBotao) == LOW) {
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rodando o dado...");
    
    for (int i = 0; i < 5; i++) {
      digitalWrite(pinoLed, HIGH);
      delay(100);
      digitalWrite(pinoLed, LOW);
      delay(100);
    }

    int numeroSorteado = random(1, 7); 
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Resultado:");
    lcd.setCursor(0, 1);
    lcd.print("Dado: ");
    lcd.print(numeroSorteado);
    
    delay(2000); 
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Aperte o botao");
  }
}

// 2 codigo

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int BTN[4] = {6, 8, 10, 12}; 
const int LED[4] = {5, 7, 9, 11}; 

const int botaoDadoPino = BTN[0];

int resultadoDado = 0;
int counter = 0;

void setup() {
    Serial.begin(9600);
  Serial.println("--- Jogo Iniciado ---");

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Jogo Iniciado ");
  delay(3000);


  randomSeed(analogRead(0));
  
  // Configura os pinos de entrada e saída
  for (int i = 0; i < 4; i++) {
    pinMode(BTN[i], INPUT_PULLUP); 
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW); 
}

void controleDado(){
    if (digitalRead(BTN[jogadorDaVez]) == LOW) {
    
    // --- FASE 1: RODAR O DADO ---
    if (faseJogada == 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      //lcd.print("Jogador " + cores[jogadorDaVez]);
      //lcd.setCursor(0, 1);
      lcd.print("Sorteando...");
      
      delay(4500); // Efeito de sorteio
      
      resultadoDado = random(1, 7);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resultado: ");
      lcd.setCursor(11, 0);
      lcd.print(resultadoDado);
      lcd.setCursor(0, 1);
      lcd.print(" Passar a vez ");
      
      Serial.println("Jogador " + cores[jogadorDaVez] + " tirou " + String(resultadoDado));
      
      faseJogada = 1; // Avança para a fase de passar a vez
    }
    // --- FASE 2: PASSAR A VEZ ---
    else if (faseJogada == 1) {
      // Apaga o LED do jogador atual antes de mudar o turno
      digitalWrite(LED[jogadorDaVez], LOW);
      
      // Passa para o próximo jogador da lista (Vermelho -> Amarelo -> Verde -> Azul -> Volta pro Vermelho)
      jogadorDaVez = (jogadorDaVez + 1) % 4; 
      
      faseJogada = 0; // Reseta a fase para que o próximo possa rodar o dado
      
      // Atualiza o painel e acende o LED do próximo jogador
      atualizarPainelELEDs();
    }

    // Trava para evitar cliques duplos (espera o jogador soltar o botão)
    while (digitalRead(BTN[jogadorDaVez]) == LOW) {
      delay(10);
    }
    delay(1000); // Debounce para estabilidade física do botão
  }
}
}

void atualizarPainelELEDs() {
  // Apaga todos os LEDs do jogo
  for (int i = 0; i < 4; i++) {
    digitalWrite(LED[i], LOW);
  }
  // Acende apenas o LED correspondente ao jogador da rodada atual
  digitalWrite(LED[jogadorDaVez], HIGH);

  // Atualiza o display LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vez do Jogador:");
  lcd.setCursor(0, 1);
  lcd.print(" " + cores[jogadorDaVez]);
  
  Serial.println("Vez do Jogador " + cores[jogadorDaVez]);
}

void controleJogadores() {
  for(int i = 0; i < 5; i++) {
    
    if (digitalRead(BTN[i]) == LOW) {
      digitalWrite(LED[i], HIGH);
      
      Serial.print("Botao do Jogador ");
      Serial.print(i + 1);
      Serial.println(" PRESSIONADO! LED ligado.");
      
    } else {
      digitalWrite(LED[i], LOW);
    }
  }
}

void loop(){
  controleDado(); 
  controleJogadores();
  
  delay(50);
}