//Incluimos la librería instalada "rgb_lcd.h" que nos permite interactuar con el lcd de forma más sencilla
#include "rgb_lcd.h"
#include <Keypad.h> //Incluimos keypad para el teclado

rgb_lcd lcd;
char keyHit = '4'; //Carácter que detecta el Keypad
int estado = 0; //0 = Pantalla Inicio, 1 = Cálculo, 2 = Memoria, 3 = Game Over
int inicio_seleccion = 0; //0 = Cálculo, 1 = Memoria
String secuencia;
String secuenciaUser;
int longsec = 4;
int secindex = 0;
String numrandom;
int auxrandom;
bool UserTurnMemo = false;
unsigned long currentMillis; 
unsigned long previousMillisCountdown = 0;
unsigned long previousMillisMemo = 0;
bool gameOn = false;
int segundos;
int segPlusMemo = 5;
long intervalMemo = 200;
int puntosMemo = 0;

void lcdsetup() {
  //Pongo las operaciones del lcd que se ejecutan al principio del programa
  lcd.begin(16,2); //16 columnas y 2 filas
  lcd.display(); //Muestra la pantalla, ocultar con lcd.noDisplay();
  lcd.setRGB(255,255,255); //Color, en valores de Rojo, Verde y Azul
  lcd.clear();
}

//Declaramos filas y columnas de nuestro Keypad
const byte Filas = 4;
const byte Columnas = 4;
//Definiremos los symbols que se pulsan al pulsar el teclado
char keyValores[Filas][Columnas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
//Asociamos los pines del sistema
byte filaPins[Filas] = {9,8,7,6};
byte columPins[Columnas] = {5,4,3,2};

//Inicializaremos una instacia de clase Keypad para usarla, igual que rgb_lcd lcd;
Keypad keypad = Keypad(makeKeymap(keyValores), filaPins, columPins, Filas, Columnas);

//Funcion que se encarga de la pantalla de inicio
void pantalla_inicio(){
  if (keyHit == '4'){
    lcd.clear(); //Limpiamos la pantalla y depende de la tecla elegida se pasa por 1 de los dos juegos
    lcd.setCursor(0,0);
    lcd.print("El Chechujuego");
    lcd.setCursor(0,1);
    lcd.print(">Calculo");
    lcd.setCursor(9,1);
    lcd.print("Memoria");
    inicio_seleccion = 0;
  }
  else if (keyHit == '6'){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("El Chechujuego");
    lcd.setCursor(0,1);
    lcd.print("Calculo");
    lcd.setCursor(8,1);
    lcd.print(">Memoria");
    inicio_seleccion = 1;
  }
  if (keyHit == '#'){
    switch(inicio_seleccion){
      case 0:
        Serial.println("Cálculo Seleccionado");
        estado = 1;
        break;
      case 1:
        Serial.println("Memoria Seleccionado");
        estado = 2;
		    inic_memo();
        break;
    }
  } 
}

void inic_memo(){
  lcd.setCursor(0,0);
  lcd.clear();
  gameOn = true;
  previousMillisMemo = currentMillis-intervalMemo;
  previousMillisCountdown = currentMillis;
  segundos = 60;
  puntosMemo = 0;
  UserTurnMemo = false;
  secuencia = "";
  secuenciaUser = "";
  secindex = 0;
  lcd.setCursor(0,1);
  lcd.print(puntosMemo);
  lcd.setCursor(14,1);
  lcd.print(segundos);
}

void countdown_memo(){
  if (segundos > 0){
    if (currentMillis - previousMillisCountdown >= 1000){
      previousMillisCountdown = currentMillis;
      segundos -= 1;
      Serial.println(segundos);
      lcd.setCursor(14,1);
      lcd.print("  ");
      lcd.setCursor(14,1);
      lcd.print(segundos);
	  }
  } else {
    gameOn = false;
    estado = 3;
    gameOver();
  }
}

void juego_memo(){
  if(!UserTurnMemo){
    if (currentMillis - previousMillisMemo >= intervalMemo){
      lcd.setCursor(0,0);
      lcd.print("           ");
      if (secindex >= longsec){
        UserTurnMemo = true;
        return;
      }
      randomSeed(millis());
      if (puntosMemo < 5){
        numrandom = random(0,9);
      } else {
        auxrandom = random(0,13);
        switch(auxrandom){
          case 10:
            numrandom = 'A';
            break;
          case 11:
            numrandom = 'B';
            break;
          case 12:
            numrandom = 'C';
            break;
          case 13:
            numrandom = 'D';
            break;
          default:
            numrandom = auxrandom;
            break;
        }
      }
      secuencia += numrandom;
      secindex++;
      lcd.setCursor(0,0);
      lcd.print(numrandom);
      tone(10, 500, 100);
      Serial.println(secuencia);
      previousMillisMemo = currentMillis;
    }
  } else {
    if (keyHit){
	    if (keyHit != '#' && keyHit != '*'){
      secuenciaUser += keyHit;
      lcd.setCursor(0,0);
      lcd.print("           ");
      lcd.setCursor(0,0);
      lcd.print(secuenciaUser);
      }
      if (keyHit == '*'){
        secuenciaUser = "";
        lcd.setCursor(0,0);
        lcd.print("           ");
      }
      if (keyHit == '#'){
        lcd.setCursor(0,0);
        lcd.print("           ");
        if (secuenciaUser == secuencia){
          puntosMemo += 1;
          segundos += segPlusMemo;
          intervalMemo -= 25;
          intervalMemo = constrain(intervalMemo, 200, 1000);
          lcd.setCursor(14,1);
          lcd.print(segundos);
        } else {
          puntosMemo -= 1;
          intervalMemo += 25;
          intervalMemo = constrain(intervalMemo, 200, 1000);
          if (puntosMemo < 0){
            puntosMemo = 0;
          }
        }
        lcd.setCursor(0,1);
        lcd.print(puntosMemo);
        UserTurnMemo = false;
        secuencia = "";
        secuenciaUser = "";
        secindex = 0;
        Serial.println(puntosMemo);
      }
    }
  }   
}

void gameOver(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Se acabo!");
  lcd.setCursor(0,1);
  lcd.print("Puntos:");
  lcd.setCursor(8,1);
  lcd.print(puntosMemo);
  lcd.setCursor(15,1);
  lcd.print(">");
  Serial.println("Game Over");
  if (keyHit){
    estado = 0;
    keyHit = '4';
    pantalla_inicio();
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcdsetup();
  pinMode(10, OUTPUT); //Altavoz
  lcd.setCursor(0,0);
  pantalla_inicio();
  }

void loop() {
  // put your main code here, to run repeatedly:
  currentMillis = millis();
  keyHit = keypad.getKey();
  if (keyHit){
    switch(estado){
      case 0:
        pantalla_inicio();
        break;
      case 1:
        Serial.println("El Chechu");
        break;
      case 2:
        break;
      case 3:
        gameOver();
        break;
    }
  }
  if (gameOn){
    switch(estado){
      case 1:
        Serial.println("El Chechu");
        break;
      case 2:
        juego_memo();
        countdown_memo();
        break;
    }
  }
}
