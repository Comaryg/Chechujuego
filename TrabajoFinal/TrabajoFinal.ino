//Incluimos la librería instalada "rgb_lcd.h" que nos permite interactuar con el lcd de forma más sencilla
#include "rgb_lcd.h"
#include <Keypad.h> //Incluimos keypad para el teclado

rgb_lcd lcd;
char keyHit = '4'; //Carácter que detecta el Keypad
int estado = 0; //0 = Pantalla Inicio, 1 = Cálculo, 2 = Memoria, 3 = Game Over
int inicio_seleccion = 0; //0 = Cálculo, 1 = Memoria
String secuencia;
bool UserTurnMemo = false;

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
  if (keyHit == 'A'){
    switch(inicio_seleccion){
      case 0:
        Serial.println("Cálculo Seleccionado");
        estado = 1;
        break;
      case 1:
        Serial.println("Memoria Seleccionado");
        estado = 2;
        break;
    }
    lcd.clear();
  } 
}

void inic_memoria(){

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcdsetup();
  lcd.setCursor(0,0);
  pantalla_inicio();
  }

void loop() {
  // put your main code here, to run repeatedly:
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
        Serial.println("Memoria");
        break;
    }
  }
}
