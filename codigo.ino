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
  Serial.println("--- Sistema Iniciado: Dado + LEDs dos Jogadores ---");

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Dado Eletronico");
  lcd.setCursor(0, 1);
  lcd.print("Aperte o botao");

  randomSeed(analogRead(0)); 

  for (int i = 0; i < 4; i++) {
    pinMode(BTN[i], INPUT_PULLUP); 
    pinMode(LED[i], OUTPUT);
    digitalWrite(LED[i], LOW);
  }
}

void controleDado(){
  if (digitalRead(botaoDadoPino) == LOW){ 
    counter++; 

    if (counter == 1){
      lcd.clear();
      lcd.print("Sorteando...");
          
      delay(2000);
          
      resultadoDado = random(1, 7);
          
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resultado: ");
      lcd.print(resultadoDado);
    }
    else if (counter == 2){
      lcd.setCursor(0, 1);
      lcd.print("  Passar a vez  ");
    }
    else if (counter == 3){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Clique para");
      lcd.setCursor(0, 1);
      lcd.print(" rodar o dado");
      
      counter = 0; 
    }

    while (digitalRead(botaoDadoPino) == LOW) {
      delay(10); 
    }
    delay(50);
  }
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