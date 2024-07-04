/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico:  EB5AV 
   Curso: Taller de electronica digital y reparacion de computadoras I
   Proyecto: Contador de aforo. 
   Dev: Alumno. Manfredo Guillermo Vásquez Gómez
   Fecha:  6 de julio.
*/

#include <Adafruit_NeoPixel.h>

enum Colores {
  verde,
  rojo
};

int Cantidad_leds = 12;   // Cantidad de leds en la rueda
int NEO = 9;              // pin conectado al Arduino
Adafruit_NeoPixel pixels(Cantidad_leds, NEO, NEO_GRB + NEO_KHZ800);

// Variables para controlar el sensor ultrasonico
int TRIG = 10;
int ECHO = 11;
int DURACION;
int DISTANCIA;

// Control del Display
int A = 2;
int B = 3;
int C = 4;
int D = 5;
int E = 6;
int F = 7;
int G = 8;
const int Pin = 7;
const int Segmentos[Pin] = {A, B, C, D, E, F, G};
const int num[10][7] = {
  {1,1,1,1,1,1,0}, // 0
  {0,1,1,0,0,0,0}, // 1
  {1,1,0,1,1,0,1}, // 2
  {1,1,1,1,0,0,1}, // 3
  {0,1,1,0,0,1,1}, // 4
  {1,0,1,1,0,1,1}, // 5
  {0,0,1,1,1,1,1}, // 6
  {1,1,1,0,0,0,0}, // 7
  {1,1,1,1,1,1,1}, // 8
  {1,1,1,0,0,1,1}  // 9
};

int contador = 0; // Contador de personas
const int botonReset = 12; // Pin del botón de reinicio

void printColor(int color);

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(botonReset, INPUT_PULLUP); // Configurar botón de reinicio con resistencia interna

  for (int i = 0; i < Pin; i++) {
    pinMode(Segmentos[i], OUTPUT);
    digitalWrite(Segmentos[i], LOW);
  }

  pixels.begin();
  mostrarNumero(contador);
  printColor(verde); // Inicializar en verde
}

void loop() {
  DISTANCIA = medirDistancia();

  if (DISTANCIA >= 6 && DISTANCIA <= 10) {
    if (contador < 9) {
      contador++;
      mostrarNumero(contador);
      printColor(verde); // Mantener en verde al incrementar
    } else {
      contador = 9; // Mantener el contador en 9
      mostrarNumero(contador);
      printColor(rojo); // Encender en rojo al alcanzar el máximo
      mostrarSoloG();
    }
    delay(1000); // Evitar múltiples incrementos por la misma persona
  }

  // Comprobar el estado del botón de reinicio
  if (digitalRead(botonReset) == LOW) {
    contador = 0; // Reiniciar contador a 0
    mostrarNumero(contador);
    printColor(verde); // Encender en verde al reiniciar
    delay(200); // Evitar múltiples reinicios por un solo pulso del botón
  }
}

int medirDistancia() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  DURACION = pulseIn(ECHO, HIGH);
  DISTANCIA = DURACION * 0.034 / 2;
  return DISTANCIA;
}

void mostrarNumero(int numero) {
  for (int i = 0; i < Pin; i++) {
    digitalWrite(Segmentos[i], num[numero][i]);
  }
}

void mostrarSoloG() {
  for (int i = 0; i < Pin; i++) {
    digitalWrite(Segmentos[i], LOW);
  }
  digitalWrite(G, HIGH);
}

void printColor(int color) {
  for (int i = 0; i < Cantidad_leds; i++) {
    if (color == verde) {
      pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Verde
    } else if (color == rojo) {
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Rojo
    }
  }
  pixels.show();
}
