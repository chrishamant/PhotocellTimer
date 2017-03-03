#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(4);

int photocellStartPin = A0;
int photocellEndPin = A3;
int buttonPin = 2;

int triggerThreshold = 700;

enum TimerState{ REST, READY, RUNNING };

volatile TimerState currentState = REST;

unsigned long currentTimer = 0;
unsigned long lastTimerResult = 0;

void setup() {
    Serial.begin(9600);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    pinMode(A0,INPUT);
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), enterReady, LOW);

    introText();
}

void loop() {

    display.setCursor(0,0);
    display.setTextColor(WHITE);
    display.clearDisplay();

    display.print("currently: ");

    switch(currentState) {
        case REST :
          display.println("resting");
          displayLastTime();
          break;
        case READY :
          display.println("ready");
          displayLastTime();
          if(analogRead(photocellStartPin) < triggerThreshold){
              currentTimer = millis();
              currentState = RUNNING;
          }
          break;
        case RUNNING :
          display.println("running");
          display.println("");
          display.println("- - - - - -");
          if(analogRead(photocellEndPin) < triggerThreshold){
              lastTimerResult = millis() - currentTimer;
              currentTimer = 0;
              currentState = REST;
          }
          break;
    }
}

// transition
void enterReady(){
  currentState = READY;
}

void displayLastTime(){
  display.println("");
  display.println("Last Time:   ");
  display.print(lastTimerResult);
  display.println(" ms");
}

void introText(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.invertDisplay(true);
  display.println("     WATTS RULES     ");
  display.println("    OTHERS DROOL     ");
  display.println("- - - - - - - - - - -");
  display.println("        2017         ");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();
  display.invertDisplay(false);
}

