

/* C++ code
Code by starsistor
The following are pins considered in OpenPLC
Digital In  2, 3, 4, 5, 6 %IX0.0 – %IX0.4
Digital Out 7, 8, 12, 13  %QX0.0 – %QX0.3
Analog In A0, A1, A2, A3, A4, A5  %IW0 – %IW5
Analog Out 9, 10, 11 %QW0 – %QW2
*/

unsigned long Var2TurnedOnAt; // when the LED was turned on
unsigned long turnOffDelay = 1000; // the delay before turning off the LED (in milliseconds)
bool State = false; // flag to check the current state of the LED

#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

#define TRIGGER_PIN  2
#define ECHO_PIN     3

#define S1 4
#define S2 5
#define S3 6
#define M1 7
#define MB2 8

bool Var1, Var2;

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(MB2, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
}

void loop() {


if ((Var1==HIGH && Var2==LOW) || (Var1==LOW && Var2 ==HIGH )) {

     digitalWrite(M1, HIGH);
      display.setCursor(0,10);
      display.println("       Faja ON");
      display.display();
      

} else {

     digitalWrite(M1, LOW);
      display.setCursor(0,10);
      display.println("       Faja OFF");
      display.display();
      

}



if (digitalRead(S2)==LOW && digitalRead(S3)==HIGH ) {

     digitalWrite(MB2, HIGH);
      display.setCursor(0,20);
      display.println("      Bomba  ON");
      display.display();
      
} else {

     digitalWrite(MB2, LOW);
      display.setCursor(0,20);
      display.println("      Bomba  OFF");
      display.display();
      
}


unsigned long currentMillis = millis(); // get the current time
  if (digitalRead(S3) == LOW && !State) { // check if the button is pressed
    Var2 = HIGH; // turn on the LED
    State = true; // update the LED state
    Var2TurnedOnAt = currentMillis; // update the time when the LED was turned on
  }

  if (State && (currentMillis - Var2TurnedOnAt >= turnOffDelay)) {
    Var2 = LOW; // turn off the LED after the delay
    State = false; // update the LED state
  }


if (digitalRead(S2) == HIGH && ( digitalRead(S1)==LOW || Var1==HIGH ) ) {
     Var1 = HIGH;
     
} else {
     Var1 = LOW;

     
}


  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  float distance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;

  //limpiar
  //display.setCursor(80,0);
  //display.print("                     ");
  //display.display();
  
  display.setCursor(0,0);
  display.print(" Distancia: ");
  display.print(distance);
  display.println(" cm");
  display.display();
  display.clearDisplay();

}
