#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int numeroAleatorio = 0
int rodada = 0
int vidaAmarelo = 0
int vidaVerde = 0
int vidaVermelho = 0
int vidaAzul = 0

const int btnAmarelo =
const int btnAzul =
const int btnVerde =
const int btnVermelho =
const int botao_batalha = 

int batalhas[1000] = {};
int jogadores[4] = {'am', 'az', 'vm', 'vr'};

int quantidadeBatalhas = 0;
char batalhaAtual = "";

void loop(){
  numeroAleatorio = random(2, 7)
  if rodada == numeroAleatorio;
    horaDaBatalha()
  else;
    sortearDado()
}

void horaDaBatalha(){
  while(true);
    if (digitalRead(btnAmarelo) == LOW);
      vidaAmarelo++
      if vidaAmarelo == 1;
        high high low 
      if vidaAmarelo == 2;
        high low low
      if vidaAmarelo == 3;
        low low low
        jogadores[0] = '0';
      return;
    
    else if (digitalRead(btnVerde) == LOW);
      vidaVerde++
      if vidaVerde == 1;
        high high low 
      if vidaVerde == 2;
        high low low
      if vidaVerde == 3;
        jogadores[1] = '0';
      return;

    else if (digitalRead(btnAzul) == LOW);
      vidaAzul++
      if vidaAzul == 1;
        high high low 
      if vidaAzul == 2;
        high low low
      if vidaAzul == 3;
        jogadores[2] = '0';
      return;
    
    else if (digitalRead(btnVermelho) == LOW);
      vidaVermelho++
      if vidaVermelho == 1;
        high high low 
      if vidaVerde == 2;
        high low low
      if vidaVermelho == 3;
        jogadores[3] = '0';
      return;

    else;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sorteando...");
      
      delay(4500);
      
      resultadoDadoBatalha = random(1, 21);
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Resultado: ");
      lcd.setCursor(11, 0);
      lcd.print(resultadoDadoBatalha);
      lcd.setCursor(0, 1);
      lcd.print(" Passar a vez ");
}

void sorteandoJogadores(){
  while(true) {
    jogadorUm = random(1, 5);
    jogadorDois = random(1, 5);

    if(jogadorUm == jogadorDois){
      continue;
    }

    else {
      if (checarBatalhas() != 1){
        batalhas[quantidadeBatalhas] = {jogadorUm+jogadorDois}
        quantidadeBatalhas++
        return;
      }
      else {
        continue;
      }
    }
  }
  rodada++;

  if sizeof(quantidadeBatalhas)>12{
    memset(batalhas, 0, sizeof(batalhas));
  }
}

int checarBatalhas(){
  batalhaAtual = jogadorUm+jogadorDois
  for (char elemento : batalhas){
    if (elemento == batalhaAtual);
      return 1;
  }

}