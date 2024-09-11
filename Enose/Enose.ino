#include <Wire.h> // Biblioteca I2C
#include <LiquidCrystal_I2C.h> // Biblioteca display 16x2

int botao = 40; // declara do botão na porta 40
int led = 41; // declara led na porta 41
bool estadobotao = 0; // variavel de controle
float mediavaloresLidosETNL = 0;
float acumuladovaloresLidosETNL = 0;
int tempo = 5; //tempo em segundos

float mediavaloresLidosGLP = 0;
float acumuladovaloresLidosGLP = 0;

int sensorETNL = A0; // Definição da porta analógica do sensor etanol
int sensorGLP = A7; // Definição da porta analógica do sensor etanol

#define col 16 // Constante que defini o número de colunas do display
#define lin  2 // Constante que definir o numero de linhas do display
#define ende  0x27 // Constante que definir o endereço do display.


LiquidCrystal_I2C lcd(ende,col,lin); // Função do LiquidCrystal para o I2C - paramentros: endereço, número de colunas, número de linhas

float valoresLidosETNL[5];
float valoresLidosGLP[5];

void setup(){
  pinMode(botao, INPUT_PULLUP); // define o pino do botao como entrada
  pinMode(led, OUTPUT);// define LED como saida

  pinMode(sensorETNL, INPUT);
  pinMode(sensorGLP, INPUT);

  lcd.init(); // initialize display
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Desativado");
  Serial.begin(9600);
}

void loop(){
  verificarBotao();
  
    if (estadobotao == 1) // Se o botão for pressionado
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Lendo dados...");

    for (int i=0; i<tempo; i++) {

    int vlrSensorETNL = analogRead(sensorETNL);
    int vlrSensorGLP = analogRead(sensorGLP);
    float valorSensorETNL = (vlrSensorETNL * 100.0) / 1024.0;  // defini saída entre 0 e 100 Etanol
    float valorSensorGLP = (vlrSensorGLP * 100.0) / 1024.0;    // defini saída entre 0 e 100 GLP
    valoresLidosETNL[i] = valorSensorETNL;  // vetor para calculo da média
    valoresLidosGLP[i] = valorSensorGLP;    // vetor para calculo da média
    delay(1000);
    }
    estadobotao = 0;
    digitalWrite(led, estadobotao);

    for (byte i=0; i<tempo; i++) {//Calcular a média dos valores lidos
   
    acumuladovaloresLidosETNL = acumuladovaloresLidosETNL + valoresLidosETNL[i];
    acumuladovaloresLidosGLP = acumuladovaloresLidosGLP + valoresLidosGLP[i];
    }
     
     
     mediavaloresLidosETNL = acumuladovaloresLidosETNL / tempo;
     mediavaloresLidosGLP = acumuladovaloresLidosGLP / tempo;
    
    Serial.print(mediavaloresLidosETNL);
     Serial.print(" ");
     Serial.print(mediavaloresLidosGLP);
    lcd.clear();
    lcd.setCursor(0,0);
   lcd.print("Etanol");
   lcd.setCursor(8,0);
   lcd.print(mediavaloresLidosETNL);
   lcd.setCursor(13,0);
   lcd.print("%");

   lcd.setCursor(0,1);
   lcd.print("GLP");
   lcd.setCursor(8,1);
   lcd.print(mediavaloresLidosGLP);
   lcd.setCursor(13,1);
   lcd.print("%");
    
  }
  
delay(1000);

mediavaloresLidosETNL = 0;
acumuladovaloresLidosETNL = 0;
mediavaloresLidosGLP = 0;
acumuladovaloresLidosGLP = 0;
  
}

// Função botão de ativação

void verificarBotao(){
if (digitalRead(botao) == LOW) // Se o botão for pressionado
  {
    estadobotao = !estadobotao; // troca o estado do LED
    digitalWrite(led, estadobotao);
    while (digitalRead(botao) == LOW);    
  }
}
