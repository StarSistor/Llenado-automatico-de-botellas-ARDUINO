/* C++ code
Code by starsistor
The following are pins considered in OpenPLC
Digital In  2, 3, 4, 5, 6 %IX0.0 – %IX0.4
Digital Out 7, 8, 12, 13  %QX0.0 – %QX0.3
Analog In A0, A1, A2, A3, A4, A5  %IW0 – %IW5
Analog Out 9, 10, 11 %QW0 – %QW2
*/

unsigned long Var2TurnedOnAt; // Momento en que el motor se encendió
unsigned long turnOffDelay = 1000; // Retardo antes de apagar el motor (en milisegundos)
bool State = false; // Bandera para verificar el estado actual del motor

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

#define TRIGGER_PIN 2
#define ECHO_PIN 3

#define S1 4
#define S2 5
#define S3 6
#define M1 7
#define MB2 8

bool Var1, Var2;

void setup() {
  // Configuración de pines
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(MB2, OUTPUT);
  pinMode(11, OUTPUT);

  // Inicialización del display OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {
  analogWrite(11, 130);
  display.clearDisplay();

  // Control de estado de la faja
  if ((Var1 == HIGH && Var2 == LOW) || (Var1 == LOW && Var2 == HIGH)) {
    digitalWrite(M1, HIGH);
    display.setCursor(0, 10);
    display.println("       Faja ON");
  } else {
    digitalWrite(M1, LOW);
    display.setCursor(0, 10);
    display.println("       Faja OFF");
  }

  // Control de estado de la bomba
  if (digitalRead(S2) == LOW && digitalRead(S3) == HIGH && Var2 == LOW) {
    digitalWrite(MB2, HIGH);
    display.setCursor(0, 20);
    display.println("      Bomba  ON");
  } else {
    digitalWrite(MB2, LOW);
    display.setCursor(0, 20);
    display.println("      Bomba  OFF");
  }

  unsigned long currentMillis = millis(); // Obtener el tiempo actual

  // Control del motor basado en el estado del interruptor S3
  if (digitalRead(S3) == LOW && !State) {
    Var2 = HIGH; // Encender el motor
    State = true; // Actualizar el estado del motor
    Var2TurnedOnAt = currentMillis; // Actualizar el momento en que se encendió el motor
  }

  if (State && (currentMillis - Var2TurnedOnAt >= turnOffDelay)) {
    Var2 = LOW; // Apagar el motor después del retardo
    State = false; // Actualizar el estado del motor
  }

  // Control de Var1 basado en los estados de los interruptores S1 y S2
  if (digitalRead(S2) == HIGH && (digitalRead(S1) == LOW || Var1 == HIGH)) {
    Var1 = HIGH;
  } else {
    Var1 = LOW;
  }

  // Medición de distancia utilizando el sensor ultrasónico
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;

  // Mostrar la distancia en el display OLED
  display.setCursor(0, 0);
  display.print(" Distancia: ");
  display.print(distance);
  display.println(" cm");
  display.display();
}

