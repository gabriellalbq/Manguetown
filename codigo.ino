#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <iostream>
#include <vector>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int BTN[4] = {6, 8, 10, 12}; 
const int BOTAO_VERMELHO = 5;
const int BOTAO_VERDE = 7;
const int BOTAO_AMARELO = 9;
const int BOTAO_AZUL = 11;
const int LED_VERMELHA[4] = {22, 23, 24};
const int LED_VERDE[4] = {25, 26, 27};
const int LED_AZUL[4] = {28, 29, 30};
const int LED_AMARELA[4] = {31, 32, 33};
int dadoBatalhas = 0;
int numeroAleatorio = 0;
int rodadas = 0;
int contRodadas = 0;
int batalhas = [];
char jogadores = [jogador_um, jogador_dois, jogador_tres, jogador_quatro];
int vidasJogadorAmarelo = 0;
int vidasJogadorVemelho = 0;
int vidasJogadorVerde = 0;
int vidasJogadorAzul = 0;

const int botaoBatalha = BTN[0];
int resultadoDadoBatalha = 0;
int contadorBotaoBatalha = 0;

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(0, 0);
  lcd.print("Dado Eletronico");
  lcd.setCursor(0, 1);
  lcd.print("Aperte o botao");

  randomSeed(analogRead(0)); 
  }

void loop () {
    numeroAleatorio = random(3, 7);
    if rodadas == numeroAleatorio;
        horadabatalha();
    numeroAleatorio = random(3, 7);
    sortearDado(sizeof(jogadores));
        for i in range(sizeof(jogadores));
            rodarDado();
            rodadas ++
}

void horadabatalha () {
    batalhador_um = random(1, 4);
    batalhador_dois = random(1,4);
    batalhaAtual = batalhador_um+batalhador_dois
    if (!batalhas.contains(batalhaAtual)) {
        numeros.push_back(batalhaAtual);
    }
    if (sizeof(batalhas) > 12) {
        batalhas = []
    }
    if (digitalRead(botaoBatalha) == LOW){ 
    contadorBotaoBatalha++; 

    while (true);

    if BOTAO_AMARELO == LOW {
        vidasJogadorAmarelo++
        // apagar led 1 amarelo
        if vidasJogadorAmarelo >=3
        // pop jogador amarelo da lista
        break
    }

    else if BOTAO_AZUL == LOW{
        vidasJogadorAzul++
        // apagar led 1 azul
        if vidasJogadorazul >=3
        // pop jogador azul da lista
        break
    }

    else if BOTAO_VERMELHO ==  LOW {
        vidasJogadorVerde++
        // apagar led 1 vermelho
        if vidasJogadorVemelho >=3
        // pop jogador vermelho da lista
        break
    }
    
    else if BOTAO_VERDE == LOW {
        vidasJogadorVerde ++
        // apagar led 1 verde
        if vidasJogadorVerde >=3
        // pop jogador verde da lista
        break
    }

    else {
        if (contadorBotaoBatalha == 1){
        lcd.clear();
        lcd.print("Sorteando...");
            
        delay(2000);
            
        resultadoDadoBatalha = random(1-21)
            
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Resultado: ");
        lcd.print(resultadoDadoBatalha);
        }
        else if (contadorBotaoBatalha == 2){
        lcd.setCursor(0, 1);
        lcd.print("  Passar a vez  ");
        }
        else if (contadorBotaoBatalha == 3){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Clique para");
        lcd.setCursor(0, 1);
        lcd.print(" rodar o dado");
        }

        while (digitalRead(botaoDadoPino) == LOW) {
        delay(10); 
        }
        delay(50);
    //rodadas = 0
    //resortear o numero aleatorio
    }

  }

  void rodarDado();

    const int pinoBotao = 8;
    const int pinoLed = 7; 
    // ajeitar pra ser um botao de cada veezzzz!!!

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