#include "customServo.h"
#include "animLcd.h"
#include "callHandler.h"
#include "header.h"

const int CURR_VOLT_PIN = A0;
const int CAP_VOLT_PIN = A1;
const int STARTUP_DURATION = 3000;

const int LOADING_PIN = 11;
const int MOTOR_STOP_PIN;

AnimatableLcd lcd(0x27, 16, 2);
CallHandler callHandler;

class ButtonHandler { //handels button clicks
    /**
       @brief Der Pin an dem der Knopf angeschlossen ist

    */
    int pin;
    /**
       @brief Gibt an, ob der Knopf momentan Gedrückt ist

    */
    bool isPressed = false;
  public:
    /**
       @brief Die Funktion die bei einem Klick, d.h. einem Drücken und loslassen des Knopfes ausgeführt wird

    */
    void (*onclick)();
    ButtonHandler() {}
    /**
       @brief Erstellt ein neues ButtonHandler Objekt

       @param pin Der Pin an dem der Knopf angeschlossen ist
       @param onclick Die Funktion die bei einem Klick, d.h. einem Drücken und loslassen des Knopfes ausgeführt wird
    */
    ButtonHandler(int pin, void (*onclick)()): pin(pin), onclick(onclick) {
      pinMode(pin, INPUT_PULLUP);
    }
    /**
       @brief Prüft, ob der Knopf gedrückt/losgelassen wurde
       @details Wird von loop() aufgerufen und ruft die ButtonHandler::onclick Funktion auf, wenn ein Klick festgestellt wurde
    */
    void update()
    {
      bool isPressedNew = digitalRead(pin) == HIGH;
      if (isPressedNew != isPressed) { //is not being pressed now, but was being pressed
        if (isPressed) {
          Serial.println("click");
          onclick();
        }
      }
      isPressed = isPressedNew;
    }
};

ButtonHandler motorButton;
bool isMotorRunning = false;
void OnStopMotorPressed(){
  static bool isMotorRunning = true;
  isMotorRunning = !isMotorRunning;
  callHandler.deleteCalls();
  if(isMotorRunning){
    auto calls = new Callable*[2]{
        new LcdDotAnim("starte Ventilator",&lcd,1000)
    }
  }else{
    
  }
}
float measureVoltage(int pin){
  float v = analogRead(pin);
  return (v/1023)*5;
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("begin");
  lcd.init();
  callHandler.setCalls(new Callable*[1] {
    new LcdLoadingAnim("Lade", &lcd, STARTUP_DURATION),
  }, 1);
  motorButton = ButtonHandler(MOTOR_STOP_PIN, &OnStopMotorPressed);
}

void loop() {
  // put your main code here, to run repeatedly:
  callHandler.update();
  lcd.update();
  float curr_volt = measureVoltage(CURR_VOLT_PIN);
  Serial.print("Spannung: ");
  Serial.print(curr_volt);
  Serial.println("V");
  if curr_volt > 1.2{
    
  }
}
